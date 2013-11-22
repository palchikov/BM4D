#include <cassert>

#include "Block.h"

Block::Block(unsigned x, unsigned y, unsigned size, const cv::Mat& frame)
: x(x), y(y), size(size)
, data(frame, cv::Rect(x, y, size, size)) // doesn't copies data; checks bounds
{
	assert(data.type() == CV_32F);
}

double Block::sqrDiff(const Block& that) const
{
	assert(size == that.size);

	double res = cv::norm(data, that.data, cv::NORM_L2);
	return res * res;
}

double Block::normSqrDiff(const Block& that) const
{
	return sqrDiff(that) / size / size;
}
