//çäåñü îïèñàíû àëãîðèòìû êðèòåðèåâ ïî Êíóòó

#include "pch.h"
#include "Generator.cpp"
#include <iostream>
#include <Vector>

using namespace std;


class Criterions {
public:
	Criterions() {};

	Criterions(long long lenght_set, long long amount_categories, long long amount_pairs, int amount_intervals, long n, double begin_interval, double end_interval, int amount_pokers) {
		this->lenght_set = lenght_set;
		this->amount_categories = amount_categories;
		this->amount_pairs = amount_pairs;
		this->amount_intervals = amount_intervals;
		this->n = n;
		this->begin_interval = begin_interval;
		this->end_interval = end_interval;
		this->amount_pokers = amount_pokers;

		categories.resize(amount_categories, 0);
		pairs.resize(amount_pairs);
		for (int i = 0; i < amount_pairs; ++i) {
			pairs[i].resize(amount_pairs, 0);
		}
		pokers.resize(amount_pairs, 0);
	};

	~Criterions() {};

	//çàïîëíåíèå êàòåãîðèé äëÿ êðèòåðèÿ ÷àñòîò
	void filling_categories(int value) {
		categories[value]++; 
	};
	//çàïîëíåíèå êàòåãîðèé äëÿ êðèòåðèÿ ñåðèé (èñïîëüçóþòñÿ ïàðû ÷èñåë)
	void filling_categories(int value1, int value2) {
		pairs[value1][value2]++; 
	};
	//çàïîëíåíèå êàòåãîðèé äëÿ êðèòåðèÿ èíòåðâàëîâ
	void filling_categories(Generator gen) {
		interval_count.resize(amount_intervals);
		int r = 0; int s = 0;
		double value;

		for (long long int i = 0; i < lenght_set; ++i) {
			value = gen.next();
		
			if (begin_interval <= value && value < end_interval) {
				//èíòåðâàë äëèíîé r íàéäåí
				if (r < amount_intervals) {
					interval_count[r]++; //ðåãèñòðèðóåì èíòåðâàë äëèíîé r
				}
				else {
					interval_count[amount_intervals - 1]++; //óâåëè÷èâàåì êîëè÷åñòâî èíòåðâàëîâ äëèíîé >= t
				}

				if (s == n) {
					break;
				} //íàøëè n èíòåðâàëîâ, âûõîäèì èç öèêëà
				r = 0;
				s++;
			}

			else {
				r++;
			}
		}
	};
	//çàïîëíåíèå êàòåãîðèé äëÿ ïîêåð-êðèòåðèÿ
	void filling_categories(vector <int> values) { 
		vector <int> temp(amount_pokers); // êîëè÷åñòâî ÷èñåë, èíäåêñû êîòîðûõ ñîîòâåòñòâóþò èíäåêñàì èç values
		for (int i = 0; i < values.size() - 1; ++i) {
			int count = 1;
			for (int j = i + 1; j < values.size(); ++j) {
				if (values[i] == values[j]) {
					count++;
					values[j] = -1;
				}
			}
			values[i] = -1;
			temp[i] = count;
		}

		if (count(temp.begin(), temp.end(), 1) == amount_pokers) 
			pokers[4]++; //5 óíèêàëüíûõ, íóëü ïàð
		else if (count(temp.begin(), temp.end(), 2) == 1) 
			pokers[3]++; //4 óíèêàëüíûõ, îäíà ïàðà
		else if (count(temp.begin(), temp.end(), 2) == 2 || count(temp.begin(), temp.end(), 3) == 1) 
			pokers[2]++; //3 óíèêàëüíûõ, îäíà ïàðà èëè îäíà òðîéêà
		else if (count(temp.begin(), temp.end(), 4) == 1 || (count(temp.begin(), temp.end(), 2) == 1  && count(temp.begin(), temp.end(), 3) == 1))
			pokers[1]++; //2 óíèêàëüíûõ, îäíà ÷åòâåðêà èëè îäíà òðîéêà+îäíà ïàðà
		else if (count(temp.begin(), temp.end(), 5) == 1) 
			pokers[0]++; //1 óíèêàëüíîå
	
	};

	//êðèòåðèé ÷àñòîò
	long double frequency_criterion() {
		long double answer = 0.0;
		double p = 1.0 / amount_categories; //âåðîÿòíîñòü ïîïàäàíèÿ â ñîîòâåòñòâóþùèå êàòåãîðèè
		#pragma omp parallel reduction (+: answer)
		{
		#pragma omp for
			for (long long i = 0; i < amount_categories; ++i) {
				answer += pow(categories[i] - lenght_set * p, 2) / (lenght_set * p);
			}
		}
		return answer;
	};
	//êðèòåðèé ñåðèé
	long double series_criterion() {
		long double answer = 0.0;
		double p = 1.0 / (amount_categories * amount_categories); //âåðîÿòíîñòü ïîïàäàíèÿ â ñîîòâåòñòâóþùèå êàòåãîðèè (ïàðû çíà÷åíèé)
		#pragma omp parallel reduction (+: answer)
		{
			#pragma omp for
			for (long long i = 0; i < amount_categories; ++i) {
				for (int j = 0; j < amount_categories; ++j) {
					answer += pow(pairs[i][j] - lenght_set * 2 * p, 2) / (lenght_set * 2 * p);
				}
			}
		}
		return answer;
	};
	//êðèòåðèé èíòåðâàëîâ
	long double interval_criterion() {
		long double answer = 0;
		double p = end_interval - begin_interval;

		//pr = p * (1-p)^i pt = (1-p)^t
		#pragma omp parallel reduction (+: answer)
		{
			#pragma omp for
			for (int i = 0; i < interval_count.size() - 1; ++i) {
				answer += pow((interval_count[i] - n * p * pow((1 - p), i)), 2) / (n * p * pow((1 - p), i));
			}
		}
		answer += pow((interval_count[interval_count.size() - 1] - n * pow((1 - p), interval_count.size() - 1)), 2) / (n * pow((1 - p), interval_count.size() - 1));

		return answer;
	}
	//ïîêåð-êðèòåðèé
	long double poker_criterion() {
		long double answer = 0.0;

		vector <double> pr;
		for (int i = 0; i < amount_pokers; ++i) {
			for (int j = 0; j < i + 1; j++) {
				pr[i] *= (amount_categories - j);
			}
			pr[i] *= (stirling_numbers(lenght_set,amount_categories)/ pow(amount_categories, amount_pokers));;
		}
		#pragma omp parallel reduction (+: answer)
		{
			#pragma omp for
			for (int i = 0; i < amount_pokers; ++i) {
				answer += pow((pokers[i] - lenght_set * amount_pokers * pr[i]), 2) / (lenght_set * amount_pokers * pr[i]);
			}
		}

		return answer;
	}

	//âû÷èñëåíèå ÷èñåë Ñòèðëèíãà äëÿ ïîêåð-êðèòåðèÿ
	int stirling_numbers(int n, int k) {
		if (n == k) {
			return 1;
		}
		else {
			if (k == 0) {
				return 0;
			}
			else {
				return stirling_numbers(n - 1, k - 1) + k * stirling_numbers(n - 1, k);
			}
		}
	}
	
	//ãåòòåðû äëÿ ÷èñëîâûõ ïîëåé
	long long getAmout_categories() {
		return amount_categories;
	}
	long long getAmount_pairs() {
		return amount_pairs;
	}
	int getAmount_pokers() {
		return amount_pokers;
	}
	long long getLenght_set() {
		return lenght_set;
	}

	void str() {
		cout << "\n";
		for (int i = 0; i < interval_count.size(); ++i)
			cout << interval_count[i] << " ";
		cout << "\n";
	}

private:
	long long lenght_set; // äëèíà ïîñëåäîâàòåëüíîñòè
	long long amount_categories; //êîëè÷åñòâî êàòåãîðèé
	long long amount_pairs; //ïàðàìåòð d äëÿ U = de
	int amount_intervals; //ïàðàìåòð t, äëèíû èíòåðâàëîâ 0..t
	long n; //êîëè÷åñòâî èíòåðâàëîâ
	double begin_interval, end_interval;
	int amount_pokers;

	vector <long long> categories; //êîëè÷åñòâî ïîïàâøèõ ÷èñåëîê â ñîîòâåòñòâóþùèå êàòåãîðèè
	vector < vector <long long> > pairs; //ïàðû äëÿ êðèòåðèÿ ñåðèé
	vector <long long> interval_count; //ñ÷åò÷èêè êîëè÷åñòâà èíòåðâàëîâ îïðåäåëåííîé äëèíû
	vector < long long > pokers; //êàòåãîðèè äëÿ ïîêåðíîãî êðèòåðèÿ
};
