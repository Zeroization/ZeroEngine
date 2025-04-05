#pragma once
#include <Function/Script/NativeCpp/CoreMinimal.h>

// 用于反射的宏
#ifdef __REFLECTION_PARSER__
#define ZERO_CLASS_API __attribute__((annotate("reflect-class;")))
#define ZERO_PROPERTY(...) __attribute__((annotate("reflect-property; " #__VA_ARGS__)))
#define ZERO_FUNCTION(...) __attribute__((annotate("reflect-function; " #__VA_ARGS__)))
#else
#define ZERO_CLASS_API
#define ZERO_PROPERTY(...)
#define ZERO_FUNCTION(...)
#endif

class ZERO_CLASS_API Animal
{
public:
    ZERO_PROPERTY()
    int age;

    ZERO_FUNCTION()
    void Fuck();
};
