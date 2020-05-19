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

int main( int argc, char *argv[] )
{
  CCapacity        capacityRequirements;
  CPoint3d         trans = CPoint3d(10.0,5.0,2.0);
  CPoint3d         receiver=CPoint3d(29.0,15.5,2.0);
  CReloadBuilding  reloader;
  //CRayTracing      raytracingObject, goraytracingObject;
  CMatlab          matlabObject;
  CBaseTransceiver btsObject;
  List<CBoundingBox> cells;
  List<CPoint3d>   centres;

  // read building environment
  reloader = CReloadBuilding( argv[1] );
  matlabObject.Story( 0, reloader.building_, "story1.txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1.txt" );
  matlabObject.Story( 1, reloader.building_, "story2.txt" );
  matlabObject.Story( 2, reloader.building_, "story3.txt" );

  // read transmitter (antenna system)
  btsObject.read( argv[2] );
  btsObject.setPosition(trans);

  // read user requirements (traffic)
  capacityRequirements = CCapacity( argv[3], btsObject );
  cells = capacityRequirements.splitIntoCells( reloader.building_, btsObject,
                                               centres );
  matlabObject.Cells( cells, centres, "cells.txt" );

  //cout << "Raytrace performed between Tx:" << btsObject.position() 
  //     << " and Rx:" << receiver << endl;

  return 0;
}
