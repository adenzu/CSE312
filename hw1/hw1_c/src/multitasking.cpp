
#include <multitasking.h>

using namespace myos;
using namespace myos::common;

Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpustate = (CPUState *)(stack + 4096 - sizeof(CPUState));

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    cpustate->eip = (uint32_t)entrypoint;
    cpustate->cs = gdt->CodeSegmentSelector();
    cpustate->eflags = 0x202;
}

/**
 * @brief Constructs a new Task object.
 *
 * @param esp The value of the stack pointer (esp) register.
 */
myos::Task::Task(uint32_t esp)
{
    cpustate = (CPUState *)(stack + 4096 - sizeof(CPUState));
    CPUState *source = (CPUState *)esp;
    *(cpustate) = *source;
    cpustate->eax = 0;
}

void printfHex(uint8_t);

int myos::Task::GetID()
{
    return id;
}

void myos::Task::SetPriority(int priority)
{
    this->priority = priority < 0 ? 0 : priority;
}

int myos::Task::GetPriority()
{
    return priority;
}

TaskState myos::Task::GetState()
{
    return state;
}

Task::~Task()
{
}

/**
 * Finds the next task to be executed in the task manager.
 * The task is selected based on its state and waiting process.
 * If a task is in the READY state, it is selected as the next task.
 * If a task is in the BLOCKED state and the process it is waiting for has exited,
 * it is also selected as the next task.
 * The selected task is marked as RUNNING.
 */
void myos::TaskManager::FindNextTask()
{
    if (checkPriority)
    {
        int selected = currentTask;
        int priority = tasks[currentTask]->priority;
        for (int i = 0; i < numTasks; i++)
        {
            if (tasks[i]->state == TaskState::READY)
            {
                if (tasks[i]->priority > priority)
                {
                    selected = i;
                    priority = tasks[i]->priority;
                }
            }
        }
        currentTask = selected;
        tasks[currentTask]->state = TaskState::RUNNING;
        return;
    }
    while (true)
    {
        if (++currentTask >= numTasks)
            currentTask %= numTasks;

        if (tasks[currentTask]->state == TaskState::READY)
            break;

        if (tasks[currentTask]->state == TaskState::BLOCKED &&
            tasks[tasks[currentTask]->waitingPid]->state == TaskState::EXITED)
            break;
    }
    tasks[currentTask]->state = TaskState::RUNNING;
}

TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager()
{
}

/**
 * Adds a task to the task manager.
 *
 * @param task A pointer to the task to be added.
 * @return True if the task was successfully added, false otherwise.
 */
bool myos::TaskManager::AddTask(Task *task)
{
    if (numTasks >= 256)
        return false;
    tasks[numTasks++] = task;
    tasks[numTasks - 1]->id = numTasks - 1;
    taskQueue.enqueue(numTasks - 1, task->priority);
    return true;
}

void printf(char *);
void printfHex32(uint32_t);

/**
 * @brief This function is responsible for scheduling tasks in a multitasking system.
 *
 * @param cpustate A pointer to the CPU state.
 * @return CPUState* A pointer to the CPU state of the next scheduled task.
 */
CPUState *TaskManager::Schedule(CPUState *cpustate)
{
    clockCounter++;

    if (waitingEnter)
    {
        return cpustate;
    }

    if (numTasks <= 0)
        return cpustate;

    if (currentTask >= 0)
    {
        // printf("Old Task PID: ");
        // printfHex32(tasks[currentTask]->GetID());
        // printf(", ");
        if (cpustate->eax == 7) // waitpid
        {
            // printf("Called waitpid, ");
            tasks[currentTask]->state = TaskState::BLOCKED;
            tasks[currentTask]->waitingPid = cpustate->ebx;
            // printf("Waiting for PID: ");
            // printfHex32(cpustate->ebx);
            // printf(", ");
        }
        else if (cpustate->eax == 1) // exit
        {
            // printf("Called exit, ");
            tasks[currentTask]->state = TaskState::EXITED;
            tasks[currentTask]->priority = -1;
        }
        else
        {
            tasks[currentTask]->state = TaskState::READY;
        }
        tasks[currentTask]->cpustate = cpustate; // Save the CPU state of the task
    }

    FindNextTask();
    // for (int i = 0; i < 5000000; i++)
    // {
    //     printf("");
    // }
    // printf("New Task PID: ");
    // printfHex32(tasks[currentTask]->GetID());
    // printf("\n");

    return tasks[currentTask]->cpustate;
}

/**
 * Retrieves the current task.
 *
 * @return A pointer to the current task if it exists, otherwise returns nullptr.
 */
Task *TaskManager::GetCurrentTask()
{
    return currentTask >= 0 ? tasks[currentTask] : 0;
}

void myos::TaskManager::SetPriority(int priority)
{
    tasks[currentTask]->SetPriority(priority);
    taskQueue.setPriority(currentTask, priority);
}

int myos::TaskManager::GetNumTasks()
{
    return numTasks;
}

Task *myos::TaskManager::GetTask(int id)
{
    if (id < 0 || id >= numTasks)
        return 0;
    return tasks[id];
}

common::size_t myos::TaskManager::GetClockCounter()
{
    return clockCounter;
}

void myos::TaskManager::WaitEnter()
{
    waitingEnter = true;
}

void myos::TaskManager::SignalEnter()
{
    waitingEnter = false;
}

void myos::TaskManager::HavePriority()
{
    checkPriority = true;
}

void myos::TaskManager::DontHavePriority()
{
    checkPriority = false;
}

/**
 * Forks a new task in the task manager.
 *
 * @param esp The value of the stack pointer of the parent task.
 * @return The ID of the newly created task.
 */
int myos::TaskManager::fork(uint32_t esp)
{
    Task *task = new Task(esp);
    task->priority = tasks[currentTask]->priority;
    AddTask(task);
    return tasks[numTasks - 1]->id;
}
