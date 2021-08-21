//здесь размещается тест генератора. только здесь можно изменить параметры тестирующих критериев и генератора

#include "pch.h"
#include "Criterions.cpp"
//#include "Generator.cpp"
#include <iostream>
#include <Vector>

using namespace std;

int main()
{
	//генератор
	long long m = 4294967296; //2^32
	long long a = 2824527309;
	long long b = 0;
	uint32_t X_0 = 1;
	Generator gen1(m, a, b, X_0);
	Generator gen2(m, a, b, X_0);
	Generator gen3(m, a, b, X_0);
	Generator gen4(m, a, b, X_0);
	cout << "generators created\n";

	//критерии
	long long lenght_set = 2147483648; //2^31
	long long amout_categories = 100;
	long long amount_pairs = 10; //в кнуте также обозначается как d, пары (0..d, 0..d)
	int amount_intervals = 40;
	long long n = lenght_set / 16384;
	double begin_interval = 0.45;
	double end_interval = 0.65;
	int amount_pokers = 5;
	Criterions crit(lenght_set, amout_categories, amount_pairs, amount_intervals, n, begin_interval, end_interval, amount_pokers);
	cout << "ctiterions created\n";

	//переменные для записи ответов критериев
	long double frequency_criterion;
	long double series_criterion;
	long double interval_criterion;
	long double poker_criterion;

	cout << "computing\n";

	//параллельно заполняем категории для каждого критерия (функция filling_categories() перегружена) и отработку критериев
	#pragma omp parallel sections
	{ 
	#pragma omp section 
		{
			//критерий частот
			for (long long i = 0; i < crit.getLenght_set(); ++i) {
				crit.filling_categories((int)floor(gen1.next() * crit.getAmount_pairs())); // переводим в U = de
			}
			frequency_criterion = crit.frequency_criterion();
		}
	#pragma omp section 
		{
			//критерий пар
			for (long long i = 0; i < crit.getLenght_set(); ++i) {
				crit.filling_categories((int)floor(gen2.next() * crit.getAmount_pairs()), 
										(int)floor(gen2.next() * crit.getAmount_pairs()));
			}
			series_criterion = crit.frequency_criterion();
		}

	#pragma omp section 
		{
			//критерий интервалов
			crit.filling_categories(gen3);
			interval_criterion = crit.frequency_criterion();
		}
	#pragma omp section 
		{
			//покер-критерий
			vector <int> values(crit.getAmount_pokers()); //вектор "пятерок"
			for (long long i = 0; i < crit.getLenght_set(); ++i) {
				for (int i = 0; i < values.size(); ++i) {
					values[i] = (int)floor(gen4.next() * crit.getAmount_pairs());
				}
				crit.filling_categories(values);
			}
			poker_criterion = crit.frequency_criterion();
		}
	}

	//вывод получившихся значений
	cout << " frequency criterion\n";
	cout << frequency_criterion;

	cout << "series criterion\n";
	cout << series_criterion;

	cout << " interval criterion\n";
	cout << interval_criterion;

	cout << " poker criterion\n";
	cout << poker_criterion;

	return 0;
}
