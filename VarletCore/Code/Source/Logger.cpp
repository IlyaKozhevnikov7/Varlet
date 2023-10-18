#include "Logger.h"
#include "Platform/Current.h"
#include <stdarg.h>

namespace Varlet::Core
{
	Log::Log(LogVerbosity verbosity, const std::wstring& message)
	{
		this->type = type;
		this->message = message;
	}

	Logger* Logger::_instance = nullptr;

	Logger* Logger::Get()
	{
		if (_instance == nullptr)
			_instance = new	Logger;

		return _instance;
	}

	void Logger::AddLog(LogVerbosity verbosity, const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);

		wchar_t buffer[MaxLogSize];
		_vsnwprintf_s(buffer, MaxLogSize, format, args);
		
		_buffer.push_back({ verbosity, buffer });

		constexpr int16_t consoleColors[] = { CC_GRAY, CC_YELLOW, CC_RED };
		Platform::SetConsoleColor(consoleColors[verbosity]);

		std::wcout << "[Log] " << buffer << '\n';

		va_end(args);
	}
}
