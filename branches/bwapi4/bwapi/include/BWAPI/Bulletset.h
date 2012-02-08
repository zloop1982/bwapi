#pragma once
#include <BWAPI/Vectorset.h>

namespace BWAPI
{
	class Bullet;
	class Bulletset : public Vectorset<BWAPI::Bullet*>
	{
	public:
		Bulletset(size_t initialSize = 16);
		Bulletset(const Bulletset &other);
	};
}
