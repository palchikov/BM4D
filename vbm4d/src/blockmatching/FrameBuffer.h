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
		return curFrame;
	}
	
	void displayVol() { // for testing
		volumes[20][0]->display();
	}

private:
	cv::Mat readFrame();
	unsigned volArrWidth();
	unsigned volArrHeight();

	void rotateFrames();

	void constructVolumes();
	std::shared_ptr<Volume> constructVolume(unsigned x, unsigned y);
	std::shared_ptr<Block> findNextBlock(const cv::Mat& frame, std::shared_ptr<Block> ref, int vx, int vy);

	cv::VideoCapture& video;
	const BmSettings& settings;

	cv::Mat curFrame;
	std::deque<cv::Mat> prevFrames;
	std::deque<cv::Mat> nextFrames;
	unsigned frameWidth;
	unsigned frameHeight;
	
	std::vector<std::vector<std::shared_ptr<Volume>>> volumes;
};
