void COptimisation::absSum(  List<CRayPath>& allRayPaths,
                             int             numberOfSectors,
                             double          sectorAngle,
                             complex*        Rsectors )
{
  double   AOD;
  int      k, l;
  complex  fieldStrength;
  CRayPath *rayPath;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( l = 0; l < allRayPaths.size(); l++ )
  {
    rayPath        = &allRayPaths.GetItem(l);
    fieldStrength  = rayPath->fieldStrength_[2].z;

    AOD = rayPath->angleOfDeparture();

    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        Rsectors[k] += fieldStrength;
        k = numberOfSectors;
      }
    }
  }
}

void COptimisation::absSum(  List<CRayPath>& allRayPaths,
                             int             numberOfSectors,
                             double          sectorAngle,
                             complex*        gain,
                             complex*        Rsectors )
{
  double   AOD;
  int      k, l;
  complex  fieldStrength;
  CRayPath *rayPath;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( l = 0; l < allRayPaths.size(); l++ )
  {
    rayPath        = &allRayPaths.GetItem(l);
    fieldStrength  = rayPath->fieldStrength_[2].z;

    AOD = rayPath->angleOfDeparture();

    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        Rsectors[k] += gain[k] * fieldStrength;
        k = numberOfSectors;
      }
    }
  }
}

void COptimisation::meanAbsSum( List<CRayPath>& allRayPaths,
                                int             numberOfSectors,
                                double          sectorAngle,
                                complex*        Rsectors )
{
  double   AOD;
  int      i, k, l;
  complex  fieldStrength;
  complex  expjtest;
  complex  *temp;
  CRayPath *rayPath;
  int      numberOfIterations = 1000;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  temp = new complex [numberOfSectors];

  for ( i = 0; i < numberOfIterations; i++ )
  {
    for ( k = 0; k < numberOfSectors; k++ )
      temp[k] = 0.0;
    for ( l = 0; l < allRayPaths.size(); l++ )
    {
      rayPath        = &allRayPaths.GetItem(l);
      fieldStrength  = rayPath->fieldStrength_[2].z;

      AOD = rayPath->angleOfDeparture();
  
      for ( k = 0; k < numberOfSectors; k++ )
      {
        if ( AOD <= ( k + 1 ) * sectorAngle )
        {
          expjtest = expj( TwoPi * ( double(rand()) / double(RAND_MAX) ) );
          temp[k] += fieldStrength * expjtest;
          k = numberOfSectors;
        }
      }
    }
    for ( k = 0; k < numberOfSectors; k++ )
      Rsectors[k] += fabs( temp[k] );
  }

  for ( k = 0; k < numberOfSectors; k++ )
  {
    Rsectors[k] = Rsectors[k] / numberOfIterations;
  }
  delete [] temp;
}

void COptimisation::meanAbsSum( List<CRayPath>& allRayPaths,
                                int             numberOfSectors,
                                double          sectorAngle,
                                complex*        gain,
                                complex*        Rsectors )
{
  double   AOD;
  int      i, k, l;
  complex  fieldStrength;
  complex  expjtest;
  complex  *temp;
  CRayPath *rayPath;
  int      numberOfIterations = 1000;

  temp = new complex [numberOfSectors];

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( i = 0; i < numberOfIterations; i++ )
  {
    for ( k = 0; k < numberOfSectors; k++ )
      temp[k] = 0.0;
    for ( l = 0; l < allRayPaths.size(); l++ )
    {
      rayPath        = &allRayPaths.GetItem(l);
      fieldStrength  = rayPath->fieldStrength_[2].z;

  
      AOD = rayPath->angleOfDeparture();
  
      for ( k = 0; k < numberOfSectors; k++ )
      {
        if ( AOD <= ( k + 1 ) * sectorAngle )
        {
          expjtest = expj( TwoPi * ( double(rand()) / double(RAND_MAX) ) );
          temp[k] += gain[k] * fieldStrength * expjtest;
          k = numberOfSectors;
        }
      }
    }
    for ( k = 0; k < numberOfSectors; k++ )
      Rsectors[k] += fabs( temp[k] );
  }

  for ( k = 0; k < numberOfSectors; k++ )
  {
    Rsectors[k] = Rsectors[k] / numberOfIterations;
  }
  delete [] temp;
}

void COptimisation::sumAbs( List<CRayPath>& allRayPaths,
                            int             numberOfSectors,
                            double          sectorAngle,
                            complex*        Rsectors )
{
  double   AOD;
  int      k, l;
  complex  fieldStrength;
  CRayPath *rayPath;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( l = 0; l < allRayPaths.size(); l++ )
  {
    rayPath        = &allRayPaths.GetItem(l);
    fieldStrength  = rayPath->fieldStrength_[2].z;

    AOD = rayPath->angleOfDeparture();

    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        Rsectors[k] += fabs(fieldStrength);
        k = numberOfSectors;
      }
    }
  }
}

void COptimisation::sumAbs( List<CRayPath>& allRayPaths,
                            int             numberOfSectors,
                            double          sectorAngle,
			    complex*        gain,
                            complex*        Rsectors )
{
  double   AOD;
  int      k, l;
  complex  fieldStrength;
  CRayPath *rayPath;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( l = 0; l < allRayPaths.size(); l++ )
  {
    rayPath        = &allRayPaths.GetItem(l);
    fieldStrength  = rayPath->fieldStrength_[2].z;

    AOD = rayPath->angleOfDeparture();

    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        Rsectors[k] += fabs(gain[k] * fieldStrength);
        k = numberOfSectors;
      }
    }
  }
}

double COptimisation::gaussianQuadrature4( double *E, int    N, double r, 
                                          double a,  double stepsize )
{
  double t[4], w[4];
  double zeta, zeta1, Wn, Jproduct, integ, a1, b1, c, m;
  int    k, n, i;

  //gaussian weights
  t[0] = -0.3399810436;
  t[1] = -t[0];
  t[2] = -0.8611363116;
  t[3] = -t[2];
  w[0] = 0.65214515;
  w[1] = w[0];
  w[2] = 0.34785485;
  w[3] = w[2];

  zeta = a;
  Wn   = 0.0;

  // integrate over zeta to obtain the PDF
  for ( i = 1; i < 100000; i++ )
  {
    a1 = zeta;
    b1 = zeta+stepsize;

    c  = ( b1 + a1 ) / 2.0;
    m  = ( b1 - a1 ) / 2.0;

    // calculate the integral at the 4 weighted positions
    for ( n = 0; n < 4; n++ )
    {
      Jproduct = 1.0;

      zeta1 = c + m * t[n];

      // calculate the product of Jo(zeta * E )
      for ( k = 0; k < N; k++ )
        Jproduct = Jproduct * j0( zeta1 * E[k] );
      
      integ = zeta1 * j0( zeta1 * r ) * Jproduct * m;
      Wn = Wn + w[n] * integ;
    }

    // if the integral has converged for increasing step then break
    if ( fabs(integ)/Wn < 1.0e-8 )
      break;

    zeta = zeta + stepsize;
  }
  if ( i == 100000 ) 
    cout << "there are too many gaussian quadrature integrations" << endl;

  return Wn;
}

double COptimisation::trapezoidal( double *E, int    N, double r, 
                                   double a,  double stepsize )
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

double COptimisation::WnFunction( double *E, int    N, double r, 
                                  double zeta, double stepsize )
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

double COptimisation::besselJ0( double x )
{
  if ( x > 3.0 )
    return SqrtTwoOverPi / sqrt(x) * cos( x - PiOver4 );
  else
    return j0( x );
}

void   COptimisation::randomWalkMean( List<CRayPath>& allRayPaths, 
                                      int             numberOfSectors,
                                      double          sectorAngle, 
                                      complex         *Rsectors )
{
  int            N, i, j, k, size2, *Ntotal;
  double         **E;
  double         stepsize1, stepsize2;
  double         randomWalkMean, integ2, r, Wn, AOD, *sumE;
  complex        fieldStrength, **Ec, *Rsector2;
  double         *sumA, *sumB, *sqrSum;
  CRayPath       *rayPath;
  CMemoryStorage storageObject;

  N  = allRayPaths.size();

  // nothing to do if no ray-paths
  if ( N == 0 )
  {
    for ( k = 0; k < numberOfSectors; k++ )
      Rsectors[k] == 0.0;
    return;
  }

  Ec = storageObject.CreateComplexMatrix( numberOfSectors, N );
  E  = storageObject.CreateDoubleMatrix(  numberOfSectors, N );
  sumE = storageObject.CreateDoubleVector( numberOfSectors );
  sumA = storageObject.CreateDoubleVector(  numberOfSectors );
  sumB = storageObject.CreateDoubleVector(  numberOfSectors );
  sqrSum  = storageObject.CreateDoubleVector(  numberOfSectors );
  Rsector2  = storageObject.CreateComplexVector(  numberOfSectors );
  Ntotal    = storageObject.CreateIntegerVector(  numberOfSectors );

  // convert data into real valued data in each sector of the server
  for ( i = 0; i < N; i++ )
  {
    rayPath       = &allRayPaths.GetItem(i);
    fieldStrength = rayPath->fieldStrength_[2].z;
  
    AOD = rayPath->angleOfDeparture();
  
    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        j          = Ntotal[k];
	sumA[k]   += fieldStrength.real();
	sumB[k]   += fieldStrength.imag();
	sqrSum[k] += qsqr( fieldStrength.real() ); // variance with zero mean

        E[k][j] = fabs( fieldStrength );
        sumE[k] = sumE[k] + E[k][j];
	Ntotal[k]++;

        k          = numberOfSectors;
      }
    }
  }

  // if rician distributed (LOS large) can not use integral formula
  // so use same mean instead and return
  for ( k = 0; k < numberOfSectors; k++ )
  {
    if ( E[k][0] / sumE[k] > 0.95 && sumE[k] != 0.0 )
    {
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, Rsectors );
      return;
    }
  }

  // if rayleigh distributed then perform integral formula for random walk
  // mean value, do this for each sectorized antenna in the current server
  for ( k = 0; k < numberOfSectors; k++ )
  {
    // check for gaussian distributed
    if ( sumA[k] / sumB[k] > 0.98 && sumA[k] / sumB[k] < 1.020408 
      && N > 20 )
    {
      cout << "used gaussian random variable" << endl;
      Rsectors[k] = sqrt( sqrSum[k] * PiOver2 / double(N) );
      cout << "mean gauss value: R: " << Rsectors[k] << endl;
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, Rsector2 );
      cout << "mean numer value: R: " << Rsector2[k] << endl;
      continue;
    }

    size2     = 1000;
    stepsize1 = 20.0;
    stepsize2 = 0.05;
    
    randomWalkMean = 0.0;
    integ2         = 1.0;
    r              = stepsize2;
    
    // integrate over r to obtain the CDF
    for ( i = 1; i < size2; i++ )
    {
      Wn = trapezoidal( E[k], Ntotal[k], r, 0, stepsize1 );
    
      integ2 = Wn * stepsize2;
      randomWalkMean    = randomWalkMean + integ2;
    
      r = r + stepsize2;
    
      if ( fabs(integ2/randomWalkMean) < 1.0e-3 ) 
        break;
    }
    if ( i == 1000 ) 
      cout << "there are too many ungained mean integrations" << endl;
  
    if ( randomWalkMean < 0.0 )
    {
      //ofstream coutput( "Pray.txt" );
  
      //if ( size2 == 1000 ) 
        //cout << " gain there are too many euler integrations" << endl;
      //for ( i = 0; i < N; i++ )
        //coutput << Ec[i] << endl;
  
      //coutput.close();
      cout << "negative value in integral, first Value %: " 
           << E[k][0] / sumE[k] << endl;
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, Rsectors );
      return;
    }
    Rsectors[k] = randomWalkMean;
  }

  storageObject.DeleteDoubleMatrix( E, numberOfSectors );
  storageObject.DeleteComplexMatrix( Ec, numberOfSectors );
  storageObject.DeleteComplexVector( Rsector2 );
  storageObject.DeleteDoubleVector( sumE );
  storageObject.DeleteDoubleVector( sumA );
  storageObject.DeleteDoubleVector( sumB );
  storageObject.DeleteDoubleVector( sqrSum );
  storageObject.DeleteIntegerVector( Ntotal );

}

void   COptimisation::randomWalkMean( List<CRayPath>& allRayPaths, 
                                      int             numberOfSectors,
                                      double          sectorAngle,
                                      complex*        gain,
                                      complex         *Rsectors )
{
  int            N, i, j, k, size2, *Ntotal;
  double         **E, stepsize1, stepsize2, randomWalkMean, integ2;
  double         r, Wn, AOD, *sumE;
  complex        fieldStrength, temp, **Ec, *Rsector2;
  double         *sumA, *sumB, *sqrSum;
  CRayPath       *rayPath;
  CMemoryStorage storageObject;

  N  = allRayPaths.size();

  // nothing to do if no ray-paths
  if ( N == 0 )
  {
    for ( k = 0; k < numberOfSectors; k++ )
      Rsectors[k] == 0.0;
    return;
  }

  Ec = storageObject.CreateComplexMatrix( numberOfSectors, N );
  E  = storageObject.CreateDoubleMatrix(  numberOfSectors, N );
  sumE = storageObject.CreateDoubleVector(  numberOfSectors );
  sumA = storageObject.CreateDoubleVector(  numberOfSectors );
  sumB = storageObject.CreateDoubleVector(  numberOfSectors );
  sqrSum  = storageObject.CreateDoubleVector(  numberOfSectors );
  Rsector2  = storageObject.CreateComplexVector(  numberOfSectors );
  Ntotal    = storageObject.CreateIntegerVector(  numberOfSectors );

  for ( i = 0; i < N; i++ )
  {
    rayPath       = &allRayPaths.GetItem(i);
    fieldStrength = rayPath->fieldStrength_[2].z;
  
    AOD = rayPath->angleOfDeparture();
  
    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        temp       = gain[k] * fieldStrength;
        j          = Ntotal[k];
	sumA[k]   += temp.real();
	sumB[k]   += temp.imag();
	sqrSum[k] += qsqr( temp.real() ); // variance with zero mean

        E[k][j] = fabs( temp );
        sumE[k] = sumE[k] + E[k][j];
	Ntotal[k]++;

        k     = numberOfSectors;
      }
    }
  }

  for ( k = 0; k < numberOfSectors; k++ )
  {
    // check for Rician distributed
    if ( E[k][0] / sumE[k] > 0.95 && sumE[k] != 0.0 )
    {
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, gain, Rsectors );
      return;
    }
  }

  for ( k = 0; k < numberOfSectors; k++ )
  {
    if ( Ntotal[k] == 0 )
      continue;

    // check for gaussian distributed
    if ( sumA[k] / sumB[k] > 0.98 && sumA[k] / sumB[k] < 1.020408 
      && N > 20 )
    {
      cout << "used gaussian random variable" << endl;
      Rsectors[k] = sqrt( sqrSum[k] * PiOver2 / double(N) );
      cout << "mean gauss value: R: " << Rsectors[k] << endl;
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, gain, Rsector2 );
      cout << "mean numer value: R: " << Rsector2[k] << endl;
      continue;
    }
    size2     = 10000;
    stepsize1 = 20.0;
    stepsize2 = 0.05;
  
    randomWalkMean = 0.0;
    integ2         = 1.0;
    r              = stepsize2;
    
    // integrate over r to obtain the CDF
    for ( i = 1; i < size2; i++ )
    {
      Wn = trapezoidal( E[k], Ntotal[k], r, 0, stepsize1 );
    
      integ2 = Wn * stepsize2;
      randomWalkMean    = randomWalkMean + integ2;
    
      //fprintf( 1, 'r: %f, randomWalkMean: %15.12f\n', r, randomWalkMean );
      r = r + stepsize2;
    
      if ( fabs(integ2/randomWalkMean) < 1.0e-3 ) 
        break;
    }
  
  
    if ( randomWalkMean < 0.0 )
    {
      cout << "negative value in integral, first Value %: " 
           << E[k][0] / sumE[k] << endl;
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, gain, Rsectors );
      return;
    }

    if ( i == size2 ) 
      cout << " gain there are too many euler integrations" << endl;

    Rsectors[k] = randomWalkMean;
  }
      
  storageObject.DeleteDoubleMatrix( E, numberOfSectors );
  storageObject.DeleteComplexMatrix( Ec, numberOfSectors );
  storageObject.DeleteComplexVector( Rsector2 );
  storageObject.DeleteDoubleVector( sumE );
  storageObject.DeleteDoubleVector( sumA );
  storageObject.DeleteDoubleVector( sumB );
  storageObject.DeleteDoubleVector( sqrSum );
  storageObject.DeleteIntegerVector( Ntotal );
}

