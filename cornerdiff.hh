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

#ifndef CORNER_DIFFRACTIONS_HH
#define CORNER_DIFFRACTIONS_HH

#include <iostream>
#include <cmath>
#include "userconst.hh"
#include "corners.hh"
#include "maths-utilities.hh"
#include "ComplexVector.hh"
#include "segment3d.hh"

class CCornerDiffraction
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
    complex cornerDiffractionCoefficient(
                               double  theta_s, double  theta_i, double n,
                               double  k1,      double  sinbeta0,  
                               double  beta0,   double  beta0c, double L,
                               double  Lc, complex Rperp1,  complex Rperp2 );
    //
    // Diffraction coefficient for E-Polarization, (soft or perpendicular )
    // basic version found in SuperNEC paper
    //

    boolean checkIfCornerDiffractionExists( CCorners& corners,
                                            const CSegment3d& diffractionEdge,
                                            CPoint3d& p1, CPoint3d& q1 );
    //
    // check if the corner diffraction points exists for 
    //
     
    complex cornerDiffractionConst( double k1, double sinBeta0, double betaC,
                                    double beta0c, double Lc, double sr );
    //
    // constant term in the corner diffraction calculation
    //
    
    double a( double x );
    //
    // a(.) function required for cos squared term.
    //

    double fi( double x, double L, double Lc, double beta0, double beta0c );
    //
    // Definition of the f(.) function which can take on 4 possible values
    // for each diffraction point, but because we are using flat plates, it
    // was changed so that only 2 possibilities are needed.
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
