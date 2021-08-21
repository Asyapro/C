/*в этом классе тестируются классы вектора и матрицы, все параметры вводятся с консоли, 
а результаты операций выводятся на экран. также здесь есть функция тестирования времени работы*/


#include "pch.h"
#include "Matrix.h"
#include "Vector.h"
#include "Solver_of_equation.h"
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;
class Test {
public:
	Test() {};
	~Test() {};

	//тест для пользователя: есть вывод на экран всех операций, все параметры пользователь задает через консоль
	void toString() {
		int n;
		cout << "\nenter size for matrix A\n";
		cin >> n;

		double** arr = new double *[n];
		for (int i = 0; i < n; i++) {
			arr[i] = new double[n];
		}

		cout << "\nenter elements of matrix A\n";
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cin >> arr[i][j];
			}
		}

		double *arr2 = new double[n];
		cout << "\nenter elements of vector b\n";
		for (int i = 0; i < n; i++) {
			cin >> arr2[i];
		}

		Matrix matrix(n, arr);
		Vector vector_b(n, arr2);
		Solver_of_equation system(matrix, vector_b);

		//if (system.convergence_condition()) {
		if (system.predominance()) {
			cout << "\ncomputation Gauss\n";
			vector <double> answer = system.Gauss_method(1);
			cout << "answers\n";
			for (int i = 0; i < answer.size(); ++i) {
				cout << answer[i] << " ";
			}

			cout << "\ncomputation Jacobi-Gauss\n";
			answer = system.Jacobi_Gauss_method(1);
			cout << "answers\n";
			for (int i = 0; i < answer.size(); ++i) {
				cout << answer[i] << " ";
			}

			cout << "\ncomputation Seidel\n";
			answer = system.Seidel_method(1);
			cout << "answers\n";
			for (int i = 0; i < answer.size(); ++i) {
				cout << answer[i] << " ";
			}
		}
		else {
			cout << "Jacobi-Gauss and Seidel methods are not available for this system";
		}
	}
	//тест на время
	void test_time(int amount_test, int n) {
		//Matrix Matrix_1(n); 
		time_t t_start, t_end;
		Gauss_time_results_no_parallel.resize(amount_test);
		Gauss_time_results_parallel.resize(amount_test);
		Jacobi_Gauss_time_results_no_parallel.resize(amount_test);
		Jacobi_Gauss_time_results_parallel.resize(amount_test);
		Seidel_time_results_no_parallel.resize(amount_test);
		Seidel_time_results_parallel.resize(amount_test);

		double **arr = new double *[n];
		for (int i = 0; i < n; i++) {
			arr[i] = new double[n];
		}
		double *arr2 = new double[n];

		for (int i = 0; i < amount_test; ++i) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					arr[i][j] = (rand() % 100) / 1000.0 + 0.0001;
				}
				arr[i][i] *= n * 10;
			}

			for (int i = 0; i < n; i++) {
				arr2[i] = (rand() % 100) / 1000.0  + 0.01;
			}

			Matrix A(n, arr);
			Vector b(n, arr2);
			Solver_of_equation system(A, b);

			//if (system.convergence_condition()) {
			if (system.predominance()) {
				t_start = clock();
				system.Gauss_method(false);
				t_end = clock();
				Gauss_time_results_no_parallel[i] = difftime(t_end, t_start);

				t_start = clock();
				system.Jacobi_Gauss_method(0);
				t_end = clock();
				Jacobi_Gauss_time_results_no_parallel.push_back(difftime(t_end, t_start));

				t_start = clock();
				system.Seidel_method(false);
				t_end = clock();
				Seidel_time_results_no_parallel[i] = difftime(t_end, t_start);

				t_start = clock();
				system.Gauss_method(true);
				t_end = clock();
				Gauss_time_results_parallel[i] = difftime(t_end, t_start);

				t_start = clock();
				system.Jacobi_Gauss_method(1);
				t_end = clock();
				Jacobi_Gauss_time_results_parallel.push_back(difftime(t_end, t_start));

				t_start = clock();
				system.Seidel_method(true);
				t_end = clock();
				Seidel_time_results_parallel[i] = difftime(t_end, t_start);

				cout << "\ncomlited test number " << i + 1 << "...";
			}
			else i--;
		}
		cout << "\ndone!\n";
	};
	//выдает среднее арифметическое по всем тестам
	double getTime(vector <double> vector) {
		double result = 0.0;
		for (int i = 0; i < vector.size(); ++i) {
			result += vector[i];
		}
		return result / (double)vector.size();
	}
	//вывод результатов в консоль
	void get_results() {
		cout << "\Gauss without parallel " << getTime(Gauss_time_results_no_parallel) << ", with parallel " << getTime(Gauss_time_results_parallel) << "\n";
		//вывод времени работы - среднее арифметическое по всем тестам
		cout << "\nGauss acceleration coefficient = time_without_parallel / time_parallel = " << getTime(Gauss_time_results_no_parallel) / getTime(Gauss_time_results_parallel) << "\n";

		cout << "\Jacobi-Gauss without parallel " << getTime(Jacobi_Gauss_time_results_no_parallel) << ", with parallel " << getTime(Jacobi_Gauss_time_results_parallel) << "\n";
		//вывод времени работы - среднее арифметическое по всем тестам
		cout << "\nJacobi-Gauss acceleration coefficient = time_without_parallel / time_parallel = " << getTime(Jacobi_Gauss_time_results_no_parallel) / getTime(Jacobi_Gauss_time_results_parallel) << "\n";

		cout << "\Seidel without parallel " << getTime(Seidel_time_results_no_parallel) << ", with parallel " << getTime(Seidel_time_results_parallel) << "\n";
		//вывод времени работы - среднее арифметическое по всем тестам
		cout << "\Seidel acceleration coefficient = time_without_parallel / time_parallel = " << getTime(Seidel_time_results_no_parallel) / getTime(Seidel_time_results_parallel) << "\n";
	}

private:

	vector <double> Gauss_time_results_parallel;
	vector <double> Gauss_time_results_no_parallel;

	vector <double> Jacobi_Gauss_time_results_parallel;
	vector <double> Jacobi_Gauss_time_results_no_parallel;

	vector <double> Seidel_time_results_parallel;
	vector <double> Seidel_time_results_no_parallel;
};