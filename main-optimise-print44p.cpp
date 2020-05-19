//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 2001
// Last Modification:     October 2015
// Previous Projects:     Storms, FMW/ARG, IP2000
// Current  Project:      ADAPT 
// Software Tool Version: 1.0
//=========================================================================
#include <iostream>
#include "point3d.hh"
#include "reloader.hh"
#include "raytracing.hh"
#include "vector.hh"
#include "capacity.hh"
#include "userconst.hh"
#include "downlink.hh"
#include "configuration.hh"

int main( int argc, char *argv[] )
{
  CCapacity          capacityGrid;
  CReloadBuilding    reloader;
  CMatlab            matlabObject;
  CBaseTransceiver   btsObject;
  CIntrinsic         intrinsicObject;
  List<CBoundingBox> cells;
  List<CPoint3d>     newCentres;
  CDownlink          OptimisationObject;
  CVector<double>    x, y, p;
  SystemParameters   parameterObject;
  string             clockTimeOutputString;
  int                i, t1, numberOfSectors;
  CProperties        directoryPathObject;
  //double             location;

  directoryPathObject.read("../directory.xml");

  t1 = intrinsicObject.StartClock();
  // read building environment
  string buildingName = "printinghouse";
  reloader = CReloadBuilding( buildingName );
  // read transmitter (antenna system)
  btsObject.read( "bts2000ph44p.dat" );

  // read user requirements (traffic)
  capacityGrid = CCapacity( "capacity-print44p.txt", btsObject );

  matlabObject.Story( 0, reloader.building_, "story1-" + btsObject.name() + ".txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1-" + btsObject.name() + ".txt" );
  //matlabObject.Story( 1, reloader.building_, "story2-" + btsObject.name() + ".txt" );
  //matlabObject.Story( 2, reloader.building_, "story3-" + btsObject.name() + ".txt" );

  // split building to form cells each containing an initial base station
  cells = capacityGrid.splitIntoCells( reloader.building_, btsObject, newCentres);
  matlabObject.Cells( cells, newCentres, "cells-" + btsObject.name() + "-start.txt" );

  parameterObject.read( "downlinkparam.dat" );
  // perform a non-linear optimisation
  //btsObject.setRandomWalk( false );
  btsObject.setRandomWalk( true );

  numberOfSectors = 1;
  btsObject.setNumberOfReflections(1);
  OptimisationObject.bestServers( newCentres,  btsObject,
                                  reloader.building_, capacityGrid );

  cout << "---------------- Starting round 1 ------------------" << endl;
  x = OptimisationObject.steepestDescentSUMT( newCentres, cells, btsObject,
                                          numberOfSectors, reloader.building_, 
                                          capacityGrid, parameterObject, 99.0 );

  //btsObject.setRandomWalk( false );
  btsObject.setRandomWalk( true );
  numberOfSectors = 1;
  btsObject.setNumberOfReflections(2);
  //newCentres = OptimisationObject.vectorToCentres( x );
  matlabObject.Cells( cells, newCentres, "cells-" + btsObject.name() + "-r1.txt" );
  cout << "---------------- Starting round 2 ------------------" << endl;
  y = OptimisationObject.steepestDescentSUMT( newCentres, cells, btsObject,
                                          numberOfSectors, reloader.building_, 
                                          capacityGrid, parameterObject, 99.0 );
  newCentres = OptimisationObject.vectorToCentres( y );
  matlabObject.Cells( cells, newCentres, "cells-" + btsObject.name() + "-r2.txt" );
  cout << "----------------- Ending round 2 -------------------" << endl;

  clockTimeOutputString = "Time for full computation";
  intrinsicObject.ClockTime( t1, clockTimeOutputString );
  // plot the possible best servers if all powers were equal
  string location ="locations-" + btsObject.name() + ".txt"; 
  ofstream coutput( location.c_str() );
  coutput << newCentres.size() << endl;
  for ( i = 0; i < newCentres.size(); i++ )
    coutput << y[3*i] << "  " << y[3*i+1] << "  " << y[3*i+2] << endl;
  coutput.close();

  cout << "Original centres at gravity centres" << endl;
  for ( i = 0; i < newCentres.size(); i++ )
    cout << newCentres.GetItem(i) << endl;
  cout << "centres are as follows for order 1 Reflection" << endl;
  for ( i = 0; i < newCentres.size(); i++ )
    cout << x[3*i] << "  " << x[3*i+1] << "  " << x[3*i+2] << endl;
  cout << "centres are as follows for order 2 Reflection" << endl;
  for ( i = 0; i < newCentres.size(); i++ )
    cout << y[3*i] << "  " << y[3*i+1] << "  " << y[3*i+2] << endl;

  newCentres.clear();
  cells.clear();


  return 1;
}
