#include <opencv2/opencv.hpp>


int main() {
  cv::Mat image = cv::imread("image.jpg");

  if (image.empty()) {
    std::cout << "Ошибка: не удалось загрузить изображение. Проверьте путь к файлу." << std::endl;
    return -1;
  }

  cv::ellipse(image, cv::Point(200, 150), cv::Size(60, 30), 30, 0, 360, cv::Scalar(0, 0, 255), 3);

  cv::ellipse(image, cv::Point(1000, 550), cv::Size(150, 100), 120, 0, 360, cv::Scalar(255, 0, 0), 2);

  cv::rectangle(image, cv::Point(500, 200), cv::Point(750, 650), cv::Scalar(0, 255, 0), 5);

  cv::line(image, cv::Point(300, 150), cv::Point(1000, 150), cv::Scalar(200, 125, 0), 4);

  cv::imshow("Geometric Shapes", image);
  cv::waitKey(0);

  return 0;
}
