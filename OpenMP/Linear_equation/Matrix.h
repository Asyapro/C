#pragma once
#include "Vector.h"
class Matrix {
public:
	Matrix(); 
	Matrix(int size); //������� �������� size, ��������� �� �����
	Matrix(int size, double value); //������� �������� size, ��� �������� �������� ����� value
	Matrix(int size, double ** array); //�������, �������� ������� �������������� �� �������
	Matrix(Matrix& matrix); //����������� �����������
	~Matrix();

	//�������� �������������� �������� � ���������
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

	//���������������� � �����
	Matrix transpose();
	double Frobenius(); 

	int getSize() { return size; }
	double getValue(int row, int column);
	void setValue(int row, int column, double value);

	//��� ������ �� �����
	void toString();
private:
	double** values; //��������� ������, � ������� ����� ��������� ���������� �������
	int size;
};

