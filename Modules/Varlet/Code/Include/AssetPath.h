#pragma once

#include "VarletCore.h"

class VARLET_API AssetPath final
{
public:

	static std::vector<int8_t> SerializePath(const std::wstring_view path);

	static std::wstring DeserializePath(std::vector<int8_t>& data);
};
