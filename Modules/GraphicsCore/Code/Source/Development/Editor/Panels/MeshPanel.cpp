#include "Development/Editor/Panels/MeshPanel.h"
#include "Assets/Mesh.h"
#include "ProjectAssetRegister.h"
#include "Platform/Current.h"

void MeshInitializationPanel::Update()
{
	const std::wstring nativeMeshPath = Varlet::Core::Platform::CreateFileDialog(WIDE("*.obj;*.fbx"));
	
	const bool isSuccessful = nativeMeshPath.empty() == false;
	*_isSuccessful = isSuccessful;
	_isOpen = false;

	if (isSuccessful == false)
		return;

	if (const auto newMesh = Mesh::LoadModel(nativeMeshPath.c_str()))
	{
		const int32_t dotPosition = nativeMeshPath.find_first_of(WIDE('.'));

		std::wstring assetPath(nativeMeshPath.c_str(), nativeMeshPath.c_str() + dotPosition);
		assetPath += WIDE(".vasset");

		ProjectAssetRegister::AddAsset(assetPath, newMesh);
	}
}
