#pragma once
#include <vector>
#include <iostream>
#include <initializer_list>

template<typename MatrixType>

class Matrix
{
private:
    int col, row;
    std::vector<std::vector<MatrixType>> matrix;
public:
    Matrix() : col(0), row(0) {}

    Matrix(int size) : col(size), row(size) {
        matrix.resize(col, std::vector<MatrixType>(row));
    }

    Matrix(int colum, int rows) : col(colum), row(rows) {
        matrix.resize(row, std::vector<MatrixType>(col));
    }

    Matrix(std::initializer_list<MatrixType> list) {
        row = 1;
        col = static_cast<int>(list.size());
        matrix.push_back(std::vector<MatrixType>(list));
    }

    Matrix(std::initializer_list<std::initializer_list<MatrixType>> list) {
        row = static_cast<int>(list.size());
        col = list.begin()->size();
        for (const auto& sublist : list) {
            if (sublist.size() != col) {
                throw std::invalid_argument("All rows must have the same number of elements.");
            }
            matrix.emplace_back(sublist);
        }
    }

    Matrix (const Matrix&) = default;
    
    std::vector<MatrixType>& operator[](int index) {
        return matrix[index];
    }

    const std::vector<MatrixType>& operator[](int index) const {
        return matrix[index];
    }

    Matrix& operator=(const Matrix& temp) {
        if(this != &temp) {
            this->col = temp.col;
            this->row = temp.row;
            this->matrix = temp.matrix;
        }
        return *this;
    }

    Matrix& operator+=(const Matrix& temp) {
        if(temp.col != this->col or temp.row != this->row) {
            throw std::logic_error("Matrixes sizes are not equal");
        }
        for(int i=0; i<row; i++) {
            for(int j=0; j<col; j++) {
                matrix[i][j] += temp[i][j];
            }
        }
        
        return *this;
    }

    Matrix& operator-=(const Matrix& temp) {
        if(temp.col != this->col or temp.row != this->row) {
            throw std::logic_error("Matrixes sizes are not equal");
        }
        for(int i=0; i<row; i++) {
            for(int j=0; j<col; j++) {
                matrix[i][j] -= temp[i][j];
            }
        }
        
        return *this;
    }

    Matrix& operator*=(const Matrix& temp) {
        if(this->col != temp.row) {
            throw std::logic_error("Matrixes sizes are not equal");
        }
        Matrix result(this->row, temp.col);
        for (int i = 0; i < this->row; ++i) {
            for (int j = 0; j < temp.col; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < this->col; ++k) {
                    result[i][j] += this->matrix[i][k] * temp.matrix[k][j];
                }
            }
        }        
        *this = std::move(result);
        return *this;
    }

    friend Matrix operator+(const Matrix& m1,const Matrix& m2) {
        if(m1.col != m2.col or m1.row != m2.row) {
            throw std::logic_error("Matrixes sizes are not equal");
        }
        Matrix result = m1;
        result += m2;
        return result;
    }

    friend Matrix operator-(const Matrix& m1, const Matrix& m2) {
        if(m1.col != m2.col or m1.row != m2.row) {
            throw std::logic_error("Matrixes sizes are not equal");
        }
        Matrix result = m1;
        result -= m2;
        return result;
    }

    friend Matrix operator*(const Matrix& m1, const Matrix& m2) {
        if(m1.col != m2.row) {
            throw std::logic_error("Matrixes sizes are not equal");
        }
        Matrix result = m1;
        result *= m2;
        return result;
    }  

    friend std::ostream& operator<<(std::ostream& out, Matrix& temp) {
        for(int i=0; i<temp.row; i++) {
            for(int j=0; j<temp.col; j++) {
                out << temp.matrix[i][j] << " ";
            }
            out << "\n";
        }
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Matrix& temp) {
        std::cout<<"Enter rows size: ";
        in>>temp.row;
        std::cout<<"Enter colums size: ";
        in>>temp.col;
        temp.matrix.resize(temp.row, std::vector<MatrixType>(temp.col));
        for(int i=0; i<temp.row; i++) {
            for(int j=0; j<temp.col; j++) {
                in>>temp.matrix[i][j];
            }
        }
        return in;
    }

    void Transpose() {
        Matrix result(this->col, this->row);
        for(int i=0; i<this->row; i++) {
            for(int j=0; j<this->col; j++) {
                result[j][i] = matrix[i][j];
            }
        }
        this->col = result.row;
        this->row = result.col;
        this->matrix = move(result.matrix);
    }

    MatrixType Determinant() const {
        if (row != col) {
            throw std::logic_error("Matrix isn't square");
        }

        Matrix<MatrixType> temp = *this;
        MatrixType det = 1;
        int n = row;
        int swapCount = 0;

        for (int i = 0; i < n; ++i) {
            int pivot = i;
            for (int j = i + 1; j < n; ++j) {
                if (std::abs(temp[j][i]) > std::abs(temp[pivot][i])) {
                    pivot = j;
                }
            }

            if (temp[pivot][i] == 0) {
                return 0;
            }

            if (i != pivot) {
                std::swap(temp[i], temp[pivot]);
                swapCount++;
            }

            for (int j = i + 1; j < n; ++j) {
                MatrixType factor = temp[j][i] / temp[i][i];
                for (int k = i; k < n; ++k) {
                    temp[j][k] -= factor * temp[i][k];
                }
            }
        }
    
        for (int i = 0; i < n; ++i) {
            det *= temp[i][i];
        }

        if (swapCount % 2 != 0) {
            det = -det;
        }

        return det;
    }
};