// Internal includes
#include "model/scene.h"

namespace noiseless
{
	TScene::TScene(bento::IAllocator& allocator)
	: _geometries(allocator)
	, allocator(allocator)
	{

	}

	void TScene::set_num_meshes(uint32_t numMeshes)
	{
		_geometries.resize(numMeshes);
	}
}