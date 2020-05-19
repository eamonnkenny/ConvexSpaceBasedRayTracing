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
  double           xStep, yStep, zStep;
  int              M, N, i, j, k, l, scalingFactor, numberOfLevels;
  CPoint3d         startPoint;
  CPathLoss        pathLossObject;
  CMemoryStorage   storageObject;
  CReloadBuilding  reloader;

  // make a building object from a file and include all convex 
  // and room connections

  BTSobject.read( "bts1000.dat" );
  BTSobject.setPosition( CPoint3d( 5.468, 6.51, 1.45 ) );

  reloader = CReloadBuilding( "printinghouse" );

  matlabObject.Story( 0, reloader.building_, "story1-" + btsObject.name() + ".txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1-" + btsObject.name() + ".txt" );


  scalingFactor = 4;

  M     = 18 * scalingFactor;
  N     = 8 * scalingFactor;
  xStep = 1.0 / double(scalingFactor); 
  yStep = 1.0 / double(scalingFactor); 
  zStep = 1.58;
  startPoint         = CPoint3d( 0.25, 1, 0.0 );
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
        receiverPointArray[k] = CPoint3d(startPoint.x + i * xStep, 
                                         startPoint.y + j * yStep,
				         startPoint.z + l * zStep );
        k++;
      }
    }
  
    rayTracingObject = CRayTracing( reloader.building_, BTSobject, 
                                    pathLossObject, receiverPointArray, l,
				    LOSSY ); 
  }

  cout << "Ray trace performed between Tx: " << BTSobject.position()
       << " and start Rx: " << receiverPointArray[0] << endl;
  storageObject.DeletePoint3dVector( receiverPointArray );

  return 0;
} 
