//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    23rd January 2002
// Last Modification:     Dated below "Added dd/mm/yy"
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef DIFFRACTION_COEFFICIENT_HH
#define DIFFRACTION_COEFFICIENT_HH

#include <iostream>
#include <cmath>
#include "userconst.hh"
#include "maths-utilities.hh"

class CDiffractionCoefficient
//==========================================================================
// = FUNCTION 
//     diffcoeff.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     The definition of a lossy diffraction coefficient for wedges 
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:

    complex softDiffraction( double theta_s, double theta_i, double n,
                             double k1,      double beta0,    double L,
			     complex Rperp1, complex Rperp2 );
    //
    // Diffraction coefficient for E-Polarization, (soft or perpendicular )
    // basic version found in Balanis P.789 (PEC)
    // Lossy version found in: Tapan K. Sarkar, "Efficient Ray-tracing methods
    // for propagation prediction for Indoor Wireless Communications",
    // IEEE AP Vol.43, No. 2, April, 2001.
    //

    complex hardDiffraction( double theta_s, double theta_i, double n,
                             double k1,      double beta0,    double L,
			     complex Rparallel1, complex Rparallel2 );
    //
    // Diffraction coefficient for H-Polarization, (hard or parallel )
    // basic version found in Balanis P.789 (PEC)
    // Lossy version found in: Tapan K. Sarkar, "Efficient Ray-tracing methods
    // for propagation prediction for Indoor Wireless Communications",
    // IEEE AP Vol.43, No. 2, April, 2001.
    //
     
    complex Dminus( double x, double n, double k1, double L );
    complex  Dplus( double x, double n, double k1, double L );
    //
    // one of the four term of the diffraction coefficient. These are
    // defined in Balanis pp. 787
    //

    complex fresnelTransitionFunction( double x );
    //
    // fresnel transition function using the fresnel integral equation
    // in Appendix III of Balanis pp. 929
    //

    complex fresnel( double x );
    //
    // Fresnel integral (Numerical Recipes in C, pp. 256-257
    //
};

#endif
