#include "AssetPath.h"
#include "ModuleService.h"

std::vector<int8_t> AssetPath::SerializePath(const std::wstring_view path)
{
	std::vector<int8_t> serialized;

	auto position = path.find(WIDE("Assets"));
	int32_t id = -1;

	if (position != std::wstring::npos)
	{
		const std::wstring_view moduleDirectory(path.data(), position - 1);
		id = Varlet::Core::ModuleService::GetModuleIdByPath(moduleDirectory);
	}

	const uint32_t sizeInBytes = id == -1
		? path.size()
		: path.size() - position;

	const int8_t* idData = std::bit_cast<const int8_t*>(&id);
	serialized.insert(serialized.end(), idData, idData + sizeof(int32_t));

	const int8_t* sizeData = std::bit_cast<const int8_t*>(&sizeInBytes);
	serialized.insert(serialized.end(), sizeData, sizeData + sizeof(int32_t));

	const int8_t* patheBegin = (const int8_t*)(path.data() + position);
	const int8_t* pathEnd = patheBegin + sizeInBytes * sizeof(wchar_t);

	serialized.insert(serialized.end(), patheBegin, pathEnd);

	return serialized;
}

std::wstring AssetPath::DeserializePath(std::vector<int8_t>& data)
{
	uint32_t offset = 0;

	const int32_t moduleId = static_cast<int32_t>(data[offset]);
	offset += sizeof(int32_t);

	const uint32_t pathSize = static_cast<uint32_t>(data[offset]);
	offset += sizeof(uint32_t);

	std::wstring path;

	if (moduleId != -1)
		path = Varlet::Core::ModuleService::GetModulePathById(moduleId) + WIDE("\\");

	path.append(pathSize, WIDE('\0'));
	memcpy(&path[path.size() - pathSize], &data[offset], pathSize * sizeof(wchar_t));

	return path;
}
