#include <opencv2/opencv.hpp>
#include <vector>


int main() {
	cv::Mat image = cv::imread("image.jpg");
	
	if (image.empty()) {
		std::cout << "Ошибка: не удалось загрузить изображение. Проверьте путь к файлу." << std::endl;
		return -1;
	}
	
	int halfHeight = image.rows / 2;
	int halfWidth = image.cols / 2;

	std::vector<cv::Mat> regions = {
		image(cv::Rect(0, 0, halfWidth, halfHeight)), 
		image(cv::Rect(halfWidth, 0, halfWidth, halfHeight)),
		image(cv::Rect(0, halfHeight, halfWidth, halfHeight)),
		image(cv::Rect(halfWidth, halfHeight, halfWidth, halfHeight))
	};
	
	// Чёрно-белый эффект
	cv::Mat temp;
	cv::cvtColor(regions[0], temp, cv::COLOR_BGR2GRAY);
	cv::cvtColor(temp, regions[0], cv::COLOR_GRAY2BGR);
	
	// Заливка цветом
	regions[1].setTo(cv::Scalar(255, 0, 0));
	
	// Увеличение контрасности
	regions[2].convertTo(regions[2], -1, 2.0, 0);
	
	cv::imshow("4 Effects", image);
	cv::waitKey(0);
	
	return 0;
}
