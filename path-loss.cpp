//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    4th January 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "path-loss.hh"

CPathLoss::CPathLoss()
{
  xSouthWest_   = 0.0;
  ySouthWest_   = 0.0;
  xNorthEast_   = 0.0;
  yNorthEast_   = 0.0;
  xStep_        = 0.0;
  yStep_        = 0.0;
  xNumberOfPts_ = 0;
  yNumberOfPts_ = 0;
}

CPathLoss::CPathLoss( double xSouthWest, double ySouthWest, double     xStep,
                      double yStep,      int  xNumberOfPts, int yNumberOfPts )
{
  double xtmp, ytmp;

  xSouthWest_   = xSouthWest;
  ySouthWest_   = ySouthWest;
  xStep_        = xStep;
  yStep_        = yStep;
  xNumberOfPts_ = xNumberOfPts;
  yNumberOfPts_ = yNumberOfPts;
  xNorthEast_   = xSouthWest_ + xStep_ * ( xNumberOfPts_ - 1 );
  yNorthEast_   = ySouthWest_ + yStep_ * ( yNumberOfPts_ - 1 );

  xtmp = ( xNorthEast_ - xSouthWest_ ) / xStep_;
  ytmp = ( yNorthEast_ - ySouthWest_ ) / yStep_;
  xNumberOfPts_ = int( xtmp ); 
  yNumberOfPts_ = int( ytmp );

  if ( fabs( xtmp - xNumberOfPts_ ) < 1.0e-10 )
    xNumberOfPts_++;
  if ( fabs( ytmp - yNumberOfPts_ ) < 1.0e-10 )
    yNumberOfPts_++;
                     
  pathLossVector_ = new double [xNumberOfPts_ * yNumberOfPts_];
  setZeros();
}

CPathLoss::CPathLoss( double  xSouthWest, double ySouthWest, double     xStep,
                      double  yStep,      int  xNumberOfPts, int yNumberOfPts,
                      double* pathLossVector )
{
  double xtmp, ytmp;
  int    i, j, k;

  xSouthWest_   = xSouthWest;
  ySouthWest_   = ySouthWest;
  xStep_        = xStep;
  yStep_        = yStep;
  xNumberOfPts_ = xNumberOfPts;
  yNumberOfPts_ = yNumberOfPts;
  xNorthEast_   = xSouthWest_ + xStep_ * ( xNumberOfPts_ - 1 );
  yNorthEast_   = ySouthWest_ + yStep_ * ( yNumberOfPts_ - 1 );

  xtmp = ( xNorthEast_ - xSouthWest_ ) / xStep;
  ytmp = ( yNorthEast_ - ySouthWest_ ) / yStep;
  xNumberOfPts_ = int( xtmp ); 
  yNumberOfPts_ = int( ytmp );

  if ( fabs( xtmp - xNumberOfPts_ ) < 1.0e-10 )
    xNumberOfPts_++;
  if ( fabs( ytmp - yNumberOfPts_ ) < 1.0e-10 )
    yNumberOfPts_++;

  pathLossVector_ = new double [xNumberOfPts_ * yNumberOfPts_];

  k = 0;
  for ( j = 0; j < yNumberOfPts_; j++ )
  {
    for ( i = 0; i < xNumberOfPts_; i++ )
    {
      pathLossVector_[k] = pathLossVector[k];
      k++;
    }
  }
}

double* CPathLoss::getPathLossVector()
{
  double* PathLossVector;
  int     i;

  PathLossVector = new double [xNumberOfPts_ * yNumberOfPts_];

  for ( i = 0; i < xNumberOfPts_ * yNumberOfPts_; i++ )
  {
    PathLossVector[i] = pathLossVector_[i];
  }

  return( PathLossVector );
}

CPoint3d CPathLoss::getPoint( int i, double z ) const
{
  double x, y, j;

  j = fmod( double(i), xNumberOfPts_ );
  x = xSouthWest_ + xStep_ * j;
  y = ySouthWest_ + yStep_ * ( ( i - j ) / xNumberOfPts_ ); 

  return CPoint3d( x, y, z );
}

void CPathLoss::setZeros()
{
  int i;

  for ( i = 0; i < xNumberOfPts_ * yNumberOfPts_; i++ )
  {
    pathLossVector_[i] = 0.0;
  }
}

void CPathLoss::setPathLoss( double pathLoss, int i, int j )
{
  int k = i + xNumberOfPts_ * j;
  pathLossVector_[k] = pathLoss;
}

double CPathLoss::getPathLoss( int i, int j ) const
{
  if ( i < xNumberOfPts_ && j < yNumberOfPts_ && i >= 0 && j >= 0 )
  {
    return ( pathLossVector_[i + xNumberOfPts_ * j] );
  }
  else
  {
    cerr << endl << "CPathLoss: grid int values outside the grid mesh." 
         << endl;
    cerr << "Start Point: " << xSouthWest_ << ", " << ySouthWest_ << endl;
    cerr << "(i,j):       ( " << i << ", " << j << ")" << endl;
    cerr << "End Point: " << xNorthEast_ << ", " << yNorthEast_ << endl;
    exit(1);
  }
}

double CPathLoss::getPathLoss( long x, long y ) const
{
  int    i, j; 
  double interpolant, w_i, w_j, i_double, j_double;
  
  i_double = ( x - xSouthWest_ ) / xStep_;
  j_double = ( y - ySouthWest_ ) / yStep_;
  i        = int( i_double );
  j        = int( j_double );

  if ( i_double == double(i) && j_double == double(j) )
  {
    return ( getPathLoss( i, j ) );
  }
  else if ( i_double == double(i) )
  {
    w_j = double( j + 1 - j_double );
    interpolant = w_j * getPathLoss( i, j )
                + ( 1.0 - w_j ) * getPathLoss( i, j+1 );
    return (interpolant);
  }
  else if ( j_double == double(j) )
  {
    w_i = double( i + 1 - i_double );
    interpolant = w_i * getPathLoss( i, j )
                + ( 1.0 - w_i ) * getPathLoss( i+1, j );
    return (interpolant);
  }
  else
  {
    w_i = double( i + 1 - i_double );
    w_j = double( j + 1 - j_double );

    if ( i < xNumberOfPts_ && j < yNumberOfPts_ && i >= 0 && j >= 0 )
    {
      interpolant = w_i * w_j * getPathLoss( i, j )
                  + ( 1.0 - w_i ) * w_j * getPathLoss( i+1, j )
                  + w_i * ( 1.0 - w_j ) * getPathLoss( i, j+1 )
                  + ( 1.0 - w_i ) * ( 1.0 - w_j ) * getPathLoss( i+1, j+1 );
      return interpolant;
    }
    else
    {
      cerr << "CPathLoss: long number path-loss outside the grid." << endl;
      cerr << "Start Point: " << xSouthWest_ << ", " << ySouthWest_ << endl;
      cerr << "(i,j):       ( " << i << ", " << j << ")" << endl;
      cerr << "(x,y):       ( " << x << ", " << y << ")" << endl;
      cerr << "End Point: " << xNorthEast_ << ", " << yNorthEast_ << endl;
      exit(2);
    }
  }
}

CPathLoss& CPathLoss::operator=( const CPathLoss& otherPathLoss )
{
  int i;

  if ( this != &otherPathLoss )
  {
    clear();

    xSouthWest_ = otherPathLoss.xSouthWest_;
    ySouthWest_ = otherPathLoss.ySouthWest_;
    xNorthEast_ = otherPathLoss.xNorthEast_;
    yNorthEast_ = otherPathLoss.yNorthEast_;
    xStep_ = otherPathLoss.xStep_;
    yStep_ = otherPathLoss.yStep_;
    xNumberOfPts_ = otherPathLoss.xNumberOfPts_;
    yNumberOfPts_ = otherPathLoss.yNumberOfPts_;
  
    if ( xNumberOfPts_ > 0 && yNumberOfPts_ > 0 )
    {
      pathLossVector_ = new double [xNumberOfPts_ * yNumberOfPts_];
      for ( i = 0; i < xNumberOfPts_ * yNumberOfPts_; i++ )
        pathLossVector_[i] = otherPathLoss.pathLossVector_[i];
    }
  }

  return *this;
}
