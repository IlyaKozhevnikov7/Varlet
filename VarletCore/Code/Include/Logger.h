#pragma once

#include "Common.h"
#include <iostream>
#include <vector>

#ifdef VARLET_DEBUG
	#define VARLET_LOG(Verbosity, Format, ...) Varlet::Core::Logger::Get()->AddLog(Verbosity, Format, __VA_ARGS__);
	#define VT_LOG(Verbosity, Format, ...) VARLET_LOG(Verbosity, WIDE(Format), __VA_ARGS__);
	#define VARLET_ASSERT(Expression, Message) assert(Expression && Message);
	#define VARLET_ERROR(Message) assert(false && Message);
#else
	#define VARLET_LOG(LevelType, Message)
	#define VARLET_ASSERT(Expression, Message)
	#define VARLET_ERROR(Message)
#endif // DEBUG

enum LogVerbosity : uint8_t
{
	Normal = 0,
	Warning,
	Error
};

namespace Varlet::Core
{
	struct CORE_API Log final
	{
	public:

		LogVerbosity type;
		std::wstring message;

	public:

		Log(LogVerbosity verbosity, const std::wstring& message);
	};

	class CORE_API Logger final
	{
	private:
		
		constexpr static int32_t MaxLogSize = 256;

		static Logger* _instance;
		
		std::vector<Log> _buffer;

	public:

		static Logger* Get();

		void AddLog(LogVerbosity verbosity, const wchar_t* format, ...);

	private:

		Logger() = default;
	};
}
