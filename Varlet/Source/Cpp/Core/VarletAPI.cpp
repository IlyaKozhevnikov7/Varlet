#include "VarletAPI.h"
#include "Engine.h"

void* VarletAPI::GetContextWindow()
{
	return Varlet::Engine::Get()->GetContext()->GetWindow();
}
