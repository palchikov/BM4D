#pragma once

#include "Volume.h"

class Group
{
public:
	Group(std::shared_ptr<Volume> vol);

	void tryAddVol(std::shared_ptr<Volume> vol);
	void finish();

private:
	const Volume& refVol;
	std::vector<std::shared_ptr<Volume>> vols;
};
