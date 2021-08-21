
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <vector>
#include <cmath>
#include <string>



std::string type2str(int type) {
	std::string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}


cv::Mat localGaussBinarisation(cv::Mat image_input, double thresh, cv::Size window_size, int d0) {
	cv::Mat image_out(image_input.size(), CV_8U);
	cv::Mat G;
	cv::GaussianBlur(image_input, G, window_size, 0, 0);
	cv::Mat M = cv::abs(G - image_input);
	cv::Mat D;
	cv::GaussianBlur(M, D, window_size, 0, 0);
	D += d0;
	cv::Mat B = (G - image_input) / D;
	B *= 255;
	cv::threshold(B, image_out, thresh, 255, cv::THRESH_BINARY_INV);
	return image_out;
}


cv::Mat newConnectedComponents(cv::Mat new_components, cv::Mat new_components_ideal) {
	cv::Mat result = cv::Mat::zeros(new_components.size(), CV_8UC3);
	for (int i = 0; i < new_components.cols; i++) {
		for (int j = 0; j < new_components.rows; j++) {
			if (new_components.at<uchar>(j, i) != 0) {
				if (new_components.at<uchar>(j, i) - new_components_ideal.at<uchar>(j, i) > 0) {
					result.at<cv::Vec3b>(j, i) = cv::Vec3b(0, 0, 255); // false positive
				}
				else if (new_components.at<uchar>(j, i) - new_components_ideal.at<uchar>(j, i) < 0) {
					result.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 0, 0); //false negative (miss)
				}
				else {
					result.at<cv::Vec3b>(j, i) = cv::Vec3b(0, 255, 0); //true positive (hit)
				}
			}
		}
	}
	return result;
}

float quality_metric(cv::Mat etalon, cv::Mat example) {
	cv::Mat stats = cv::Mat::zeros(cv::Size(2, 3), CV_32F);
	for (int i = 0; i < etalon.cols; ++i) {
		for (int j = 0; j < etalon.rows; ++j) {
			if (etalon.at<uchar>(j, i) == 0) {
				if (example.at<uchar>(j, i) == 0) {
					stats.at<float>(0, 0) += (float)1;
				}
				else {
					stats.at<float>(0, 1) += (float)1;;
				}
			}
			else {

				if (example.at<uchar>(j, i) == 255) {
					stats.at<float>(1, 1) += (float)1;
				}
				else {
					stats.at<float>(1, 0) += (float)1;
				}
			}
		}
	}
	float precision = stats.at<float>(0, 0) / (stats.at<float>(0, 0) + stats.at<float>(0, 1));
	return precision;
}

void show_image(std::string text, cv::Mat image_input) {
	cv::Mat image_input_show;
	cv::resize(image_input, image_input_show, cv::Size(), 0.25, 0.25);
	cv::imshow(text, image_input_show);

}

int main() {
	try {
		std::string path = "lab04.src.jpg";


		cv::Mat orig = cv::imread(path);
		if (orig.empty())
		{
			std::cout << "Could not read the image: " << path << std::endl;
			return 0;
		}

		cv::Mat ideal = cv::imread("ideal.jpg");
		cv::cvtColor(ideal, ideal, cv::COLOR_RGB2GRAY);
		cv::Mat gray;

		cv::cvtColor(orig, gray, cv::COLOR_RGB2GRAY); cv::imwrite("lab04.g1.png", gray);
		show_image("g1", gray);
	

		cv::Mat binarized, binarized2;
		cv::threshold(gray, binarized, 90, 255, cv::THRESH_BINARY);
		cv::imwrite("lab04.b1.png", binarized);
		show_image("b1", binarized);

		cv::Mat filtered;
		cv::GaussianBlur(binarized, filtered, cv::Size(5, 5), 0, 0);
		cv::imwrite("lab04.f1.png", filtered);
		show_image("f1", filtered);

		cv::Mat localGauss = localGaussBinarisation(gray, 127, cv::Size(19, 19), 95);
		cv::imwrite("lab04.b2.png", localGauss);
		show_image("b2", localGauss);

		cv::Mat localGauss_filtered;
		cv::GaussianBlur(localGauss, localGauss_filtered, cv::Size(7, 7), 0, 0);
		cv::imwrite("lab04.f2.png", localGauss_filtered);
		show_image("f2", localGauss_filtered);

		cv::Mat connected_localGauss(orig.size(), CV_8U);
		cv::Mat connected_gaussian(orig.size(), CV_8U);
		cv::Mat connected_ideal(orig.size(), CV_8U);
		localGauss_filtered = 255 - localGauss_filtered;
		ideal = 255 - ideal;
		filtered = 255 - filtered;

		int amount_labels_filtered = cv::connectedComponents(filtered, connected_gaussian, 8, 4);
		int amount_labels_ideal = cv::connectedComponents(ideal, connected_ideal, 8, 4);
		int amount_labels_localGauss_filtered = cv::connectedComponents(localGauss_filtered, connected_localGauss, 8, 4);

		cv::Mat new_components = cv::Mat::zeros(orig.size(), CV_8U);
		cv::Mat new_components_gauss = cv::Mat::zeros(orig.size(), CV_8U);
		cv::Mat new_components_ideal = cv::Mat::zeros(orig.size(), CV_8U);
	
		for (int i = 0; i < orig.cols; i++) {
			for (int j = 0; j < orig.rows; j++) {
				if (connected_gaussian.at<int>(j, i) > 0) {
					new_components.at<uchar>(j, i) = 255;
				}
				if (connected_ideal.at<int>(j, i) > 0) {
					new_components_ideal.at<uchar>(j, i) = 255;
				}
				if (connected_localGauss.at<int>(j, i) > 0) {
					new_components_gauss.at<uchar>(j, i) = 255;
				}
			}
		}
		cv::Mat components_filtered;
		cv::GaussianBlur(new_components, components_filtered, cv::Size(3, 3), 0, 0);
		cv::imwrite("lab04.v1.png", components_filtered);
		show_image("v1", components_filtered);

		cv::Mat components_fgl_filtered;
		cv::GaussianBlur(new_components_gauss, components_fgl_filtered, cv::Size(3, 3), 0, 0);
		cv::imwrite("lab04.v2.png", components_fgl_filtered);
		show_image("v2", components_fgl_filtered);

		
		cv::Mat result_classic = newConnectedComponents(new_components, new_components_ideal);
		cv::Mat result_fastclassic = newConnectedComponents(new_components_gauss, new_components_ideal);
		cv::imwrite("lab04.e1.png", result_classic);
		show_image("e1", result_classic);

		cv::imwrite("lab04.e2.png", result_fastclassic);
		show_image("e2", result_fastclassic);

		float precision = quality_metric(new_components_ideal, new_components);
		float precision_gauss = quality_metric(new_components_ideal, new_components_gauss);
		
		std::cout << precision << std::endl;
		std::cout << precision_gauss << std::endl;
	}
	catch (cv::Exception e) {
		std::cout << e.msg;
	}
	cv::waitKey(0);
	
	
	return 0;
}
