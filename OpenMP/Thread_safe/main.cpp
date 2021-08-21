
#include "Test.cpp"
#include "Stack.h"
#include "Queue.h"
#include "Vector.h"
#include <iostream>

Stack<int> s;
Queue<int> q;
Vector<int> v;
clock_t time_general, time_connection, time_working;
double coefficient_efficient_use_s, coefficient_efficient_use_q, coefficient_efficient_use_v;
double time_connection_s, time_connection_q, time_connection_v;
std::vector <double> time_work;
std::vector <double> time_conn;

void  StackConnect() {
	time_connection = s.CheckConnection();
	time_conn.push_back(difftime(time_connection, time_working) / CLOCKS_PER_SEC);
}

void  QueueConnect() {
	time_connection = q.CheckConnection();
	time_conn.push_back(difftime(time_connection, time_working) / CLOCKS_PER_SEC);
}

void  VectorConnect() {
	time_connection = v.CheckConnection();
	time_conn.push_back(difftime(time_connection, time_working) / CLOCKS_PER_SEC);
}

void StackCheck() {
	std::thread ts1(StackConnect);
	std::thread ts2(StackConnect);
	std::thread ts3(StackConnect);
	std::thread ts4(StackConnect);
	std::thread ts5(StackConnect);

	time_working = clock();
	ts1.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	ts2.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	ts3.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	ts4.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	ts5.join();
	time_work.push_back(difftime(clock(), time_working));
}

void QueueCheck() {
	std::thread tq1(QueueConnect);
	std::thread tq2(QueueConnect);
	std::thread tq3(QueueConnect);
	std::thread tq4(QueueConnect);
	std::thread tq5(QueueConnect);
	time_general = clock();
	time_working = clock();
	tq1.join();
	time_work.push_back(difftime(clock(), time_general));

	time_working = clock();
	tq2.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	tq3.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	tq4.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	tq5.join();
	time_work.push_back(difftime(clock(), time_working));
}

void VectorCheck() {
	std::thread tv1(VectorConnect);
	std::thread tv2(VectorConnect);
	std::thread tv3(VectorConnect);
	std::thread tv4(VectorConnect);
	std::thread tv5(VectorConnect);
	time_general = clock();
	time_working = clock();
	tv1.join();
	time_work.push_back(difftime(clock(), time_general));

	time_working = clock();
	tv2.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	tv3.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	tv4.join();
	time_work.push_back(difftime(clock(), time_working));

	time_working = clock();
	tv5.join();
	time_work.push_back(difftime(clock(), time_working));
}

int main()
{
	int n = 10;
	for (int i = 0; i < n; ++i) {
		s.push(1); 
		q.push(1);
		v.push_back(1);
	}

	Test test;

	std::cout << "\nTest Stack\n";
	test.checkStack();
	std::cout << "\nTest Queue\n";
	test.checkQueue();
	std::cout << "\nTest Vector\n";
	test.checkVector();

	std::cout << "\nComputation time-test\n";

	time_general = clock();
	coefficient_efficient_use_s = 0, coefficient_efficient_use_q = 0, coefficient_efficient_use_v = 0;
	time_conn.resize(0);
	time_work.resize(0);
	time_connection = clock();
	time_working = clock();
	
	time_conn.resize(0);
	time_work.resize(0);
	std::cout << "\nTEST STACK LOCK\n";
	StackCheck();
	s.StateUnlock(true);
	std::cout << "\nTEST STACK UNLOCK\n";
	time_general = clock();
	StackCheck();
	time_general = clock() - time_general;
	coefficient_efficient_use_s = ((double)(time_work[0] + time_work[1] + time_work[2] + time_work[3] + time_work[4])) / ((double)time_general);
	time_connection_s = ((double)(time_conn[0] + time_conn[1] + time_conn[2] + time_conn[3] + time_conn[4]) / 5.0); 
	std::cout << std::endl << "coefficient of efficient use for stack = " << coefficient_efficient_use_s << std::endl;
	std::cout << std::endl << "middle time of respose for stack = " << fabs(time_connection_s) << std::endl;
	
	time_conn.resize(0);
	time_work.resize(0);
	std::cout << "\nTEST QUEUE LOCK\n";
	QueueCheck();
	q.StateUnlock(true);
	std::cout << "\nTEST QUEUE UNLOCK\n";
	time_general = clock();
	QueueCheck();
	time_general = clock() - time_general;
	coefficient_efficient_use_q = ((double)(time_work[0] + time_work[1] + time_work[2] + time_work[3] + time_work[4])) / ((double)time_general);
	time_connection_q = ((double)(time_conn[0] + time_conn[1] + time_conn[2] + time_conn[3] + time_conn[4]) / 5.0);
	std::cout << std::endl << "coefficient of efficient use for queue = " << coefficient_efficient_use_q << std::endl;
	std::cout << std::endl << "middle time of respose for queue = " << fabs(time_connection_q) << std::endl;

	time_conn.resize(0);
	time_work.resize(0);
	std::cout << "\nTEST VECTOR LOCK\n";
	VectorCheck();
	v.StateUnlock(true);
	std::cout << "\nTEST VECTOR UNLOCK\n";
	time_general = clock();
	VectorCheck();
	time_general = clock() - time_general;
	coefficient_efficient_use_v = ((double)(time_work[0] + time_work[1] + time_work[2] + time_work[3] + time_work[4])) / ((double)time_general);
	time_connection_v = ((double)(time_conn[0] + time_conn[1] + time_conn[2] + time_conn[3] + time_conn[4]) / 5.0); 
	std::cout << std::endl << "coefficient of efficient use for vector = " << coefficient_efficient_use_v << std::endl;
	std::cout << std::endl << "middle time of respose for vector = " << fabs(time_connection_v) << std::endl;
	
	return 0;
}
