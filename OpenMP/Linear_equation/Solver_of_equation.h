#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "Arithmetic_error.cpp"
#include <vector>
#include <C:/Users/Àñÿ/source/repos/Lab3_BPM172_LomakinaAA/Lab3_BPM172_LomakinaAA/Eigen/Dense>
#include <C:/Users/Àñÿ/source/repos/Lab3_BPM172_LomakinaAA/Lab3_BPM172_LomakinaAA/Eigen/Eigenvalues>
using Eigen::MatrixXd;

class Solver_of_equation {
public:
	Solver_of_equation();
	Solver_of_equation(Matrix A, Vector b);
	~Solver_of_equation();

	bool predominance();
	bool convergence_condition();
	std::vector <double> Gauss_method(bool parallel);
	std::vector <double> Jacobi_Gauss_method(bool parallel);
	std::vector <double> Seidel_method(bool parallel);

	void setSize(int size) {
		this->size = size;
	}
	void setEps(int eps) {
		this->eps = eps;
	}

private:
	int size;
	double eps = 0.001;
	Matrix A;
	Vector b;
	std::vector <double> Gauss_answer;
	std::vector <double> Jacobi_Gauss_answer;
	std::vector <double> Seidel_answer;
};

