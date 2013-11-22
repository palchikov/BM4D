#include <utility>
#include <cmath>

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(cv::VideoCapture& video, Settings& settings)
: video(video)
, settings(settings)
, prevFrames()
, nextFrames()
, frameWidth(video.get(CV_CAP_PROP_FRAME_WIDTH))
, frameHeight(video.get(CV_CAP_PROP_FRAME_HEIGHT))
, volumes(volArrHeight(), std::vector<std::shared_ptr<Volume>>(volArrWidth()))
{
	curFrame = readFrame();

	for (unsigned i = 0; i < settings.getHtExtent(); ++i) {
		cv::Mat frame = readFrame();
		if (frame.empty()) break;

		nextFrames.push_back(frame);
	}
}

unsigned FrameBuffer::volArrWidth()
{
	unsigned res = (frameWidth - settings.getHtBlockSize()) / settings.getHtNStep();
	unsigned rem = (frameWidth - settings.getHtBlockSize()) % settings.getHtNStep();

	return rem == 0 ? res : res + 1;
}

unsigned FrameBuffer::volArrHeight()
{
	unsigned res = (frameHeight - settings.getHtBlockSize()) / settings.getHtNStep();
	unsigned rem = (frameHeight - settings.getHtBlockSize()) % settings.getHtNStep();

	return rem == 0 ? res : res + 1;
}

bool FrameBuffer::hasFrame()
{
	return !curFrame.empty();
}

void FrameBuffer::nextFrame()
{
	// construct volumes for current frame
	constructVolumes();

	// read next frame from video stream
	rotateFrames();
}

void FrameBuffer::constructVolumes()
{
	for (unsigned i = 0; i < volumes.size(); ++i) {
		unsigned y = i * settings.getHtNStep();
		if (i == volumes.size() - 1) {
			y = frameHeight - settings.getHtBlockSize();
		}

		for (unsigned j = 0; j < volumes[0].size(); ++j) {
			unsigned x = j * settings.getHtNStep();
			if (j == volumes[0].size() - 1) {
				x = frameWidth - settings.getHtBlockSize();
			}

			volumes[i][j] = constructVolume(x, y);
		}
	}
}

std::shared_ptr<Volume> FrameBuffer::constructVolume(unsigned x, unsigned y)
{
	auto refBlock = std::make_shared<Block>(x, y, settings.getHtBlockSize(), curFrame);
	auto res = std::make_shared<Volume>(refBlock);

	int vx = 0;
	int vy = 0;
	auto current = refBlock;
	for (unsigned i = 0; i < nextFrames.size(); ++i) {
		auto next = findNextBlock(nextFrames.at(i), current, vx, vy);
		if (next) {
			res->addForward(next);
			vx = next->getX() - current->getX();
			vy = next->getY() - current->getY();
			current = next;
		} else {
			break;
		}
	}

	vx = 0;
	vy = 0;
	current = refBlock;
	for (unsigned i = 0; i < prevFrames.size(); ++i) {
		auto next = findNextBlock(prevFrames.at(i), current, vx, vy);
		if (next) {
			res->addBackward(next);
			vx = next->getX() - current->getX();
			vy = next->getY() - current->getY();
			current = next;
		} else {
			break;
		}
	}

	return res;
}

std::shared_ptr<Block> FrameBuffer::findNextBlock(const cv::Mat& frame, std::shared_ptr<Block> ref, int vx, int vy)
{
	// search window
	double xc = settings.getHtGammaP() * vx + ref->getX();
	double yc = settings.getHtGammaP() * vy + ref->getY();
	double sigmaw = settings.getHtSigmaW();
	double Npr = settings.getHtNs() *
	             (1 - settings.getHtGammaW() * exp(
		     -(vx * vx + vy * vy) / 2 / sigmaw / sigmaw)) / 2;

	unsigned xl = static_cast<unsigned>(round(std::max(0.0, xc - Npr)));
	unsigned yl = static_cast<unsigned>(round(std::max(0.0, yc - Npr)));
	unsigned xh = static_cast<unsigned>(round(std::min<double>(frameWidth - settings.getHtBlockSize(), xc + Npr)));
	unsigned yh = static_cast<unsigned>(round(std::min<double>(frameHeight - settings.getHtBlockSize(), yc + Npr)));

	std::shared_ptr<Block> res;
	double min = settings.getHtTauTraj();

	for (unsigned y = yl; y <= yh; ++y) {
		for (unsigned x = xl; x <= xh; ++x) {
			int dx = ref->getX() - x;
			int dy = ref->getY() - y;
			double dist = sqrt(dx * dx + dy * dy);

			auto tmp = std::make_shared<Block>(x, y, settings.getHtBlockSize(), frame);
			
			double deltab = ref->normSqrDiff(*tmp) +
			                settings.getHtGammaD() * dist;

			if (deltab < min) {
				min = deltab;
				res = tmp;
			}
		}
	}

	return res;
}

// TODO: optimize and remove redundant copying of curFrame into prevFrames
// (should be moved instead of copied)
void FrameBuffer::rotateFrames()
{
	if (prevFrames.size() == settings.getHtExtent()) {
		prevFrames.pop_back();
	}

	if (!curFrame.empty()) {
		prevFrames.push_front(curFrame);
	}

	if (!nextFrames.empty()) {
		std::swap(curFrame, nextFrames.front());
		nextFrames.pop_front();

		cv::Mat frame = readFrame();
		if (!frame.empty()) {
			nextFrames.push_back(frame);
		}
	} else {
		curFrame.release();
	}
}

Block FrameBuffer::getBlock(unsigned x, unsigned y, int dt)
{
	if (dt == 0) {
		return Block(x, y, settings.getHtBlockSize(), curFrame);
	} else if (dt > 0) {
		return Block(x, y, settings.getHtBlockSize(), nextFrames.at(dt - 1));
	} else {
		return Block(x, y, settings.getHtBlockSize(), prevFrames.at(-dt - 1));
	}
}

cv::Mat FrameBuffer::readFrame() {
	cv::Mat res;
	video >> res;
	if (!res.empty()) {
		cv::cvtColor(res, res, CV_BGR2GRAY);
		res.convertTo(res, CV_32F, 1.0/256);
	}

	return res;
}
