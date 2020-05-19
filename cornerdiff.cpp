//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    6th Dec 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "cornerdiff.hh"

boolean CCornerDiffraction::checkIfCornerDiffractionExists( CCorners& corners,
                            const     CSegment3d& diffractionEdge,
                            CPoint3d& firstPoint, CPoint3d& lastPoint )
{
  int        i, j, k;
  CPoint3d   cornerItem;
  CSegment3d edgeItem;

  k = 0;

  for ( i = 0; i < corners.size(); i++ )
  {
    for ( j = 0; j < 3; j++ )
    {
      edgeItem = corners.edges[j].GetItem(i);

      if ( edgeItem == diffractionEdge )
      {
        if ( k == 0 )
        {
          firstPoint = corners.listOfCorners.GetItem(i);
          k++;
        }
        else if ( k == 1 )
        {
          lastPoint = corners.listOfCorners.GetItem(i);
          k++;
        }
      }
    }
  }

  if ( k > 0 ) 
    return true;
  else
    return false;
}

complex CCornerDiffraction::cornerDiffractionConst(
                                 double k1,     double sinBeta0, double betaC,
                                 double beta0c, double Lc,       double sr )
{
  double  A;
  complex C, B;

  A = sqrt( sin( betaC ) * sin( beta0c ) ) / ( cos( beta0c ) - cos( betaC ) );
  B = fresnelTransitionFunction( k1 * Lc * a( Pi + beta0c - betaC ) );
  C = CMaths::expminusj( k1 * sr + PiOver4 ) / sr / sqrt( TwoPi * k1 );

  return ( C * B * A );
}

complex CCornerDiffraction::cornerDiffractionCoefficient(
                                                  double  theta_s,
                                                  double  theta_i,
                                                  double  n,
                                                  double  k1,
                                                  double  sinBeta0,
                                                  double  betaC,
                                                  double  beta0c,
                                                  double  L,
                                                  double  Lc,
                                                  complex Rperp1,
                                                  complex Rperp2 )
{
  double  minusAngles, plusAngles, fminus, fplus;
  complex D_mm, D_mp, D_pm, D_pp, C;

  minusAngles = theta_s - theta_i;
  plusAngles  = theta_s + theta_i;

  C = CMaths::expminusj( Pi / 4.0 ) / ( -2.0 * n * sqrt( TwoPi * k1 ) * sinBeta0 );

  fminus = fi( minusAngles, L, Lc, beta0c, betaC );
  fplus  = fi( plusAngles,  L, Lc, beta0c, betaC ); 

  D_pm = Dplus(  minusAngles, n, k1, L );
  D_mm = Dminus( minusAngles, n, k1, L ); // D_2
  D_pp = Dplus(   plusAngles, n, k1, L ); // D_3
  D_mp = Dminus(  plusAngles, n, k1, L ); // D_4


  if ( fabs( theta_s - Pi ) > TOL || fabs( theta_s ) > TOL )
  {
    return C * ( ( D_pm + D_mm ) * fminus 
               + ( Rperp1 * D_mp + Rperp2 * D_pp ) * fplus );
  }
  else 
  {
    if ( Rperp1 == -1.0 && fabs( theta_s ) <= TOL )
      return complex( 0.0 );
    else if ( Rperp1 == 1.0 && fabs( theta_s <= TOL ) )
      return C * ( ( D_pm + D_mm ) * fminus );
    else
      return C * ( ( D_pm + D_mm ) * fminus 
               + ( Rperp1 * D_mp + Rperp2 * D_pp ) * fplus );
  }
}

double CCornerDiffraction::a( double x )
{
  return 2.0 * qsqr( cos( x / 2.0 ) ); 
}

double CCornerDiffraction::fi( double x, double L, double Lc,
                               double beta0c, double betaC )
{
  return cabs( fresnelTransitionFunction( a( x ) * L 
           / ( TwoPi * Lc * a( Pi + beta0c - betaC ) ) )); 
}

complex CCornerDiffraction::Dminus( double x, double n, double k1,
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

complex CCornerDiffraction::Dplus( double x, double n, double k1,
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

complex CCornerDiffraction::fresnelTransitionFunction( double x )
{
  complex  ftfterm1, C;
  complex  ftfintermed1, ftfintermed2;

  ftfterm1     = complex( 0.0, 2.0 ) * sqrt( fabs( x ) ) * CMaths::expj( x );
  C            = SqrtPiOver2 * complex( 0.5, -0.5 );
  ftfintermed1 = SqrtPiOver2 * fresnel( SqrtTwoOverPi * sqrt( fabs(x) ) );
  ftfintermed2 = C - ftfintermed1.cconj();
  
  return ftfterm1 * ftfintermed2;
}

complex CCornerDiffraction::fresnel( double x )
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
  //CCornerDiffraction diffCoeff;
  double x;

  //x = diffCoeff.softDiffraction( Pi / 4.0, 5.0 * Pi / 4.0, 1.5, TwoPi, 
                                 //1.0, 10, -1.0, -1.0 );
  ofstream outputfile( "fresnel.txt" );

  for ( x = 0; x < 150; x+=0.1 )
    outputfile << x << "   " << fresnel( x ) << endl;

  outputfile.close();
}

#endif 
