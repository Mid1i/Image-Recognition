#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

int main() {
    cv::Mat image1 = cv::imread("C:/Programming/C++/Image-Recognition/11.03/src/image1.png");
    cv::Mat image2 = cv::imread("C:/Programming/C++/Image-Recognition/11.03/src/image2.png");

    if (image1.empty() || image2.empty()) {
        std::cerr << "Не удалось загрузить изображения!" << std::endl;
        return -1;
    }

    cv::Mat gray1, gray2;
    cv::cvtColor(image1, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(image2, gray2, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    orb->detectAndCompute(gray1, cv::Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(gray2, cv::Mat(), keypoints2, descriptors2);

    cv::BFMatcher matcher(cv::NORM_HAMMING, true);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    double max_dist = 0;
    double min_dist = 100;

    for (size_t i = 0; i < matches.size(); i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    std::vector<cv::DMatch> goodMatches;
    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i].distance <= std::max(2 * min_dist, 30.0)) {
            goodMatches.push_back(matches[i]);
        }
    }

    if (goodMatches.empty()) {
        std::cerr << "Не найдено хороших совпадений!" << std::endl;
        return -1;
    }

    std::vector<cv::Point2f> points1, points2;
    for (size_t i = 0; i < goodMatches.size(); i++) {
        points1.push_back(keypoints1[goodMatches[i].queryIdx].pt);
        points2.push_back(keypoints2[goodMatches[i].trainIdx].pt);
    }

    cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC);

    cv::Mat result;
    cv::warpPerspective(image2, result, H, cv::Size(image1.cols + image2.cols, image1.rows));
    cv::Mat half(result, cv::Rect(0, 0, image1.cols, image1.rows));
    image1.copyTo(half);

    cv::Mat imgMatches;
    cv::drawMatches(image1, keypoints1, image2, keypoints2, goodMatches, imgMatches,
        cv::Scalar::all(-1), cv::Scalar::all(-1),
        std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    cv::imshow("Matches", imgMatches);
    cv::imshow("Panorama", result);
    cv::waitKey(0);

    return 0;
}
