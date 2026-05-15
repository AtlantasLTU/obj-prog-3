#ifndef VECTOR_COMPARE_H
#define VECTOR_COMPARE_H

#ifdef _WIN32
    #ifdef BUILD_MYDLL
        #define DLL_API __declspec(dllexport)
    #else
        #define DLL_API __declspec(dllimport)
    #endif
#else
    #define DLL_API
#endif

#include "vector.h"
#include <vector>
#include "Timer.h"
#include <iostream>

/** @brief Palygina STL vektorių su nuosavu vektoriu. */
    DLL_API void vector_compare();

#endif