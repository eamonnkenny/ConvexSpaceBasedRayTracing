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
  int              M, N, i, j, k, l, scalingFactor, numberOfLevels;
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
 
  r     = 10.0;
  theta = Pi / 3.0;
  phi   = 0.0;

  BTSobject.setPosition( CPoint3d( r * sin( theta ) * cos( phi ), 
                                   r * sin( theta ) * sin( phi ), 
                                   r * cos( theta ) ) );
  scalingFactor = 8;

  M      = 46;
  N      = 46;
  //M      = 5;
  //N      = 5;
  //M      = 1;
  //N      = 1;
  dtheta = 90.0 / double( M - 1 );
  dphi   = 360.0 / double( N - 1 );
  r      = 20.0;

  startPoint         = CPoint3d( -1, -1, 1.0 );
  receiverPointArray = storageObject.CreatePoint3dVector( M * N );

  pathLossObject = CPathLoss( 0, 0, dtheta, dphi, M, N );
  cout << "Number of path-loss Points: " << pathLossObject.numberOfPoints() 
       << endl;
  
  numberOfLevels = 1;
  cout << "BTS Position: " << BTSobject.position() << endl;

  for ( l = 0; l < numberOfLevels; l++ )
  {
    k = 0;
    for ( j = 0; j < N; j++ )
    {
      for ( i = 0; i < M; i++ )
      {
        theta = Pi * double(i) * dtheta / 180.0;
        phi   = Pi * double(j) *   dphi / 180.0;
        receiverPointArray[k] = CPoint3d( r * sin( theta ) * cos( phi ), 
                                          r * sin( theta ) * sin( phi ), 
                                          r * cos( theta ) );
        k++;
      }
    }
  
    rayTracingObject = CRayTracing( reloader.building_, BTSobject, 
                                   pathLossObject, receiverPointArray, l, PEC );
  }

  cout << "Ray trace performed between Tx: " << BTSobject.position()
       << " and start Rx: " << receiverPointArray[0] << endl;
  storageObject.DeletePoint3dVector( receiverPointArray );

  return 0;
} 
