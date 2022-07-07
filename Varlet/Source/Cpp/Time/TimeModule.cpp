#include "TimeModule.h"
#include "VarletTime.h"

namespace Varlet
{
    int32_t TimeModule::Init()
    {
        return Time::Init(this);
    }
}
