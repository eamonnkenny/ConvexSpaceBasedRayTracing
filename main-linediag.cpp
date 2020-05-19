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
  double           xStep, yStep, zStep, r, theta, phi;
  int              M, N, i, j, k, l, scalingFactor, numberOfLevels;
  CPoint3d         startPoint;
  CPathLoss        pathLossObject;
  CMemoryStorage   storageObject;
  CReloadBuilding  reloader;

  // make a building object from a file and include all convex 
  // and room connections

  BTSobject.read( "bts750.dat" );

  r     = 0.0;
  theta = 0.0;
  phi   = Pi / 4.0;

  BTSobject.setPosition( CPoint3d( r * sin( theta ) * cos( phi ), 
                                   r * sin( theta ) * sin( phi ), 
                                   r * cos( theta ) ) );

  reloader = CReloadBuilding( "plate" );

  matlabObject.Story(0, reloader.building_, "story1.txt" );
  matlabObject.Floor(0, reloader.building_, "floor1.txt" );
  matlabObject.Story(1, reloader.building_, "story2.txt" );
  matlabObject.Story(2, reloader.building_, "story3.txt" );

  scalingFactor = 1;

  M     = 91 * scalingFactor;
  N     = 1 * scalingFactor;
  xStep = 1 / double(scalingFactor); 
  yStep = 1 / double(scalingFactor); 
  zStep = 0.0;
  startPoint         = CPoint3d( -100, 0, 20 );
  receiverPointArray = storageObject.CreatePoint3dVector( M * N );

  pathLossObject = CPathLoss( startPoint.x, startPoint.y, xStep, yStep, M, N );
  numberOfLevels = 1;

  for ( l = 0; l < numberOfLevels; l++ )
  {
    k = 0;

    for ( j = 0; j < N; j++ )
    {
      for ( i = 0; i < M; i++ )
      {
        theta = double( i ) * Pi / 180.0;
	phi   = Pi / 4.0;
        receiverPointArray[k] = CPoint3d( sin( theta ) * cos( phi ), 
                                          sin( theta ) * sin( phi ),
				          cos( theta ) );
        k++;
      }
    }

    rayTracingObject = CRayTracing( reloader.building_, BTSobject, 
                                    pathLossObject, receiverPointArray, l,
                                    PEC ); 
  }

  cout << "Ray trace performed between Tx: " << BTSobject.position()
       << " and start Rx: " << receiverPointArray[0] << endl;
  storageObject.DeletePoint3dVector( receiverPointArray );

  return 0;
} 
