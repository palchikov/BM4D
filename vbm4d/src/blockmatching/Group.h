#pragma once

#include "Volume.h"

class Group
{
public:
	Group(const Volume& vol);

	//void tryAddVol(const Volume& vol);
	void finish();

private:
	const Volume& refVol;
	//std::vector<std::pair<double, const Volume&>> vols;
};
