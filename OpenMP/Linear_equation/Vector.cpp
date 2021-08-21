#include "pch.h"
#include "Vector.h"
#include "Arithmetic_error.cpp"
#include <omp.h>
#include <iostream>
#include <cmath>
#include "omp.h"

/*������������*/
//���������
Vector::Vector() {}
//������ �������� size, ��������� �� �����
Vector::Vector(int row) {
	if (row < 0) {
		throw std::out_of_range("Vector's size can not be less zero");
	}
	this->rows = row;
	values = new double[row];

	for (int i = 0; i < row; i++) {
		values[i] = 0.0;
	}

}
//������ �������� size, ��� �������� �������� ����� value
Vector::Vector(int row, double value) {
	if (row < 0) {
		throw std::out_of_range("Vector's size can not be less zero");
	}
	this->rows = row;
	values = new double[row];

	for (int i = 0; i < row; i++) {
		values[i] = value;
	}

}
//������, �������� �������� �������������� �� �������
Vector::Vector(int size, double* array) {
	if (size < 0) {
		throw std::out_of_range("Vector's size can not be less zero");
	}
	this->rows = size;
	values = new double[size];

	for (int i = 0; i < size; i++) {
		values[i] = array[i];
	}
}
//����������� �����������
Vector::Vector(Vector& Vector) {
	rows = Vector.getRows();
	values = new double[rows];

	for (int i = 0; i < rows; i++) {
		values[i] = Vector.getValue(i);
	}
}

Vector::~Vector() {
	delete[] values;
}

/*�������� �������������� ��������*/
Vector Vector::operator=(Vector& vector) {
	if (this == &vector) {
		return *this; //��� ��������� ������� ���������� ������� ������ ����
	}
	rows = vector.getRows();
	values = new double[rows];

	for (int i = 0; i < rows; i++) {
		values[i] = vector.getValue(i);
	}
	return *this;
}

void Vector::operator+=(Vector& vector) {
	for (int i = 0; i < vector.getRows(); ++i) {
		values[i] += vector.getValue(i);
	}
}

void Vector::operator-=(Vector& vector) {
	for (int i = 0; i < vector.getRows(); ++i) {
		values[i] -= vector.getValue(i);
	}
}

void Vector::operator*=(double number) {
	for (int i = 0; i < rows; ++i) {
		values[i] *= number;
	}
}

Vector Vector::operator+(Vector& vector) {
	Vector result(*this);
	result += vector;
	return result;
}

Vector Vector::operator-(Vector& vector) {
	Vector result(*this);
	result -= vector;
	return result;
}

Vector Vector::operator*(double number) {
	Vector result(*this);
	result *= number;
	return result;
}

double Vector::scalar_multiplication(Vector& vector) {
	if (rows != vector.getRows()) {
		throw Arithmetic_error("Matrixes must have the same size");
	}

	double result = 0.0;
	for (int i = 0; i < rows; ++i) {
		result += values[i] * vector.getValue(i);
	}
	return result;
}

double Vector::Lenght() {
	double result = 0.0;
	for (int i = 0; i < rows; i++) {
		result += values[i] * values[i];
	}
	return sqrt(result);
}

double Vector::getValue(int row) {
	if (row < 0 || row >= rows) {
		throw std::out_of_range("Index is more than size or less zero");
	}
	return values[row];
}

void Vector::setValue(int row, double value) {
	if (row < 0 || row >= rows) {
		throw std::out_of_range("Index is more than size or less zero");
	}
	values[row] = value;
}

void Vector::toString() {
	for (int i = 0; i < getRows(); i++) {
		std::cout << values[i] << " ";
	}
	std::cout << "\n";
}