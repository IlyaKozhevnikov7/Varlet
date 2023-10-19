#include "Components/LightSource.h"

Event<LightSource*> LightSource::OnLightSourceCreated;
Event<LightSource*> LightSource::OnLightSourceDestroyed;

void LightSource::Start()
{
	OnLightSourceCreated.Invoke(this);
}

void LightSource::OnDestroyed()
{
	OnLightSourceDestroyed.Invoke(this);
}
