//========================================================================= 
// COPYRIGHT (c) 2017 by Trinity College Dublin, Dublin 2, IRELAND 
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// First Modification:    March 2017
// Last Modification:     10th March 2017
// Previous Projects:     ADAPT
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include <iostream>
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
  CBaseTransceiver btsObject;
  string           buildingFileName, buildingRooms, btsFileName;
  double           xStep, yStep, zStep, receiverHeight;
  int              M, N, i, j, k, l, scalingFactor, numberOfLevels;
  CPoint3d         startPoint;
  CPathLoss        pathLossObject;
  CMemoryStorage   storageObject;
  CReloadBuilding  reloader;
  CProperties      directoryPathObject;

  directoryPathObject.read( "../directory.xml" );

  btsObject.read( "bts915.dat" );
  btsObject.setRandomWalk( false );
  //btsObject.setNumberOfReflections( numReflections );
  //btsObject.setNumberOfTransmissions( numTransmissions );

  // conor's house with door item 13 open. 13 convex spaces in total
  reloader = CReloadBuilding( "conorhouse" );
  // conor's house with 21 doors open
  matlabObject.Story( 0, reloader.building_, "story1-" + btsObject.name() + ".txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1-" + btsObject.name() + ".txt" );
 
  scalingFactor = 10;

  receiverHeight = 1.39;
  M     = 6.8 * scalingFactor;
  N     = 8.0 * scalingFactor;
  xStep = 1.0 / double(scalingFactor);
  yStep = 1.0 / double(scalingFactor);
  zStep = 0.0;
  startPoint         = CPoint3d( -3.4, -4.0, -1.0+receiverHeight );
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

    rayTracingObject = CRayTracing( reloader.building_, btsObject,
                                    pathLossObject, receiverPointArray, LOSSY );
  }

  cout << "Ray trace performed between Tx: " << btsObject.position()
       << " and start Rx: " << receiverPointArray[0] << endl;
  storageObject.DeletePoint3dVector( receiverPointArray );


  return 0;
}
