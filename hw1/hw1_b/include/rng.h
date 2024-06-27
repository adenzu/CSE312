#ifndef __MYOS__RNG_H
#define __MYOS__RNG_H

#include <common/types.h>

namespace myos
{
    class RandomNumberGenerator
    {
    private:
        common::uint32_t seed;
        common::uint32_t memUpper;
        common::uint32_t memoryManagerAddr;

    public:
        RandomNumberGenerator(common::uint32_t memUpperAddr, common::uint32_t memoryManagerAddr);
        ~RandomNumberGenerator();

        int NextInt();
    };
}

#endif // __MYOS__RNG_H