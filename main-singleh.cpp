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

int main()
{
  //CPoint3d         trans    = CPoint3d(10.0 * sqrt(3),0.0,10.0);
  //CPoint3d         receiver = CPoint3d(-35,0.0,20.0);

  CPoint3d         trans    = CPoint3d( 2.0,10.5,1.5);
  CPoint3d         receiver = CPoint3d(29.0,10.5,1.5);

  CBuilding        build;
  CReloadBuilding  reloader;
  CRayTracing      raytracingObject, goraytracingObject;
  CMatlab          matlabObject;
  CBaseTransceiver btsObject;
  CVector<double>  vector1;
  CProperties      directoryPathObject;
  string          directoryString = "../directory.xml";
  string          btsString = "bts900.dat";
  
  directoryPathObject.read( directoryString.c_str() );

  btsObject.read( btsString.c_str() );
  btsObject.setPosition(trans);

  reloader = CReloadBuilding( "building" );

  matlabObject.Story( 0, reloader.building_, "story1.txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1.txt" );
  //matlabObject.Story( 1, reloader.building_, "story2.txt" );
  //matlabObject.Floor( 1, reloader.building_, "floor2.txt" );
  //matlabObject.Story( 2, reloader.building_, "story3.txt" );
 

  raytracingObject = CRayTracing( reloader.building_, btsObject, receiver,
	                          PEC );

  goraytracingObject.geometricalOptics( reloader.building_, btsObject, 
                                        receiver );

  cout << "Raytrace performed between Tx:" << btsObject.position() 
       << " and Rx:" << receiver << endl;

  return 0;
}
