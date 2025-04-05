#include "Student.h"

Student::Student()
{
    LOG_DEBUG(std::format("Invoke func: {}", __FUNCTION__));
}

void Student::constSetID(const int Val)
{
    LOG_DEBUG(std::format("Invoke func: {}, overload const int val, set to {}", __FUNCTION__, Val));
    mClassID = Val;
}

void Student::pointerSetID(int* pVal)
{
    LOG_DEBUG(std::format("Invoke func: {}, overload int* val, set to {}", __FUNCTION__, *pVal));

    mClassID = *pVal;
}

void Student::SetClassID(int Val)
{
    LOG_DEBUG(std::format("Invoke func: {}, set to {}", __FUNCTION__, Val));

    mClassID = Val;
}

void Student::SetClassID(int Val, int inc)
{
    LOG_DEBUG(std::format("Invoke func: {}, overload int int, set to {}", __FUNCTION__, Val + inc));

    mClassID = Val + inc;
}

void Student::SetClassID(float Val)
{
    LOG_DEBUG(std::format("Invoke func: {}, overload float, set to {}", __FUNCTION__, static_cast<int>(Val)));

    mClassID = static_cast<int>(Val);
}

void Student::SetGender(bool val)
{
    LOG_DEBUG(std::format("Invoke func: {}", __FUNCTION__));
    mGender = val;
}

void Student::SetClassIDAndGender(int _classID, bool _gender)
{
    LOG_DEBUG(std::format("Invoke func: {}", __FUNCTION__));
    mClassID = _classID;
    mGender = _gender;
}

int Student::GetClassID() const
{
    LOG_DEBUG(std::format("Invoke func: {}, should return {}", __FUNCTION__, mClassID));
    return mClassID;
}

bool Student::GetGender() const
{
    LOG_DEBUG(std::format("Invoke func: {}", __FUNCTION__));
    return mGender;
}
