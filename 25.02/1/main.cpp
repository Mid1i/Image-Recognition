#include <opencv2/opencv.hpp>
#include <iostream>


int main() {
  cv::Mat image = cv::imread("image.jpg");

  if (image.empty()) {
    std::cout << "Ошибка: не удалось загрузить изображение. Проверьте путь к файлу." << std::endl;
    return -1;
  }

	// ---Перевод в другие цветовые пространства и фильтр Гаусса---
	cv::Mat hsv, lab, yuv, xyz, gray;

	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
	cv::cvtColor(image, lab, cv::COLOR_BGR2Lab);
	cv::cvtColor(image, yuv, cv::COLOR_BGR2YUV);
	cv::cvtColor(image, xyz, cv::COLOR_BGR2XYZ);
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	cv::Mat gaus;
	cv::GaussianBlur(gray, gaus, cv::Size(9, 9), 2, 2);

	cv::imshow("Original", image);
	cv::imshow("HSV", hsv);
	cv::imshow("LAB", lab);
	cv::imshow("YUV", yuv);
	cv::imshow("XYZ", xyz);
	cv::imshow("Grayscale", gray);
	cv::imshow("Gaus", gaus);


	// ---Определение кругов и линий---
	cv::Mat imageCircles = image.clone();
	cv::Mat imageLines = image.clone(); 

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(gaus, circles, cv::HOUGH_GRADIENT, 1, gaus.rows / 8, 50, 50, 20, 200);

	for (size_t i = 0; i < circles.size(); i++) {
		cv::Vec3i c = circles[i];
		cv::Point center(c[0], c[1]);
		int radius = c[2];

		cv::circle(imageCircles, center, 3, cv::Scalar(0, 255, 0), -1, cv::LINE_AA);
		cv::circle(imageCircles, center, radius, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
	}

	cv::Mat edges;
	cv::Canny(gaus, edges, 50, 150, 3);

	std::vector<cv::Vec2f> lines;
	cv::HoughLines(edges, lines, 1, CV_PI / 180, 80);

	for (size_t i = 0; i < lines.size(); i++) {
		float rho = lines[i][0];
		float theta = lines[i][1];

		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;

		cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
		cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
		cv::line(imageLines, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
	}

	cv::imshow("Original", image);
  cv::imshow("Circles", imageCircles);
	cv::imshow("Lines", imageLines);

  cv::waitKey(0);
	cv::destroyAllWindows();

  return 0;
}
