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
#ifndef VECTOR_HH
#define VECTOR_HH

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdio.h>

#include "userconst.hh"
#include "complex.hh"

#ifndef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport) 
#else
  #define EXPORT_CLASS_DECLSPEC class
#endif

template<class TYPE>
EXPORT_CLASS_DECLSPEC CVector
//=========================================================================
// = FUNCTION 
//     vector.hh    
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     operations for n-vector of the form (x_1,...,x_n)
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
    TYPE *x_;
    int  n_;

    CVector()
    {
      //   cout << " Constructing CVector!!" << endl;
      n_ = 0;
    }

    CVector( TYPE *y, int n )
    {
      int i;
      n_ = n;
      x_ = new TYPE [n_];
      for ( i = 0; i < n_; i++ )
        x_[i] = y[i];
    }

    CVector( int n )
    {
      int i;
      n_ = n;
      x_ = new TYPE [n_];
      for ( i = 0; i < n_; i++ )
        x_[i] = 0.0;
    }

    CVector( const CVector<TYPE>& v1 );
    // Copy constructor 

    int  size() const { return n_; }
    //
    // return size of the vector
    //

    double euclideanNorm();
    // Added 14/06/02
    // calculate the L2-norm on the vector (euclidean norm)
    //

    void set( const TYPE& T, int i ) { x_[i] = T; }
    void setOnes();
    inline TYPE get( int i ) { return x_[i]; }
    //
    // set or get components of the vector
    //

    CVector<TYPE>&  operator=( const CVector<TYPE>& v1 );
    //
    // Overload = operator
    //

    boolean   operator==( const CVector<TYPE>& v1 ) const;
    //
    // Overload == operator
    //

    boolean   operator!=( const CVector<TYPE>& v1 ) const;
    //
    // Overload != operator
    //

    CVector<TYPE> operator-( const CVector<TYPE>& v1 ) const;
    //
    // Overload - operator
    //

    CVector<TYPE>  operator+( const CVector<TYPE>& v1 ) const;
    //
    // Overload + operator
    //

    CVector<TYPE> operator*( const double& d1 ) const;
    //CVector<TYPE> operator*( const double& d1 );
    //
    // Overloaded multiplication by a scalar operator
    //

    TYPE    operator*( const CVector<TYPE>& v ) const;
    //
    // Overloaded dot product
    //

    CVector<TYPE> operator/( const double& d1 ) const;
    //
    // Overloaded division by a scalar operator
    //

    CVector<TYPE> operator/( const CVector<TYPE>& p ) const;
    //
    // Overloaded division by a scalar operator
    //

    TYPE operator[]( int j ) const;
    //
    // Overloaded array item update
    //

    void write( ostream& os );
    //
    // write the vector to a file
    //

    TYPE abs() const; 
    TYPE abs( const CVector<TYPE>& p ) const;
    //
    // Define a norm on the point (distance from an origin).
    //

    CVector<TYPE> vlog();
    //
    // logarithm of vector
    //

    TYPE sum();
    //
    // sum of all elements of vector
    //

    double linfinitynorm( int& j );
    //
    // calculate the l1-norm of a vector
    //

    boolean positive();
    //
    // test to see if all elements of vector are positive
    //

    CVector<TYPE> unitVector() const;
    //
    // create unit vector from current vector 3d point
    //

    void clear()
    {
      if ( n_ > 0 && n_ < 10000 )
      {
        delete [] x_;
      }
      n_ = 0;
    };
    //
    // destroys dynamically allocated vector
    //

    ~CVector<TYPE>() 
    {
      clear();
    };
};

template<class TYPE>
CVector<TYPE>& CVector<TYPE>::operator=( const CVector<TYPE>& v1 )
{
  if ( this == &v1 )
    return *this;
  
  clear();

  n_ = v1.n_;
  x_ = new TYPE [n_];

  for ( int i = 0; i < n_; i++ )
    x_[i] = v1.x_[i]; 

  return *this;
}

template<class TYPE>
CVector<TYPE>::CVector( const CVector<TYPE>& v1 )
{
  n_ = v1.n_;
  x_ = new TYPE [n_];

  for ( int i = 0; i < n_; i++ )
    x_[i] = v1.x_[i];
}

template<class TYPE>
boolean CVector<TYPE>::operator==( const CVector<TYPE>& v1 ) const
{
  for ( int i = 0; i < v1.n_; i++ )
    if ( fabs( x_[i] - v1.x_[i] ) > TOL )
      return false;

  return true;
}

template<class TYPE>
boolean CVector<TYPE>::operator!=( const CVector<TYPE>& v1 ) const
{
  for ( int i = 0; i < v1.n_; i++ )
    if ( fabs( x_[i] - v1.x_[i] ) > TOL )
      return true;

  return false;
}

template<class TYPE>
CVector<TYPE> CVector<TYPE>::operator-( const CVector<TYPE>& v1 ) const
{
  CVector<TYPE> v2;

  v2 = CVector<TYPE>( v1.n_ );

  if ( n_ != v1.n_ )
  {
    cout << "size error in vector subtract" << endl;
    exit(1);
  }
  for ( int i = 0; i < v2.n_; i++ )
  {
    v2.x_[i] = x_[i] - v1.x_[i];
    //cout << "v2: " << v2.x_[i] << " = " << x_[i] << " - " << v1.x_[i] << endl;
  }

  return v2;
}

template<class TYPE>
CVector<TYPE> CVector<TYPE>::operator+( const CVector<TYPE>& v1 ) const
{
  CVector<TYPE> v2;

  v2 = CVector<TYPE>( v1.n_ );

  for ( int i = 0; i < v2.n_; i++ )
    v2.x_[i] = x_[i] + v1.x_[i];

  return v2;
}

template<class TYPE>
CVector<TYPE> CVector<TYPE>::operator*( const double& dd ) const
{
  CVector<TYPE> v2;

  v2 = CVector<TYPE>( n_ );

  for ( int i = 0; i < v2.n_; i++ )
    v2.x_[i] = x_[i] * dd;
  return v2;
}

template<class TYPE>
TYPE CVector<TYPE>::operator*( const CVector<TYPE>& v ) const
{
  TYPE T=0.0;

  for ( int i = 0; i < n_; i++ )
    T += x_[i] * v.x_[i];

  return( T );
}

template<class TYPE>
CVector<TYPE> CVector<TYPE>::operator/( const double& dd ) const
{
  CVector<TYPE> v2;

  v2 = CVector<TYPE>( n_ );

  for ( int i = 0; i < v2.n_; i++ )
    v2.x_[i] = x_[i] / dd;
  return v2;
}

template<class TYPE>
CVector<TYPE> CVector<TYPE>::operator/( const CVector<TYPE>& p ) const
{
  CVector<TYPE> v2;

  v2 = CVector<TYPE>( n_ );

  for ( int i = 0; i < v2.n_; i++ )
    v2.x_[i] = x_[i] / p.x_[i];
  return v2;
}

template<class TYPE>
TYPE CVector<TYPE>::operator[]( int j ) const
{
  return x_[j];
}

template<class TYPE>
CVector<TYPE> CVector<TYPE>::unitVector() const
{
  CVector<TYPE> p2;

  p2 = *this / ( this->abs() );
  return p2;
}

template<class TYPE>
void CVector<TYPE>::setOnes()
{
  for ( int i = 0; i < n_; i++ )
    x_[i] = 1.0;
}

template<class TYPE>
TYPE  CVector<TYPE>::abs( const CVector<TYPE>& p ) const
{
  TYPE T = 0.0;

  for ( int i = 0; i < n_; i++ )
    T += qsqr( x_[i] - p.x_[i] );

  return sqrt( T );
}

template<class TYPE>
TYPE  CVector<TYPE>::abs() const
{
  TYPE T = 0.0;

  for ( int i = 0; i < n_; i++ )
    T += qsqr( x_[i] );

  return sqrt( T );
}

template<class TYPE>
CVector<TYPE> CVector<TYPE>::vlog()
{
  CVector<TYPE> p;

  p = CVector<TYPE>( n_ );
  for ( int i = 0; i < p.n_; i++ )
    p.x_[i] = log( x_[i] );

  return p;
}

template<class TYPE>
TYPE CVector<TYPE>::sum()
{
  TYPE p = 0.0;
  for ( int i = 0; i < n_; i++ )
    p += x_[i];

  return p;
}
 
template<class TYPE>
void CVector<TYPE>::write( ostream& os )
{
  for ( int i = 0; i < n_; i++ )
    os << "  " << x_[i];
}

template<class TYPE>
double CVector<TYPE>::linfinitynorm( int& j )
{
  double p = 0.0;

  j = 0;
  for ( int i = 0; i < n_; i++ )
  {
    if ( fabs(x_[i]) > p )
    {
      p = fabs(x_[i]);
      j = i;
    }
  }
  return p;
}

template<class TYPE>
boolean CVector<TYPE>::positive()
{
  for ( int i = 0; i < n_; i++ )
    if ( x_[i] < 0.0 ) 
    {
      return false;
    }
 
  return true;
}

template<class TYPE>
double CVector<TYPE>::euclideanNorm()
{
  double sum=0.0;

  for ( int i = 0; i < n_; i++ )
    sum += qsqr( fabs( x_[i] ) );

  return sqrt( sum );
}

#endif
