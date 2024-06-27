
#include <syscalls.h>

using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;

SyscallHandler::SyscallHandler(InterruptManager *interruptManager, uint8_t InterruptNumber)
    : InterruptHandler(interruptManager, InterruptNumber + interruptManager->HardwareInterruptOffset())
{
}

SyscallHandler::~SyscallHandler()
{
}

void printf(char *);
void printfHex32(uint32_t);
int __fork(uint32_t);

/**
 * Handles the interrupt for system calls.
 *
 * This function takes the CPU state as input and handles the system call interrupt
 * based on the value of the EAX register in the CPU state.
 *
 * @param esp The stack pointer value (ESP) representing the CPU state.
 * @return The updated stack pointer value after handling the interrupt.
 */
uint32_t SyscallHandler::HandleInterrupt(uint32_t esp)
{
    CPUState *cpu = (CPUState *)esp;

    switch (cpu->eax)
    {
    case 2:
        cpu->eax = (uint32_t)__fork(esp);
        break;
    case 4:
        printf((char *)cpu->ebx);
        break;
    default:
        break;
    }

    return esp;
}
