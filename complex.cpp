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

#include "complex.hh"

complex& complex::operator=( const complex& z )
{
  re = z.re;
  im = z.im;
  return( *this );
}

complex& complex::operator=( double d )
{
  re = d;
  im = 0.0;
  return( *this );
}

boolean complex::operator!=( const complex& z ) const
{
  if( re != z.re || im != z.im ) return true;
  return false;
}

boolean complex::operator==( const complex& z ) const
{
  if( re == z.re && im == z.im ) return true;
  return false;
}

complex& complex::operator+=( const complex& z )
{
  re += z.re;
  im += z.im;
  return *this;
}

complex& complex::operator-=( const complex& z )
{
  re -= z.re;
  im -= z.im;
  return *this;
}

complex& complex::operator*=( const complex& z )
{
  double d = re * z.im + im * z.re;
  re = re * z.re - im * z.im;
  im = d;
  return *this;
}

complex& complex::operator*=( double d )
{
  re = re * d;
  im = im * d;
  return *this;
}

complex& complex::operator/=( double d )
{
  re /= d;
  im /= d;
  return *this;
}

complex complex::operator-() const
{
  return ( complex( -re, -im ) );
}

complex complex::operator-( double d ) const
{
  return ( complex( re - d, im ) );
}

complex complex::operator-( const complex& z ) const
{
  return ( complex( re - z.real(), im - z.imag() ) );
}

complex operator-( double d, const complex& z )
{
  return ( complex( d - z.real(), - z.imag() ) );
}

complex complex::operator+( double d )
{
  return( complex( re + d, im ) );
}

complex operator+( double d, const complex& z )
{
  return( complex( d + z.re, z.im ) );
}

complex complex::operator+( const complex& z )
{
  return( complex( re + z.re, im + z.im ) );
}

complex complex::operator*( double d )
{
  return( complex( re * d, im * d ) );
}

complex operator*( double d, const complex& z )
{
  return( complex( z.re * d, z.im * d ) );
}

complex complex::operator*( const complex& v ) const
{
  return( complex( re * v.re - im * v.im, re * v.im + im * v.re ) );
}

complex complex::operator/( double d ) const
{
  return( complex( re / d, im / d ) );
}

complex operator/( double d, const complex& z )
{
  return complex( z.re * d, -z.im * d ) / ( z.real() * z.real() + z.imag() * z.imag() );
}

complex complex::operator/( const complex& v ) const
{
  complex v2;
  double  denom;

  denom = v.re * v.re + v.im * v.im;
  v2.re = ( re * v.re + im * v.im ) / denom;
  v2.im = ( im * v.re - re * v.im ) / denom;
  return v2;
}

double cabs( const complex& v )
{
  return( sqrt( v.real() * v.real() + v.imag() * v.imag() ) );
}

complex complex::cconj() const
{
  return ( complex( re, -im ) );
}

complex complex::pow( const complex& v, int i )
{
  complex x;
  int     j;

  x = complex( 1.0, 0.0 );

  for ( j = 0; j < i; j++ )
    x *= v;

  return( x );
}

double complex::real() const
{
  return( re );
}

double complex::imag() const
{
  return( im );
}

ostream& operator<<( ostream& s, const complex& v )
{
  return s << v.re << " " << v.im << " ";
}

#ifdef CLASS_DEBUG

main()
{
  double  d;
  complex z, z1, z2;

  d  = 5.0;
  z1 = complex( 1.0, 1.0 );
  z2 = complex( 0.0, 5.0 );
  cout << z1 << endl;
  cout << z1.real() << endl;
  cout << fabs(z1) << endl;

  z  = z1 * z2;
  cout << z << endl;
  z  = z1 + z2;
  cout << z << endl;
  z  = z1 - z2;
  cout << z << endl;
  z  = z1 / z2;
  cout << z << endl;

  z  = z1 / d;
  cout << z << endl;
  z  = z1 * d;
  cout << z << endl;
  z  = d * z1;
  cout << z << endl;
}

#endif
