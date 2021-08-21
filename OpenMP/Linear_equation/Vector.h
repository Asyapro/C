#pragma once
class Vector {
public:
	Vector();
	Vector(int size); //вектор размером size, состоящий из нулей
	Vector(int size, double value); //вектор размером size, все значения которого равны value
	Vector(int size, double * array); //вектор, значения которого конструируются из массива
	Vector(Vector& Vector); //конструктор копирования
	~Vector();

	//основные арифметические операции с векторами
	void operator+=(Vector& Vector);
	void operator-=(Vector& Vector);
	void operator*=(double& number);

	Vector operator=(Vector& Vector);
	Vector operator+(Vector& Vector);
	Vector operator-(Vector& Vector);
	Vector operator*(double& number); 
	
	//скалярное произведение и вычисление длины
	double scalar_multiplication(Vector& vector);
	double Lenght();

	int getRows() { return rows; }
	double getValue(int column);
	void setValue(int column, double value);

	//для вывода на экран
	void toString();
private:
	double* values; //одномерный массив, в котором будет храниться содержимое вектора
	int rows; //вектор предполагается размерности nx1, поскольку в классе Matrix должно поддерживаться умножение Ax
};

