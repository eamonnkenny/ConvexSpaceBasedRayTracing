//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    Oct 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include <iostream.h>
#include "contact.hh"
#include "complex.hh"
#include "userconst.hh"
#include "ComplexVector.hh"
#include "segment3d.hh"
#include "storage.hh"
#include "list.hh"

void localReflectionCoefficient(      double     cosTheta_i,
                                const complex&   permittivity,
                                const mediaType& media,
                                      complex&   Rparallel,
                                      complex&   Rperpendicular );

void transmissionFreeSpaceToDielectric(      double     cosTheta_i,
                                       const complex&   permittivity,
                                       const mediaType& media,
                                             complex&   Tparallel,
                                             complex&   Tperpendicular );

void transmissionDielectricToFreeSpace(      double     cosTheta_i,
                                       const complex&   permittivity,
                                       const mediaType& media,
                                             complex&   Tparallel,
                                             complex&   Tperpendicular );

void bisectionAlgorithm( double& theta_i, double& theta_t,
                         const complex& permittivity, double h1, 
                         double h2, double d, double l );

boolean newtonRaphson( double theta_prev, double& theta_i, double& theta_t, 
                    const complex& permittivity, double h1,
                    double h2, double d, double l );

void setWaveNumberVariables( double  f, complex permittivity, double d,
                             double theta_i, double theta_t, double& k1, 
			     complex& k2, double& l,  double& t, complex& Pd,
			     complex& Pa, double& Eamp, complex& Pt );
boolean reflectionCoefficient(       CComplexVector&   Ei,
                               const CSegment3d&       si,
                               const CSegment3d&       sr,
                               const double&           pathLength,
                               const double&           k1,
                                     complex           permittivity,
                               const mediaType&        media );

boolean infiniteReflection(          CComplexVector&   Ei,
                               const CSegment3d&       si,
                               const CSegment3d&       sr,
                               const double&           pathLength,
                               const double&           k1,
                                     complex           permittivity,
                                     complex           Pa,
                                     complex           Pd,
                                     double            Eamp,
                               const mediaType&        media );

boolean nthImageReflection(          CComplexVector&   Ei,
                               const CSegment3d&       si,
                               const CSegment3d&       sr,
                               const double&           pathLength,
                               const double&           k1,
                                     complex           permittivity,
                                     complex           Pa,
                                     complex           Pd,
				     double            Eamp,
                                     double            t,
                                     int               n,
                               const mediaType&        media );

boolean singleInfiniteTerm(          CComplexVector&   Ei,
                                     List<complex>&    Eterms,
                               const CSegment3d&       si,
                               const CSegment3d&       sr,
                               const double&           pathLength,
                               const double&           k1,
                                     complex           permittivity,
                                     complex           Pa,
                                     complex           Pd,
				     double            l,
                               const mediaType&        media );

CComplexVector incidentField( CSegment3d& si, double k1 );

void meanAbsSum( List<complex>& allRayPaths, double& randomWalkMean );
void randomWalkMean( List<complex>& allRayPaths, double& randomWalkMean );
double besselJ0( double x );
double WnFunction( double *E, int N, double r, double zeta, double stepsize );
double trapezoidal( double *E, int N, double r, double a, double stepsize );

complex clemmowFlatPlate( double theta_i, double k2, double s, 
                          complex permittivity );

void main()
{
  boolean    done;

  int        i, n, numberOfFrequencies;

  double     pathLength, angle, halfBandWidth, theta_t, theta_prev, Eamp;
  double     r, theta_i, cosTheta_i, sinTheta_i, f, error, meanClemmow;
  double     d, k1, frequency, lowerFrequency, upperFrequency, frequencyStep;
  double     h1, h2, l2, l, t, meanFullRayTrace, firstOrderMean, meanBurnside;

  complex    parallel, perpendicular, Ei_parallel, Ei_perpendicular;
  complex    permittivity, Tparallel, Tperpendicular;
  complex    Pa, Pd, Pt, k2, Rparallel, Rperpendicular;
  complex    Tparallel_12, Tparallel_23, Tperpendicular_12, Tperpendicular_23;
  complex    clemmow;

  List<complex>  EfullRayTrace, EfullRayTraceOfFirstOrderReflection;
  List<complex>  EburnsidePlanewaveApproxMean, EclemmowExact;
  mediaType      media;
  CComplexVector Et, Ei, EiPolar, EburnsidePlanewaveApprox, EsingleReflection;
  CSegment3d     si, sr;
  CPoint3d       ei_perpendicular, ei_parallel, sihat, source, receiver;
  CPoint3d       reflectionPoint, transmissionPoint;

  ofstream   coutput1("singleReflect.txt" );
  //ofstream   coutput1("r4.txt" );
  ofstream   coutput2("singleTransmission.txt" );
  //ofstream   coutput2("t4.txt" );
  ofstream   coutput3("infiniteReflect.txt" );
  ofstream   coutput4("correctedReflect.txt" );
  ofstream   coutput5("infiniteTrans.txt" );
  ofstream   coutput6("correctedTrans.txt" );
  ofstream   coutput7("infiniteReflectPS.txt" );
  ofstream   coutput8("geometry.txt" );
  ofstream   coutput9("infiniteReflectPW.txt" );
  ofstream   coutput10("meanInfinitePS.txt" );

  // wide band frequency information
  //numberOfFrequencies = 100;
  numberOfFrequencies = 1;
  //halfBandWidth       = 5.0;
  halfBandWidth       = 0.0;
  Eamp                = 0.0;

  frequency      = 2000.0;
  lowerFrequency = frequency - halfBandWidth;
  upperFrequency = frequency + halfBandWidth;
  frequencyStep  = ( upperFrequency - lowerFrequency ) / numberOfFrequencies;
  if ( frequencyStep == 0 ) frequencyStep = 1;

  numberOfFrequencies++; 

  // slab properties (width, and permittivity)
  d            = 0.2;
  //media = LOSSY;
  //permittivity = complex( 4.0, -1.0e-2 
  //                           / (TwoPi * frequency * HERTZ * Permittivity) );
  cout << "relative permittivity: " << permittivity << endl;
  media = LOSSLESS;
  permittivity = complex( 4.0, 0.0 );

  done = newtonRaphson( 0.5, theta_i, theta_t, permittivity, 
                        5.0, 5.0, 2.0, 10.0 );

  if ( done == false )
    bisectionAlgorithm( theta_i, theta_t, permittivity, 5.0, 5.0, 2.0, 10.0 );

  // for differing angles [0,90] degrees, computes coefficients

  cout << "Performing single and infinite reflection cases" << endl;
  for ( i = 0; i < 90; i++ )
  {
    theta_i           = double(i) * PiOver2 / 90.0;
    cosTheta_i        = cos( theta_i );
    sinTheta_i        = sin( theta_i );

    //---------------- single Reflection Coefficient ------------------------
    localReflectionCoefficient( cosTheta_i, permittivity, media, 
                                Rparallel, Rperpendicular );

    coutput1 << i << "  "  << fabs(Rparallel) << "  " << fabs(Rperpendicular)
             << endl;

    //-------------- single Transmission Coefficient ------------------------

    transmissionFreeSpaceToDielectric( cosTheta_i, permittivity, media, 
                                       Tparallel, Tperpendicular );

    coutput2 << i << "  "  << fabs(Tparallel) << "  " << fabs(Tperpendicular)
             << endl;

    //-------------- reflection Coefficient Burnside ------------------------

    parallel      = complex(0.0);
    perpendicular = complex(0.0);

    for ( f = lowerFrequency; f <= upperFrequency; f+=frequencyStep )
    {
      // infinite reflection coefficient found by multiplying single term
      // by a correction
      setWaveNumberVariables( f, permittivity, d, theta_i, theta_t,
                              k1, k2, l, t, Pd, Pa, Eamp, Pt );

      localReflectionCoefficient( cosTheta_i, permittivity, media, 
                                  Rparallel, Rperpendicular );
      transmissionFreeSpaceToDielectric( cosTheta_i, permittivity, media, 
                                         Tparallel_12, Tperpendicular_12 );
      transmissionDielectricToFreeSpace( cosTheta_i, permittivity, media, 
                                         Tparallel_23, Tperpendicular_23 );
      Rparallel = Rparallel * ( 1 - qsqr( Pd ) * Pa )
                            / ( 1 - qsqr( Rparallel ) * qsqr( Pd ) * Pa );
      Rperpendicular = Rperpendicular * ( 1 - qsqr( Pd ) * Pa )
                     / ( 1 - qsqr( Rperpendicular ) * qsqr( Pd ) * Pa );
      parallel      += Rparallel;
      perpendicular += Rperpendicular;
    }
    parallel      /= numberOfFrequencies;
    perpendicular /= numberOfFrequencies;

    coutput3 << i << "  "  << fabs(parallel) << "  " << fabs(perpendicular) 
             << endl;

    //-------------- reflection Coefficient Burnside Check ------------------

    parallel      = complex(0.0);
    perpendicular = complex(0.0);

    for ( f = lowerFrequency; f <= upperFrequency; f+=frequencyStep )
    {
      // infinite reflection coefficient found by multiplying single term
      // by a correction
      setWaveNumberVariables( f, permittivity, d, theta_i, theta_t,
                              k1, k2, l, t, Pd, Pa, Eamp, Pt );

      localReflectionCoefficient( cosTheta_i, permittivity, media, 
                                  Rparallel, Rperpendicular );
      transmissionFreeSpaceToDielectric( cosTheta_i, permittivity, media, 
                                         Tparallel_12, Tperpendicular_12 );
      transmissionDielectricToFreeSpace( cosTheta_i, permittivity, media, 
                                         Tparallel_23, Tperpendicular_23 );

      Rparallel = Rparallel 
                - Rparallel * Tparallel_12 * Tparallel_23 * qsqr( Pd ) * Pa 
                / ( 1.0 - qsqr( Rparallel ) * qsqr( Pd ) * Pa );

      Rperpendicular = Rperpendicular 
                     - Rperpendicular * Tperpendicular_12 * Tperpendicular_23
                     * qsqr( Pd ) * Pa 
                     / ( 1.0 - qsqr( Rperpendicular ) * qsqr( Pd ) * Pa );

      parallel      += Rparallel;
      perpendicular += Rperpendicular;
    }
    parallel      /= numberOfFrequencies;
    perpendicular /= numberOfFrequencies;
     
    coutput4 << i << "  "  << fabs(parallel) << "  " << fabs(perpendicular) 
             << endl;

    //-------------- transmission Coefficient Burnside ---------------------
    parallel      = complex(0.0);
    perpendicular = complex(0.0);

    for ( f = lowerFrequency; f <= upperFrequency; f+=frequencyStep )
    {
      // infinite transmission coefficient found by multiplying single term
      // by a correction
      setWaveNumberVariables( f, permittivity, d, theta_i, theta_t,
                              k1, k2, l, t, Pd, Pa, Eamp, Pt );

      localReflectionCoefficient( cosTheta_i, permittivity, media, 
                                  Rparallel, Rperpendicular );
      transmissionFreeSpaceToDielectric( cosTheta_i, permittivity, media, 
                                         Tparallel_12, Tperpendicular_12 );
      transmissionDielectricToFreeSpace( cosTheta_i, permittivity, media, 
                                         Tparallel_23, Tperpendicular_23 );
  
      Tparallel = ( 1.0 - qsqr(Rparallel) ) * qsqr( Pd ) * Pt 
                / ( 1.0 - qsqr( Rparallel ) * qsqr( Pd ) * Pa );

      Tperpendicular = ( 1.0 - qsqr( Rperpendicular ) ) * qsqr( Pd ) * Pt 
                     / ( 1.0 - qsqr( Rperpendicular ) * qsqr( Pd ) * Pa );

      parallel      += Tparallel;
      perpendicular += Tperpendicular;
    }
    parallel      /= numberOfFrequencies;
    perpendicular /= numberOfFrequencies;

    coutput5 << i << "  "  << fabs(parallel) << "  " << fabs(perpendicular) 
             << endl;

    //-------------- transmission Coefficient Burnside Check ---------------
    parallel      = complex(0.0);
    perpendicular = complex(0.0);

    for ( f = lowerFrequency; f <= upperFrequency; f+=frequencyStep )
    {
      // infinite transmission coefficient found by multiplying single term
      // by a correction
      setWaveNumberVariables( f, permittivity, d, theta_i, theta_t,
                              k1, k2, l, t, Pd, Pa, Eamp, Pt );

      localReflectionCoefficient( cosTheta_i, permittivity, media, 
                                  Rparallel, Rperpendicular );
      transmissionFreeSpaceToDielectric( cosTheta_i, permittivity, media, 
                                         Tparallel_12, Tperpendicular_12 );
      transmissionDielectricToFreeSpace( cosTheta_i, permittivity, media, 
                                         Tparallel_23, Tperpendicular_23 );
  
      Tparallel =  Tparallel_12 * Tparallel_23 * qsqr( Pd ) * Pt 
                / ( 1.0 - qsqr( Rparallel ) * qsqr( Pd ) * Pa );
  
      Tperpendicular = Tperpendicular_12 * Tperpendicular_23 * qsqr( Pd ) * Pt 
                     / ( 1.0 - qsqr( Rperpendicular ) * qsqr( Pd ) * Pa );

      parallel      += Tparallel;
      perpendicular += Tperpendicular;
    }
    parallel      /= numberOfFrequencies;
    perpendicular /= numberOfFrequencies;

    coutput6 << i << "  "  << fabs(parallel) << "  " << fabs(perpendicular) 
             << endl;
  }

  r = 10.0;

  cout << "permittivity: " << permittivity << endl;
  for ( i =1; i <= 90; i++ )
  {
    angle         = double( i ) / 90.0 * PiOver2;
    //source        = CPoint3d(0.0 - r * cos(angle), 0.0, r * sin(angle));
    source        = CPoint3d(0.0 -  10 * r * cos(angle), 0.0, 10 * r * sin(angle));
    receiver      = CPoint3d( r, 0.0, r);
    h1            = source.z;
    h2            = receiver.z;
    l2            = receiver.x - source.x;
    //receiver      = CPoint3d( 3.0*r, 0.0,r/2.0 );
  
    theta_prev        = 0.5;
  
    Et = CComplexVector( 0.0, 0.0, 0.0 );
  
    for ( n = 0; n < 10; n++ )
    {
      //done = newtonRaphson( theta_prev, theta_i, theta_t, permittivity, 
                             //h1, h2, n*d, l2 );
      bisectionAlgorithm( theta_i, theta_t, permittivity, 
                          h1, h2, n*d, l2 );
  
      reflectionPoint   = CPoint3d(source.x+source.z * tan(theta_i), 0.0, 0.0);
      transmissionPoint = CPoint3d( receiver.x-receiver.z * tan( theta_i ),
                                    0.0, 0.0 );
  
      setWaveNumberVariables( frequency, permittivity, n*d, theta_i, 
                              theta_t, k1, k2, l, t, Pd, Pa, Eamp, Pt );
  
      if ( n == 0 )
      {
        si = CSegment3d( source, receiver );
        Et = incidentField( si, k1 );
        cout << "Et(0): " << Et << endl;
        EfullRayTrace.append( Et.z );
        EfullRayTraceOfFirstOrderReflection.append( Et.z );
        EburnsidePlanewaveApproxMean.append( Et.z );
        EclemmowExact.append( Et.z );
      }
  
      si         = CSegment3d( source, reflectionPoint );
      sr         = CSegment3d( transmissionPoint, receiver );
      pathLength = si.length();
      Ei         = incidentField( si, k1 );

      if ( n == 0 )
      {
        EburnsidePlanewaveApprox = Ei;
        reflectionCoefficient( Ei, si, sr, pathLength, k1, permittivity, 
                               media );
        EfullRayTraceOfFirstOrderReflection.append( Ei.z );

        setWaveNumberVariables( frequency, permittivity, d, theta_i, 
                                theta_t, k1, k2, l, t, Pd, Pa, Eamp, Pt );

        EburnsidePlanewaveApproxMean.append( Ei.z );
        infiniteReflection( EburnsidePlanewaveApprox, si, sr, pathLength, k1, 
	                    permittivity, Pa, Pd, Eamp, media );
        EburnsidePlanewaveApprox = EburnsidePlanewaveApprox + Et;
        EburnsidePlanewaveApproxMean.append( Ei.z );

        EclemmowExact.append( Ei.z );
	clemmow = clemmowFlatPlate( theta_i, k1, si.length() + sr.length(), 
	                            permittivity );
        EclemmowExact.append( clemmow );
      }
      else 
      {
        nthImageReflection( Ei, si, sr, pathLength, k1, permittivity, 
	                    Pa, Pd, Eamp, t, n, media );
      }

      EfullRayTrace.append( Ei.z );

      Et += Ei;

      if ( n == 0 )
        EsingleReflection = Et;

      cout << "Et(" << n << "): " << Et << endl;

      theta_prev = theta_i;
    }
    error = (Et - EsingleReflection).euclideanNorm() / Et.euclideanNorm();

    coutput7 << angle << "  " << error << endl;
    //coutput7 << angle << "   " << ( Et.z - EsingleReflection.z ) / Et.z
             //<< "   " << ( Et.z - EsingleReflection.z ) / Et.z << endl;
    error = (Et - EburnsidePlanewaveApprox).euclideanNorm() 
          / Et.euclideanNorm();

    coutput9 << angle << "  " << error << endl;

    meanAbsSum( EfullRayTrace,    meanFullRayTrace );
    meanAbsSum( EfullRayTraceOfFirstOrderReflection,  firstOrderMean );
    meanAbsSum( EburnsidePlanewaveApproxMean, meanBurnside );
    meanAbsSum( EclemmowExact, meanClemmow );

    cout << "----------------------------------------------" << endl;
    cout << "Relative Error(" << i << ") : " << error << endl;
    cout << "Infinite Total Field(" << i << "): " << Et << endl;
    cout << "Single Reflection(" << i << "): " << EsingleReflection << endl;
    cout << "Burnside (" << i << "): " << EburnsidePlanewaveApprox << endl;
    cout << "Mean Full Ray Trace(" << i << "): " << meanFullRayTrace << endl;
    cout << "Mean 1st Reflection(" << i << "): " << firstOrderMean << endl;
    cout << "Mean Burnside(" << i << "): " << meanBurnside << endl;
    cout << "Mean Clemmow(" << i << "): " << meanClemmow << endl;

    coutput10 << angle << "  " << meanFullRayTrace << "  " << firstOrderMean 
              << "  " << fabs( EfullRayTrace.GetItem(0) ) << "  " 
              << fabs( EfullRayTrace.GetItem(0) + EfullRayTrace.GetItem(1) ) 
	      << "  " << fabs( meanBurnside ) << "  "
	      << fabs( EfullRayTrace.sum() ) << "   " << meanClemmow
	      << "  " << fabs( EclemmowExact.sum() ) 
	      << "  " << fabs( EburnsidePlanewaveApproxMean.sum() ) << endl;

    EfullRayTrace.clear();
    EclemmowExact.clear();
    EfullRayTraceOfFirstOrderReflection.clear();
    EburnsidePlanewaveApproxMean.clear();
    coutput8 << source << endl;
  }
  coutput8 << receiver << endl;

  coutput1.close();
  coutput2.close();
  coutput3.close();
  coutput4.close();
  coutput5.close();
  coutput6.close();
  coutput7.close();
  coutput8.close();
  coutput9.close();
  coutput10.close();
}  

void localReflectionCoefficient(      double     cosTheta_i,
                                const complex&   permittivity,
                                const mediaType& media,
                                      complex&   Rparallel,
                                      complex&   Rperpendicular )
{
  complex sqrtTerm;

  if ( media == PEC )
  {
    Rparallel      =  1.0;
    Rperpendicular = -1.0;
  }
  else
  {
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rparallel      = ( -permittivity * cosTheta_i + sqrtTerm )
                   / (  permittivity * cosTheta_i + sqrtTerm );
    Rperpendicular = ( cosTheta_i - sqrtTerm ) / ( cosTheta_i + sqrtTerm );
  }
}

void transmissionFreeSpaceToDielectric(      double     cosTheta_i,
                                       const complex&   permittivity,
                                       const mediaType& media,
                                             complex&   Tparallel,
                                             complex&   Tperpendicular )
{
  complex sqrtTerm;

  // define the transmission matrix for different dielectric types
  if ( media == PEC )
  {
    Tparallel      = 0.0;
    Tperpendicular = 0.0;
  }
  else
  {
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = sqrt(permittivity) * cosTheta_i * 2.0
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  complex( 2.0 * cosTheta_i ) / ( cosTheta_i + sqrtTerm );
  }
}

void transmissionDielectricToFreeSpace(      double     cosTheta_i,
                                       const complex&   permittivity,
                                       const mediaType& media,
                                             complex&   Tparallel,
                                             complex&   Tperpendicular )
{
  complex sqrtTerm;

  // define the transmission matrix for different dielectric types
  if ( media == PEC )
  {
    Tparallel      = 0.0;
    Tperpendicular = 0.0;
  }
  else
  {
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = 2.0 * sqrt(permittivity) * sqrtTerm 
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  sqrtTerm * 2.0 / ( cosTheta_i + sqrtTerm );
  }
}

void setWaveNumberVariables( double f, complex permittivity, double d, 
                             double theta_i, double theta_t,
                             double& k1, complex& k2, 
                             double& l,  double&  t, complex& Pd,
                             complex& Pa, double& Eamp, complex& Pt )
{
  double cosTheta_t, sinTheta_t, sinTheta_i, cosTheta_i;
  double abssqr, a, b, alpha, beta, alpha_2, beta_2, p, q, k2e;

  k1 = TwoPi / ( SpeedOfLight / f / HERTZ );
  k2 = k1 * sqrt( permittivity );

  sinTheta_i = sin( theta_i );
  cosTheta_i = cos( theta_i );
  sinTheta_t = sin( theta_t );
  cosTheta_t = cos( theta_t );

  l  = d / cosTheta_t;
  t  = l * cos( theta_i - theta_t );

  if ( fabs( permittivity.imag() ) < TOL )
    Pd = expminusj( k2 * l );
  else
  {
    abssqr = qsqr( permittivity.real() ) + qsqr( permittivity.imag() );
    a = abssqr - permittivity.real() * qsqr( sinTheta_i );
    b = permittivity.imag() * qsqr( sinTheta_i );
    alpha = sqrt( a / 2.0 / abssqr ) 
          * sqrt( sqrt( 1.0 + qsqr( b/a ) ) - 1 );
    beta  = sqrt( a / 2.0 / abssqr ) 
          * sqrt( sqrt( 1.0 + qsqr( b/a ) ) + 1 );
    alpha_2 = sqrt( permittivity.real() / 2.0 
            * sqrt( 1.0 + qsqr( permittivity.imag() ) ) - 1.0 );
    beta_2  = sqrt( permittivity.real() / 2.0
            * sqrt( 1.0 + qsqr( permittivity.imag() ) ) + 1.0 );
    q   = alpha_2 * beta + beta_2 * alpha; 
    p   = alpha_2 * alpha + beta_2 * beta; 
    Eamp= exp( -d * p );

    k2e = sqrt( qsqr( k1 * sinTheta_i ) + qsqr(q) );
    Pd  = expminusj( k2e * l );
  }

  Pa = expj( complex( 2.0 * k1 * l * sinTheta_t * sinTheta_i ) );
  Pt = expj( k1 * t );
}

void bisectionAlgorithm(       double&    theta_i, double&  theta_t,
                         const complex&   permittivity,
                               double     h1, double h2, double d, double l )
{
  double upper, lower, middle;
  double xlower, xupper, xmiddle, x;
  double t, alpha, beta, alpha_2, beta_2, q, abssqr, a, b;
  int    i=0;

  upper  = 1.0;
  lower  = 0.0;

  if ( fabs( permittivity.imag() ) <= TOL )
  {
    do 
    {
      middle = ( upper + lower ) / 2.0;
      x = upper;
  
      xupper = ( (h1+h2) * sqrt( permittivity * x - qsqr(x) )
               + 2.0 * d * sqrt( x - qsqr( x ) )
               - l * sqrt( 1.0 - x ) * sqrt( permittivity - x ) ).real();
  
      x = lower;
  
      xlower = ( (h1+h2) * sqrt( permittivity * x - qsqr(x) )
               + 2.0 * d * sqrt( x - qsqr( x ) )
               - l * sqrt( 1.0 - x ) * sqrt( permittivity - x ) ).real();
      
      x = middle;
  
      xmiddle= ( (h1+h2) * sqrt( permittivity * x - qsqr(x) )
               + 2.0 * d * sqrt( x - qsqr( x ) )
               - l * sqrt( 1.0 - x ) * sqrt( permittivity - x ) ).real();
  
      theta_i = asin( sqrt(x) );
      //cout << "step " << i << " theta_i: " << theta_i << endl; 
      if ( xmiddle / xlower > 0.0 )
        lower = middle;
      else
        upper = middle;
      i++;
  
    } while ( fabs( xupper - xlower ) > TOL2 );
  }
  else
  {
    do 
    {
      middle = ( upper + lower ) / 2.0;

      // upper value
      x = upper;
  
      t = x; 
      abssqr = qsqr( permittivity.real() ) + qsqr( permittivity.imag() );
      a = abssqr - permittivity.real() * qsqr( x );
      b = permittivity.imag() * qsqr( x );
      alpha = sqrt( a / 2.0 / abssqr ) 
            * sqrt( sqrt( 1.0 + qsqr( b/a ) ) - 1 );
      beta  = sqrt( a / 2.0 / abssqr ) 
            * sqrt( sqrt( 1.0 + qsqr( b/a ) ) + 1 );
      alpha_2 = sqrt( permittivity.real() / 2.0 
              * sqrt( 1.0 + qsqr( permittivity.imag() ) ) - 1.0 );
      beta_2  = sqrt( permittivity.real() / 2.0
              * sqrt( 1.0 + qsqr( permittivity.imag() ) ) + 1.0 );
      q       = alpha_2 * beta + beta_2 * alpha;

      xupper = (h1+h2) * x / sqrt( 1.0 - qsqr(x) ) + 2.0 * d * t / q - l;
  
      // lower value
      x = lower;

      t = x; 
      abssqr = qsqr( permittivity.real() ) + qsqr( permittivity.imag() );
      a = abssqr - permittivity.real() * qsqr( x );
      b = permittivity.imag() * qsqr( x );
      alpha = sqrt( a / 2.0 / abssqr ) 
            * sqrt( sqrt( 1.0 + qsqr( b/a ) ) - 1 );
      beta  = sqrt( a / 2.0 / abssqr ) 
            * sqrt( sqrt( 1.0 + qsqr( b/a ) ) + 1 );
      alpha_2 = sqrt( permittivity.real() / 2.0
              * sqrt( 1.0 + qsqr( permittivity.imag() ) ) - 1.0 );
      beta_2  = sqrt( permittivity.real() / 2.0
              * sqrt( 1.0 + qsqr( permittivity.imag() ) ) + 1.0 );
      q       = alpha_2 * beta + beta_2 * alpha;

      xlower = (h1+h2) * x / sqrt( 1.0 - qsqr(x) ) + 2.0 * d * t / q - l;
  
      x = middle;
  
      t = x; 
      abssqr = qsqr( permittivity.real() ) + qsqr( permittivity.imag() );
      a = abssqr - permittivity.real() * qsqr( x );
      b = permittivity.imag() * qsqr( x );
      alpha = sqrt( a / 2.0 / abssqr ) 
            * sqrt( sqrt( 1.0 + qsqr( b/a ) ) - 1 );
      beta  = sqrt( a / 2.0 / abssqr ) 
            * sqrt( sqrt( 1.0 + qsqr( b/a ) ) + 1 );
      alpha_2 = sqrt( permittivity.real() / 2.0
              * sqrt( 1.0 + qsqr( permittivity.imag() ) ) - 1.0 );
      beta_2  = sqrt( permittivity.real() / 2.0
              * sqrt( 1.0 + qsqr( permittivity.imag() ) ) + 1.0 );
      q       = alpha_2 * beta + beta_2 * alpha;

      // middle value
      xmiddle = (h1+h2) * x / sqrt( 1.0 - qsqr(x) ) + 2.0 * d * t / q - l;
  
      theta_i = asin( x );
      //cout << "step " << i << " theta_i: " << theta_i << endl; 
      if ( xmiddle / xlower > 0.0 )
        lower = middle;
      else
        upper = middle;
      i++;
  
    } while ( fabs( xupper - xlower ) > TOL2 );
  }

  x = middle;
  theta_i = asin( sqrt(x) );
  theta_t = asin( sqrt(x) / sqrt( permittivity.real() ) );

  x = (h1+h2) * tan( theta_i ) + 2.0 * d * tan( theta_t ) - l; 
}

boolean newtonRaphson( double theta_prev, double&    theta_i, double&  theta_t,
                       const complex&   permittivity,
                       double     h1, double h2, double d, double l )
{
  double xnew, fx, dfxdx, newton, x;
  int    i = 0;

  xnew  = qsqr(sin(theta_prev));

  do 
  {
    x     = xnew;
    fx    = ( (h1+h2) * sqrt( permittivity * x - qsqr(x) )
             + 2.0 * d * sqrt( x - qsqr( x ) )
             - l * sqrt( 1.0 - x ) * sqrt( permittivity - x ) ).real();

    dfxdx = ( (h1+h2) / 2.0 * ( permittivity.real() - 2.0 * x ) 
            / sqrt( permittivity.real() * x - qsqr(x) )
             + d * ( 1.0 - 2.0 * x ) / sqrt( x - qsqr( x ) )
             - l / 2.0 * ( 2.0 * x - 1.0 - permittivity.real() ) 
               / sqrt( permittivity.real() - x ) / sqrt( 1.0 - x ) );

    if ( dfxdx == 0.0 ) return false;
    
    newton = fx / dfxdx;
    xnew   = x - newton;

    i++;
    //cout << "step  " << i << ":  " << xnew << "newton: " << newton << endl;
  } while ( fabs( newton ) > TOL );

  x = xnew;
  theta_i = asin( sqrt(x) );
  theta_t = asin( sqrt(x) / sqrt( permittivity.real() ) );

  //cout << "theta_i: " << theta_i/Pi*180.0 << "  theta_t: " << theta_t/Pi*180.0
       //<< endl;
  x = (h1+h2) * tan( theta_i ) + 2.0 * d * tan( theta_t ) - l; 
  //cout << "check: error in bisected formula" << x << endl;

  return true;
}

boolean reflectionCoefficient(       CComplexVector&   Ei,
                               const CSegment3d&       si,
                               const CSegment3d&       sr,
                               const double&           pathLength,
                               const double&           k1,
                                     complex           permittivity,
                               const mediaType&        media )
{
  CPoint3d nhat, sihat, srhat;
  CPoint3d ei_perpendicular, ei_parallel, er_perpendicular, er_parallel;
  double   p, s, astigmatic, cosTheta_i;
  complex  Rparallel, Rperpendicular, phase;
  complex  E_iparallel, E_iperpendicular, E_rparallel, E_rperpendicular;

  nhat = CPoint3d(0.0,0.0,1.0);

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  srhat            = sr.unitVector();
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = ei_perpendicular ^ sihat;
  }

  er_perpendicular = ei_perpendicular;
  er_parallel      = nhat * ( nhat * ei_parallel ) * 2.0 - ei_parallel;

  // define unit vectors and lengths along the ray
  p                = pathLength;
  s                = sr.length();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  // components of reflection coefficient along the incoming ray
  cosTheta_i = sihat * nhat * -1.0;
  localReflectionCoefficient( cosTheta_i, permittivity, media, 
                              Rparallel, Rperpendicular );

  // define the reflection matrix for different dielectric types
  // multiply out all terms to give the electric field
  astigmatic      = p / ( p + s );
  phase            = expminusj( k1 * s );
  E_rperpendicular = Rperpendicular * E_iperpendicular * astigmatic * phase;
  E_rparallel      = Rparallel      * E_iparallel      * astigmatic * phase;

  // convert back into global coordinate system
  //cout << "Ei before reflection: " << Ei << endl;
  Ei = CComplexVector( er_perpendicular ) * E_rperpendicular 
     + CComplexVector( er_parallel      ) * E_rparallel;
  //cout << "Ei after reflection: " << Ei << endl;

  return true;
}

boolean nthImageReflection(          CComplexVector&   Ei,
                               const CSegment3d&       si,
                               const CSegment3d&       sr,
                               const double&           pathLength,
                               const double&           k1,
                                     complex           permittivity,
                                     complex           Pa,
                                     complex           Pd,
                                     double            Eamp,
                                     double            t,
                                     int               n,
                               const mediaType&        media )
{
  CPoint3d nhat, sihat, srhat;
  CPoint3d ei_perpendicular, ei_parallel, er_perpendicular, er_parallel;
  double   p, s, astigmatic, cosTheta_i;
  complex  Rparallel, Rperpendicular, phase;
  complex  E_iparallel, E_iperpendicular, E_rparallel, E_rperpendicular;
  complex  Tparallel_12, Tparallel_23, Tperpendicular_12, Tperpendicular_23;

  nhat = CPoint3d(0.0,0.0,1.0);

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  srhat            = sr.unitVector();
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = ei_perpendicular ^ sihat;
  }

  er_perpendicular = ei_perpendicular;
  er_parallel      = nhat * ( nhat * ei_parallel ) * 2.0 - ei_parallel;

  // define unit vectors and lengths along the ray
  p                = pathLength;
  s                = sr.length();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  // components of reflection coefficient along the incoming ray
  cosTheta_i = sihat * nhat * -1.0;
  localReflectionCoefficient( cosTheta_i, permittivity, media, 
                              Rparallel, Rperpendicular );
  //cout << "R: " << Rparallel << "  " << Rperpendicular << endl;
  //cout << "Pa & Pd: " << Pa << "  " << Pd << endl;
  transmissionFreeSpaceToDielectric( cosTheta_i, permittivity, media, 
                                     Tparallel_12, Tperpendicular_12 );
  transmissionDielectricToFreeSpace( cosTheta_i, permittivity, media, 
                                     Tparallel_23, Tperpendicular_23 );
  
  Rparallel = -Tparallel_12 * Tparallel_23 * pow( Rparallel, 2*n-1)
            * pow( qsqr( Pd ) * Pa, n );
  Rperpendicular = -Tperpendicular_12 * Tperpendicular_23
                 * pow( Rperpendicular, 2*n-1 ) * pow( qsqr( Pd ) * Pa, n );

  //cout << "p & s & t: " << p << "  " << s << "  " << t << endl;
  // define the reflection matrix for different dielectric types
  // multiply out all terms to give the electric field
  astigmatic       = p / ( p + s + 2.0 * t );

  if ( permittivity.imag() > TOL )
    astigmatic *= Eamp;

  phase            = expminusj( k1 * s );
  E_rperpendicular = Rperpendicular * E_iperpendicular * astigmatic * phase;
  E_rparallel      = Rparallel      * E_iparallel      * astigmatic * phase;

  // convert back into global coordinate system
  Ei = CComplexVector( er_perpendicular ) * E_rperpendicular 
     + CComplexVector( er_parallel      ) * E_rparallel;

  return true;
}

boolean infiniteReflection(          CComplexVector&   Ei,
                               const CSegment3d&       si,
                               const CSegment3d&       sr,
                               const double&           pathLength,
                               const double&           k1,
                                     complex           permittivity,
                                     complex           Pa,
                                     complex           Pd,
                                     double            Eamp,
                               const mediaType&        media )
{
  CPoint3d nhat, sihat, srhat;
  CPoint3d ei_perpendicular, ei_parallel, er_perpendicular, er_parallel;
  double   p, s, astigmatic, cosTheta_i;
  complex  Rparallel, Rperpendicular, phase;
  complex  E_iparallel, E_iperpendicular, E_rparallel, E_rperpendicular;

  nhat = CPoint3d(0.0,0.0,1.0);

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  srhat            = sr.unitVector();
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = ei_perpendicular ^ sihat;
  }

  er_perpendicular = ei_perpendicular;
  er_parallel      = nhat * ( nhat * ei_parallel ) * 2.0 - ei_parallel;

  // define unit vectors and lengths along the ray
  p                = pathLength;
  s                = sr.length();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  // components of reflection coefficient along the incoming ray
  cosTheta_i = sihat * nhat * -1.0;
  localReflectionCoefficient( cosTheta_i, permittivity, media, 
                              Rparallel, Rperpendicular );

  Rparallel      = Rparallel * ( 1.0 - qsqr( Pd ) * Pa )
                 / ( 1.0 - qsqr( Rparallel ) * qsqr( Pd ) * Pa );
  Rperpendicular = Rperpendicular * ( 1.0 - qsqr( Pd ) * Pa )
                 / ( 1.0 - qsqr( Rperpendicular ) * qsqr( Pd ) * Pa );

  // define the reflection matrix for different dielectric types
  // multiply out all terms to give the electric field
  astigmatic       = p / ( p + s );

  if ( permittivity.imag() > TOL )
    astigmatic *= Eamp;

  phase            = expminusj( k1 * s );

  E_rperpendicular = Rperpendicular * E_iperpendicular * astigmatic * phase;
  E_rparallel      = Rparallel      * E_iparallel      * astigmatic * phase;

  // convert back into global coordinate system
  Ei = CComplexVector( er_perpendicular ) * E_rperpendicular 
     + CComplexVector( er_parallel      ) * E_rparallel;

  return true;
}

boolean singleInfiniteTerm(          CComplexVector&   Ei,
                                     List<complex>&    Eterms,
                               const CSegment3d&       si,
                               const CSegment3d&       sr,
                               const double&           pathLength,
                               const double&           k1,
                                     complex           permittivity,
                                     complex           Pa,
                                     complex           Pd,
				     double            l,
                               const mediaType&        media )
{
  CPoint3d nhat, sihat, srhat;
  CPoint3d ei_perpendicular, ei_parallel, er_perpendicular, er_parallel;
  double   p, s, astigmatic, cosTheta_i;
  complex  Rparallel, Rperpendicular, phase;
  complex  Tparallel_12, Tparallel_23, Tperpendicular_12, Tperpendicular_23;
  complex  E_iparallel, E_iperpendicular, E_rparallel, E_rperpendicular;
  complex  temp1, temp2;
  int      n;

  nhat = CPoint3d(0.0,0.0,1.0);

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  srhat            = sr.unitVector();
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = ei_perpendicular ^ sihat;
  }

  er_perpendicular = ei_perpendicular;
  er_parallel      = nhat * ( nhat * ei_parallel ) * 2.0 - ei_parallel;

  // define unit vectors and lengths along the ray
  p                = pathLength;
  s                = sr.length();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  // components of reflection coefficient along the incoming ray
  cosTheta_i = sihat * nhat * -1.0;
  localReflectionCoefficient( cosTheta_i, permittivity, media, 
                              Rparallel, Rperpendicular );

  transmissionFreeSpaceToDielectric( cosTheta_i, permittivity, media, 
                                     Tparallel_12, Tperpendicular_12 );
  transmissionDielectricToFreeSpace( cosTheta_i, permittivity, media, 
                                     Tparallel_23, Tperpendicular_23 );
  
  for ( n = 0; n < 40; n++ )
  {
    if ( n == 0 )
    {
      temp1 = Rparallel;
      temp2 = Rperpendicular;
    }
    else
    {
      temp1 = -Tparallel_12 * Tparallel_23 * pow( Rparallel, 2*n-1) 
            * pow( qsqr( Pd ) * Pa, n );
      temp2 = -Tperpendicular_12 * Tperpendicular_23 
            * pow( Rperpendicular, 2*n-1 ) * pow( qsqr( Pd ) * Pa, n );
    }

    // define the reflection matrix for different dielectric types
    // multiply out all terms to give the electric field
    astigmatic       = p / ( p + s );
    phase            = expminusj( k1 * s );
    E_rparallel      = temp1 * E_iparallel      * astigmatic * phase;
    E_rperpendicular = temp2 * E_iperpendicular * astigmatic * phase;

    // convert back into global coordinate system
    Ei = CComplexVector( er_perpendicular ) * E_rperpendicular 
       + CComplexVector( er_parallel      ) * E_rparallel;
    Eterms.append( Ei.x );
  }

  return true;
}

void transmissionFreeSpaceToDielectric(     CComplexVector&   Ei,
                                      const CSegment3d&       si,
                                            double            cosTheta_i,
                                            complex           permittivity,
                                            double            k1,
                                      const mediaType&        media )
{
  CPoint3d       nhat, sihat, srhat;
  CPoint3d       ei_perpendicular, ei_parallel;
  double         p;
  complex        Tparallel, Tperpendicular, sqrtTerm;
  complex        E_iparallel, E_iperpendicular;

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  nhat             = CPoint3d( 0.0, 0.0, 1.0 );
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = sihat ^ ei_perpendicular;
  }

  // define unit vectors and lengths along the ray
  p                = si.length();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  // define the transmission matrix for different dielectric types

  cosTheta_i     = sihat * nhat * -1.0;
  sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
  Tparallel      = 2.0 * sqrt(permittivity) * cosTheta_i 
                 / ( permittivity * cosTheta_i + sqrtTerm );
  Tperpendicular =  complex( 2.0 * cosTheta_i ) / ( cosTheta_i + sqrtTerm );

  // multiply out all terms to give the electric field
  E_iperpendicular *= Tperpendicular;
  E_iparallel      *= Tparallel;

  // convert back into global coordinate system
  Ei = CComplexVector( ei_perpendicular ) * E_iperpendicular 
     + CComplexVector( ei_parallel ) * E_iparallel;
}

void transmissionDielectricToFreeSpace(     CComplexVector&   Ei,
                                      const CSegment3d&       si,
                                            double            cosTheta_i,
                                            complex           permittivity,
                                            double            k1,
                                      const mediaType&        media )
{
  CPoint3d       nhat, sihat, srhat;
  CPoint3d       ei_perpendicular, ei_parallel;
  complex        Tparallel, Tperpendicular, sqrtTerm;
  complex        E_iparallel, E_iperpendicular;

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  nhat             = CPoint3d( 0.0, 0.0, 1.0 );
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = sihat ^ ei_perpendicular;
  }

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  // define the transmission matrix for different dielectric types
  sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
  Tparallel      = 2.0 * sqrt(permittivity) * sqrtTerm 
                 / ( permittivity * cosTheta_i + sqrtTerm );
  Tperpendicular =  sqrtTerm * 2.0 / ( cosTheta_i + sqrtTerm );

  // multiply out all terms to give the electric field
  E_iperpendicular *= Tperpendicular;
  E_iparallel      *= Tparallel;

  // convert back into global coordinate system
  Ei = CComplexVector( ei_perpendicular ) * E_iperpendicular 
     + CComplexVector( ei_parallel ) * E_iparallel;
}

CComplexVector incidentField(    CSegment3d& si, double k1 ) 
{
  CPoint3d       xhat, yhat, zhat, unitIncInPlane;
  CComplexVector Et, EiPolar;
  CPoint3d       x_l, y_l, z_l;
  double         phi, theta;

  unitIncInPlane = si.unitVector();

  if ( fabs( fabs(unitIncInPlane.z) - 1.0 ) < TOL )
    Et = CComplexVector();
  else
  {
    x_l  = CPoint3d( 1.0, 0.0, 0.0 );
    y_l  = CPoint3d( 0.0, 1.0, 0.0 );
    z_l  = CPoint3d( 0.0, 0.0, 1.0 );
    theta= si.angle( z_l );
    phi  = atan2( unitIncInPlane * y_l, unitIncInPlane * x_l );
    //EiPolar = CComplexVector( 0.0, expminusj( k1 * si.length() )
                            /// si.length(), 0.0 );
    EiPolar = CComplexVector( 0.0, expminusj( k1 * si.length() )
                            / sqrt( k1 * si.length() ), 0.0 );
    cout << "incidentField: " << endl << EiPolar << endl;
    Et = EiPolar.sphericalToRectangular( x_l, y_l, z_l, theta, phi );
  }

  return Et;
}

void meanAbsSum( List<complex>& allRayPaths, double& randomWalkMean )
{
  int      i, l;
  complex  fieldStrength, expjtest, temp;
  //int      numberOfIterations = 1000;
  int      numberOfIterations = 1;


  for ( i = 0; i < numberOfIterations; i++ )
  {
    temp = 0.0;
    for ( l = 0; l < allRayPaths.size(); l++ )
    {
      fieldStrength = allRayPaths.GetItem(l);

      //expjtest = expj( TwoPi * ( double(rand()) / double(RAND_MAX) ) );
      expjtest = complex( 1.0, 0.0 );
      temp += fieldStrength * expjtest;
    }
    //randomWalkMean += fabs( temp );
    //
  }

  //randomWalkMean = randomWalkMean / numberOfIterations;
  randomWalkMean = fabs( temp );
}

double trapezoidal( double *E, int N, double r, double a, double stepsize )
{
  double zeta, startStep, integ1, integ2, a1, b1, m1;
  double stepsize2, totalIntegral;
  int    m, M, i;

  totalIntegral = 0.0;
  a1            = a;
  integ1        = 0.0;

  // integrate over zeta to obtain the PDF
  for ( i = 1; i < 100000; i++ )
  {
    b1        = a1 + stepsize;
    stepsize2 = stepsize / 2.0;
    m1        = ( b1 + a1 ) / 2.0;

    M = 1;
    // set up initial trapezoidal integration rule
    integ2  = WnFunction( E, N, r, a1, stepsize2 / 2.0 );
    integ2 += WnFunction( E, N, r, m1, stepsize2 );
    integ2 += WnFunction( E, N, r, b1, stepsize2 / 2.0 );

    // calculate the integral by using an adaptive version of rule
    while ( fabs( ( integ1 - integ2 ) / integ2 ) > 1.0e-3 )
    {
      integ1    = integ2;
      M         = M * 2;
      stepsize2 = stepsize2 / 2.0;
      startStep = a1 - stepsize2;
      integ2    = integ1 / 2.0;
      for ( m = 1; m <= M; m++ )
      {
        zeta = startStep + m * stepsize2 * 2.0;
        integ2 = integ2 + WnFunction( E, N, r, zeta, stepsize2 );
      }
    }
    totalIntegral = totalIntegral + integ2;

    if ( fabs( integ2 / totalIntegral ) < 1.0e-3 )
      break;

    a1 = b1;
  } 

  return totalIntegral;
}

double WnFunction( double *E, int N, double r, double zeta, double stepsize )
{
  double Jproduct;
  int    k;

  Jproduct = 1.0;

  // calculate the product of Jo(zeta * E )
  for ( k = 0; k < N; k++ )
  {
    Jproduct = Jproduct * j0( zeta * E[k] );
  }

  return r * r * zeta * j0( zeta * r ) * Jproduct * stepsize;
}

double besselJ0( double x )
{
  if ( x > 3.0 )
    return SqrtTwoOverPi / sqrt(x) * cos( x - PiOver4 );
  else
    return j0( x );
}

void randomWalkMean( List<complex>& allRayPaths, double& randomWalkMean )
{
  int            N, i, size2;
  double         *E;
  double         stepsize1, stepsize2;
  double         integ2, r, Wn, sumE;
  complex        fieldStrength;
  CMemoryStorage storageObject;

  N  = allRayPaths.size();

  E  = storageObject.CreateDoubleVector( N );
  sumE = 0.0;

  // convert data into real valued data in each sector of the server
  for ( i = 0; i < N; i++ )
  {
    fieldStrength = allRayPaths.GetItem(i);
    E[i]          = fabs( fieldStrength );
    sumE         += E[i];
  }

  // if rician distributed (LOS large) can not use integral formula
  // so use same mean instead and return
  if ( E[0] / sumE > 0.95 && sumE != 0.0 )
  {
    meanAbsSum( allRayPaths, randomWalkMean );
    return;
  }

  // if rayleigh distributed then perform integral formula for random walk
  // mean value, do this for each sectorized antenna in the current server
  size2     = 1000;
  stepsize1 = 20.0;
  stepsize2 = 0.05;
    
  randomWalkMean = 0.0;
  integ2         = 1.0;
  r              = stepsize2;
  
  // integrate over r to obtain the CDF
  for ( i = 1; i < size2; i++ )
  {
    Wn             = trapezoidal( E, N, r, 0, stepsize1 );
    integ2         = Wn * stepsize2;
    randomWalkMean = randomWalkMean + integ2;
  
    r = r + stepsize2;
  
    if ( fabs(integ2/randomWalkMean) < 1.0e-3 ) 
      break;
  }

  if ( i == 1000 ) 
    cout << "there are too many ungained mean integrations" << endl;
 
  if ( randomWalkMean < 0.0 )
  {
    //ofstream coutput( "Pray.txt" );
  
    cout << "negative value in integral, first Value %: " 
         << E[0] / sumE << endl;
    meanAbsSum( allRayPaths, randomWalkMean );
    return;
  }

  storageObject.DeleteDoubleVector( E );
}

complex clemmowFlatPlate( double theta_i, double k1,
                          double s, complex permittivity )
{
  complex x;
  x = complex( 2.0 * cos( theta_i ) )
       / ( cos( theta_i ) + sqrt( permittivity - qsqr( sin( theta_i ) ) ) )
       * expminusj( k1 * s ) / sqrt( k1 * s );
  cout << "incidentField.z: " << endl << x << endl;
  return x;
}
