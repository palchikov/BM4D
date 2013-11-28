#pragma once

#include <deque>
#include <vector>
#include <memory>

#include "opencv2/opencv.hpp"

#include "../settings/Settings.h"
#include "Group.h"

class FrameBuffer
{
public:
	FrameBuffer(cv::VideoCapture& video, const BmSettings& settings);

	void nextFrame();
	bool hasFrame();
	cv::Mat& getCurFrame() { // for testing
		return frameBuffer[curFrameIndex];
	}
	
private:
	cv::Mat readFrame();

	void rotateFrames();

	void constructVolumes();
	void constructVolume(unsigned x, unsigned y);
	std::pair<unsigned, unsigned> findNextBlock(const cv::Mat& frame, const cv::Mat& nextFrame, unsigned refx, unsigned refy, int vx, int vy);

	cv::VideoCapture& video;
	const BmSettings& settings;

	std::deque<cv::Mat> frameBuffer;
	unsigned curFrameIndex;
	unsigned frameWidth;
	unsigned frameHeight;
	
	std::vector<std::vector<std::vector<std::pair<unsigned, unsigned>>>> indexSets;
};
