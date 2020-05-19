//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    April 4th 2002
// Last Modification:     Dated below "Added dd/mm/yy"
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef LINEAR_PROGRAMMING_H
#define LINEAR_PROGRAMMING_H

#include <iostream>
#include "matrix.hh"

class CLinearProgram
//==========================================================================
// = FUNCTION 
//     linear-program.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     The method of steepest descents is implemented here, which is 
//     required as part of the SUMT (sequential unconstrained minimisation
//     technique). Each of these methods are described in Boyd's Convex
//     Optimisation, Chapters 7 and 8.
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
    boolean internalPenalizedFunction( const CVector<double>& x, double t, 
                                       const CVector<double>& c, 
				       const CMatrix<double>& A,
                                       const CVector<double>& b, double& f0 );
    //
    // defined the internal penalized function
    // f(x) = tc^{T}x - \sum_{i=1}^{m} log( b_i - A^{T}x )
    //

    CVector<double> gradient( double          t,
                              const CVector<double>& c, 
                              const CMatrix<double>& A,
			      const CVector<double>& d );
    //
    // gradient = derivative of each term
    //          = tc + A^{T}d = \nabla f(x)  (defined above)
    //

    CVector<double> steepestDescentSUMT(
                                  double beta, double alpha,
                                  double eta,  double epsilon,
                                  double mu,   double t0,
                                  int    m,    int    n,
                                  const CVector<double>& c, 
				  const CMatrix<double>& A,
                                  const CVector<double>& x0,
				  const CVector<double>& b );
    //
    // Sequential unconstrained minimization technique (SUMT).
    // returns the result as a vector
    //
};

#endif
