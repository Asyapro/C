
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

	std::string file_name = borders["_via_image_id_list"][0];
	std::vector<cv::Point> scan_points;
	for (int i = 0; i < borders["_via_img_metadata"][file_name]
		["regions"].size(); i++) {
		for (int j = 0; j < borders["_via_img_metadata"][file_name]["regions"][i]["shape_attributes"]["all_points_x"].size(); j++) {
			int x = borders["_via_img_metadata"][file_name]["regions"]
				[i]["shape_attributes"]["all_points_x"][j];
			int y = borders["_via_img_metadata"][file_name]["regions"]
				[i]["shape_attributes"]["all_points_y"][j];
			scan_points.push_back(cv::Point(x, y));
		}
	}
	return scan_points;

}


cv::Mat get_homography(cv::Mat scan, cv::Mat image, std::vector<cv::Point> scan_points, std::vector<cv::Point> image_points) {
	cv::Mat scan_inv = 255 - scan;
	cv::Mat homography = cv::findHomography(scan_points, image_points);
	cv::Mat image_out(image.size(), CV_8U);
	warpPerspective(scan_inv, image_out, homography, image.size());
	image_out = 255 - image_out;
	return image_out;
}


void components(cv::Mat image_input, cv::Mat image_ideal, int blocksize, int C, int gauss, std::string image_name) {
	cv::Mat binarized;
	cv::adaptiveThreshold(image_input, binarized, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blocksize, C);
	cv::imwrite("binarized\\" + image_name + "binarized.png", binarized);
	//show_image("binarized" + std::to_string(blocksize) + std::to_string(C) + std::to_string(gauss), binarized);
	cv::Mat filtered;
	cv::GaussianBlur(binarized, filtered, cv::Size(gauss, gauss), 0, 0);
	//show_image("filtered" + std::to_string(blocksize) + std::to_string(C) + std::to_string(gauss), filtered);
	cv::imwrite("filtered\\" + image_name + "filtered.png", binarized);

	cv::Mat connected_gaussian(image_input.size(), CV_8U);
	cv::Mat connected_ideal(image_ideal.size(), CV_8U);

	cv::Mat ideal = 255 - image_ideal;
	filtered = 255 - filtered;

	int amount_labels_filtered = cv::connectedComponents(filtered, connected_gaussian, 8, 4);
	int amount_labels_ideal = cv::connectedComponents(ideal, connected_ideal, 8, 4);

	cv::Mat new_components = cv::Mat::zeros(image_input.size(), CV_8U);
	cv::Mat new_components_ideal = cv::Mat::zeros(image_ideal.size(), CV_8U);

	for (int i = 0; i < image_input.cols; i++) {
		for (int j = 0; j < image_input.rows; j++) {
			if (connected_gaussian.at<int>(j, i) > 0) {
				new_components.at<uchar>(j, i) = 255;
			}
			if (connected_ideal.at<int>(j, i) > 0) {
				new_components_ideal.at<uchar>(j, i) = 255;
			}

		}
	}
	cv::Mat components_filtered;
	cv::GaussianBlur(new_components, components_filtered, cv::Size(3, 3), 0, 0);
	cv::imwrite("v\\" + image_name + "v.png", components_filtered);
	//show_image("v1" + std::to_string(blocksize) + std::to_string(C) + std::to_string(gauss), components_filtered);
	
	cv::Mat result_classic = newConnectedComponents(new_components, new_components_ideal);
	cv::imwrite("e\\" + image_name + "e.png", result_classic);
	//show_image("e1" + std::to_string(blocksize) + std::to_string(C) + std::to_string(gauss), result_classic);

	float precision = quality_metric(new_components_ideal, new_components);
	std::cout << image_name << " precision " << precision << std::endl;
	return;
}


int main() {
	try {

		cv::Mat scan_image = cv::imread("images\\scan.png", cv::IMREAD_GRAYSCALE);
		std::ifstream scan_borders_stream("scan.json");
		nlohmann::json scan_borders;
		scan_borders_stream >> scan_borders;
		
		std::vector<std::string> image_names;
		image_names.push_back("P757");
		image_names.push_back("P129");
		image_names.push_back("P813");
		image_names.push_back("P941");
		image_names.push_back("P956");

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

		
		/*std::ifstream image_borders_stream("P_20210410_170757_vHDR_On.json");
		image_borders_stream >> image_borders[0];
		std::ifstream image_borders_stream("P_20210410_171129_vHDR_On.json");
		image_borders_stream >> image_borders[1];
		std::ifstream image_borders_stream("P_20210410_170813_vHDR_On.json");
		image_borders_stream >> image_borders[2];
		std::ifstream image_borders_stream("P_20210410_170941_vHDR_On.json");
		image_borders_stream >> image_borders[3];
		std::ifstream image_borders_stream("P_20210410_170956_vHDR_On.json");
		image_borders_stream >> image_borders[4];*/

		/*for (int i = 0; i < images.size(); ++i){
			show_image(std::to_string(i),  images[i]);
		}*/
		

		std::vector<cv::Point> scan_points = get_mark(scan_borders);
		
		std::vector<cv::Mat> homographies;
		for (int i = 0; i < images.size(); ++i) {
			std::vector<cv::Point> image_points = get_mark(images_borders[i]);
			cv::Mat homography = get_homography(scan_image, images[i], scan_points, image_points);
			homographies.push_back(homography);
			cv::imwrite("homography\\" + image_names[i] + "homography.png", homography);
		}
		
		components(images[0], homographies[0], 57, 30, 5, image_names[0]); //thios
		//components(images[0], homographies[0], 57, 30, 7, image_names[0]);
		components(images[1], homographies[1], 53, 25, 5, image_names[1]); //this
		//components(images[1], homographies[1], 53, 25, 7, image_names[1]);
		components(images[2], homographies[2], 69, 30, 5, image_names[2]); //this
		//components(images[2], homographies[2], 83, 30, 5, image_names[2]);
		components(images[3], homographies[3], 71, 20, 5, image_names[3]); //this
		//components(images[3], homographies[3], 51, 15, 5, image_names[3]);
		//components(images[4], homographies[4], 31, 30, 7, image_names[4]);
		components(images[4], homographies[4], 31, 30, 5, image_names[4]); //this

		
	}
	catch (cv::Exception e) {
		std::cout << e.msg;
	}
	cv::waitKey(0);
	
	
	return 0;
}
