
namespace noiseless
{
	IVector3 ivector3(const uint32_t x,const uint32_t y, const uint32_t z)
	{
		IVector3 result = {x, y, z};
		return result;
	}

	IVector3 ivector3(const uint32_t v)
	{
		IVector3 result = {v, v, v};
		return result;
	}

		// Index access
	uint32_t& at_index(IVector3& val, uint32_t _index)
	{
		return *(&val.x + _index);
	}

	const uint32_t& at_index(const IVector3& val, uint32_t _index)
	{
		return *(&val.x + _index);
	}
}