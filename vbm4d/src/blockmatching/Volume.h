#pragma once

#include <vector>
#include <memory>
#include <string>
#include <sstream>

#include "Block.h"

class Volume
{
public:
	Volume(std::shared_ptr<Block> block);
	Volume(const Volume&) = default;


	void display() { // for testing
		std::string name = "frame ";
		int s = 0;
		for (unsigned i = 0; i < hminus; ++i) {
			++s;
			std::stringstream ss;
			ss << name << s;
			cv::imshow(ss.str(), backward[hminus - i - 1]->getData());
		}
		++s;
		std::stringstream ss;
		ss << name << s;
		cv::imshow(ss.str(), block->getData());
		for (unsigned i = 0; i < hplus; ++i) {
			++s;
			std::stringstream ss;
			ss << name << s;
			cv::imshow(ss.str(), forward[i]->getData());
		}
	}

	void addForward(std::shared_ptr<Block> block);
	void addBackward(std::shared_ptr<Block> block);

	unsigned getForwardExtent() const {
		return hplus;
	}

	unsigned getBackwardExtent() const {
		return hminus;
	}

	Volume subVolume(unsigned hmin, unsigned hpl);

	double normSqrDiff(const Volume& that) const;

private:
	std::shared_ptr<Block> block;
	std::vector<std::shared_ptr<Block>> forward;
	std::vector<std::shared_ptr<Block>> backward;
	unsigned hminus = 0;
	unsigned hplus = 0;
};
