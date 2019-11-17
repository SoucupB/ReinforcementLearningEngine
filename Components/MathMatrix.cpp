#include "MathMatrix.h"
double Matrix::getSignPer(int per)
{
    int resp = (rand() * rand()) % 101;
    return resp <= per ? -1 : 1;
}
double Matrix::sgn(double an)
{
    return an < 0 ? -1 : 1;
}
Matrix::Matrix(int height, int width, const char *firstAs, double deviation)
{
    this->height = height;
    this->width = width;
    if(!strcmp(firstAs, "random"))
    {
        for(int i = 0; i < height * width; i++)
        {
            this->matr.push_back(getSignPer(0) * 100.0 / (pow(10.0, deviation) + (rand() * rand()) % int(pow(10.0, deviation + 1))));
        }
    }
    if(!strcmp(firstAs, "zero"))
    {
        for(int i = 0; i < height * width; i++)
        {
            this->matr.push_back(0.0);
        }
    }
}
void Matrix::show()
{
    for(int i = 0; i < this->height; i++)
    {
        for(int j = 0; j < this->width; j++)
            printf("%f ", matr[i * this->width + j]);
        printf("\n");
    }
}
double Matrix::getElem(int x, int y)
{
    return this->matr[x * this->width + y];
}
void Matrix::mat_clear() {
    this->matr.clear();
}
int Matrix::get_height() {
    return this->height;
}
Matrix Matrix::operator + (Matrix const &summ)
{
    if(this->height != summ.height || this->width != summ.width)
    {
        //throw std::invalid_argument("matrices size's are incorrect for adding");
        printf("matrices size's are incorrect for adding");
    }
    Matrix rez = *this;
    for(int i = 0; i < this->matr.size(); i++)
    {
        rez.matr[i] += summ.matr[i];
    }
    return rez;
}
Matrix Matrix::matMul(Matrix a, Matrix b)
{
    if(a.width != b.height)
    {
        //throw std::invalid_argument("matrices size's are incorrect for multypling");
        printf("matrices size's are incorrect for multypling");
    }
    int rezIndex = 0;
    const char* zeros = "zero";
    Matrix result = Matrix(a.height, b.width, zeros, 0);
    for(int i = 0; i < a.height; i++)
    {
        for(int j = 0; j < b.width; j++)
        {
            double rowCol = 0.0;
            for(int k = 0; k < a.width; k++)
            {
                rowCol += a.matr[i * a.width + k] * b.matr[j + k * b.width];
            }
            result.matr[rezIndex++] = rowCol;
        }
    }
    return result;
}
void Matrix::func(double (*fun)(double))
{
    for(int i = 0; i < this->matr.size(); i++)
        this->matr[i] = (*fun)(this->matr[i]);
}
void Matrix::deepcopy(Matrix mat)
{
    this->height = mat.height;
    this->width = mat.width;
    matr.clear();
    for(int i = 0; i < mat.matr.size(); i++)
        matr.push_back(mat.matr[i]);
}
void Matrix::atribMatrix(std::vector<double> vect)
{
    for(int i = 0; i < vect.size(); i++)
    {
        this->matr[i] = vect[i];
    }
}
Matrix Matrix::operator -(Matrix const &den)
{
    if(this->height != den.height || this->width != den.width)
    {
       // throw std::invalid_argument("matrices size's are incorrect for adding");
       printf("matrices size's are incorrect for adding");
    }
    Matrix rez = *this;
    for(int i = 0; i < den.matr.size(); i++)
    {
        rez.matr[i] -= den.matr[i];
    }
    return rez;
}
Matrix Matrix::operator -=(Matrix const &den)
{
    if(this->height != den.height || this->width != den.width)
    {
        //throw std::invalid_argument("matrices size's are incorrect for adding");
        printf("matrices size's are incorrect for subtracting");
    }
    Matrix rez = *this;
    for(int i = 0; i < den.matr.size(); i++)
    {
        rez.matr[i] -= den.matr[i];
    }
    return rez;
}
Matrix Matrix::scalarProd(Matrix mat, double scalar)
{
    for(int i = 0; i < mat.matr.size(); i++)
        mat.matr[i] *= scalar;
    return mat;
}
Matrix Matrix::hdProduct(Matrix a, Matrix b)
{
    if(a.height != b.height)
    {
      //  throw std::invalid_argument("matrices size's are incorrect for adding");
      printf("matrices size's are incorrect for adding");
    }
    for(int i = 0; i < a.matr.size(); i++)
    {
        a.matr[i] *= b.matr[i];
    }
    return a;
}
Matrix Matrix::sgnhdProduct(Matrix a, Matrix b)
{
    if(a.height != b.height)
    {
       // throw std::invalid_argument("matrices size's are incorrect for adding");
       printf("matrices size's are incorrect for adding");
    }
    for(int i = 0; i < a.matr.size(); i++)
    {
        a.matr[i] = Matrix::sgn(a.matr[i]) * b.matr[i] * a.matr[i];
    }
    return a;
}
void Matrix::transpose()
{
    std::swap(this->height, this->width);
}