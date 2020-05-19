//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    March 3rd 2003
// Last Modification:     Dated below "Added dd/mm/yy"
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef UPLINK_OPTIMISATION_HH
#define UPLINK_OPTIMISATION_HH

#include <iostream.h>
#include "storage.hh"
#include "list.hh"
#include "capacity.hh"
#include "path-loss.hh"
#include "parameters.hh"
#include "building.hh"
#include "bts.hh"
#include "vector.hh"
#include "matrix.hh"
#include "random.hh"
#include "downlink.hh"
#include "box.hh"
#include "storage.hh"
#include "userconst.hh"
#include "equipment.hh"

class CUplink
//==========================================================================
// = FUNCTION 
//     uplink.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Implementation of convex and non-linear uplink Optimisation for 
//     multiple transmitters in a building
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
    double voiceProbability_, nonRealTimeDataProbability_;
    double realTimeDataProbability_;
    boolean multiuserDetection_;

    CUplink()
    {
      // default constructor 
    };

    int generateRandomPopulation( List<CPoint3d>& btsPositions, 
                                  CBaseTransceiver&   btsObject,
                                  const CBuilding&    buildingObject,
                                  const SystemParameters& uplinkParam );
    //
    // generate the 8N random positions in the building given that there
    // are N base stations.
    //

    CPoint3d generateNewPoint( const CBuilding& buildingObject,
                               double& capacity );
    //
    // generate a random point inside the building, repeatedly do this until
    // we find a point that is inside a free convex space.
    //

    void readDataProbabilities( ifstream& cinput );
    //
    // reads numbers between 0 and 1 which inform the software of bands 
    // bands to obtain voice, non real-time data or real-time data 
    //

    double generateCapacity( double randomNumber );
    //
    // A random number along with the probability bands stated by the file
    // read through readDataProbability() informs the UE of which sort of 
    // data connection will be associated with the position of the UE
    // if ( randomNumber < a ) return voice
    // else if ( randomNumber < b ) return non real-time data
    // else if ( randomNumber >= a ) return real-time data
    //


    CUnitEquipment  pathLossToEachServer(
                                CPoint3d&          unitEquipmentPosition, 
                                CBaseTransceiver&  btsObject, 
                                List<CPoint3d>&    baseStationLocations, 
                          const CBuilding&         buildingObject,
                          const int&               numberOfServers );
    //
    // The best server is found for each piece of unit equipment in the
    // building. The index of the base station is returned to the caller.
    //

    void   updateSIR(       CUnitEquipment&       unitEquipment,
                            List<CUnitEquipment>& equipmentList,
                      const CBaseTransceiver&     btsObject,
                            double                capacity,
                            int                   numberOfUEs,
                            int                   numberOfServers );
    //
    // calculate the SIR at the new point to be included as a new
    // connection, including mutliuser detection if appropriate.
    // Then assuming this point can be included, the contribution to the
    // SIR due to this point must be included at all other points. 
    //
    
    double setTargetSIR( int     index,    double* capacity, 
                         const   SystemParameters& uplinkParam,
			 double& processingGain );
                         
    //
    // set the target SIR for all UE's.
    //

    boolean calculateBestServer( CUnitEquipment& unitEquipment, double tSIR );
    //
    // given that we know the SIR to each server for the current unit
    // equipment and that we know the required target SIR, we can calculate
    // the best server for the UE.
    //

    boolean addConnection( List<CUnitEquipment>& equipmentList,
                           CUnitEquipment&       unitEquipment,
                           int*                  serverConnections,
                           double*               totalServerCapacity,
                           boolean               doesBestServerMeetSIR );
    //
    // check to see that there are not too many connections to the best
    // server, if there are then try to use the next best server for a 
    // connection, making sure that in both case, that the capacity
    // requirements are also met.
    //

    boolean checkCapacity( CUnitEquipment& unitEquipment,
                           double*         totalServerCapacity );
    //
    // if capacity can not be met for new unit equipment, then half it
    // and see if it can then be met and return true, otherwise return false.
    //

    void sort( double* rayPathLoss, int* reorderedIndices, int noOfServers );
    //
    // sort the servers in order of best server down to worst server.
    //

    void print( List<CUnitEquipment>& equipmentList );
    //
    // print each item of the equipment list
    //

    boolean updateExistingConnections( List<CUnitEquipment>& equipmentList,
				    int*                  serverConnections,
				    double*               totalServerCapacity,
                                    const CBaseTransceiver& btsObject, 
                                    int                   numberOfServers );
    //
    // assuming the SIR is still met update all SIR's at each UE assuming
    // that the new UE has been included and now interfers with existing 
    // equipment. If SIR is not met, then remove the next connection from
    // the list of connections.
    //
};

#endif

