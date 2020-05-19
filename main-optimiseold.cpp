//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 2001
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include <iostream>
#include "point3d.hh"
#include "reloader.hh"
#include "raytracing.hh"
#include "vector.hh"
#include "capacity.hh"
#include "userconst.hh"
#include "optimisation.hh"

int main( int argc, char *argv[] )
{
  CCapacity          capacityGrid;
  CReloadBuilding    reloader;
  CMatlab            matlabObject;
  CBaseTransceiver   btsObject;
  CIntrinsic         intrinsicObject;
  List<CBoundingBox> cells;
  List<CPoint3d>     centres, newCentres;
  COptimisation      OptimisationObject;
  CVector<double>    x, y, p;
  SystemParameters   parameterObject;
  int                i, t1, numberOfSectors;
  //double             location;

  t1 = intrinsicObject.StartClock();
  // read building environment
  reloader = CReloadBuilding( "foyer" );
  //reloader = CReloadBuilding( "foyer" );
  matlabObject.Story( 0, reloader.building_, "story1.txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1.txt" );
  matlabObject.Story( 1, reloader.building_, "story2.txt" );
  matlabObject.Story( 2, reloader.building_, "story3.txt" );

  // read transmitter (antenna system)
  btsObject.read( "bts2000.dat" );

  // read user requirements (traffic)
  capacityGrid = CCapacity( "capacity.txt", btsObject );

  // split building to form cells each containing an initial base station
  cells = capacityGrid.splitIntoCells( reloader.building_, btsObject, centres);
  matlabObject.Cells( cells, centres, "cells.txt" );

  parameterObject.read( "downlinkparam.dat" );
  // perform a non-linear optimisation
  //btsObject.setRandomWalk( true );
  btsObject.setRandomWalk( false );

  numberOfSectors = 1;
  btsObject.setNumberOfReflections(1);
  OptimisationObject.bestServers( centres,  btsObject,
                                  reloader.building_, capacityGrid );
  
  x = OptimisationObject.steepestDescentSUMT( centres, cells, btsObject,
                                          numberOfSectors, reloader.building_, 
                                          capacityGrid, parameterObject, 83 );

  btsObject.setRandomWalk( true );
  //btsObject.setRandomWalk( false );
  numberOfSectors = 1;
  btsObject.setNumberOfReflections(2);
  newCentres = OptimisationObject.vectorToCentres( x );
  matlabObject.Cells( cells, newCentres, "newcells.txt" );
  y = OptimisationObject.steepestDescentSUMT( newCentres, cells, btsObject,
                                          numberOfSectors, reloader.building_, 
                                          capacityGrid, parameterObject, 83 );

  intrinsicObject.ClockTime( t1, "Time for full computation" );
  // plot the possible best servers if all powers were equal
  ofstream coutput("locations.txt");
  for ( i = 0; i < centres.size(); i++ )
    coutput << x[3*i] << "  " << x[3*i+1] << "  " << x[3*i+2] << endl;
  //for ( i = 0; i < newCentres.size(); i++ )
    //coutput << y[3*i] << "  " << y[3*i+1] << "  " << y[3*i+2] << endl;
  //coutput.close();
  cout << "centres are as follows for order 1 Reflection" << endl;
  for ( i = 0; i < centres.size(); i++ )
    cout << x[3*i] << "  " << x[3*i+1] << "  " << x[3*i+2] << endl;
  //cout << "centres are as follows for order 2 Reflection" << endl;
  //for ( i = 0; i < newCentres.size(); i++ )
    //cout << y[3*i] << "  " << y[3*i+1] << "  " << y[3*i+2] << endl;

  centres.clear();
  newCentres.clear();
  cells.clear();


  return 1;
}
