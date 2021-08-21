
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
#include "json.hpp"
#include <fstream> 


void show_image(std::string text, cv::Mat image_input) {
	cv::Mat image_input_show;
	cv::resize(image_input, image_input_show, cv::Size(), 0.25, 0.25);
	cv::imshow(text, image_input_show);
}

 cv::Mat Bernsen(cv::Mat matrix, std::string image_name, int R, double t_g, double c_min) {
	
	 double min, max;
	 cv::Mat new_matrix = matrix.clone();
	 cv::Mat I_max = matrix.clone();
	 cv::Mat I_min = matrix.clone();
	 cv::Mat I_middle;
	 matrix.convertTo(I_middle, CV_32FC1);

	 for (int y = 0; y < matrix.rows ; ++y) {
		 for (int x = 0; x < matrix.cols; ++x) {
			 
			 int left = std::max((x - R), 0);
			 int top = std::max((y - R), 0);
			 int size_rect_w = R;
			 int size_rect_h = R; 
			 int k = R;
			 while ((y + 2 * k >= matrix.rows) and k > 1) {
				 k--; 
				 size_rect_h = k;
			 }
			 k = R;
			 while ((x + 2 * k >= matrix.cols) and k > 1) {
				 k--;
				 size_rect_w = k;
			 }
			 cv::Rect sub_matrix = cv::Rect(left, top, size_rect_w, size_rect_h);
			 cv::minMaxLoc(matrix(sub_matrix), &min, &max);
			 float middle = (max + min) / 2.0;
		
			 I_max.at<uchar>(y, x) = max;
			 I_min.at<uchar>(y, x) = min;
			
			 if (max - min > c_min) {
				I_middle.at<float>(y, x) = middle;
				if (matrix.at<uchar>(y, x) > middle) {
					new_matrix.at<uchar>(y, x) = 255;
				}
				else {
					new_matrix.at<uchar>(y, x) = 0;
				}
			 }
			 else {
				 I_middle.at<float>(y, x) = t_g;
				 if (matrix.at<uchar>(y, x) > t_g) {
					 new_matrix.at<uchar>(y, x) = 255;
				 }
				 else{
					 new_matrix.at<uchar>(y, x) = 0;
				 }
			 }
		 }
	 }

	 cv::imwrite("I_max" + image_name + "R" + std::to_string(R) + std::to_string(t_g) + "c_min" + std::to_string(c_min) + ".png", I_max);
	 cv::imwrite("I_min" + image_name + "R" + std::to_string(R) + std::to_string(t_g) + "c_min" + std::to_string(c_min) + ".png", I_min);
	 cv::imwrite("I_middle" + image_name + "R" + std::to_string(R) + std::to_string(t_g) + "c_min" + std::to_string(c_min) + ".png", I_middle);
	 cv::imwrite("res" + image_name + "R" + std::to_string(R) + std::to_string(t_g) + "c_min" + std::to_string(c_min) + ".png", new_matrix);
	 
	
	 //show_image("I_max " + image_name + "R" + std::to_string(R) + std::to_string(t_g) + "c" + std::to_string(c_min), I_max);
	 //show_image("I_min " + image_name + "R" + std::to_string(R) + std::to_string(t_g) + "c" + std::to_string(c_min), I_min);
	// show_image("I_middle " + image_name + "R" + std::to_string(R) + std::to_string(t_g) + "c" + std::to_string(c_min), I_middle);
	 show_image("res " + image_name + "R" + std::to_string(R)  + std::to_string(t_g) + "c" + std::to_string(c_min), new_matrix);

	 return new_matrix;
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

cv::Mat get_homography(cv::Mat scan, cv::Mat image, std::vector<cv::Point> scan_points, std::vector<cv::Point> image_points) {
	cv::Mat scan_inv = 255 - scan;
	cv::Mat homography = cv::findHomography(scan_points, image_points);
	cv::Mat image_out(image.size(), CV_8U);
	warpPerspective(scan_inv, image_out, homography, image.size());
	image_out = 255 - image_out;
	return image_out;
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


double DICE(cv::Mat ideal, cv::Mat image) {
	double ideal_card = 0;
	double nonideal_card = 0;
	double crosssection_card = 0;
	for (int i = 0; i < ideal.cols; i++) {
		for (int j = 0; j < ideal.rows; j++) {
			bool check = false;
			if (ideal.at<uchar>(j, i) != 0) {
				ideal_card += 1.0;
				check = true;
			}
			if (image.at<uchar>(j, i) != 0) {
				nonideal_card += 1.0;
				if (check == true) {
					crosssection_card += 1.0;
				}
			}
		}
	}
	double res = 2 * crosssection_card / (ideal_card + nonideal_card);
	return res;
}


int main() {
	try {


		std::vector<std::string> image_names; 
		//image_names.push_back("photo3953");
		//image_names.push_back("IMG_329"); 
		image_names.push_back("doc1");
		//image_names.push_back("robertson");
		

		std::vector< cv::Mat > images;
		for (int i = 0; i < image_names.size(); ++i) {
			std::string image_path = image_names[i] + ".jpg";
			images.push_back(load_grey(image_path));
		}
		
		cv::Mat scan = load_grey("doc.png");
		
		cv::Mat scan_binary;
		cv::threshold(scan, scan_binary, 177, 255, cv::THRESH_BINARY);
		cv::imwrite("scan.png", scan_binary);
		//show_image("scan", scan_binary);


		std::ifstream scan_borders_stream("scan.json");
		nlohmann::json scan_borders;
		scan_borders_stream >> scan_borders;

		std::vector<nlohmann::json> images_borders;
		for (int i = 0; i < image_names.size(); ++i) {
			std::string image_path = image_names[i] + ".json";
			std::ifstream image_borders_stream(image_path);
			nlohmann::json image_borders_;
			image_borders_stream >> image_borders_;
			images_borders.push_back(image_borders_);

		}

		std::vector<cv::Point> scan_points = get_mark(scan_borders);
		std::vector<cv::Mat> homographies;
		for (int i = 0; i < images.size(); ++i) {
			std::vector<cv::Point> image_points = get_mark(images_borders[i]);
			cv::Mat homography = get_homography(scan_binary, images[i], scan_points, image_points);
			homographies.push_back(homography);
			cv::imwrite(image_names[i] + "homography.png", homography);
			show_image("homography" + image_names[i], homography);
		}

		std::vector< std::vector<cv::Mat> > binary;

		for (int i = 0; i < images.size(); ++i) {
			int R = 5;
			double t_g = 130;
			double c_min = 70;
			cv::Mat image0 = Bernsen(images[i], image_names[i], R, t_g, c_min);
			std::cout <<  "R " << std::to_string(R) 
				<< ", t_g " << std::to_string(t_g) 
				<< ", c_min " << std::to_string(c_min) 
				<< ", dice " << DICE(homographies[i], image0)
				<< std::endl;
			

			

		}		


	}
	catch (cv::Exception e) {
		std::cout << e.msg;
	}
	cv::waitKey(0);


	return 0;
}
