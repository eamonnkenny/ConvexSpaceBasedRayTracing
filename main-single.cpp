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

int main( int argc, char *argv[] )
{
  CPoint3d         receiver = CPoint3d( 3.0,18.0,8.5);
  CBuilding        build;
  CReloadBuilding  reloader;
  CRayTracing      raytracingObject, goraytracingObject;
  CMatlab          matlabObject;
  CBaseTransceiver btsObject;
  CVector<double>  vector1;
  ifstream         filePtr;
  ofstream         samplepoints("receivers.txt");
  string           fullReceiverPath;
  CProperties      directoryPathObject;
  
  if ( argc != 5 )
  {
    cout << "Usage:  " << argv[0] << " building_name bts_file.dat receiver_file configuration_file" << endl;
    exit(1);
  }

  directoryPathObject.read( argv[4] );

  btsObject.read( argv[2] );

  fullReceiverPath = CProperties::btsDirectory + "/" + argv[3];
  cout << "Opening receivers for reading" << endl;
  filePtr.open( fullReceiverPath.c_str() );
  filePtr >> receiver;
  filePtr.close();
  cout << "Closing receivers for reading" << endl;

  reloader = CReloadBuilding( argv[1] );
  matlabObject.Story( 0, reloader.building_, "story1.txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1.txt" );

  if ( string(argv[2]).compare( "building" ) == 0 )
  {
    matlabObject.Story( 1, reloader.building_, "story2.txt" );
    matlabObject.Floor( 1, reloader.building_, "floor2.txt" );
    matlabObject.Story( 2, reloader.building_, "story3.txt" );
    matlabObject.Floor( 2, reloader.building_, "floor3.txt" );
  }

  raytracingObject = CRayTracing( reloader.building_, btsObject, receiver, LOSSLESS );

  //goraytracingObject.geometricalOptics( reloader.building_, btsObject, 
                                        //receiver );

  cout << "Raytrace performed between Tx:" << btsObject.position() 
       << " and Rx:" << receiver << endl;
  samplepoints << receiver << endl;
  samplepoints.close();

  return 0;
}
