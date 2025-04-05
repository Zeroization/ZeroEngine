#pragma once
#include <Function/Script/NativeCpp/CoreMinimal.h>

class StudentBase
{
};

ZERO_CLASS(Student, : public StudentBase)
{
    ZERO_BODY(Student)

private:
    ZERO_PROPERTY(BlueprintEditOnly; Category = 123)
    int mClassID = -1;
    ZERO_PROPERTY()
    bool mGender = false;

public:
    bool operator<(const Student& rhs) const
    {
        return mClassID < rhs.mClassID;
    }

    ZERO_FUNCTION()

    static void staticTest()
    {
        LOG_DEBUG(std::format("Invoke func: {}, STATIC_TEST", __FUNCTION__));
    }

    Student();

    Student(int _classID, bool _gender)
        : mClassID(_classID), mGender(_gender)
    {
        LOG_DEBUG(std::format("Invoke func: {}", __FUNCTION__));
    }

    virtual ~Student() = default;

    ZERO_FUNCTION()
    void constSetID(const int Val);

    ZERO_FUNCTION()
    void pointerSetID(int* pVal);

    ZERO_FUNCTION()
    void SetClassID(int Val);
    ZERO_FUNCTION()
    void SetClassID(int Val, int inc);
    ZERO_FUNCTION()
    void SetClassID(float Val);

    ZERO_FUNCTION()
    void SetGender(bool val);

    ZERO_FUNCTION()
    void SetClassIDAndGender(int _classID, bool _gender);

    ZERO_FUNCTION()
    int GetClassID() const;

    bool GetGender() const;
};
