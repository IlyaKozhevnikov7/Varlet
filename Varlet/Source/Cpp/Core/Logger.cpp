#include "Logger.h"

namespace Varlet
{
	// - Log

	Log::Log(const LevelType& type, const std::string& message)
	{
		this->type = type;
		this->message = message;
	}

	// - Logger

	std::vector<Log> Logger::_buffer;

	void Logger::ConsoleLog(LevelType&& type, std::string&& message)
	{
		const auto newLog = CreateLog(std::move(type), std::move(message));
		_buffer.push_back(newLog);

		const std::string levelTypeStr = LevelTypeToString(std::move(type));

		std::cout << "[LOG] " + message << std::endl;
	}

	Log Logger::CreateLog(LevelType&& type, std::string&& message)
	{
		return Log(type, message);
	}

	std::string Logger::LevelTypeToString(LevelType&& type)
	{
		switch (type)
		{
		case LevelType::Normal:
			return "Normal";

		case LevelType::Warning:
			return "Warning";

		case LevelType::Error:
			return "Error";
		}
	}
}
