#pragma once
#include <Function/Script/NativeCpp/CoreMinimal.h>

ZERO_CLASS(Animal)
{
    ZERO_BODY(Animal)

public:
    ZERO_PROPERTY()
    int age;

    ZERO_FUNCTION()

    void DoWalk()
    {
        LOG_DEBUG(std::format("[{}] I am walking...", __FUNCTION__));
    }
};
