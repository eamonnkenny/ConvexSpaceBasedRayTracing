//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny, William O' Brien
// Project Manager:       Dr. Peter J. Cullen
// First Modification:    November 19th 2001
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef USERCONST_HH
#define USERCONST_HH

using namespace std;
#include <cmath>
// 300903 #include <process.h>
#include <stdlib.h>

#include "complex.hh"
#include "boolean.hh"

inline int myIsnan(double x) { return (__isnan(x)); }

// commonly used numbers in numerical methods
const double Pi            = acos(-1.0);
const double PiOver180     = ( Pi / 180.0 );
const double PiOver4       = ( Pi / 4.0 );
const double PiOver2       = ( Pi / 2.0 );
const double ThreePiOver2  = ( 1.5  * Pi );
const double ThreePiOver4  = ( 0.75 * Pi );
const double TwoOverPi     = ( 2.0  / Pi );
const double TwoPi         = 2.0  * Pi;
const double FourPi        = 4.0  * Pi;
const double SqrtTwoOverPi = sqrt( TwoOverPi );
const double SqrtPiOver2   = sqrt( PiOver2 );
const double EXP1          = exp( 1.0 );
const double Permittivity  = 8.85419412e-12;
const double Permeability  = 1.256637061e-6;
const double SpeedOfLight  = 1.0 / sqrt( Permittivity * Permeability );
const double Impedance     = sqrt( Permeability / Permittivity );
const double SelfTermConst = ( 1.781 / (4.0 * EXP1) );

// BTS types
const int    LINE_SOURCE = 0;
const int    PLANE_WAVE  = 1;

// Line of sight flags
const int    LOS               = 1;
const int    NLOS              = 0;
const int    DIFFRACTION_POINT = 0;
const int    MOBILE_POINT      = 1;

// Define numbering flags from 3d meshes in plotmtv
const int    NUMBERS      = 0;
const int    NONUMBERS    = 1;
const int    FLOAT_MAX    = 9999;

// pre-defined numbers
const complex ComplexZero =  complex( 0.0, 0.0 );
const double  ZERO        =  0.0;
const double  TOL         =  1.0e-10;
const double  TOL2        =  1.0e-6;
const double  TOL3        =  1.0e-3;
const double  TOL4        =  1.0e-1;
const double  BIG_PLUS    =  1.0e+20;
const double  BIG_MINUS   = -1.0e+20;
const double  HERTZ       =  1.0e+6;

// 3d mfie angle tables
const int    NUM_THETA    = 10;
const int    NUM_PHI      = 240;

// ratio of minor to major axes of Fresnel Zone
const double FRESNEL_RATIO =  0.05;

// boolean value to signify that the Janaswamy parabolic equation 
//method is being used to calculate the propagation over rooftops
const boolean   JANASWAMY_METHOD =  false;

// boolean value to signify Fresnel zone checking
const boolean   FRESNEL_ZONE    =  true;

const boolean   TERRAIN_REFLECT =  true;

// efie profile differential speedup
const int    DIFFERENTIAL_OFF = 10;
const int    DIFFERENTIAL_ON  =  0;

// Clockwise and anti-clockwise orientations
const int    ANTI_CLOCKWISE   =  1;
const int    CLOCKWISE        = -1;

// define the profile differential to be turned on or off
const boolean   PROFILE_DIFFERENTIAL = true;

// LEDA window definitions with on/off facility
const boolean   GUI_OPTION  = true;

// LEDA window Mouse button definitions
const int    RIGHT_MOUSE =   -3;
const int    MIDDLE_MOUSE=   -2;
const int    LEFT_MOUSE  =   -1;

// LEDA window size
// Screen size - Window size (1024) allow for border of width 6 pixels
const int    WINDOW_SIZE =  500;
const int    PLOTMTV_PLACEMENT = 512; 

// Number of RGB colors defined within a LEDA window
const int    NUM_COLORS     = 18;
const int    RGB_NUM_COLORS = 64;

// matrix sizes (redundant since matrix.hh inclusion)
const int    MAX_ROW     = 300000;
const int    MAX_COL     = 300000;
const int    SCOL        = 6000;
const int    MAX_SCR     = 70000;

const long   EARTH_RADIUS = 6366200;

// plotmtv on or off option
const int    PLOTMTV     = 1;

// Step-size for step along profiles
const double RURAL_STEP_SIZE = 50.0;
const double URBAN_STEP_SIZE = 10.0;

//  constants for ray tracing model
//  19/11/01
const int      MAX_CONVEX         = 1000;        // Added 09/01/01
const int      NUMBER_OF_MIMO_TRANSMITTERS = 6;  // Added 05/04/02

// complex or double squared.
inline double qsqr( const double& x ) { return x * x; }
inline complex qsqr( const complex& x ) { return x * x; }
inline double qabs( complex z1, complex z2 ) 
{ return ( sqrt( qsqr( z1.real() - z2.real() ) + qsqr( z1.imag() - z2.imag() ) ) ); }
inline double qabs( complex z1 ) 
{ return ( sqrt( qsqr( z1.real() ) + qsqr( z1.imag() ) ) ); }
inline double RAD( double AngleInSeconds )
{ return (AngleInSeconds/3600.0*Pi/180.0); }
// 300903 inline int round( double x ) { return ( x < 0 ) ? int(x-0.5): int(x+0.5); }
inline int sign( double x )  { return ( x < 0 ) ? -1: 1; }

#endif 

