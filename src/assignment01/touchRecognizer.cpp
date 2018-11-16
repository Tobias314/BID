#include "touchRecognizer.hpp"

#include <algorithm>

void TouchRecognizer::calibrate(const cv::Mat& background)
{
    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/
	m_background = background.clone();
	m_calibrated = true;
	std::cout << "TouchRecognizer calibrated" << std::endl;
}

std::vector<cv::RotatedRect> TouchRecognizer::recognize(const cv::Mat& depthFrame) const
{
	if (!m_calibrated) {
		return std::vector<cv::RotatedRect>();
	}
    std::vector<cv::RotatedRect> positions;

	cv::Mat difference;
	cv::Mat thresholdHigh;
	cv::Mat thresholdLow;
	cv::Mat blur;
	cv::Mat lowPassFiltered;

	std::vector<std::vector<cv::Point>> contours;

	cv::absdiff(depthFrame, m_background, difference);
											//1600
	cv::threshold(difference, thresholdHigh, 1600, UINT16_MAX, 4);
											//900
	cv::threshold(thresholdHigh, thresholdLow, 450, UINT16_MAX, 0); //TODO
	thresholdLow.convertTo(thresholdLow, CV_8UC1);
	cv::blur(thresholdLow, blur, cv::Size(30, 30));// (thresholdLow, blur, cv::Size(61, 61));#
	//Maybe do open or close (look it up in opencv doku)
	cv::threshold(thresholdLow, lowPassFiltered, 254, UINT8_MAX, 0);

	cv::findContours(lowPassFiltered, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cv::imshow("depth", thresholdLow);
	for (std::vector<cv::Point> contour : contours) {
		if (cv::contourArea(contour) < 500)
			continue;
		cv::RotatedRect rect = cv::fitEllipse(contour);
		positions.push_back(rect);
		//std::cout << "detected foot press at: x" << rect.center.x << "y" << rect.center.y << std::endl;
	}
    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/
	for (auto pos : positions) {
		std::cout << pos.center << " ";
	}
	std::cout << std::endl;

    return positions;
}
