//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Prof. Frank Boland
// First Modification:    April 1st 2003
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef UNIT_EQUIPMENT_HH
#define UNIT_EQUIPMENT_HH

#include <iostream.h>
#include "point3d.hh"
#include "plane.hh"
#include "userconst.hh"

class CUnitEquipment
//==========================================================================
// = FUNCTION 
//         equipment.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      The definition of the information required to define the user
//      equipment
//        - Contains a location.
//        - Capacity requirements.
//        - SIR obtained.
//        - target SIR needed.
//        - Best Server Index.
//        - Transmit power requirement
//        - ray path loss to best server
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2 IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
      CPoint3d location;
      double   capacity;
      double   *SIR, targetSIR;
      double   *rayPathLoss;
      double   *power;
      double   processingGain_;
      int      *bestServer_;
      int      theBestServer;
      int      numberOfServers;

      CUnitEquipment()
      {
        // default constructor
                numberOfServers = 0;
      }

      CUnitEquipment( const CPoint3d& aLocation, double *rayPathLossArray,
                      double *powerArray,        int noOfServers )
      {
        int i;
        numberOfServers = noOfServers;
        location        = aLocation;
        rayPathLoss     = new double [numberOfServers];
        power           = new double [numberOfServers];
        for ( i = 0; i < numberOfServers; i++ )
        {
          rayPathLoss[i] = rayPathLossArray[i];
          power[i]       = powerArray[i];
        }
      }

      void setSIR( double *SIRarray )
      {
        SIR = new double [numberOfServers];
        for ( int i = 0; i < numberOfServers; i++ )
        {
          SIR[i] = SIRarray[i];
        }
      }

      void setBestServer( int* bestServerIndex )
      {
        bestServer_ = new int [numberOfServers];
        for ( int i = 0; i < numberOfServers; i++ )
        {
          bestServer_[i]  = bestServerIndex[i];
        }
        theBestServer = bestServer_[0];
      }

      void setBestServer( int bestServerIndex )
      {
        theBestServer = bestServerIndex;
      }

      void setTargetSIR( double target )
      {
        targetSIR = target;
      }

      void setCapacity( double userCapacity )
      {
        capacity = userCapacity;
      }

      inline double processingGain() { return processingGain_; };

      int bestServer() const    { return theBestServer; }
      int secondBestServer() const { return bestServer_[1]; }
      //
      // return the best server and second best server indices
      //

      void clear()
      {
        // default destructor
        if ( numberOfServers > 0  && numberOfServers < 100 )
        {
          delete [] power;
          delete [] rayPathLoss;
          delete [] bestServer_;
        }
      }

      ~CUnitEquipment()
      {
        // default destructor
      }
};

#endif
