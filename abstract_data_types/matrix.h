
#include <iosfwd>

class Matrix {
 public:
    Matrix() = default;
    Matrix(const ptrdiff_t row, const ptrdiff_t col);
    Matrix(const Matrix &mat);
    ~Matrix();
    ptrdiff_t const rows() const { return row_; }
    ptrdiff_t const columns() const { return col_; }
    int& get (const ptrdiff_t i, const ptrdiff_t j) const;
    int& get (const ptrdiff_t i, const ptrdiff_t j);
    Matrix& operator=(const Matrix& mat);

 private:
    ptrdiff_t row_{ 0 };
    ptrdiff_t col_{ 0 };
    int **data_{ nullptr };
};

