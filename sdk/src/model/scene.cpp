// Internal includes
#include "model/scene.h"

namespace noiseless
{
	TScene::TScene(bento::IAllocator& allocator)
	: _meshes(allocator)
	, allocator(allocator)
	{

	}

	void TScene::set_num_meshes(uint32_t numMeshes)
	{
		_meshes.resize(numMeshes);
	}
}