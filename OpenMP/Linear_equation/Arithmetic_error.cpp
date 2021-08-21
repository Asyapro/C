#pragma once
#include "pch.h"
#include<string>
#include <iostream>

class Arithmetic_error {
public:
	Arithmetic_error() {};
	Arithmetic_error(std::string message) {
		this->message = message;
		std::cout << "\n" << message << "\n";
	};
	~Arithmetic_error(){};
private:
	std::string message;
};