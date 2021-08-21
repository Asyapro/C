#include "matrix.h"
#include <iostream>
#include <utility>
#include <sstream>

using namespace std;

Matrix::Matrix(const ptrdiff_t row, const ptrdiff_t col) {
    if (row <= 0 || col <= 0) {
        throw invalid_argument("Not Matrix!");
    }
    row_ = row;
    col_ = col;
    data_ = new int*[row_];
    for (int i = 0; i < row_; i++) {
        data_[i] = new int[col_];
        for (int j = 0; j < col_; j++) {
            data_[i][j] = 0;
        }
    }
}

Matrix::Matrix(const Matrix &mat) {
    row_ = mat.row_;
    col_ = mat.col_;
    data_ = new int*[row_];
    for (int i = 0; i < row_; i++) {
        data_[i] = new int[col_];
        for (int j = 0; j < col_; j++)
            data_[i][j] = mat.data_[i][j];
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < row_; i++) {
        delete[] data_[i];
    }
    delete[] data_;
}

Matrix& Matrix::operator=(const Matrix& mat) {
    if (&mat == this) {
        return *this;
    }
    Matrix temp = Matrix(mat);
    swap(temp.col_, col_);
    swap(temp.row_, row_);
    swap(temp.data_, data_);
    return (*this);
}

int& Matrix::get(const ptrdiff_t i, const ptrdiff_t j) {
    if (i >= row_ || i < 0 || j >= col_ || j < 0) {
        throw out_of_range("Out of range!");
    }
    return data_[i][j];
}

int& Matrix::get(const ptrdiff_t i, const ptrdiff_t j) const {
    if (i >= row_ || i < 0 || j >= col_ || j < 0) {
        throw out_of_range("Out of range!");
    }
    return data_[i][j];
}
