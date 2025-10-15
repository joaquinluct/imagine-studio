#pragma once

#include <cstdlib>
#include <iostream>
#ifndef NDEBUG
#define CORE_ASSERT(cond, msg)                                   \
    do {                                                         \
        if (!(cond)) {                                            \
            std::cerr << "Assertion failed: " << (msg) << "\n"; \
            std::abort();                                         \
        }                                                        \
    } while (0)
#else
#define CORE_ASSERT(cond, msg) do { (void)sizeof(cond); } while(0)
#endif
