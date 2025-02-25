#include <opencv2/opencv.hpp>
#include <iostream>


int main() {
  cv::Mat image = cv::imread("image.jpg");

  if (image.empty()) {
    std::cout << "Ошибка: не удалось загрузить изображение. Проверьте путь к файлу." << std::endl;
    return -1;
  }

	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
  
	cv::Mat mask;
	cv::inRange(hsv, cv::Scalar(100, 100, 50), cv::Scalar(140, 255, 255), mask);

	cv::Mat blue;
	cv::bitwise_and(image, image, blue, mask);

	cv::imshow("Original", image);
	cv::imshow("Mask", mask);
	cv::imshow("Blue", blue);

  cv::waitKey(0);
	cv::destroyAllWindows();

  return 0;
}
