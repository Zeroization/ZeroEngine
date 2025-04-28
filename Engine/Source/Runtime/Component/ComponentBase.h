#pragma once
#include "pch.h"
#include "Core/Reflection/ReflectionManager.h"

namespace ZeroEngine
{
    class GameObject;
    class Transform;

    ZERO_INTERNAL_CLASS(ComponentBase)
    {
        ZERO_BODY(ComponentBase)

    public:
        static ComponentType GetType();
        static bool IsUnique(ComponentType type);

        ComponentBase()
        {
        }

        virtual ~ComponentBase()
        {
        }

        virtual void Awake();
        virtual ComponentType GetInsType() = 0;

        bool IsActive() const;
        Transform* GetTransform() const;

    public:
        ZERO_PROPERTY()
        bool mIsEnabled = true;

        GameObject* mGameObject = nullptr;

    protected:
        bool mIsAwake = false;
    };
} // ZeroEngine
