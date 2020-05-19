#include <iostream.h>
#include "diffcoeff.hh"

void main()
{
  CDiffractionCoefficient diff;
  // test to see if code is identical to matlab version for PECs
  //CDiffractionCoefficient diffCoeff;
  double x;

  //x = diffCoeff.softDiffraction( Pi / 4.0, 5.0 * Pi / 4.0, 1.5, TwoPi, 
                                 //1.0, 10, -1.0, -1.0 );
  ofstream outputfile( "fresnel.txt" );

  for ( x = 0; x < 150; x+=0.1 )
    outputfile << x << "   " << diff.fresnelTransitionFunction( x ) << endl;

  outputfile.close();
}

