//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
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
#include <iostream.h>
#include "raytracing.hh"
#include "building.hh"
#include "convex.hh"
#include "matlab.hh"
#include "reloader.hh"

int main()
{
  CRayTracing      rayTracingObject;
  CMatlab          matlabObject;
  CPoint3d         *receiverPointArray;
  CBaseTransceiver BTSobject;
  string          buildingFileName, buildingRooms, btsFileName;
  double           dtheta, dphi, r, theta, phi;
  int              M, N, i, j, k, scalingFactor, numberOfLevels;
  CPoint3d         startPoint;
  CPathLoss        pathLossObject;
  CMemoryStorage   storageObject;
  CReloadBuilding  reloader;

  // make a building object from a file and include all convex 
  // and room connections

  BTSobject.read( "bts900.dat" );

  reloader = CReloadBuilding( "plate" );

  matlabObject.Story(0, reloader.building_, "story1.txt" );
  matlabObject.Floor(0, reloader.building_, "floor1.txt" );
  matlabObject.Story(1, reloader.building_, "story2.txt" );
  matlabObject.Story(2, reloader.building_, "story3.txt" );
 
  r     = 20.0;
  theta = Pi / 3.0;
  phi   = 0.0;

  BTSobject.setPosition( CPoint3d( r * sin( theta ) * cos( phi ), 
                                   r * sin( theta ) * sin( phi ), 
                                   r * cos( theta ) ) );

  cout << "BTS Position: " << BTSobject.position() << endl;

  rayTracingObject = CRayTracing( reloader.building_, BTSobject, 
                                  CPoint3d( -7.0, 0.0, 20.0 ), PEC );

  cout << "Ray trace performed between Tx: " << BTSobject.position()
       << " and start Rx: " << CPoint3d( -7.0, 0.0, 20.0 ) << endl;
  //storageObject.DeletePoint3dVector( receiverPointArray );

  return 0;
} 
