//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    April 4th 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef MATRIX_HH
#define MATRIX_HH

#include <iostream>
#include <iomanip>

#include "userconst.hh"
#include "complex.hh"
#include "vector.hh"

#ifndef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport) 
#else
  #define EXPORT_CLASS_DECLSPEC class
#endif

template<class TYPE>
EXPORT_CLASS_DECLSPEC CMatrix
//=========================================================================
// = FUNCTION 
//     matrix.hh    
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     operations for mxn-matrix of the form (A_1,...,A_m) Where A_i are
//     vectors
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-1999 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//=========================================================================
{
  public:
    TYPE **x_;
    int  m_;
    int  n_;

    CMatrix()
    {
      //   cout << " Constructing CMatrix!!" << endl;
      m_ = 0;
      n_ = 0;
    }

    CMatrix( TYPE **x, int m, int n )
    {
      int i, j;
      m_ = m;
      n_ = n;

      x_ = new TYPE* [m_];
      for ( i = 0; i < m_; i++ )
        x_[i] = new TYPE [n_];

      for ( i = 0; i < m_; i++ )
        for ( j = 0; j < n_; j++ )
          x_[i][j] = x[i][j];
    }

    CMatrix( int m, int n )
    {
	  int i, j;
      // default constructor
      m_ = m;
      n_ = n;

      x_ = new TYPE* [m_];
      for ( i = 0; i < m_; i++ )
        x_[i] = new TYPE [n_];

      for ( i = 0; i < m_; i++ )
        for ( j = 0; j < n_; j++ )
          x_[i][j] = 0.0;
    }

    void set( TYPE x, int i, int j )
    {
      x_[i][j] = x;
    }

    int rowSize()    { return m_; }
    int columnSize() { return n_; }

    CMatrix( const CMatrix<TYPE>& v1 );
    // Copy constructor 

    CMatrix<TYPE>&  operator=( const CMatrix<TYPE>& v1 );
    //
    // Overload = operator
    //

    CMatrix<TYPE>& transpose() const;
    CVector<TYPE>  transpose( const CVector<TYPE>& b ) const;
    //
    // transpose of a matrix or vector by matrix vector multiplication
    //

    //boolean   operator==( const CMatrix<TYPE>& v1 ) const;
    //
    // Overload == operator
    //

    //boolean   operator!=( const CMatrix<TYPE>& v1 ) const;
    //
    // Overload != operator
    //

    //CMatrix<TYPE> operator-( const CMatrix<TYPE>& v1 ) const;
    //
    // Overload - operator
    //

    //CMatrix<TYPE>  operator+( const CMatrix<TYPE>& v1 ) const;
    //
    // Overload + operator
    //

    //CMatrix<TYPE> operator*( const double& d1 ) const;
    //CMatrix<TYPE> operator*( const double& d1 );
    //
    // Overloaded multiplication by a scalar operator
    //

    CVector<TYPE>    operator*( const CVector<TYPE>& v ) const;
    //
    // Overloaded multiplication by a vector to form a new vector 
    //

    TYPE* operator[]( const int& i ) const;
    //
    // point into matrix 
    //

    //TYPE abs() const; 
    //TYPE abs( const CMatrix<TYPE>& p ) const;
    //
    // Define a norm on the point (distance from an origin).
    //

    //CMatrix<TYPE> unitMatrix() const;
    //
    // create unit vector from current vector 3d point
    //

    void clear()
    {
      if ( n_ > 0 && m_ > 0 && m_ < 100000 && n_ < 100000 )
      {
        for ( int i = m_; i > 0; i-- )
	  delete [] x_[i-1];
        delete [] x_;
      }
      n_ = 0;
      m_ = 0;
    };
    //
    // destroys dynamically allocated matrix elements
    //

    ~CMatrix<TYPE>() 
    {
      clear();
    };
};

template<class TYPE>
CMatrix<TYPE>& CMatrix<TYPE>::operator=( const CMatrix<TYPE>& v1 )
{
  int i, j;
  if ( this->x_ != v1.x_ )
  {
    clear();

    m_ = v1.m_;
    n_ = v1.n_;

    x_ = new TYPE* [m_];
    for ( i = 0; i < m_; i++ )
      x_[i] = new TYPE [n_];

    for ( i = 0; i < m_; i++ )
      for ( j = 0; j < n_; j++ )
        x_[i][j] = v1.x_[i][j];
  }

  return *this;
}

template<class TYPE>
CMatrix<TYPE>& CMatrix<TYPE>::transpose() const
{
  int i, j;
  CMatrix<TYPE> *M;

  M = new CMatrix<TYPE>;

  M->m_ = n_;
  M->n_ = m_;

  M->x_ = new TYPE* [m_];
  for ( i = 0; i < M->m_; i++ )
    M->x_[i] = new TYPE [M->n_];

  for ( i = 0; i < M->m_; i++ )
    for ( j = 0; j < M->n_; j++ )
      M->x_[i][j] = x_[j][i];

  return *M;
}

template<class TYPE>
CVector<TYPE> CMatrix<TYPE>::transpose( const CVector<TYPE>& b ) const
{
  int  i, j;
  TYPE *x;

  x = new TYPE [n_];

  for ( j = 0; j < n_; j++ )
    for ( i = 0; i < m_; i++ )
      x[j] = x[j] + x_[i][j] * b[i];

  return CVector<TYPE>( x, n_ );
}

template<class TYPE>
CMatrix<TYPE>::CMatrix( const CMatrix<TYPE>& v1 )
{
  int i, j;

  m_ = v1.m_;
  n_ = v1.n_;

  x_ = new TYPE* [m_];
  for ( i = 0; i < m_; i++ )
    x_[i] = new TYPE [n_];

  for ( i = 0; i < m_; i++ )
    for ( j = 0; j < n_; j++ )
      x_[i][j] = v1.x_[i][j];
}

template<class TYPE>
TYPE* CMatrix<TYPE>::operator[]( const int& i ) const
{
  return x_[i];
}

/*template<class TYPE>
boolean CMatrix<TYPE>::operator!=( const CMatrix<TYPE>& v1 ) const
{
  for ( int i = 0; i < v1.n_; i++ )
    if ( fabs( x_[i] - v1.x_[i] ) > TOL )
      return true;

  return false;
}

template<class TYPE>
CMatrix<TYPE> CMatrix<TYPE>::operator-( const CMatrix<TYPE>& v1 ) const
{
  CMatrix<TYPE> v2;

  v2 = new TYPE [v1.n_];

  for ( int i = 0; i < v1.n_; i++ )
    v2.x_[i] = x_[i] - v1.x_[i];

  return v2;
}

template<class TYPE>
CMatrix<TYPE> CMatrix<TYPE>::operator+( const CMatrix<TYPE>& v1 ) const
{
  CMatrix<TYPE> v2;

  v2 = new TYPE [v1.n_];

  for ( int i = 0; i < v1.n_; i++ )
    v2.x_[i] = x_[i] + v1.x_[i];

  return v2;
}

template<class TYPE>
CMatrix<TYPE> CMatrix<TYPE>::operator*( const double& dd ) const
{
  CMatrix<TYPE> v2;

  v2 = new TYPE [n_];

  for ( int i = 0; i < n_; i++ )
    v2.x_[i] = x_[i] * dd;
  return v2;
}*/

template<class TYPE>
CVector<TYPE> CMatrix<TYPE>::operator*( const CVector<TYPE>& v ) const
{
  int  i, j;
  TYPE *x;

  x = new TYPE [m_];
  for ( i = 0; i < m_; i++ )
    x[i] = 0.0;

  for ( i = 0; i < m_; i++ )
    for ( j = 0; j < n_; j++ )
      x[i] += x_[i][j] * v.x_[j];
  
  //for ( i = 0; i < m_; i++ )
    //cout << "x at " << i << ": " << x[i] << " ->" << x_[i][0] << " " << x_[i][1] << endl;

  return( CVector<TYPE>( x, m_ ) );
}

/*template<class TYPE>
CMatrix<TYPE> CMatrix<TYPE>::operator/( const double& dd ) const
{
  CMatrix<TYPE> v2;

  v2 = new TYPE [n_];

  for ( int i = 0; i < n_; i++ )
    v2.x_[i] = x_[i] / dd;
  return v2;
}

template<class TYPE>
CMatrix<TYPE> CMatrix<TYPE>::unitMatrix() const
{
  CMatrix<TYPE> p2;

  p2 = *this / ( this->abs() );
  return p2;
}

template<class TYPE>
TYPE  CMatrix<TYPE>::abs( const CMatrix<TYPE>& p ) const
{
  TYPE T = 0.0;

  for ( int i = 0; i < n_; i++ )
    T += qsqr( x_[i] - p.x_[i] );

  return sqrt( T );
}*/

#endif
