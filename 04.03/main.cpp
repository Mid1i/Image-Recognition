#include <opencv2/opencv.hpp>
#include <iostream>


// Классификация фигуры
std::string classifyShape(std::vector<cv::Point> contour) {
	double epsilon = 0.01 * cv::arcLength(contour, true);

	std::vector<cv::Point> approx;
	cv::approxPolyDP(contour, approx, epsilon, true);
	
	if (approx.size() == 3) return "Triangle";

	if (approx.size() == 4) {
		cv::Rect rect = cv::boundingRect(approx);
		double aspectRatio = static_cast<double>(rect.width) / rect.height;

		if (aspectRatio >= 0.9 && aspectRatio <= 1.1) return "Square";
		return "Rectangle";
	}

	if (approx.size() < 7) return "Polygon";
	return "Circle";
}


int main() {
  cv::Mat image = cv::imread("image.png");

  if (image.empty()) {
    std::cout << "Ошибка: не удалось загрузить изображение. Проверьте путь к файлу." << std::endl;
    return -1;
  }

	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	cv::Mat edges;
	cv::Canny(gray, edges, 50, 150);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	for (size_t i = 0; i < contours.size(); i++) {
		std::string shape = classifyShape(contours[i]);
		cv::drawContours(image, contours, static_cast<int>(i), cv::Scalar(0, 255, 0), 2);
		cv::Moments m = cv::moments(contours[i]);

		if (m.m00 == 0) continue;

		int cx = static_cast<int>(m.m10 / m.m00);
		int cy = static_cast<int>(m.m01 / m.m00);

		cv::putText(image, shape, cv::Point(cx, cy), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 2);
	}

	cv::imshow("Shapes", image);
  cv::waitKey(0);
	
  return 0;
}
