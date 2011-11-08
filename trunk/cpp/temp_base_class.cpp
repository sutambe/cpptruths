#include <cstdio>
#include <algorithm>
#include <iterator>
#include <iostream>

class Matrix;

void do_addition(int * dest, const int * src1, const int * src2, size_t dim)
{
  for(size_t i = 0; i < dim * dim; ++i, ++dest, ++src1, ++src2)
    *dest = *src1 + *src2;
}

void do_self_addition(int * dest, const int * src, size_t dim)
{
  for(size_t i = 0; i < dim * dim; ++i, ++dest, ++src)
    *dest += *src;
}

class TMatrix 
{
  size_t dim;
  int * data;
  bool freeable;
 
  void real_destroy();
 
public:
  TMatrix(size_t d);
  TMatrix(const TMatrix & tm);
  TMatrix(const Matrix & m);
  TMatrix & operator = (const Matrix & m);
  TMatrix & operator = (const TMatrix & tm);
  TMatrix & operator + (const Matrix & m);
  TMatrix & operator + (TMatrix tm);
  ~TMatrix();

  void swap(TMatrix &) throw();
  void print();
  
  friend class Matrix;
};

class Matrix : public TMatrix 
{
public:
  Matrix(size_t dim);
  Matrix(const Matrix & tm);
  Matrix(const TMatrix & tm);
  Matrix & operator = (const Matrix & m);
  Matrix & operator = (const TMatrix & tm);
  TMatrix operator + (const Matrix & m) const;
  TMatrix operator + (const TMatrix & tm) const;
  ~Matrix();
};


/***** Implementation *****/

TMatrix::TMatrix(size_t d) 
: dim(d), data (new int[dim*dim]()), freeable(0)
{
  for(size_t i = 0;i < dim * dim; ++i)
    data[i] = i*i;
}

TMatrix::TMatrix(const TMatrix & tm)
: dim(tm.dim), data(tm.data), freeable(0)
{}

TMatrix::TMatrix(const Matrix & m)
: dim(m.dim), data(new int[dim*dim]), freeable(0)
{
  std::copy(data, data + dim*dim, m.data);
}

TMatrix & TMatrix::operator = (const Matrix & m)
{
  TMatrix temp(m);
  temp.swap(*this);
  temp.freeable = 1;
  return *this;
}

TMatrix & TMatrix::operator = (const TMatrix & tm)
{
  real_destroy();
  dim = tm.dim;
  data = tm.data;
  freeable = tm.freeable;
  return *this;
}

TMatrix & TMatrix::operator + (const Matrix & m)
{
  do_self_addition(this->data, m.data, dim);
  return *this;
}

TMatrix & TMatrix::operator + (TMatrix tm)
{
  do_self_addition(this->data, tm.data, dim);
  tm.real_destroy();
  return *this;
}

TMatrix::~TMatrix() 
{
  if(freeable) real_destroy();
}

void TMatrix::swap(TMatrix & tm) throw()
{
  std::swap(dim, tm.dim);
  std::swap(data, tm.data);
  std::swap(freeable, tm.freeable);
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

Matrix::Matrix(const Matrix & m)
: TMatrix(m)
{}

Matrix & Matrix::operator = (const Matrix & m)
{
  Matrix temp(m);
  temp.swap(*this);
  return *this;
}

Matrix & Matrix::operator = (const TMatrix & tm)
{
  return static_cast<Matrix &> (TMatrix::operator = (tm));
}

TMatrix Matrix::operator + (const Matrix & m) const
{
  TMatrix temp_result(this->dim);
  do_addition(temp_result.data, this->data, m.data, dim);
  return temp_result;
}

TMatrix Matrix::operator + (const TMatrix & tm) const 
{
  TMatrix temp_result(tm);
  do_addition(temp_result.data, this->data, tm.data, dim);
  return temp_result;
}

Matrix::~Matrix() 
{
  freeable = 1;
}

void TMatrix::print()
{
  std::copy(data, data + (dim*dim), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

int main(void)
{
  const int DIMS = 3;
  Matrix m1(DIMS), m2(DIMS), m3(DIMS);
  
  Matrix result = (m1 + m2) + m3;
  result.print(); 
  
  result = m1 + (m2 + m3);
  result.print(); 
  
  result = (m1 + m3) + (m2 + m3);
  result.print(); 
  
  result = ((m1 + m3) = (m2 + m3));
  result.print(); 
}
