#include "pch.h"
#include "Matrix.h"
#include "Arithmetic_error.cpp"
#include <omp.h>
#include <iostream>
#include <cmath>

/*конструкторы*/

//дефолтный
Matrix::Matrix() {}
//матрица размером size, состоящая из нулей
Matrix::Matrix(int size) {
	if (size < 0) {
		throw std::out_of_range("Matrix's size can not be less or equal zero");
	}
	this->size = size;
	values = new double*[size];

	for (int i = 0; i < size; i++) {
		values[i] = new double[size];
		for (int j = 0; j < size; j++)
			values[i][j] = 0.0;
	}
}
//матрица размером size, все значения которого равны value
Matrix::Matrix(int size, double value) {
	if (size < 0) {
		throw std::out_of_range("Matrix's size can not be less or equal zero");
	}

	this->size = size;
	values = new double*[size];

	for (int i = 0; i < size; i++) {
		values[i] = new double[size];
		for (int j = 0; j < size; j++) {
			values[i][j] = value;
		}
	}
}
//матрица, значения которой конструируются из массива
Matrix::Matrix(int size, double** array) {
	if (size < 0) {
		throw std::out_of_range("Matrix's size can not be less or equal zero");
	}
	this->size = size;
	values = new double*[size];

	for (int i = 0; i < size; i++) {
		values[i] = new double[size];
		for (int j = 0; j < size; j++) {
			values[i][j] = array[i][j];
		}
	}
}
//конструктор копирования
Matrix::Matrix(Matrix& matrix) {

	this->size = matrix.size;
	this->values = new double*[size];

	for (int i = 0; i < size; ++i) {
		this->values[i] = new double[size];
		for (int j = 0; j < size; ++j) {
			this->values[i][j] = matrix.values[i][j];
		}
	}
}

Matrix::~Matrix() {

	if (size > 0) {
		for (int i = 0; i < size; ++i) {
			delete[] values[i];
		}
		delete[] values;

		size = 0;
	}
}

/*основные арифметические операции*/
Matrix Matrix::operator=(Matrix& matrix) {
	if (this == &matrix) {
		return *this; //для избежания попытки присвоения матрицы самой себе
	}
	/*if (size > 0) {
		for (int i = 0; i < size; i++) {
			delete[] values[i];
		}
		delete[] values;
	}*/

	this->size = matrix.size;
	this->values = new double*[size];
	for (int i = 0; i < size; i++) {
		this->values[i] = new double[size];
		for (int j = 0; j < size; j++) {
			this->values[i][j] = matrix.values[i][j];
		}
	}

	return *this;
}

void Matrix::operator+=(Matrix& matrix) {
#pragma omp for nowait
	for (int i = 0; i < matrix.getSize(); ++i) {
		for (int j = 0; j < matrix.getSize(); ++j) {
			values[i][j] += matrix.getValue(i, j);
		}
	}
}
void Matrix::operator-=(Matrix& matrix) {
#pragma omp for nowait
	for (int i = 0; i < matrix.getSize(); ++i) {
		for (int j = 0; j < matrix.getSize(); ++j) {
			values[i][j] -= matrix.getValue(i, j);
		}
	}
}

void Matrix::operator*=(Matrix& matrix) {
	if (size != matrix.getSize()) {
		throw Arithmetic_error("Matrixes must have the same size");
	}

	Matrix temp(this->getSize()); //временная матрица для записи результата умножения

#pragma omp for nowait
	for (int i = 0; i < temp.getSize(); ++i) {
		for (int j = 0; j < temp.getSize(); ++j) {
			double sum = 0.0;
			for (int k = 0; k < temp.getSize(); ++k) {
				sum += this->getValue(i, k) * matrix.getValue(k, j);
			}
			temp.setValue(i, j, sum);
		}
	}

	*this = temp;
}

void Matrix::operator*=(double& number) {
#pragma omp for nowait
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			values[i][j] *= number;
		}
	}

}

Matrix Matrix::operator+(Matrix& matrix) {
	Matrix result(*this);
	result += matrix;
	return result;
}

Matrix Matrix::operator-(Matrix& matrix) {
	Matrix result(*this);
	result -= matrix;
	return result;
}

Matrix Matrix::operator*(Matrix& matrix) {
	Matrix result(*this);
	result *= matrix;
	return result;
}

Vector Matrix::operator*(Vector& vector) {
	Vector result(size);
	if (size != vector.getRows()) {
		throw Arithmetic_error("Matrixes must have the same size");
	}	

	#pragma omp for nowait
	for (int i = 0; i < size; ++i) {
		double sum = 0.0;
		for (int j = 0; j < size; ++j) {
			sum += this->values[i][j] * vector.getValue(j);
		}
		result.setValue(i, sum);
	}

	return result;
}


Matrix Matrix::operator*(double& number) {
	Matrix result(*this);
	result *= number;
	return result;

}

//транспонирование
Matrix Matrix::transpose() {
	Matrix result(getSize());

#pragma omp for nowait
	for (int i = 0; i < getSize(); i++) {
		for (int j = 0; j < getSize(); j++) {
			result.setValue(i, j, values[j][i]);
		}
	}

	return result;
}

//норма
double Matrix::Frobenius() {
	double result = 0.0;

#pragma omp parallel for reduction(+:result)
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			result += values[i][j] * values[i][j];
		}
	}

	return sqrt(result);
}

double Matrix::getValue(int row, int column) {
	if (row < 0 || row >= size || column < 0 || column >= size) {
		throw std::out_of_range("Index is more than size or less zero");
	}
	return values[row][column];
}

void Matrix::setValue(int row, int column, double value) {
	if (row < 0 || row >= size || column < 0 || column >= size) {
		throw std::out_of_range("Index is more than size or less zero");
	}
	values[row][column] = value;
}

//для вывода на экран
void Matrix::toString() {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			std::cout << values[i][j] << " ";
		}
		std::cout << "\n";
	}
}
