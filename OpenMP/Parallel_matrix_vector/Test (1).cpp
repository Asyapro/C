/*в этом классе тестируютс€ классы вектора и матрицы, все параметры ввод€тс€ с консоли, 
а результаты операций вывод€тс€ на экран. также здесь есть функци€ тестировани€ времени работы*/


#include "pch.h"
#include "Matrix.h"
#include "Vector.h"
#include <iostream>
#include <time.h>
#include <vector>
#include <omp.h>

class Test {
public:
	Test() {};
	~Test() {};

	/* тест на врем€ был необходим дл€ выбора наиболее оптимального параметра распараллеливани€
	функци€ test_time проводит amount_test тестов при фиксированных параметрах:
	n - размерность, value1, value2 - значени€, которыми заполн€ютс€ перва€-втора€ матрица и первый-второй вектор
	coefficient - скал€р, умножаемый на матрицу и на вектор 
	
	тест проводитс€ по следующему функционалу (по пор€дку): 
		матрицы:
			создание 2 матриц
			вычисление норм ‘робениуса
			суммирование матриц дважды (операторы += и =)
			произведение матриц
			транспонирование матрицы
			домножение матрицы на коэффициент
		вектора:
			создание 2 векторов
			вычисление длин
			суммирование векторов дважды (операторы += и =)
			вычисление скал€рного произведени€ векторов
			домножение вектора на коэффициент
			умножение матрицы на коэффициент */
	void test_time(int amount_test, int n, double value1, double value2, double coefficient) {
		Matrix Matrix_1(n, value1);
		Matrix Matrix_2(n, value2);
		Vector Vector_1(n, value1);
		Vector Vector_2(n, value2);
		time_t t_start, t_end, t_start_1, t_end_1;
		for (int i = 0; i < amount_test; ++i) {
			
			t_start = clock();
			omp_set_num_threads(4);
			/*test matrix*/
			/*Matrix_1.Frobenius();*/
			Matrix_1 + Matrix_2;
			Matrix_1 += Matrix_2;
			Matrix_1 * Matrix_2;
			Matrix_2.transpose();
			Matrix_2 * coefficient;

			/*test vector*/
			Vector_1.Lenght();
			Vector_1 += Vector_2;
			Vector_1 + Vector_2;
			Vector_1.scalar_multiplication(Vector_2);
			Vector_1 * coefficient;
			Matrix_2 * Vector_2;
			t_end = clock();

			time_results.push_back(difftime(t_end, t_start));
		}
	};

	//выдает вектор с результатами тестов
	std::vector<double> getTime_results() {
		return time_results;
	};

	//выдает среднее арифметическое по всем тестам
	double getTime() {
		double result = 0;
		for (int i = 0; i < time_results.size(); ++i) {
			result += time_results[i];
		}
		return result / time_results.size();
	}

	//тест дл€ пользовател€: есть вывод на экран всех операций, все параметры пользователь задает через консоль
	void toString() {
		int n;
		std::cout << "\nenter size for matrixes and vectors\n";
		std::cin >> n;
		std::cout << "\nTEST MATRIX\n";
		double value1, value2;
		std::cout << "\nenter value1 and value2 for filling matrixes\n";
		std::cin >> value1 >> value2;

		/*test matrix*/
		Matrix Matrix_1(n, value1);
		Matrix Matrix_2(n, value2);
		
		std::cout << "\nMatrix_1\n";
		//Matrix_1.toString();
		std::cout << "\nMatrix_2\n";
		//Matrix_2.toString();

		std::cout << "\nFrobenius Matrix_1\n";
		std::cout << Matrix_1.Frobenius();
		std::cout << "\nFrobenius Matrix_2\n";
		std::cout << Matrix_2.Frobenius();

		std::cout << "\nMatrix_1 + Matrix_2\n";
		(Matrix_1 + Matrix_2).toString();
		std::cout << "\nMatrix_1+=Matrix_2\n";
		Matrix_1 += Matrix_2;
		Matrix_1.toString();

		std::cout << "\ntransposed Matrix_2\n";
		Matrix_2.transpose().toString();

		double coefficient;
		std::cout << "\nenter coefficient for multiplication with Matrix_2 \n";
		std::cin >> coefficient;
		std::cout << "\nMatrix_2 * " << coefficient << "\n";
		(Matrix_2 * coefficient).toString();

		/*test vector*/
		std::cout << "\n\n\nTEST VECTOR\n";
		std::cout << "\nenter value1 and value2 for filling vectors\n";
		std::cin >> value1 >> value2;

		Vector Vector_1(n, value1);
		Vector Vector_2(n, value2);

		std::cout << "\nVector_1\n";
		Vector_1.toString();
		std::cout << "\nVector_2\n";
		Vector_2.toString();

		std::cout << "\nlenght Vector_1\n";
		std::cout << Vector_1.Lenght();
		std::cout << "\nlenght Vector_2\n";
		std::cout << Vector_2.Lenght();

		std::cout << "\nVector_1+=Vector_2\n";
		Vector_1 += Vector_2;
		Vector_1.toString();

		std::cout << "\nscalar multiplication Vector_1 and Vector_2\n";
		std::cout << Vector_1.scalar_multiplication(Vector_2) << "\n";
		
		std::cout << "\nMatrix_2\n";
		Matrix_2.toString();
		std::cout << "Vector_2\n";
		Vector_2.toString();
		std::cout << "\nmultiplication Matrix_2 and Vector_2\n";
		(Matrix_2 * Vector_2).toString();
	}
private:
	std::vector <double> time_results;
};