//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    31st Mar 2003
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "diffcoeff.hh"

void main()
{
  // test to see if code is identical to matlab version for PECs
  CDiffractionCoefficient diffCoeff;
  double  x;
  complex y;
  ofstream outputfile( "diffract.txt" );

  // calculate the incident shadow boundary term
  for ( x = 0; x < 360; x++ )
  {
    y = diffCoeff.softDiffraction( Pi / 6.0, x * Pi / 180.0, 2, TwoPi, 
                                   1.0, 100, 0.0, 0.0 ) * expminusj( TwoPi *
				   100 ) / 10.0;
    outputfile << x << "   " << fabs(y) << endl;
  }
  outputfile.close();

  
  outputfile.open( "diffract2.txt" );

  // calculate the reflective shadow boundary term
  for ( x = 0; x < 360; x++ )
  {
    y = diffCoeff.softDiffraction( Pi / 6.0, x * Pi / 180.0, 2, TwoPi, 
                                   1.0, 100, 1.0, 1.0 );
    y -= diffCoeff.softDiffraction( Pi / 6.0, x * Pi / 180.0, 2, TwoPi, 
                                    1.0, 100, -1.0, -1.0 );
    y *= expminusj( TwoPi * 100 ) / 20.0;
    outputfile << x << "   " << fabs(y) << endl;
  }
  outputfile.close();
}

