#pragma once

#include <vector>

#include "opencv2/opencv.hpp"

// cv::Mat assumed to contain CV_32F

class Block
{
public:
	Block(unsigned x, unsigned y, unsigned size, const cv::Mat& frame); // throw(cv::Exeption);

	const cv::Mat& getData() const {
		return data;
	}

	unsigned getX() const {
		return x;
	}

	unsigned getY() const {
		return y;
	}

	double sqrDiff(const Block& that) const;
	double normSqrDiff(const Block& that) const;

private:
	unsigned x;
	unsigned y;
	unsigned size;

	const cv::Mat data;
};
