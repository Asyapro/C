/*â ýòîì êëàññå òåñòèðóþòñÿ êëàññû âåêòîðà è ìàòðèöû, âñå ïàðàìåòðû ââîäÿòñÿ ñ êîíñîëè, 
à ðåçóëüòàòû îïåðàöèé âûâîäÿòñÿ íà ýêðàí. òàêæå çäåñü åñòü ôóíêöèÿ òåñòèðîâàíèÿ âðåìåíè ðàáîòû*/


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

	/* òåñò íà âðåìÿ áûë íåîáõîäèì äëÿ âûáîðà íàèáîëåå îïòèìàëüíîãî ïàðàìåòðà ðàñïàðàëëåëèâàíèÿ
	ôóíêöèÿ test_time ïðîâîäèò amount_test òåñòîâ ïðè ôèêñèðîâàííûõ ïàðàìåòðàõ:
	n - ðàçìåðíîñòü, value1, value2 - çíà÷åíèÿ, êîòîðûìè çàïîëíÿþòñÿ ïåðâàÿ-âòîðàÿ ìàòðèöà è ïåðâûé-âòîðîé âåêòîð
	coefficient - ñêàëÿð, óìíîæàåìûé íà ìàòðèöó è íà âåêòîð 
	
	òåñò ïðîâîäèòñÿ ïî ñëåäóþùåìó ôóíêöèîíàëó (ïî ïîðÿäêó): 
		ìàòðèöû:
			ñîçäàíèå 2 ìàòðèö
			âû÷èñëåíèå íîðì Ôðîáåíèóñà
			ñóììèðîâàíèå ìàòðèö äâàæäû (îïåðàòîðû += è =)
			ïðîèçâåäåíèå ìàòðèö
			òðàíñïîíèðîâàíèå ìàòðèöû
			äîìíîæåíèå ìàòðèöû íà êîýôôèöèåíò
		âåêòîðà:
			ñîçäàíèå 2 âåêòîðîâ
			âû÷èñëåíèå äëèí
			ñóììèðîâàíèå âåêòîðîâ äâàæäû (îïåðàòîðû += è =)
			âû÷èñëåíèå ñêàëÿðíîãî ïðîèçâåäåíèÿ âåêòîðîâ
			äîìíîæåíèå âåêòîðà íà êîýôôèöèåíò
			óìíîæåíèå ìàòðèöû íà êîýôôèöèåíò */
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

	//âûäàåò âåêòîð ñ ðåçóëüòàòàìè òåñòîâ
	std::vector<double> getTime_results() {
		return time_results;
	};

	//âûäàåò ñðåäíåå àðèôìåòè÷åñêîå ïî âñåì òåñòàì
	double getTime() {
		double result = 0;
		for (int i = 0; i < time_results.size(); ++i) {
			result += time_results[i];
		}
		return result / time_results.size();
	}

	//òåñò äëÿ ïîëüçîâàòåëÿ: åñòü âûâîä íà ýêðàí âñåõ îïåðàöèé, âñå ïàðàìåòðû ïîëüçîâàòåëü çàäàåò ÷åðåç êîíñîëü
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
