
#include "pch.h"
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;

Mat Gamma_correction_pixel(Mat image_input, float gamma){
	//Mat image_out(Mat::zeros(image_input.rows, image_input.cols, CV_32F));
	Mat image_out;
	image_input.convertTo(image_out, CV_32F);
	for (int i = 0; i < image_input.cols; i++) {
		for (int j = 0; j < image_input.rows; j++) {
			image_out.at<float>(j, i) = std::pow(float(image_input.at<uchar>(j, i)), gamma);
		}
	}
	return image_out;
}

Mat Gamma_correction_pow(Mat image_input, float gamma) {
	Mat image_out;
	image_input.convertTo(image_out, CV_32F);
	pow(image_out, gamma, image_out);
	return image_out;
}

int main()
{
	double gamma1 = 2.3;
	double gamma2 = 2.4;
	int height = 60;
	int width = 768;
	int width_gradient = 3;

	Mat image_test(Mat::zeros(height, width, CV_8U));
	for (int i = 0; i < width / width_gradient; i++) {
		for (int j = 0; j < width_gradient; j++) {
			cv::Point line_begin(width_gradient * i + j, 0), line_end(width_gradient * i + j, height);
			cv::line(image_test, line_begin, line_end, cv::Scalar::all(i));
		}
	}
	imwrite("test_image.png", image_test);
	auto t1 = std::chrono::steady_clock::now();
	Mat image_pow = Gamma_correction_pow(image_test, gamma2);
	auto t2 = std::chrono::steady_clock::now();
	auto time_pow = std::chrono::duration_cast<std::chrono::duration<double>> (t2 - t1);
	

	t1 = std::chrono::steady_clock::now();
	Mat image_pixel = Gamma_correction_pixel(image_test, gamma1);
	t2 = std::chrono::steady_clock::now();
	auto time_pixel = std::chrono::duration_cast<std::chrono::duration<double>> (t2 - t1);

	std::cout << "Pow time " << time_pow.count() << std::endl;
	std::cout << "Pixel time " << time_pixel.count() << std::endl;
	std::cout << "Pow_time / Pixel_time " << time_pow.count() / time_pixel.count() << std::endl;
	
	normalize(image_pow, image_pow, 255, 0, NORM_MINMAX, CV_8U);
	normalize(image_pixel, image_pixel, 255, 0, NORM_MINMAX, CV_8U);
	imwrite("image_pow.png", image_pow);
	Mat newImage = image_test.clone();
	newImage.push_back(image_pow);
	newImage.push_back(image_pixel);
	imshow("I1, G1, G2", newImage);
	imwrite("lab01.png", newImage);

	waitKey(0);
	return 0;
}
