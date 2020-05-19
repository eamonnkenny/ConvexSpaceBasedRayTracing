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
#include "uplink.hh"
#include "downlink.hh"

void main( int argc, char *argv[] )
{
  CCapacity          capacityGrid;
  CReloadBuilding    reloader;
  CMatlab            matlabObject;
  CBaseTransceiver   btsObject;
  List<CBoundingBox> cells;
  List<CPoint3d>     centres, newCentres;
  CDownlink          OptimisationObject;
  CVector<double>    x, y, p;
  SystemParameters   uplinkParameters, downlinkParameters;
  int                i, numberOfSectors, numberOfIterations;
  int                numberOfConnections, totalNumberOfConnections;
  CUplink            uplinkObject;
  //double             location;

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

  downlinkParameters.read( "downlinkparam.dat" );
  uplinkParameters.read(   "uplinkparam.dat" );
  // perform a non-linear optimisation
  //btsObject.setRandomWalk( true );
  btsObject.setRandomWalk( false );

  numberOfSectors = 1;
  btsObject.setNumberOfReflections(1);
  OptimisationObject.bestServers( centres,  btsObject,
                                  reloader.building_, capacityGrid );
  
  /*x = OptimisationObject.steepestDescentSUMT( centres, cells, btsObject,
                                      numberOfSectors, reloader.building_, 
                                      capacityGrid, downlinkParameters, 90.5 );
                                      */

  x = CVector<double>(9);
  //x.set( 5.2778, 0 );
  //x.set( 5.5387, 1 );
  //x.set( 2.575, 2 );
  //x.set( 24.988577, 3 );
  //x.set( 8.7188489, 4 );
  //x.set( 2.5729167, 5 );
  //x.set( 15.5, 6 );
  //x.set( 20.2900185052, 7 );
  //x.set( 2.5, 8 );

  x.set( 7.61427, 0 );
  x.set( 1.31392, 1 );
  x.set( 2.53646, 2 );
  x.set( 20.459, 3 );
  x.set( 8.77557, 4 );
  x.set( 2.5, 5 );
  x.set( 3, 6 );
  x.set( 17.3932, 7 );
  x.set( 3.19676, 8 );

  btsObject.setRandomWalk( true );

  btsObject.setRandomWalk( false );
  numberOfSectors = 1;
  btsObject.setNumberOfReflections(1);
  newCentres = OptimisationObject.vectorToCentres( x );
  matlabObject.Cells( cells, newCentres, "newcells.txt" );

  for ( i = 0; i < centres.size(); i++ )
    cout << "bts: " << i << "  =  ( " << x[3*i] << ", " << x[3*i+1] 
            << ", " << x[3*i+2] << " ) " << endl;
  /*y = OptimisationObject.steepestDescentSUMT( newCentres, cells, btsObject,
                                          numberOfSectors, reloader.building_, 
                                          capacityGrid, parameterObject, 90.5 );

  // plot the possible best servers if all powers were equal
  ofstream coutput("locations.txt");
  for ( i = 0; i < centres.size(); i++ )
    coutput << x[3*i] << "  " << x[3*i+1] << "  " << x[3*i+2] << endl;
  for ( i = 0; i < newCentres.size(); i++ )
    coutput << y[3*i] << "  " << y[3*i+1] << "  " << y[3*i+2] << endl;
  coutput.close();
  cout << "centres are as follows for order 1 Reflection" << endl;
  for ( i = 0; i < centres.size(); i++ )
    cout << x[3*i] << "  " << x[3*i+1] << "  " << x[3*i+2] << endl;
  cout << "centres are as follows for order 2 Reflection" << endl;
  for ( i = 0; i < newCentres.size(); i++ )
    cout << y[3*i] << "  " << y[3*i+1] << "  " << y[3*i+2] << endl;

  centres.clear();
  newCentres.clear();
  cells.clear();*/

  numberOfIterations = 100;

  totalNumberOfConnections = 0.0;
  for ( i = 0; i < numberOfIterations; i++ )
  {
    numberOfConnections = uplinkObject.generateRandomPopulation( newCentres, 
                                           btsObject, 
                                           reloader.building_,
                                           uplinkParameters );
    totalNumberOfConnections += numberOfConnections;
  }

  totalNumberOfConnections /= numberOfIterations;

  cout << "total Number of Connections: " << totalNumberOfConnections << endl;
}
