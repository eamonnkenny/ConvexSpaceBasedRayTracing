//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    23rd Jan 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "diffcoeff.hh"

complex CDiffractionCoefficient::softDiffraction( double  theta_s,
                                                  double  theta_i,
						  double  n,
						  double  k1,
						  double  sinBeta0,
						  double  L,
						  complex Rperp1,
						  complex Rperp2 )
{
  double  minusAngles, plusAngles;
  complex D_mm, D_mp, D_pm, D_pp, C, coeff;

  minusAngles = theta_s - theta_i;
  plusAngles  = theta_s + theta_i;

  C = CMaths::expminusj( Pi / 4.0 ) / ( -2.0 * n * sqrt( TwoPi * k1 ) * sinBeta0 );

  D_pm = Dplus(  minusAngles, n, k1, L );
  //cout << "D_+( phi_s - phi_i ) : " << D_pm << endl;
  D_mm = Dminus( minusAngles, n, k1, L );
  //cout << "D_-( phi_s - phi_i ) : " << D_mm << endl;
  D_mp = Dminus(  plusAngles, n, k1, L );
  //cout << "D_-( phi_s + phi_i ) : " << D_mp << endl;
  D_pp = Dplus(   plusAngles, n, k1, L );
  //cout << "D_+( phi_s + phi_i ) : " << D_pp << endl;

  coeff = C * ( D_pm + D_mm + Rperp1 * D_mp + Rperp2 * D_pp );
  //cout << "Coeff: " << coeff << endl;
  return coeff;
}

complex CDiffractionCoefficient::hardDiffraction( double  theta_s,
                                                  double  theta_i,
						  double  n,
						  double  k1,
						  double  sinBeta0,
						  double  L,
						  complex Rparallel1,
						  complex Rparallel2 )
{
  double  minusAngles, plusAngles;
  complex D_mm, D_mp, D_pm, D_pp, C;

  minusAngles = theta_s - theta_i;
  plusAngles  = theta_s + theta_i;
  
  C = CMaths::expminusj( Pi / 4.0 ) / ( -2.0 * n * sqrt( TwoPi * k1 ) * sinBeta0 );

  D_pm = Dplus(  minusAngles, n, k1, L );
  D_mm = Dminus( minusAngles, n, k1, L );
  D_mp = Dminus(  plusAngles, n, k1, L );
  D_pp = Dplus(   plusAngles, n, k1, L );
  
  return C * ( D_pm + D_mm + Rparallel1 * D_mp + Rparallel2 * D_pp );
}


complex CDiffractionCoefficient::Dminus( double x, double n, double k1,
                                         double L )
{
  double  eps, Xminus, Napprox;
  complex term1, term2;
  int     Nminus;

  eps = 1.0e-10;

  if( fabs( x - Pi ) < eps || fabs( x + Pi ) < eps )
  {
    // in the transition region (on the RSB or ISB)
    if ( x < 0.0 )
      return -CMaths::expj( Pi / 4.0 ) * n * sqrt( TwoPi * k1 * L ) / 2.0;
    else
      return CMaths::expj( Pi / 4.0 ) * n * sqrt( TwoPi * k1 * L ) / 2.0;
  }
  else
  {
    // outside the transition region
    term1   = 1.0 / tan( ( Pi - x ) / ( 2.0 * n ) );
    Napprox = ( -Pi + x ) / ( TwoPi * n );
    Nminus = round( Napprox );

    Xminus  = cos( ( TwoPi * n * Nminus - x ) / 2.0 );
    Xminus  = 2.0 * k1 * L * qsqr( Xminus );

    if ( Xminus > 100.0 )
      term2 = 1.0;
    else
      term2   = fresnelTransitionFunction( Xminus );

    return term1 * term2;
  }
}

complex CDiffractionCoefficient::Dplus( double x, double n, double k1,
                                        double L )
{
  double  eps, Xplus, Napprox;
  int     Nplus;
  complex term1, term2;

  eps = 1.0e-10;

  if( fabs( x - Pi ) < eps || fabs( x + Pi ) < eps )
  {
    // in the transition region (on the RSB or ISB)
    if ( x < 0.0 )
      return -CMaths::expj( Pi / 4.0 ) * n * sqrt( TwoPi * k1 * L ) / 2.0;
    else
      return CMaths::expj( Pi / 4.0 ) * n * sqrt( TwoPi * k1 * L ) / 2.0;
  }
  else
  {
    // outside the transition region
    term1   = 1.0 / tan( ( Pi + x ) / ( 2.0 * n ) );
    Napprox = ( Pi + x ) / ( TwoPi * n );
    Nplus   = round( Napprox );

    Xplus   = cos( ( TwoPi * n * Nplus - x ) / 2.0 );
    Xplus   = 2.0 * k1 * L * qsqr( Xplus );

    if ( Xplus > 100.0 )
      term2 = 1.0;
    else
      term2 = fresnelTransitionFunction( Xplus );
  
    return term1 * term2;
  }
}

complex CDiffractionCoefficient::fresnelTransitionFunction( double x )
{
  complex  ftfterm1, C;
  complex  ftfintermed1, ftfintermed2;

  ftfterm1     = complex( 0.0, 2.0 ) * sqrt( fabs( x ) ) * CMaths::expj( x );
  C            = SqrtPiOver2 * complex( 0.5, -0.5 );
  ftfintermed1 = SqrtPiOver2 * fresnel( SqrtTwoOverPi * sqrt( fabs(x) ) );
  ftfintermed2 = C - ftfintermed1.cconj();
  
  return ftfterm1 * ftfintermed2;
}

complex CDiffractionCoefficient::fresnel( double x )
{
  double  eps, tol, ax, asum, sums, sumc, isign, fact, term, test, pix2;
  complex a, b, d, f, cc, del, h;
  int     odd, n, k;

  //define user constants
  eps     = 6.0e-08;
  tol     = 1.0e-30;

  ax = fabs(x);

  if ( ax < 1.0e-15 )
  {
    f = ax;
  }
  else if ( ax <= 1.5 )
  {
    asum  = 0.0;
    sums  = 0.0;
    sumc  = ax;
    isign = 1.0;
    fact  = PiOver2 * ( ax * ax );
    odd   = 1;
    term  = ax;
    n     = 3;
  
    for ( k = 1; k <= 100; k++ )
    {
      term *= fact / k;
      asum += isign * term / n;
      test  = fabs( asum ) * eps; 
  
      if ( odd == 1 )
      {
        isign = -isign;
        sums  = asum;
        asum  = sumc;
      }
      else
      {
        sumc  = asum;
        asum  = sums;
      }
  
      if ( term < test ) 
        break;
  
      if ( odd == 1 )
        odd = 0;
      else
        odd = 1;
  
      n += 2;
    }    

    f = complex( sumc, sums );
  }
  else if ( ax > 1.5 )
  {
    pix2 = Pi * ax * ax;
    b    = complex( 1.0, -pix2 );
    cc   = complex( 1.0 / tol );
    d    = complex( 1.0 ) / b;
    h    = d;
    n    = -1;
  
    for ( k = 2; k <= 100; k++ )
    {
      n   += 2;
      a    = -n * (n+1);
      b    = b + 4;
      d    = complex( 1.0 ) / ( a * d + b );
      cc   = b + a / cc;
      del  = cc * d;
      h    = h * del;

      if ( fabs( del.real() - 1.0 ) + fabs( del.imag() ) < eps ) 
        break;
    }
  
    h = complex( ax, -ax ) * h;
    f = complex( 0.5, 0.5 ) * ( 1.0 - CMaths::expj( pix2 / 2.0 ) * h );
  }

  if ( x < 0.0 )
    f = -f;

  return f;
}
  
#ifdef DEBUG_CLASS

main()
{
  // test to see if code is identical to matlab version for PECs
  CDiffractionCoefficient diffCoeff;
  double x, y;
  ofstream outputfile( "diffract.txt" );

  for ( x = 0; x < 360; x+=0.1 )
  {
    y = diffCoeff.softDiffraction( Pi / 3.0, x * Pi / 180.0, 2, TwoPi, 
                                   1.0, 10, -1.0, -1.0 );
    outputfile << x << "   " << y << endl;
  }
  //ofstream outputfile( "fresnel.txt" );

  //for ( x = 0; x < 150; x+=0.1 )
    //outputfile << x << "   " << fresnel( x ) << endl;

  outputfile.close();
}

#endif 
