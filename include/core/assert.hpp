#pragma once
#include <iostream>

#ifdef DEBUG
#define ASSERT(condition)                                                      \
    if (!(condition)) {                                                        \
        std::cerr << "Assertion failed: " << #condition                        \
                  << " File: " << __FILE__ << " Line: " << __LINE__            \
                  << std::endl;                                                \
        std::abort();                                                          \
    }
#else
#define ASSERT(condition)
#endif
