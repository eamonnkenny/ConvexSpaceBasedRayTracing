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

int main()
{
  CPoint3d         transceiver[NUMBER_OF_MIMO_TRANSMITTERS], receiver;
  CBuilding        build;
  CReloadBuilding  reloader;
  CRayTracing      raytracingObject, goraytracingObject;
  CMatlab          matlabObject;
  CBaseTransceiver btsObject;
  int              i, j, numberOfTransmitters = 0;
  char             extensionNumber[4];
  string          buildingSaveFile, buildingFileName, outputFileName;
  CIntrinsic       intrinsicObject;
  List<CRayPath>   validRays;

  btsObject.read( "bts900.dat" );

  matlabObject.readMimoLocations( "points.dat", transceiver, 
                                  numberOfTransmitters );

  // if building file used before then use it again: reloader
  buildingFileName = "building\0";
  reloader = CReloadBuilding( buildingFileName.c_str() );
  for ( i = 0; i < numberOfTransmitters; i++ )
  {
    // set transmitter position
    btsObject.setPosition(transceiver[i]);

    for ( j = 0; j < i; j++ )
    {
      // set the position of a number of receiver points and perform ray-trace
      receiver = transceiver[j];

      raytracingObject.MimoRayTracing( validRays, reloader.building_, 
                                       btsObject, receiver, LOSSLESS );
      cout << "Raytrace between Tx:" << btsObject.position() 
           << " and Rx:" << receiver << endl;

      // print rays paths to a file
      outputFileName  = "raypaths_\0";
      intrinsicObject.itoa( j+1, extensionNumber );
      outputFileName  = outputFileName + extensionNumber;
      extensionNumber[0] = '\0';
      intrinsicObject.itoa( i+1, extensionNumber );
      outputFileName  = outputFileName + "_\0";
      outputFileName  = outputFileName + extensionNumber;
      extensionNumber[0] = '\0';
      outputFileName  = outputFileName + ".txt\0";

      matlabObject.printRays(reloader.building_, validRays, 
		                     outputFileName.c_str() );
      validRays.clearAndClearSubList();
    }
  }
  matlabObject.Story( 0, reloader.building_, "story1.txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1.txt" );
  matlabObject.Story( 1, reloader.building_, "story2.txt" );
  matlabObject.Story( 2, reloader.building_, "story3.txt" );

  return 0;
}
