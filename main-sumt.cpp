#include <iostream.h>
#include <cmath>

#include "matrix.hh"
#include "storage.hh"
#include "linear-program.hh"

void main()
{
  double **array;
  double *bvector;
  double beta, alpha, eta, t0, mu, epsilon;
  CMatrix<double> A;
  CVector<double> b, x0, x, c;
  CMemoryStorage  memoryObject;
  CLinearProgram  linearProgramObject;
  int             m, n;

  m = 7;
  n = 2;
  array   = memoryObject.CreateDoubleMatrix( m, n );
  bvector = memoryObject.CreateDoubleVector( n );
  
  array[0][0] = -6;  array[0][1] = -1;
  array[1][0] = -3;  array[1][1] = -1;
  array[2][0] = -4;  array[2][1] = -6;
  array[3][0] =  1;  array[3][1] =  0;
  array[4][0] =  0;  array[4][1] =  1;
  array[5][0] =  -1; array[5][1] =  0;
  array[6][0] =  0;  array[6][1] =  -1;
  bvector[0] = -12;
  bvector[1] =  -8;
  bvector[2] = -24;
  bvector[3] =   5;
  bvector[4] =   5;
  bvector[5] =   0;
  bvector[6] =   0;

  A = CMatrix<double>(   array, m, n );
  b = CVector<double>( bvector, m );

  // define an initial guess for x
  x0 = CVector<double>( n ); 
  x0.set(4.9, 0);
  x0.set(4.9, 1);
  c  = CVector<double>( n );
  c.set( 180.0, 0 );
  c.set( 160.0, 1 );

  // define variables required to obtain a convergent solution
  beta    = 0.5;    
  alpha   = 0.3;
  eta     = 1.0e-3;
  epsilon = 1.0e-5;
  mu      = 5;
  t0      = 0.001;

  x = linearProgramObject.steepestDescentSUMT(
                          beta, alpha, eta, epsilon, mu, t0, m, n,
                          c, A, x0, b );
  cout << "Result is: ( " << x[0]*7 << "/7, " << x[1]*7 << "/7 )" << endl;
}
