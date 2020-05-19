//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    22nd May 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef SYSTEM_PARAMETERS_HH
#define SYSTEM_PARAMETERS_HH

#include "bts.hh"
#include "raytracing.hh"
#include "capacity.hh"

class SystemParameters
//==========================================================================
// = FUNCTION 
//     parameters.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Describes the parameters required by the user in the ray-tracing
//     model
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
    double    interferenceMargin_; 

    double    EbNoVoice_, fadeMarginVoice_, processingGainVoice_;

    double    EbNoRealTimeData_, fadeMarginRealTimeData_;
    double    processingGainRealTimeData_;

    double    EbNoNonRealTimeData_, fadeMarginNonRealTimeData_;
    double    processingGainNonRealTimeData_;

    inline double fromdB( double x ) { return pow( 10.0, x / 10.0 ); }
    //
    // convert decibel values to units
    //

    void read( char* parameterFileName );
    //
    // read in the parameters from a file
    //

    double interferenceMargin() { return interferenceMargin_; }
    //
    // return parameters
    //

    double maximumPermissiblePower( const CBaseTransceiver& btsObjbect,
                                    const complex&          rayPathLoss )
				    const;
    //
    // calculate the maximum permissible power at the base station 
    //

    void inputs( double  userCapacity, double& EbNo, 
                 double& fadeMargin,   double& processingGain ) const;
    //
    // speed (voice, data) related parameters for the calculation of 
    // user capacity requirements
    //

    boolean checkTargetSIR( int&    bestServer,       double *maximumPower,
                            complex **gain,           int    NumberOfServers, 
			    int     numberOfSectors,  List<CPoint3d>& centres,
                            const   CBuilding&        buildingObject,
                                    CBaseTransceiver& btsObject,
                            double  userCapacity,     const CPoint3d& userPoint,
                            const   mediaType&  media );
    //
    // check to see that the target signal to interference ratio (SIR) is met 
    //
};

#endif
