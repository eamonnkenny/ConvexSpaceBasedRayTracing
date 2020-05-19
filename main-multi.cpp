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
#include <iostream>
#include "raytracing.hh"
#include "building.hh"
#include "convex.hh"
#include "matlab.hh"
#include "reloader.hh"
#include "configuration.hh"

int main( int argc, char *argv[] )
{
  CRayTracing      rayTracingObject;
  CMatlab          matlabObject;
  CPoint3d         *receiverPointArray;
  CBaseTransceiver BTSobject;
  string           buildingFileName, buildingRooms, btsFileName;
  double           xStep, yStep, zStep;
  int              M, N, i, j, k, l, scalingFactor, numberOfLevels;
  CPoint3d         receiverStartPoint;
  CPathLoss        pathLossObject;
  CMemoryStorage   storageObject;
  CReloadBuilding  reloader;
  CProperties      directoryPathObject;
  string           fullReceiverPath, receiver;
  ifstream         filePtr;

  // make a building object from a file and include all convex 
  // and room connections

  if ( argc < 5 )
  {
    cerr << "Usage:   multi building_file bts_file receiverGrid_file config_file" << endl;
    exit(1);
  }

  directoryPathObject.read( argv[4] );

  BTSobject.read( argv[2] );

  reloader = CReloadBuilding( argv[1] );

  matlabObject.Story(0, reloader.building_, "story1.txt" );
  matlabObject.Floor(0, reloader.building_, "floor1.txt" );
  matlabObject.Story(1, reloader.building_, "story2.txt" );
  matlabObject.Story(2, reloader.building_, "story3.txt" );

  fullReceiverPath = CProperties::btsDirectory + "/" + argv[3];
  filePtr.open( fullReceiverPath.c_str() );
  filePtr >> receiverStartPoint;
  filePtr >> M;
  filePtr >> N;
  filePtr >> scalingFactor;
  cout << receiverStartPoint << " " << M << " " << N << " "<< scalingFactor << endl;
  filePtr.close();

  M     = M * scalingFactor;
  N     = N * scalingFactor;
  xStep = 1.0 / double(scalingFactor); 
  yStep = 1.0 / double(scalingFactor); 
  zStep = 0.2;
  receiverPointArray = storageObject.CreatePoint3dVector( M * N );

  pathLossObject = CPathLoss( receiverStartPoint.x, receiverStartPoint.y, xStep, yStep, M, N );
  
  numberOfLevels = 1;

  for ( l = 0; l < numberOfLevels; l++ )
  {
    k = 0;
    for ( j = 0; j < N; j++ )
    {
      for ( i = 0; i < M; i++ )
      {
        receiverPointArray[k] = CPoint3d(receiverStartPoint.x + i * xStep, 
                                         receiverStartPoint.y + j * yStep,
                                         receiverStartPoint.z + l * zStep );
        k++;
      }
    }
  
    rayTracingObject = CRayTracing( reloader.building_, BTSobject, 
                                    pathLossObject, receiverPointArray, LOSSY ); 
  }

  cout << "Ray trace performed between Tx: " << BTSobject.position()
       << " and start Rx: " << receiverPointArray[0] << endl;
  storageObject.DeletePoint3dVector( receiverPointArray );

  return 0;
} 
