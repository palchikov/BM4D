#include <cassert>

#include "Volume.h"

Volume::Volume(std::shared_ptr<Block> block)
: block(block)
, forward()
, backward()
{}

void Volume::addForward(std::shared_ptr<Block> block)
{
	forward.push_back(block);
	++hplus;
}

void Volume::addBackward(std::shared_ptr<Block> block)
{
	backward.push_back(block);
	++hminus;
}

/*Volume Volume::subVolume(unsigned hmin, unsigned hpl)
{
	assert(hmin <= hminus && hpl <= hplus);

	Voulme res(*this);
	if (hmin < hminus) {
		res.backward.resize(hmin);
		res.hminus = hmin;
	}
	if (hpl < hplus) {
		res.forward.resize(hpl);
		res.hplus = hpl;
	}

	return res;
}

double Volume::normSqrDiff(const Volume& that) const
{
	assert(hminus == that.hminus && hplus == that.hplus);

	double res = 0.0;

	for (unsigned i = 0; i < hminus; ++i) {
		res += backward[i].sqrDiff(that.backward[i]);
	}
	res += block.sqrDiff(that.block);
	for (unsigned i = 0; i < hplus; ++i) {
		res += forward[i].sqrDiff(that.forward[i]);
	}

	return res / (1 + hminus + hplus);
}*/
