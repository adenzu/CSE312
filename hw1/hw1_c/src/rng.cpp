#include <rng.h>

using namespace myos;
using namespace myos::common;

myos::RandomNumberGenerator::RandomNumberGenerator(uint32_t memUpperAddr, uint32_t memoryManagerAddr)
    : memUpper(memUpperAddr), memoryManagerAddr(memoryManagerAddr)
{
    seed = ((uint32_t)memUpper) ^ (uint32_t)memoryManagerAddr;
}

myos::RandomNumberGenerator::~RandomNumberGenerator()
{
}

int myos::RandomNumberGenerator::NextInt()
{
    seed = seed * 1103515245 + 12345;
    return seed;
}
