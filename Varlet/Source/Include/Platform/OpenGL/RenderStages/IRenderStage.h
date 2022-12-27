#pragma once 

namespace Varlet::OpenGL
{
	class IRenderStage
	{
	public:

		bool isActive = true;

	public:

		virtual ~IRenderStage() = default;

		virtual void Init() = 0;
		
		virtual void Shutdown() = 0;

		virtual void Update() = 0;
	};
}
