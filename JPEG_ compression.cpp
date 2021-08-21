
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <vector>
#include <cmath>


std::vector<cv::Mat> mosaic(cv::Mat image, std::vector<cv::Mat> spl) {
	
	cv::Mat blank_channel(image.rows, image.cols, CV_8UC1, cv::Scalar(0));
	cv::Mat mosaic_blue;
	std::vector<cv::Mat> channels;
	channels.push_back(spl[0]);
	channels.push_back(blank_channel);
	channels.push_back(blank_channel);
	cv::merge(channels, mosaic_blue);
	
	cv::Mat mosaic_green;
	std::vector<cv::Mat> channels1;
	channels1.push_back(blank_channel);
	channels1.push_back(spl[1]);
	channels1.push_back(blank_channel);
	cv::merge(channels1, mosaic_green);
	
	cv::Mat mosaic_red;
	std::vector<cv::Mat> channels2;
	channels2.push_back(blank_channel);
	channels2.push_back(blank_channel);
	channels2.push_back(spl[2]);
	cv::merge(channels2, mosaic_red);
	
	std::vector<cv::Mat> mosaic = { mosaic_blue, mosaic_green, mosaic_red };

	return mosaic;
}

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


cv::Mat convert(cv::Mat image_in) {
	
	if (type2str(image_in.type()) == "32FC1") {
		cv::Mat image_in_convert_32FC3;
		std::vector<cv::Mat> image_in_channels;
		image_in_channels.push_back(image_in);
		image_in_channels.push_back(image_in);
		image_in_channels.push_back(image_in);
		cv::merge(image_in_channels, image_in_convert_32FC3);
		cv::Mat out = convert(image_in_convert_32FC3);
		return out;
	}
	else if (type2str(image_in.type()) == "32FC3") {
	
		cv::Mat3b image_in_8UC3;
		image_in.convertTo(image_in_8UC3, CV_8UC3, 255);
		return image_in_8UC3;
	}	
	else
		return image_in;
}


cv::Mat intensity_conversion(cv::Mat image_input) {
	cv::Mat image_out;
	image_input.convertTo(image_out, CV_32F);
	for (int i = 0; i < image_input.cols; i++) {
		for (int j = 0; j < image_input.rows; j++) {
			image_out.at<uchar>(j, i) = sin(image_input.at<uchar>(j, i)) + 1 + (image_input.at<uchar>(j, i)) * 2;
		}
	}
	return image_out;
}

double intensity_conversion_(double x) {
	return (log(abs(x) + 1)); 
}



int main()
{
	try {
		std::string path = "lab03_rgb.png";

		
		cv::Mat orig = cv::imread(path);
		if (orig.empty())
		{
			std::cout << "Could not read the image: " << path << std::endl;
			return 0;
		}
		
		cv::Mat orig_convert;
		orig.convertTo(orig_convert, CV_32F);
		imwrite("lab03_rgb.png", orig_convert);

		cv::Mat image_grayscale, image_luted;
		cv::cvtColor(orig, image_grayscale, cv::COLOR_BGR2GRAY);
		imshow("lab03_gre.png", image_grayscale);
		imwrite("lab03_gre.png", image_grayscale);
		
		cv::Mat image_function(512, 512, CV_32FC1, cv::Scalar(255));
		for (int i = 0; i < 512; i++) {
			image_function.at<float>(image_function.rows - 1 - intensity_conversion_(i)* 70 , i) = 0;
		}
		imshow("lab03_viz_func.png", image_function);
		imwrite("lab03_viz_func.png", image_function);
		
		cv::Mat image_gratscale_res(256, 256, CV_32F);
		for (int i = 0; i < image_gratscale_res.cols; i++) {
			for (int j = 0; j < image_gratscale_res.rows; j++) {
				image_gratscale_res.at<float>(j, i) = intensity_conversion_(orig_convert.at<float>(j, i));
			}
		}

		cv::Mat image_gratscale_res_convert = convert(image_gratscale_res);
		cv::imshow("lab03_gre_res.png", image_gratscale_res);

		cv::Mat image_lut(1, 256, CV_32F);

		for (int i = 0; i < 256; ++i) {
			image_lut.at<float>(0, i) = intensity_conversion_(i);
		}

		cv::Mat orig_convert_U8;
		orig.convertTo(orig_convert_U8, CV_8U);
		
		cv::LUT(image_grayscale, image_lut, image_luted);
		cv::Mat image_luted_convert = convert(image_luted);
		cv::imshow("lab03_gre_res.png", image_luted_convert);
		cv::imwrite("lab03_gre_res.png", image_luted_convert);

		
		std::vector<cv::Mat> channels;
		split(orig_convert_U8, channels);
		std::vector<cv::Mat> channels_for_lut = mosaic(orig_convert_U8, channels);
		cv::Mat image_g, image_r, image_b;
		

		cv::LUT(channels_for_lut[0], image_lut, image_b);
		cv::LUT(channels_for_lut[1], image_lut, image_g);
		cv::LUT(channels_for_lut[2], image_lut, image_r);
		
		
		cv::Mat channels_mosaic;
		cv::Mat channels_mosaic_part;
		
		cv::hconcat(orig_convert, image_r, channels_mosaic);
		cv::hconcat(image_g, image_b, channels_mosaic_part);
		channels_mosaic.push_back(channels_mosaic_part);
	
		
		cv::imshow("lab03_rgb_res", channels_mosaic);
		cv::Mat channels_mosaic_convert = convert(channels_mosaic);
		cv::imwrite("lab03_rgb_res.png", channels_mosaic_convert);
	
	}
	catch (cv::Exception e) {
		std::cout << e.msg;
	}
	cv::waitKey(0);
	
	
	return 0;
}
