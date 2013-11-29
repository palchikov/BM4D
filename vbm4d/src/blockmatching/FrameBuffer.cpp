#include <utility>
#include <cmath>

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(cv::VideoCapture& video, const BmSettings& settings)
: video(video)
, settings(settings)
, frameBuffer()
, curFrameIndex(0)
, frameWidth(video.get(CV_CAP_PROP_FRAME_WIDTH))
, frameHeight(video.get(CV_CAP_PROP_FRAME_HEIGHT))
, indexSets(frameHeight - settings.getBlockSize() + 1,
            std::vector<std::vector<std::pair<unsigned, unsigned>>>(frameWidth - settings.getBlockSize() + 1,
            std::vector<std::pair<unsigned, unsigned>>(2 * settings.getExtent() + 1,
            std::pair<unsigned, unsigned>(-1, -1))))
{
	cv::Mat frame = readFrame();
	if (!frame.empty()) {
		frameBuffer.push_back(frame);

		for (unsigned i = 0; i < settings.getExtent(); ++i) {
			frame = readFrame();
			if (frame.empty()) break;

			frameBuffer.push_back(frame);
		}
	}
}

bool FrameBuffer::hasFrame()
{
	return curFrameIndex < frameBuffer.size();
}

void FrameBuffer::nextFrame()
{
	// construct volumes for current frame
	constructVolumes();

	// read the next frame from video stream
	rotateFrames();
}

void FrameBuffer::constructVolumes()
{
	// clear data from prev frame
	for (auto& row : indexSets) {
		for (auto& col : row) {
			for (auto& block : col) {
				block.first = -1;
				block.second = -1;
			}
		}
	}

	auto rows = indexSets.size();
	auto cols = indexSets[0].size();
	for (unsigned y = 0; y < rows; ++y) {
		for (unsigned x = 0; x < cols; ++x) {
			constructVolume(x, y);
		}
	}
}

void FrameBuffer::constructVolume(unsigned x, unsigned y)
{
	auto h = settings.getExtent();
	indexSets[y][x][h].first = x;
	indexSets[y][x][h].second = y;

	unsigned cx = x;
	unsigned cy = y;
	int vx = 0;
	int vy = 0;
	for (unsigned i = curFrameIndex + 1; i < frameBuffer.size(); ++i) {
		auto& frame = frameBuffer[i - 1];
		auto& nextFrame = frameBuffer[i];
		auto nextBlock = findNextBlock(frame, nextFrame, cx, cy, vx, vy);
		if (nextBlock.first != (unsigned)-1 && nextBlock.second != (unsigned)-1) {
			indexSets[y][x][h - curFrameIndex + i] = nextBlock;
			vx = nextBlock.first - cx;
			vy = nextBlock.second - cy;
			cx = nextBlock.first;
			cy = nextBlock.second;
		} else {
			break;
		}
	}

	cx = x;
	cy = y;
	vx = 0;
	vy = 0;
	for (unsigned i = curFrameIndex; i != 0; --i) {
		auto& frame = frameBuffer[i];
		auto& nextFrame = frameBuffer[i - 1];
		auto nextBlock = findNextBlock(frame, nextFrame, cx, cy, vx, vy);
		if (nextBlock.first != (unsigned)-1 && nextBlock.second != (unsigned)-1) {
			indexSets[y][x][h - curFrameIndex + i - 1] = nextBlock;
			vx = nextBlock.first - cx;
			vy = nextBlock.second - cy;
			cx = nextBlock.first;
			cy = nextBlock.second;
		} else {
			break;
		}
	}
}

std::pair<unsigned, unsigned> FrameBuffer::findNextBlock(const cv::Mat& frame, const cv::Mat& nextFrame, unsigned refx, unsigned refy, int vx, int vy)
{
	// search window
	double xc = refx + settings.getGammaP() * vx;
	double yc = refy + settings.getGammaP() * vy;
	double sigmaw = settings.getSigmaW();
	double Npr = settings.getNs() *
	             (1 - settings.getGammaW() * exp(
		     -(vx * vx + vy * vy) / 2 / sigmaw / sigmaw)) / 2;

	auto xl = static_cast<unsigned>(round(std::max(0.0, xc - Npr / 2)));
	auto yl = static_cast<unsigned>(round(std::max(0.0, yc - Npr / 2)));
	auto xh = static_cast<unsigned>(round(std::min<double>(frameWidth - settings.getBlockSize(), xc + Npr / 2)));
	auto yh = static_cast<unsigned>(round(std::min<double>(frameHeight - settings.getBlockSize(), yc + Npr / 2)));

	auto N = settings.getBlockSize();
	cv::Mat block(frame, cv::Rect(refx, refy, N, N));

	unsigned xres = -1;
	unsigned yres = -1;
	auto min = settings.getTauTraj();

	for (unsigned y = yl; y <= yh; ++y) {
		for (unsigned x = xl; x <= xh; ++x) {
			int dx = refx - x;
			int dy = refy - y;
			double dist = sqrt(dx * dx + dy * dy);

			cv::Mat nextBlock(nextFrame, cv::Rect(x, y, N, N));
			double norm = cv::norm(block, nextBlock, cv::NORM_L2);
			double deltab = norm * norm / N / N +
			                settings.getGammaD() * dist;

			if (deltab < min) {
				min = deltab;
				xres = x;
				yres = y;
			}
		}
	}

	return std::make_pair(xres, yres);
}

void FrameBuffer::rotateFrames()
{
	if (curFrameIndex >= settings.getExtent()) {
		frameBuffer.pop_front();
	} else {
		++curFrameIndex;
	}

	cv::Mat frame = readFrame();
	if (!frame.empty()) {
		frameBuffer.push_back(frame);
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
