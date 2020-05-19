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

#include "linear-program.hh"

boolean CLinearProgram::internalPenalizedFunction( 
                                  const CVector<double>& x, double t, 
                                  const CVector<double>& c,
                                  const CMatrix<double>& A, 
                                  const CVector<double>& b, double& f0 )
{
  CVector<double> barrierFunction;

  barrierFunction = b - A * x;
  if ( !barrierFunction.positive() )
  {
    f0 = 0.0;
    barrierFunction.clear();
    return false;
  }

  barrierFunction = barrierFunction.vlog(); 
  f0 = t * ( c * x ) - barrierFunction.sum();
  barrierFunction.clear();

  return true;
}


CVector<double> CLinearProgram::gradient( double t, const CVector<double>& c, 
                          const CMatrix<double>& A, const CVector<double>& d )
{
  return ( c * t + A.transpose( d ) ); 
}


CVector<double> CLinearProgram::steepestDescentSUMT(
                                  double beta, double alpha,
                                  double eta,  double epsilon,
                                  double mu,   double t0,
                                  int    m,    int    n,
                                  const CVector<double>& c, 
                                  const CMatrix<double>& A,
                                  const CVector<double>& x0, 
                                  const CVector<double>& b ) 
{
  CVector<double> V, d, x, gradfx, xPlusVr;
  double          t, r, Vsd;
  boolean         fFeasible;
  double          fnextPoint, fcurrent, fprevious;
  int             i, e_i, numberOfIterations = 2000;
  ofstream        coutput( "sumt.txt" );

  //xpath = zeros(2,numberOfIterations);

  //cout << "initialisation" << endl;
  // set initial values
  x = x0;
  t = t0;

  // initial vector sizes
  //cout << "initialisation sizes" << endl;
  d = CVector<double>( m );
  V = CVector<double>( n );
  
  // sequential part of SUMT
  for ( i = 0; i < numberOfIterations; i++ )
  {
    r         = 0.0; 
    fFeasible = true;

    gradfx = gradient( t, c, A, d );
    while ( sqrt( gradfx * gradfx ) > eta )
    {
      if ( fFeasible == false )
      {
        cout << "problem with function logarithm" << endl;
        break;
      }
  
      x = x + V * r;

      // use an L_infinity-norm to find the steepest descent direction
      // this direction is in the direction of maximum increase
      // define the diagonal term d

      d.setOnes(); 
      d = d / ( b - A * x );

      gradfx = gradient( t, c, A, d );
      Vsd    = gradfx.linfinitynorm(e_i); // the L_infinity-norm of grad f
      //cout << "gradient: " << sqrt( gradfx * gradfx ) << endl;

      V = CVector<double>(n);
      V.set( -gradfx[e_i], e_i );
  
      // backtracking line search algorithm
      r = 1.0;
      fFeasible = internalPenalizedFunction(     x, t, c, A, b, fcurrent );
      xPlusVr = x+V*r;
      fFeasible = internalPenalizedFunction( xPlusVr, t, c, A, b, fnextPoint );
      fprevious = fcurrent + alpha * r * ( gradfx * V );

      while ( fnextPoint > fprevious || fFeasible == false )
      {
        r *= beta;
        xPlusVr = x+V*r;
        fFeasible = internalPenalizedFunction(xPlusVr, t, c, A, b, fnextPoint );
        fprevious = fcurrent + alpha * r * ( gradfx * V );

        if ( r < 1.e-12 && fFeasible == true )
        {
          //cout << "we need to cut down the number of iterations here" << endl;
          break;
        }
      }
      if ( r < 1.e-12 && fFeasible == true )
      {
        //cout << "we need to cut down the number of iterations here 2" << endl;
        break;
      }
    }
  
    if ( fFeasible == false )
    {
      cout << "no feasible solution possible: stop" << endl;
      break;
    }

    /*if ( r < 1.e-15 )
    {
      cout << "we need to cut down the number of iterations here 3" << endl;
      break;
    }*/
  
    // condition for stopping algorithm
    if ( m / t < epsilon ) 
    {
      cout << "Stopping Criteria for Convex Optimisation met" << endl;
      break;
    }
    cout << "increasing t: " << t << endl;
    x.write( coutput );
    coutput << endl;
  
    t = mu * t;
  }
  x = xPlusVr;
  V.clear();
  gradfx.clear();
  xPlusVr.clear();
  d.clear();

  return x;
}
  
