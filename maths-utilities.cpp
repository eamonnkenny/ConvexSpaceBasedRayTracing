//=========================================================================
// COPYRIGHT (c) 2015-2017 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  None
// Last Modification:     August 2015
// Previous Projects:     Storms, FMW/ARG, IP2000 STIWRO
// Current  Project:      None
// Software Tool Version: 1.0
//=========================================================================

#include "maths-utilities.hh"

complex CMaths::expj( const double& d ) 
{ 
  if (d > 1.0e+6 ) 
    cerr << "expj( const double& d )" << endl;
  return complex( cos(d), sin(d) ); 
}

complex CMaths::expminusj( const double& d )
{
  if (d > 1.0e+6 ) 
    cerr << "expminusj( const double& d )" << endl;
  return complex( cos(d), -sin(d) ); 
}

complex CMaths::expj( const complex& v ) 
{
  if (cabs(v) > 1.0e+6 ) 
    cerr << "expsj( const complex& v )" << endl;
  return expj( v.real() ) * exp( -v.imag() ); 
}

complex CMaths::expminusj( const complex& v ) 
{ 
  if (cabs(v) > 1.0e+6 ) 
    cerr << "expminusj( const complex& v )" << endl;
  return expj( -v.real() ) * exp( v.imag() );
}

complex CMaths::exp( const complex& v )
{ 
  if (cabs(v) > 1.0e+6 ) 
    cerr << "exp( const complex& v )" << endl;
  return std::exp( v.real() ) * expj( v.imag() ); 
}

complex CMaths::csqrt( const complex& v )
{
  double Angle;

  Angle = atan2( v.imag(), v.real() ) / 2.0;

  return( sqrt( cabs( v ) ) * expj( Angle ) );
}

#ifdef CLASS_DEBUG

main()
{
  complex z, z1, z2;

  z1 = complex( 1.0, 1.0 );
  z2 = complex( 0.0, 5.0 );
  cout << z1 << endl;
  cout << z1.real() << endl;
  cout << expj(z1) << endl;
  cout << expj(z2) << endl;
  cout << expminusj(z2) << endl;
  cout << cabs(z1) << endl;

  z  = z1 * z2;
  cout << z << endl;

}

#endif
