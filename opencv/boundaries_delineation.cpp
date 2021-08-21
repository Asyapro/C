
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
#include "nlohmann/json.hpp"
#include <fstream> 

//


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

cv::Mat load_grey(std::string path) {
	cv::Mat gray;
	cv::Mat orig = cv::imread(path);
	if (orig.empty())
	{
		std::cout << "Could not read the image: " << path << std::endl;
		return gray;
	}
	cv::cvtColor(orig, gray, cv::COLOR_RGB2GRAY);
	return gray;
}


std::vector<cv::Point> get_mark(nlohmann::json borders) {
	std::string metadata_index = borders["_via_image_id_list"][0];
	std::vector<cv::Point> points;
	for (int i = 0; i < borders["_via_img_metadata"][metadata_index]["regions"].size(); i++) {
		for (int k = 0; k < borders["_via_img_metadata"][metadata_index]["regions"][i]["shape_attributes"]["all_points_x"].size(); k++) {
			int x = borders["_via_img_metadata"][metadata_index]["regions"][i]["shape_attributes"]["all_points_x"][k];
			int y = borders["_via_img_metadata"][metadata_index]["regions"][i]["shape_attributes"]["all_points_y"][k];
			points.push_back(cv::Point(x, y));
		}
	}

	return points;
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

double DICE(cv::Mat& img, cv::Mat& target) {
	double intersection = 0, img_area = 0, target_area = 0;
	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j) {
			if (img.at<uchar>(i, j) == 0 && target.at<uchar>(i, j) == 0) {
				++intersection;
			}
			if (img.at<uchar>(i, j) == 0) {
				++img_area;
			}
			if (target.at<uchar>(i, j) == 0) {
				++target_area;
			}
		}
	}
	return 2 * intersection / (img_area + target_area);
}

void components(cv::Mat image_input, int median, int gauss, int gauss_, int threshold_line, std::string image_name, nlohmann::json borders) {
	cv::Mat filtered_;
	cv::GaussianBlur(image_input, filtered_, cv::Size(gauss - 14, gauss - 14), 0, 0);

	cv::medianBlur(filtered_, filtered_, median);
	cv::Mat binarized, binarized2;
	binarized = localGaussBinarisation(filtered_, gauss_, cv::Size(gauss, gauss), 5);
	cv::imwrite("binarized\\" + image_name + "binarized.png", binarized);
	//show_image("binarized" + image_name, binarized);

	cv::Mat filtered;
	cv::GaussianBlur(binarized, filtered, cv::Size(gauss, gauss), 0, 0);
	cv::imwrite("filtered\\" + image_name + "filtered.png", filtered);
	cv::Mat canny;
	cv::Canny(filtered, canny, 100, 200, 5, true);
	//show_image("canny " + image_name, canny);

	cv::Mat image_input_clone;
	cv::cvtColor(image_input, image_input_clone, cv::COLOR_GRAY2BGR);
	cv::Mat hough = canny.clone();

	
	cv::Mat  image_corners = filtered.clone();
	std::vector<cv::Point2f> corners;
	goodFeaturesToTrack(canny,
		corners,
		4,            
		0.01,          
		100,             
		cv::Mat(),         
		100,          
		false,        
		0.04);          

	cv::Point2f lt = corners[0];
	cv::Point2f ld = corners[0];
	cv::Point2f rd = corners[0];
	cv::Point2f rt = corners[0];

	std::vector < std::pair <float, float> > corners_pair;
	corners_pair.push_back(std::make_pair(corners[0].x, corners[0].y));
	corners_pair.push_back(std::make_pair(corners[1].x, corners[1].y));
	corners_pair.push_back(std::make_pair(corners[2].x, corners[2].y));
	corners_pair.push_back(std::make_pair(corners[3].x, corners[3].y));
	sort(corners_pair.begin(), corners_pair.end());
	
	if (corners_pair[0].second > corners_pair[1].second) {
		lt = cv::Point2f(corners_pair[0].first, corners_pair[0].second);
		ld = cv::Point2f(corners_pair[1].first, corners_pair[1].second);
	}
	else {
		ld = cv::Point2f(corners_pair[0].first, corners_pair[0].second);
		lt = cv::Point2f(corners_pair[1].first, corners_pair[1].second);
	}

	if (corners_pair[2].second > corners_pair[3].second) {
		rt = cv::Point2f(corners_pair[2].first, corners_pair[2].second);
		rd = cv::Point2f(corners_pair[3].first, corners_pair[3].second);
	}
	else {
		rt = cv::Point2f(corners_pair[3].first, corners_pair[3].second);
		rd = cv::Point2f(corners_pair[2].first, corners_pair[2].second);
	}


	std::vector<cv::Point> corner_mark = get_mark(borders);
	
	line(image_input_clone, corner_mark[0], corner_mark[1], cv::Scalar(0, 225, 0), 3, cv::LINE_AA);
	line(image_input_clone, corner_mark[1], corner_mark[2], cv::Scalar(0, 225, 0), 3, cv::LINE_AA);
	line(image_input_clone, corner_mark[2], corner_mark[3], cv::Scalar(0, 225, 0), 3, cv::LINE_AA);
	line(image_input_clone, corner_mark[3], corner_mark[0], cv::Scalar(0, 225, 0), 3, cv::LINE_AA);

	line(image_input_clone, lt, rt, cv::Scalar(0, 0, 225), 3, cv::LINE_AA);
	line(image_input_clone, rt, rd, cv::Scalar(0, 0, 225), 3, cv::LINE_AA);
	line(image_input_clone, rd, ld, cv::Scalar(0, 0, 225), 3, cv::LINE_AA);
	line(image_input_clone, ld, lt, cv::Scalar(0, 0, 225), 3, cv::LINE_AA);
	show_image("difference " + image_name, image_input_clone);
	cv::imwrite("difference\\" + image_name + "difference.png", image_input_clone);

	cv::Mat image_find_points(image_input.rows, image_input.cols, CV_8U, cv::Scalar(255));
	cv::Mat image_mark_points(image_input.rows, image_input.cols, CV_8U, cv::Scalar(255));
	line(image_mark_points, corner_mark[0], corner_mark[1], cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
	line(image_mark_points, corner_mark[1], corner_mark[2], cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
	line(image_mark_points, corner_mark[2], corner_mark[3], cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
	line(image_mark_points, corner_mark[3], corner_mark[0], cv::Scalar(0, 0, 0), 3, cv::LINE_AA);

	line(image_find_points, lt, rt, cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
	line(image_find_points, rt, rd, cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
	line(image_find_points, rd, ld, cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
	line(image_find_points, ld, lt, cv::Scalar(0, 0, 0), 3, cv::LINE_AA);

	float dice = DICE(image_mark_points, image_find_points);
	std::cout << image_name << " dice " << dice << std::endl;
	return;
}


int main() {
	try {

		std::vector<std::string> image_names;
		image_names.push_back("P129");
		image_names.push_back("P858");
		image_names.push_back("P941");
		image_names.push_back("P136");
		image_names.push_back("P334");

		std::vector< cv::Mat > images;
		for (int i = 0; i < image_names.size(); ++i) {
			std::string image_path = "images\\" + image_names[i] + ".jpg";
			images.push_back(load_grey(image_path));
		}

		std::vector<nlohmann::json> images_borders;
		for (int i = 0; i < image_names.size(); ++i) {
			std::string image_path = "jsons\\" + image_names[i] + ".json";
			std::ifstream image_borders_stream(image_path);
			nlohmann::json image_borders_;
			image_borders_stream >> image_borders_;
			images_borders.push_back(image_borders_);
		}


		components(images[0], 73, 21, 120, 1000, image_names[0], images_borders[0]); 
		components(images[1], 73, 19, 180, 200, image_names[1], images_borders[1]);  
		components(images[2], 43, 19, 180, 1000, image_names[2], images_borders[2]);	
		components(images[3], 73, 21, 180, 1000, image_names[3], images_borders[3]);  
		components(images[4], 101, 35, 210, 300, image_names[4], images_borders[4]);  


	}
	catch (cv::Exception e) {
		std::cout << e.msg;
	}
	cv::waitKey(0);


	return 0;
}
