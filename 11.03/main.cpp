#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


int main() {
	cv::VideoCapture cap("C:/Programming/C++/Image-Recognition/11.03/src/video.mp4");

	if (!cap.isOpened()) {
		std::cout << "Ошибка при открытии видео!" << std::endl;
		return -1;
	}

	cv::namedWindow("Object Tracking", cv::WINDOW_NORMAL);

	cv::Mat frame, gray, blurred, edges;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	while (true) {
		cap >> frame;
		if (frame.empty()) break;

		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		
		cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

		cv::Canny(blurred, edges, 50, 150, 3);

		cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		cv::Mat result = frame.clone();
		for (const auto& contour : contours) {
			double perimeter = cv::arcLength(contour, true);
			std::vector<cv::Point> approx;
			approxPolyDP(contour, approx, 0.02 * perimeter, true);

			if (approx.size() == 4 && cv::isContourConvex(approx) && perimeter < 900.0) {
				drawContours(result, std::vector<std::vector<cv::Point>>{approx}, -1, cv::Scalar(0, 255, 0), 2);
				putText(result, "square", approx[0], cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
			}
		}

		cv::imshow("Object Tracking", result);

		if (cv::waitKey(1) == 27) break;
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}
