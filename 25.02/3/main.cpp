#include <opencv2/opencv.hpp>
#include <iostream>


int main() {
  cv::Mat image = cv::imread("stop.png");

  if (image.empty()) {
    std::cout << "Ошибка: не удалось загрузить изображение. Проверьте путь к файлу." << std::endl;
    return -1;
  }

	// ---Перевод в hsv---
	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

	// ---Определение красного цвета на картинке---
	cv::Mat mask, mask1, mask2, red;
	cv::inRange(hsv, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask1);
	cv::inRange(hsv, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), mask2);

	cv::bitwise_or(mask1, mask2, mask);
	cv::bitwise_and(image, image, red, mask);

	// ---Перевод в серый формат---
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// ---Размытие для уменьшения шумов---
	cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2);

	// ---Определение кругов---
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, gray.rows, 55, 45, 300, 380);

	for (size_t i = 0; i < circles.size(); i++) {
		cv::Vec3i c = circles[i];
		cv::Point center(c[0], c[1]);
		int radius = c[2];

		cv::circle(red, center, 3, cv::Scalar(0, 255, 0), -1, cv::LINE_AA);
		cv::circle(red, center, radius, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
	}

	cv::imshow("Original", image);
	cv::imshow("Mask", mask);
	cv::imshow("Red", red);

  cv::waitKey(0);
	cv::destroyAllWindows();

  return 0;
}
