#pragma once
#include "Vector.h"
class Matrix {
public:
	Matrix(); 
	Matrix(int size); //матрица размером size, состоящая из нулей
	Matrix(int size, double value); //матрица размером size, все значения которого равны value
	Matrix(int size, double ** array); //матрица, значения которой конструируются из массива
	Matrix(Matrix& matrix); //конструктор копирования
	~Matrix();

	//основные арифметические операции с матрицами
	void operator+=(Matrix& matrix);
	void operator-=(Matrix& matrix);
	void operator*=(Matrix& matrix);
	void operator*=(double& number);
	
	Matrix operator=(Matrix& matrix);
	Matrix operator+(Matrix& matrix);
	Matrix operator-(Matrix& matrix);
	Matrix operator*(Matrix& matrix);
	Vector operator*(Vector& vector);
	Matrix operator*(double& number);

	//транспонирование и норма
	Matrix transpose();
	double Frobenius(); 

	int getSize() { return size; }
	double getValue(int row, int column);
	void setValue(int row, int column, double value);

	//для вывода на экран
	void toString();
private:
	double** values; //двумерный массив, в котором будет храниться содержимое матрицы
	int size;
};

