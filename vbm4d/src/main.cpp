#include <iostream>
#include "settings/Settings.h"
#include "blockmatching/FrameBuffer.h"

//#include "opencv2/highgui/highgui.hpp"

int main(int argc, char** argv)
{
	try {
		// parse cmdline options and store the settings
		Settings settings(argc, argv);

		// open input video file
		cv::VideoCapture videoCapture(settings.getInputFile());
		if (!videoCapture.isOpened()) {
			std::cerr << "Cannot open input video file " << settings.getInputFile() << std::endl;
			return 1;
		}

		FrameBuffer frames(videoCapture, settings);
		while (frames.hasFrame()) {
			cv::Mat frame = frames.getCurFrame();
			cv::imshow("slonik", frame);
			frames.nextFrame();
			//frames.displayVol();

			char c = cv::waitKey(1);
			if (c == 27) break;
			if (c == 32) {
				for (;;) {
					char c2 = cv::waitKey();
					if (c2 == 32) break;
				}
			}
		}

	} catch (const Settings::BadArgs& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
