#pragma once

#include "Varlet.h"
#include <iostream>
#include <vector>

#ifdef DEBUG
	#define VARLET_LOG(LevelType, Message) Varlet::Logger::ConsoleLog(LevelType, Message);
#else
	#define VARLET_LOG(LevelType, Message)
#endif // DEBUG

enum class LevelType : uint8_t
{
	Normal = 0,
	Warning,
	Error
};

namespace Varlet
{
	struct CORE_API Log
	{
	public:

		LevelType type;
		std::string message;

	public:

		Log(const LevelType& type, const std::string& message);
	};

	class CORE_API Logger final
	{
	private:

		static std::vector<Log> _buffer;

	public:

		static void ConsoleLog(LevelType&& type, std::string&& message);

	private:

		static Log CreateLog(LevelType&& type, std::string&& message);

		static std::string LevelTypeToString(LevelType&& type);
	};
}
