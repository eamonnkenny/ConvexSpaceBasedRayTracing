//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    20th May 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "capacity.hh"

CCapacity::CCapacity()
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

CCapacity::CCapacity( char* capacityGridFileName, 
                      const CBaseTransceiver& btsObject )
{
  ifstream capacityGridFile;
  string  dummyString, fullFileName;
  double   xmin, ymin, zmin, xstep, ystep;
  int      M, N, i;

  
  fullFileName = CProperties::matlabOptimisationDirectory + "/" + capacityGridFileName;
  capacityGridFile.open( fullFileName.c_str() );

  capacityGridFile >> dummyString;
  capacityGridFile >> xmin;
  capacityGridFile >> ymin;
  capacityGridFile >> zmin;
  capacityGridFile >> dummyString;
  capacityGridFile >> xstep;
  capacityGridFile >> ystep;
  capacityGridFile >> dummyString;
  capacityGridFile >> M;
  capacityGridFile >> N;
  setValues( xmin, ymin, zmin, xstep, ystep, M, N );

  totalCapacity_ = 0.0;
  cout << "total number points in capacity grid: " << numberOfPoints() << endl;
  for ( i = 0; i < numberOfPoints(); i++ )
  {
    capacityGridFile >> capacityVector_[i];
    totalCapacity_ += capacityVector_[i];
  }

  numberOfServers_ = int(ceil(( totalCapacity_ / btsObject.serverCapacity() )));

  capacityGridFile.close();
}

void CCapacity::setValues( double xSouthWest, double ySouthWest, 
                           double zSouthWest, double xStep,  
                           double yStep,      int  xNumberOfPts, 
                           int    yNumberOfPts )
{
  double xtmp, ytmp;

  xSouthWest_   = xSouthWest;
  ySouthWest_   = ySouthWest;
  zSouthWest_   = zSouthWest;
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
                     
  capacityVector_ = new double [xNumberOfPts_ * yNumberOfPts_];
  setZeros();
}

CCapacity::CCapacity( double  xSouthWest, double ySouthWest, double zSouthWest,
                      double  xStep,      double yStep,      int  xNumberOfPts, 
                      int   yNumberOfPts, double* capacityVector )
{
  double xtmp, ytmp;
  int    i, j, k;

  xSouthWest_   = xSouthWest;
  ySouthWest_   = ySouthWest;
  zSouthWest_   = zSouthWest;
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

  capacityVector_ = new double [xNumberOfPts_ * yNumberOfPts_];

  k = 0;
  for ( j = 0; j < yNumberOfPts_; j++ )
  {
    for ( i = 0; i < xNumberOfPts_; i++ )
    {
      capacityVector_[k] = capacityVector[k];
      k++;
    }
  }
}

double* CCapacity::getCapacityVector()
{
  double* capacityVector;
  int     i;

  capacityVector = new double [xNumberOfPts_ * yNumberOfPts_];

  for ( i = 0; i < xNumberOfPts_ * yNumberOfPts_; i++ )
  {
    capacityVector[i] = capacityVector_[i];
  }

  return( capacityVector );
}

void CCapacity::setZeros()
{
  int i;

  for ( i = 0; i < xNumberOfPts_ * yNumberOfPts_; i++ )
  {
    capacityVector_[i] = 0.0;
  }
}

void CCapacity::setCapacity( double capacity, int i, int j )
{
  int k = i + xNumberOfPts_ * j;
  capacityVector_[k] = capacity;
}

CPoint3d CCapacity::getPoint( int i, int j ) const
{
  return CPoint3d( xSouthWest_ + i * xStep_, ySouthWest_ + j * yStep_,
                   zSouthWest_ );
}

double CCapacity::getCapacity( int i, int j ) const
{
  if ( i < xNumberOfPts_ && j < yNumberOfPts_ && i >= 0 && j >= 0 )
  {
    return ( capacityVector_[i + xNumberOfPts_ * j] );
  }
  else
  {
    cerr << endl << "CCapacity: grid int values outside the grid mesh." 
         << endl;
    cerr << "Start Point: " << xSouthWest_ << ", " << ySouthWest_ << endl;
    cerr << "(i,j):       ( " << i << ", " << j << ")" << endl;
    cerr << "End Point: " << xNorthEast_ << ", " << yNorthEast_ << endl;
    exit(1);
  }
}

double CCapacity::getCapacity( long x, long y ) const
{
  int    i, j; 
  double interpolant, w_i, w_j, i_double, j_double;
  
  i_double = ( x - xSouthWest_ ) / xStep_;
  j_double = ( y - ySouthWest_ ) / yStep_;
  i        = int( i_double );
  j        = int( j_double );

  if ( i_double == double(i) && j_double == double(j) )
  {
    return ( getCapacity( i, j ) );
  }
  else if ( i_double == double(i) )
  {
    w_j = double( j + 1 - j_double );
    interpolant = w_j * getCapacity( i, j )
                + ( 1.0 - w_j ) * getCapacity( i, j+1 );
    return (interpolant);
  }
  else if ( j_double == double(j) )
  {
    w_i = double( i + 1 - i_double );
    interpolant = w_i * getCapacity( i, j )
                + ( 1.0 - w_i ) * getCapacity( i+1, j );
    return (interpolant);
  }
  else
  {
    w_i = double( i + 1 - i_double );
    w_j = double( j + 1 - j_double );

    if ( i < xNumberOfPts_ && j < yNumberOfPts_ && i >= 0 && j >= 0 )
    {
      interpolant = w_i * w_j * getCapacity( i, j )
                  + ( 1.0 - w_i ) * w_j * getCapacity( i+1, j )
                  + w_i * ( 1.0 - w_j ) * getCapacity( i, j+1 )
                  + ( 1.0 - w_i ) * ( 1.0 - w_j ) * getCapacity( i+1, j+1 );
      return interpolant;
    }
    else
    {
      cerr << "CCapacity: long number path-loss outside the grid." << endl;
      cerr << "Start Point: " << xSouthWest_ << ", " << ySouthWest_ << endl;
      cerr << "(i,j):       ( " << i << ", " << j << ")" << endl;
      cerr << "(x,y):       ( " << x << ", " << y << ")" << endl;
      cerr << "End Point: " << xNorthEast_ << ", " << yNorthEast_ << endl;
      exit(2);
    }
  }
}

CCapacity& CCapacity::operator=( const CCapacity& otherCapacity )
{
  int i;

  if ( this != &otherCapacity )
  {
    clear();

    xSouthWest_   = otherCapacity.xSouthWest_;
    ySouthWest_   = otherCapacity.ySouthWest_;
    zSouthWest_   = otherCapacity.zSouthWest_;
    xNorthEast_   = otherCapacity.xNorthEast_;
    yNorthEast_   = otherCapacity.yNorthEast_;
    xStep_        = otherCapacity.xStep_;
    yStep_        = otherCapacity.yStep_;
    xNumberOfPts_ = otherCapacity.xNumberOfPts_;
    yNumberOfPts_ = otherCapacity.yNumberOfPts_;
    numberOfServers_ = otherCapacity.numberOfServers_;
    totalCapacity_   = otherCapacity.totalCapacity_;
  
    if ( xNumberOfPts_ > 0 && yNumberOfPts_ > 0 )
    {
      capacityVector_ = new double [xNumberOfPts_ * yNumberOfPts_];
      for ( i = 0; i < xNumberOfPts_ * yNumberOfPts_; i++ )
        capacityVector_[i] = otherCapacity.capacityVector_[i];
    }
  }

  return *this;
}

CPoint3d CCapacity::centreOfCapacity( const CBoundingBox& box, double& total ) const
{
  int      i, j, k;
  CPoint3d centre;
  double   x, z, cap;
  double   *y;

  k = 0;
  z = zSouthWest();
  total = 0.0;
  centre = CPoint3d(); // set to zero initially

  y = new double [yNumberOfPts_];
  for ( j = 0; j < yNumberOfPts_; j++ )
    y[j] = ySouthWest_ + double(j) * yStep_;

  for ( j = 0; j < yNumberOfPts_; j++ )
  {
    for ( i = 0; i < xNumberOfPts_; i++ )
    {
      x = xSouthWest_ + double(i) * xStep_;
      cap = capacityVector_[k];
      if ( box.intersection( x, y[j] ) )
      {
        // calculates the centre of capacity if inside the box
        // also calculates the total capacity in the box
        centre = centre + CPoint3d( x * cap, y[j] * cap, z * cap );
        total += cap;
      }
      k++;
    }
  }
  centre = centre / total;
  delete [] y;
  cout << "centre: " << centre << endl;

  return centre;
}

double CCapacity::totalCapacity( const CBoundingBox& box ) const
{
  int      i, j, k;
  double   x;
  double   *y, total;

  k = 0;
  //z = zSouthWest();
  total = 0.0;

  y = new double [yNumberOfPts_];
  for ( j = 0; j < yNumberOfPts_; j++ )
    y[j] = ySouthWest_ + double(j) * yStep_;

  for ( i = 0; i < xNumberOfPts_; i++ )
  {
    x = xSouthWest_ + double(i) * xStep_;

    for ( j = 0; j < yNumberOfPts_; j++ )
    {
      if ( box.intersection( x, y[j] ) )
      {
        // calculates the total capacity in the box
        total += capacityVector_[k];
      }
      k++;
    }
  }
  delete [] y;

  return total;
}

List<CBoundingBox>& CCapacity::splitIntoCells( 
                                     const CBuilding&        buildingObject,
                                     const CBaseTransceiver& btsObject, 
                                           List<CPoint3d>&   centres )
{
  CPoint3d           centre;
  CBoundingBox       boundingBox, b1, b2, b3, b4;
  List<CBoundingBox> *oldListOfCells, newListOfCells; 
  double             total, xSplitTotalServers, ySplitTotalServers;
  int                i;

  oldListOfCells = new List<CBoundingBox>;
  boundingBox    = buildingObject.boundingBox_;
  newListOfCells.append( boundingBox );

  cout << "Building bounding box is: " << endl << boundingBox << endl;

  // Stop here if only one server required
  // Stop if the new list is the same as the old list, this happens when 
  // all server areas are of the correct size
  while ( newListOfCells.size() != oldListOfCells->size() ) 
  {
    *oldListOfCells = newListOfCells;
    newListOfCells.clear();
    centres.clear();
    cout << "---------- beginning split ----------" << endl;
    for ( i = 0; i < oldListOfCells->size(); i++ )
    {
      boundingBox = oldListOfCells->GetItem(i);
      centre      = centreOfCapacity( boundingBox, total );
      centres.append( centre );
  
      // if capacity of box is greater than 2 megabits then split again
      if ( total > btsObject.serverCapacity() ) 
      {
        // split across the x-axis through the centre of capacity
        b1 = CBoundingBox( boundingBox.xSouthWest_,
                           boundingBox.ySouthWest_,
                           boundingBox.zSouthWest_,
                           centre.x,
                           boundingBox.yNorthEast_,
                           boundingBox.zNorthEast_  );
        
        b2 = CBoundingBox( centre.x,
                           boundingBox.ySouthWest_,
                           boundingBox.zSouthWest_,
                           boundingBox.xNorthEast_,
                           boundingBox.yNorthEast_,
                           boundingBox.zNorthEast_  );

        // split across the y-axis through the centre of capacity
        b3 = CBoundingBox( boundingBox.xSouthWest_,
                           boundingBox.ySouthWest_,
                           boundingBox.zSouthWest_,
                           boundingBox.xNorthEast_,
                           centre.y,
                           boundingBox.zNorthEast_ );
    
        b4 = CBoundingBox( boundingBox.xSouthWest_,
                           centre.y,
                           boundingBox.zSouthWest_,
                           boundingBox.xNorthEast_,
                           boundingBox.yNorthEast_,
                           boundingBox.zNorthEast_ );

        xSplitTotalServers = ceil( totalCapacity( b1 ) / 2.0 ) 
                           + ceil( totalCapacity( b2 ) / 2.0 );
        ySplitTotalServers = ceil( totalCapacity( b3 ) / 2.0 ) 
                           + ceil( totalCapacity( b4 ) / 2.0 );
        cout << "xsplit gives: " << int( xSplitTotalServers ) << endl;
        cout << "ysplit gives: " << int( ySplitTotalServers ) << endl;

        if ( boundingBox.length() <= boundingBox.height()*2)
        {
          if ( xSplitTotalServers < ySplitTotalServers )
          {
            cout << "Vertical Split 1" << endl;
            newListOfCells.append( b1 );
            newListOfCells.append( b2 );
          }
          else
          {
            cout << "Horizontal Split 1" << endl;
            newListOfCells.append( b3 );
            newListOfCells.append( b4 );
          }
        }
        else
        {
          if ( xSplitTotalServers <= ySplitTotalServers )
          {
            cout << "Vertical Split 2" << endl;
            newListOfCells.append( b1 );
            newListOfCells.append( b2 );
          }
          else
          {
            cout << "Horizontal Split 2" << endl;
            newListOfCells.append( b3 );
            newListOfCells.append( b4 );
          }
        }
      }
      else
      {
        // place an unchanged bounding box into the new list
        newListOfCells.append( boundingBox );
      }

      cout << "capacity of item( " << newListOfCells.size() << "): " 
           << total << endl;
    }
  }
  cout << "Number Of Cells:        " << oldListOfCells->size() << endl;
  cout << "Actual number required: " << numberOfServers_ << endl;
  newListOfCells.clear();
  /*centres.clear();
  for ( i = 0; i < oldListOfCells->size(); i++ )
  {
    centres.append( oldListOfCells->GetItem(i).centroid() );
    cout << "centre of box " << i << ": " << oldListOfCells->GetItem(i).centroid() << endl;
  }*/

  //N = oldListOfCells->size();
  //boxTotalCapacity = storageObject.CreateDoubleVector( N );
  //for ( i = 0; i < N; i++ )
    //boxTotalCapacity[i] = totalCapacity( oldListOfCells->GetItem(i) );

  return *oldListOfCells;
}

CPoint3d CCapacity::getPoint( int i, double z ) const
{
  double x, y, j;

  j = fmod( double(i), xNumberOfPts_ );
  x = xSouthWest_ + xStep_ * j;
  y = ySouthWest_ + yStep_ * ( ( i - j ) / xNumberOfPts_ ); 

  return CPoint3d( x, y, z );
}
