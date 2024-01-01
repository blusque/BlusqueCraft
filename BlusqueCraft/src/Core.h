#pragma once

#ifdef _DEBUG
    #define ASSERT(x) if (!(x)) { __debugbreak(); }
#else
    #define ASSERT(x) (x)
#endif

#define BIT(x) (1 << (x))

#define to(type, v) static_cast<type>(v)