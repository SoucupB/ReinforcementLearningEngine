#pragma once
#include <vector>
#include <stdio.h>
#include <math.h>
#include <cstring>
class Matrix {
private:
    double getSignPer(int per);
public:
    int height, width;
    static double sgn(double an);
    std::vector<double> matr;
    Matrix(int height, int width, const char *firstAs, double deviation);
    void show();
    double getElem(int x, int y);
    Matrix operator + (Matrix const &summ);
    static Matrix matMul(Matrix a, Matrix b);
    void func(double (*fun)(double));
    void deepcopy(Matrix mat);
    void atribMatrix(std::vector<double> vect);
    Matrix operator -(Matrix const &den);
    Matrix operator -=(Matrix const &den);
    static Matrix scalarProd(Matrix mat, double scalar);
    static Matrix hdProduct(Matrix a, Matrix b);
    static Matrix sgnhdProduct(Matrix a, Matrix b);
    void transpose();
    void mat_clear();
    int get_height();
};
