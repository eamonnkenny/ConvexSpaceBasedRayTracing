//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// Last Modification:     November 2001
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef COMPLEX_NUMBERS_HH
#define COMPLEX_NUMBERS_HH

#include <iostream>
#include <cmath>
#include <fstream>
#include "boolean.hh"

using std::ostream;

class complex
//==========================================================================
// = FUNCTION 
// 	complex.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      Definition of a complex number class along with all the operators
//      necessary to use it.
//
// = VERSION
//     1.2
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  private:
      double    re, im;
 
  public:
      complex( const double& realval, const double& imagval )
      // constructor
      {
        re = realval;
        im = imagval;
      }

      complex()
      // constructor
      {
        re = 0.0;
        im = 0.0;
      }
      
      complex( double d )
      // constructor
      {
        re = d;
        im = 0.0;
      }

      complex( int d )
      // constructor
      {
        re = double( d );
        im = 0.0;
      }
      //===================================================================
      // = OPERATORS
      //===================================================================

      //complex  *array[10];

               complex& operator=( const complex& z );
               complex& operator=( double d );

               boolean  operator!=( const complex& z ) const;
               boolean  operator==( const complex& z ) const;

               complex& operator+=( const complex& z );
               complex& operator-=( const complex& z );
               complex& operator*=( const complex& z );
               complex& operator*=(       double   d );
               complex& operator/=(       double   d );

               complex  operator-() const;
               complex  operator-( double d ) const;
               complex  operator-( const complex& z ) const;
       friend  complex  operator-( double d, const complex& z );

               complex  operator+( double d );
               complex  operator+( const complex& z );
       friend  complex  operator+( double d, const complex& z );

               complex  operator*( double d );
               complex  operator*( const complex& v ) const;
       friend  complex  operator*( double d, const complex& z );

               complex  operator/( double d ) const;
               complex  operator/( const complex& v ) const;
       friend  complex  operator/( double d, const complex& z );

       friend  double   cabs( const complex& v );
               complex  cconj() const;
               complex  pow( const complex& d, int i );
               double   real() const;
               double   imag() const;
               complex  csqrt( const complex& v );

      friend   ostream& operator<<( ostream& s, const complex& v );
};

#endif
