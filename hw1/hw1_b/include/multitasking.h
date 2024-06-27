
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>
#include <queue.h>

namespace myos
{
    enum TaskState
    {
        RUNNING,
        READY,
        BLOCKED,
        EXITED
    };

    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;
    } __attribute__((packed));

    class Task
    {
        friend class TaskManager;

    private:
        common::uint8_t stack[4096]; // 4 KiB
        CPUState *cpustate;
        int id;
        int waitingPid;
        TaskState state = TaskState::READY;
        int priority = 0;

    public:
        common::uint32_t locals[64];
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        Task(common::uint32_t esp);
        int GetID();
        void SetPriority(int priority);
        int GetPriority();
        TaskState GetState();
        ~Task();
    };

    class TaskManager
    {
    private:
        Task *tasks[256];
        int numTasks;
        int currentTask;
        void FindNextTask();
        bool waitingEnter = false;
        bool checkPriority = false;
        common::size_t clockCounter = 0;
        PriorityQueue taskQueue;

    public:
        TaskManager();
        ~TaskManager();
        bool AddTask(Task *task);
        CPUState *Schedule(CPUState *cpustate);
        Task *GetCurrentTask();
        int GetNumTasks();
        Task *GetTask(int id);
        common::size_t GetClockCounter();
        void WaitEnter();
        void SignalEnter();
        void HavePriority();
        void DontHavePriority();
        int fork(common::uint32_t esp);
    };
}

#endif