#include <cstdio>
#include <algorithm>
#include <iterator>
#include <iostream>

class Matrix;

void do_addition(int * dest, const int * src1, const int * src2, size_t dim)
{
  for(int i = 0;i < dim * dim; ++i, ++dest, ++src1, ++src2)
    *dest = *src1 + *src2;
}

void do_self_addition(int * dest, const int * src, size_t dim)
{
  for(int i = 0;i < dim * dim; ++i, ++dest, ++src)
    *dest += *src;
}

class TMatrix {
  size_t dim;
  int * data;
  bool freeable;
 
  void real_destroy();
 
public:
  TMatrix(size_t d);
  TMatrix(const TMatrix & tm);
  TMatrix & operator + (const Matrix & m);
  ~TMatrix();
  
  void print();
  
  friend class Matrix;
};

class Matrix : public TMatrix {
public:
  Matrix(size_t dim);
  Matrix(const TMatrix & tm);
  TMatrix operator + (const Matrix & m);
  ~Matrix();
};


/***** Implementation *****/

TMatrix::TMatrix(size_t d) 
: dim(d), data (new int[dim*dim]()), freeable(0)
{
  for(int i = 0;i < dim * dim; ++i)
    data[i] = i*i;
}

TMatrix::TMatrix(const TMatrix & tm)
: dim(tm.dim), data(tm.data), freeable(0)
{}

TMatrix & TMatrix::operator + (const Matrix & m)
{
  do_self_addition(this->data, m.data, dim);
  return *this;
}

TMatrix::~TMatrix() 
{
  if(freeable) real_destroy();
}

void TMatrix::real_destroy()
{
  delete [] data;
}

Matrix::Matrix(size_t dim) 
: TMatrix(dim) 
{}

Matrix::Matrix(const TMatrix & tm)
: TMatrix(tm)
{}

TMatrix Matrix::operator + (const Matrix & m) 
{
  TMatrix temp_result(this->dim);
  do_addition(temp_result.data, this->data, m.data, dim);
  return temp_result;
}

Matrix::~Matrix() 
{
  freeable = 0;
}



void TMatrix::print()
{
  std::copy(data, data + (dim*dim), std::ostream_iterator<int>(std::cout, " "));
}



int main(void)
{
  const int DIMS = 3;
  Matrix m1(DIMS), m2(DIMS), m3(DIMS);
  
  Matrix result = m1 + m2 + m3;
  result.print();
}
