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

#include "parameters.hh"

void SystemParameters::read( char* parameterFileName )
{
  ifstream istr;
  string   fullFilePath;
 
  fullFilePath = CProperties::matlabOptimisationDirectory + "/" + parameterFileName;

  istr.open( fullFilePath.c_str() );
  istr >> interferenceMargin_;

  istr >> EbNoVoice_;
  istr >> fadeMarginVoice_;
  istr >> processingGainVoice_;

  istr >> EbNoRealTimeData_;
  istr >> fadeMarginRealTimeData_;
  istr >> processingGainRealTimeData_;

  istr >> EbNoNonRealTimeData_;
  istr >> fadeMarginNonRealTimeData_;
  istr >> processingGainNonRealTimeData_;
  istr.close();

  interferenceMargin_            = fromdB( interferenceMargin_ );
  EbNoVoice_                     = fromdB( EbNoVoice_ );
  fadeMarginVoice_               = fromdB( fadeMarginVoice_ );
  processingGainVoice_           = fromdB( processingGainVoice_ );
  EbNoRealTimeData_              = fromdB( EbNoRealTimeData_ );
  fadeMarginRealTimeData_        = fromdB( fadeMarginRealTimeData_ );
  processingGainRealTimeData_    = fromdB( processingGainRealTimeData_ );
  EbNoNonRealTimeData_           = fromdB( EbNoNonRealTimeData_ );
  fadeMarginNonRealTimeData_     = fromdB( fadeMarginNonRealTimeData_ );
  processingGainNonRealTimeData_ = fromdB( processingGainNonRealTimeData_ );
}

double SystemParameters::maximumPermissiblePower( 
                                 const   CBaseTransceiver& btsObject,
                                 const   complex& rayPathLoss ) const
{
  return ( btsObject.receiverSensitivityUE()
         / qsqr( cabs(rayPathLoss) / ( 2.0 * btsObject.wavenumber() ) ) );
  // divide ray path loss by 2k = two times the wavenumber to give Friis 
  // equation for free space loss.
}

void SystemParameters::inputs( double  userCapacity, double& EbNo, 
                               double& fadeMargin,   double& processingGain )
			       const
{
  if ( userCapacity <= 0.0122 )
  {
    EbNo           = EbNoVoice_;
    fadeMargin     = fadeMarginVoice_;
    processingGain = processingGainVoice_;
  }
  else if ( userCapacity >= 0.128 )
  {
    EbNo           = EbNoNonRealTimeData_;
    fadeMargin     = fadeMarginNonRealTimeData_;
    processingGain = processingGainNonRealTimeData_;
  }
  else
  {
    EbNo           = EbNoRealTimeData_;
    fadeMargin     = fadeMarginRealTimeData_;
    processingGain = processingGainRealTimeData_;
  }
}

boolean SystemParameters::checkTargetSIR( 
                          int&   bestServer,
                          double *maximumPower,    complex **gain,
                          int    numberOfServers,  int     numberOfSectors,
                                 List<CPoint3d>&   centres,
                          const  CBuilding&        buildingObject,
                                 CBaseTransceiver& btsObject,
                                 double            userCapacity,
                          const  CPoint3d&         userPoint,
                          const  mediaType&        media )
{
  CRayTracing    rayTracingObject;
  complex        *fieldStrength, interferingField;
  List<CRayPath> allRayPaths;
  CRayPath       rayPath;
  double         EbNo, fadeMargin, processingGain, totalInterference;
  double         pathLoss, SIR, targetSIR, angularSector, AOD;
  int            i, j, k;
  ofstream       coutput( "checkSIR.txt", ios::app );
  CMemoryStorage storageObject;

  angularSector = TwoPi / numberOfSectors;
  fieldStrength = storageObject.CreateComplexVector( numberOfServers );

  for ( i = 0; i < numberOfServers; i++ )
  {
    btsObject.setPosition( centres.GetItem(i) );
    rayTracingObject.MimoRayTracing( allRayPaths, buildingObject, btsObject,
                                     userPoint, media );
    for ( j = 0; j < allRayPaths.size(); j++ )
    {
      rayPath = allRayPaths.GetItem(j);
      AOD     = rayPath.angleOfDeparture();
  
      for ( k = 0; k < numberOfSectors; k++ )
      {
        if ( AOD <= ( k + 1 ) * angularSector )
        { 
          fieldStrength[i] += gain[i][k] * rayPath.fieldStrength_[2].z;
          k = numberOfSectors;
        }
      }
    }
    allRayPaths.clearAndClearSubList();
  }

  bestServer = 0;

  for ( i = 0; i < numberOfServers; i++ )
  {
    if ( cabs( sqrt( maximumPower[bestServer] ) * fieldStrength[bestServer] ) 
       < cabs( sqrt( maximumPower[i] ) * fieldStrength[i] ) )
      bestServer = i;
  }

  interferingField = 0.0;
  for ( i = 0; i < numberOfServers; i++ )
  {
    if ( i != bestServer )
      interferingField += sqrt( maximumPower[i] ) * fieldStrength[i];
  }
  pathLoss = cabs(fieldStrength[bestServer]) / ( 2.0 * btsObject.wavenumber() );
  totalInterference = cabs( interferingField ) / ( 2.0 * btsObject.wavenumber() );

  SIR = maximumPower[bestServer] * qsqr(pathLoss)
      / ( qsqr(totalInterference) + btsObject.receiverSensitivityUE() );

  inputs( userCapacity, EbNo, fadeMargin, processingGain );
  targetSIR = EbNo / processingGain;
  coutput << SIR << " > " << targetSIR << "?" << endl; 
  coutput.close();

  storageObject.DeleteComplexVector( fieldStrength );
  if ( targetSIR > SIR )
  {
    cout << "Target was: " << targetSIR << " but got: " << SIR << endl;
    for ( i = 0; i < numberOfServers; i++ )
    {
      cout << maximumPower[i] << "  " << cabs( fieldStrength[i] ) << endl;
    }
  }

  if ( SIR > targetSIR )
    return true;
  else
    return false;
}
