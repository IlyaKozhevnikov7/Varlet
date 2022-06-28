#include "InputModule.h"
#include "Input.h"

namespace Varlet
{
    int32_t InputModule::Init()
    {
        return Input::Init(this);
    }
}
