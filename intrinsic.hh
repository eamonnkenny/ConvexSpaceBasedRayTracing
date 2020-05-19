//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     October 2001
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef INTRINSIC_HH
#define INTRINSIC_HH

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <time.h>

#include "complex.hh"
#include "userconst.hh"
#include "bts.hh"
#include <string>
using namespace std;

#ifndef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport) 
#else
  #define EXPORT_CLASS_DECLSPEC class
#endif

EXPORT_CLASS_DECLSPEC CIntrinsic
//==========================================================================
// = FUNCTION
//      intrinsic.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
// This class contains all definitions of intrinsic complex number and 
// real (double) numbers operations used by the propagation model classes.
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved
//==========================================================================

{ 
  public:
    //======================================================================
    // = FUNCTIONS
    //======================================================================

    complex AntennaPosition( double, double, double );
    //
    // Determine the exact position of the antenna given the point on the 
    // ground and the relative height of the antenna.
    //

    int     Round( double x );
    //
    // Round-off of a double to integer.
    //

    int     Minimum( int i, int j );
    double  Minimum( double i, double j );
    //
    // Calculate the minimum of two integers, and return that integer.
    //

    double  Minimum( double p1, double p2, double p3, double p4 );
    //
    // Calculate the minimum of four doubles.
    //

    double  Minimum( double, double, double );
    //
    // Calculate the minimum of three doubles, and return that double.
    //

    double  Minimum( double, double, double, double, double, double );
    //
    // Calculate the minimum of six doubles, and return that double.
    //

    double  Maximum( double, double, double );
    //
    // Calculate the maximum of three doubles, and return that double.
    //

    double  Maximum( double, double, double, double, double, double );
    //
    // Calculate the maximum of six doubles, and return that double.
    //

    double  Maximum( double, double, double, double );
    //
    // Calculate the maximum of four doubles, and return that double.
    //

    int     Maximum( int i, int j );
    double  Maximum( double i, double j );
    //
    // Calculate the maximum of two integers, and return that integer.
    //

    void    itoa( int i, char* c );
    //
    // Convert an integer to its character representation.
    //

    bool   CheckFileExists( const string& Inputfile );
    //
    // Check that the Inputfile is contained in $DTM_PATH directory.
    //

    int  occurrencesInFile( const string& str, const string& Inputfile );
    //
    // Calculate the number of occurences of a work in a file
    //

    bool readToString( string str, ifstream& filePtr );
    bool readToString( string str, ifstream& filePtr, const string& terminationString );
    //
    // in a file read to the string containing characters_ and stop there.
    //

    string readFullString( string str, ifstream& filePtr );
    //
    // In the string is a substring of a string in the file, then return
    // it's full string.
    //

    void   CheckTerrainFileExists( char* Inputfile );
    //
    // Check that the Inputfile is contained in $DTM_PATH directory.
    //

    void   CheckParameterFileExists( char* Inputfile );
    //
    // Check that the Inputfile is contained in current directory.
    //

    void   PrintTen( int i );
    //
    // Print the integer i and return if (i-1) mod 10 = zero
    //

    void   ReturnFileName( char *FileName, char *InputFileName );
    //
    // Given an InputFileName return the exact FileName as $(DTM_PATH)
    // plus the InputFileName.
    //

    double LogHorizontalGain( double HorizontalAngle,
                              const  CBaseTransceiver& BTSobject );
    double LogVerticalGain(   double VerticalAngle,
                              const CBaseTransceiver& BTSobject );

    void   swap( long&    i, long&    j );
    void   swap( double&  x, double&  y );
    void   swap( complex& x, complex& y );
    //
    // swap i and j and return.
    //

    int  StartClock();
    //
    // Start a clock counter and return the time
    //

    void ClockTime( int StartTime );
    void ClockTime( int StartTime, const string& str );
    //
    // Given the start time for a timer, print the elapsed time to the screen
    // Print an appropriate print statement str if given.
    //

    void  IntegerToHexidecimal( int i, string& str );
    char  OneIntegerToHexidecimal( int i );
    //
    // Convert an integer (base 10) to hexidecimal (base 16). Do this for a
    // single integer less than 16 or an integer greater than equal to 16.
    //

    double RadiansToDegrees( double AngleInRadians );
    double DegreesToRadians( double AngleInDegrees );
    //
    // Conversion of Angles to Radians or Degrees
    //

    double mantissa( double FullDoubleNumber );
    //
    // Calculate the mantissa of a double number (after the decimal point)
    //
};

#endif
