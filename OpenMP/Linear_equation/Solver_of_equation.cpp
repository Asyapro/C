#include "pch.h"
#include "Solver_of_equation.h"
#include <iostream>
#include "omp.h"

Solver_of_equation::Solver_of_equation() {};

Solver_of_equation::Solver_of_equation(Matrix A, Vector b) {
	if (A.getSize() != b.getRows())
		throw Arithmetic_error("A and b must have the same size");
	this->A = A;
	this->b = b;
	this->size = A.getSize();
	this->Gauss_answer.resize(size);
	this->Jacobi_Gauss_answer.resize(size);
	this->Seidel_answer.resize(size);
};

Solver_of_equation::~Solver_of_equation() {};

bool Solver_of_equation::predominance() {
	double sum = 0.0;
	for (int i = 0; i < size; ++i) {
		sum = 0.0;
		for (int j = 0; j < size; ++j) {
			sum += fabs(A.getValue(i, j));
		}
		
		if (2 * fabs(A.getValue(i, i)) - sum < 0)
			return false;
	}

	return true;
};

bool Solver_of_equation::convergence_condition() {
	MatrixXd A_(size, size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			A_(i, j) = A.getValue(i, j);
		}
	}

	//стороняя библиотека для получения собственных значений матрицы коэффициентов А
	Eigen::EigenSolver<MatrixXd> es(A_);
	double max = 0;
	for (int i = 0; i < size; ++i) {
		max = abs(es.eigenvalues()[i].real()) > max ? abs(es.eigenvalues()[0].real()) : max;
	}
	
	//если все собственные числа по модулю меньше единицы, алгоритмы сходятся
	if (max < 1) return true;
	else return false;
}

std::vector<double> Solver_of_equation::Gauss_method(bool parallel) {
	Matrix A_(A); 
	Vector b_(b);
	double temp;

	for (int k = 0; k < size - 1; k++) {
		for (int i = k + 1; i < size; ++i) {
			temp = A_.getValue(i, k) / A_.getValue(k, k); //коэффициент умножение для получения очередного нуля под главной диагональю
			b_.setValue(i, b_.getValue(i) - temp * b_.getValue(k));//bi = bi - tik * bk; преобразование вектора свободных членов
			
			if (parallel) {
				omp_set_num_threads(4);
				#pragma omp parallel for
				for (int j = k; j < size; ++j) {
					A_.setValue(i, j, A_.getValue(i, j) - temp * A_.getValue(k, j)); //a[i][j] = aij - tik * akj уменьшаем каждый элемент строки
				}
			} else {
				for (int j = k; j < size; ++j) {
					A_.setValue(i, j, A_.getValue(i, j) - temp * A_.getValue(k, j));//a[i][j] = aij - tik * akj уменьшаем каждый элемент строки
				}
			}

		}
	}

	Gauss_answer[size - 1] = b_.getValue(size - 1) / A_.getValue(size - 1, size - 1); // xn = bn/ann;
	double sum;
	//"раскручивание" треугольной матрицы
	for (int k = size - 2; k >= 0; k--) {
		sum = 0.0;

		if (parallel) {
			#pragma omp parallel for reduction(+:sum)
			for (int j = k + 1; j < size; ++j) {
				sum += A_.getValue(k, j) * Gauss_answer[j];//akj * xj;
			}
		} else {
			for (int j = k + 1; j < size; ++j) {
				sum += A_.getValue(k, j) * Gauss_answer[j];//akj * xj;
			}
		}

		Gauss_answer[k] = (b_.getValue(k) - sum) / A_.getValue(k, k);// (bk - sum) / akk;
	}
	return Gauss_answer;
};

std::vector<double> Solver_of_equation::Jacobi_Gauss_method(bool parallel) {
	if (parallel) {
		omp_set_num_threads(2);
		for (int i = 0; i < size; ++i) {
			Jacobi_Gauss_answer[i] = 4;
		}
	} else {
		for (int i = 0; i < size; ++i) {
			Jacobi_Gauss_answer[i] = 4;
		}
	}
	
	std::vector <double> current_iter(size, 0); //для текущей итерации
	double error; // норма, определяемая максимальная разность элементов между Х

	do {
		for (int i = 0; i < size; ++i) {
			current_iter[i] = b.getValue(i);

			#pragma omp parallel sections
			{
				#pragma omp section 
				{
					for (int j = 0; j < i; ++j) {
						current_iter[i] -= A.getValue(i, j) * Jacobi_Gauss_answer[j];
					}
				}
				#pragma omp section		
				{
					for (int j = i + 1; j < size; ++j) {
						current_iter[i] -= A.getValue(i, j) * Jacobi_Gauss_answer[j];
					}
				}
			}

			current_iter[i] /= A.getValue(i, i);
		}

		error = fabs(Jacobi_Gauss_answer[0] - current_iter[0]);
		for (int i = 0; i < size; ++i) {
			error = fabs(Jacobi_Gauss_answer[i] - current_iter[i]) > error ? fabs(Jacobi_Gauss_answer[i] - current_iter[i]) : error;
		}

		if (parallel) {
			omp_set_num_threads(2);
			#pragma omp parallel for
			for (int i = 0; i < size; ++i) {
				Jacobi_Gauss_answer[i] =  current_iter[i];
			}
		} else {
			for (int i = 0; i < size; ++i) {
				Jacobi_Gauss_answer[i] = current_iter[i];
			}
		}
	} while (error > eps);

	return Jacobi_Gauss_answer;
};

std::vector<double> Solver_of_equation::Seidel_method(bool parallel) {
	std::vector <double> current_iter(size);
	double error = 1000;
	double relax = 0.5;
	while (error > eps) {
		for (int i = 0; i < size; ++i) {
			current_iter[i] = b.getValue(i);
			if (parallel) {
			#pragma omp parallel sections
			{
				#pragma omp section 
				{
					for (int j = 0; j < i; ++j) {
						current_iter[i] -= A.getValue(i, j) * current_iter[j];
					}
				}
				#pragma omp section 
				{
					for (int j = i + 1; j < size; ++j) {
						current_iter[i] -= A.getValue(i, j) * Seidel_answer[j];
					}
				}
			}
			} else {
				for (int j = 0; j < i; ++j) {
					current_iter[i] -= A.getValue(i, j) * current_iter[j];
				}
				for (int j = i + 1; j < size; ++j) {
					current_iter[i] -= A.getValue(i, j) * Seidel_answer[j];
				}
			}

			current_iter[i] /= A.getValue(i, i);
		}

		error = 0.0;
		if (parallel) {
			#pragma omp parallel for reduction(+:error) 
			for (int i = 0; i < size; ++i) {
				error += fabs(current_iter[i] - Seidel_answer[i]);
			}
		} else {
			for (int i = 0; i < size; ++i) {
				error += fabs(current_iter[i] - Seidel_answer[i]);
			}
		}

		if (parallel) {
			omp_set_num_threads(4);
			#pragma omp parallel for
			for (int i = 0; i < size; ++i) {
				Seidel_answer[i] = relax * Seidel_answer[i] + (1 - relax) * current_iter[i];
			}
		} else {
			for (int i = 0; i < size; ++i) {
				Seidel_answer[i] = relax * Seidel_answer[i] + (1 - relax) * current_iter[i];
			}
		}
	}

	return Seidel_answer;
};
