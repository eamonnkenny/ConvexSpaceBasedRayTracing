//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// Last Modification:     November 19th 2000
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "bts.hh"

CBaseTransceiver::CBaseTransceiver()
{
  name_             = "";
  referenceNumber_  = 0;
  regionName_       = "";
  position_         = CPoint3d( 0.0, 0.0, 0.0 );
  power_            = 0.0;
  lowerFrequency_   = 0.0;
  upperFrequency_   = 0.0;
  tilt_             = 0.0;
  azimult_          = 0.0;
  cost_             = 0;
  numberOfChannels_ = 0;
  diagramName_      = "";
  receiverNoiseFactorBTS_ = 0.0;
  receiverNoiseFactorUE_ = 0.0;
  numberOfReflections_ = 0;
  numberOfTransmissions_ = 0;
  numberOfDiffractions_ = 0;
  hertzianDipole_   = false;
  sourceType_       = LINE_SOURCE;
  serverCapacity_   = 1.4;
  gaussianRandomVariance_ = 0.0;
  randomWalk_       = false;
  cornerDiffraction_= false;
}

string CBaseTransceiver::fullDirectoryPath( const string& FileName )
{
  string fullPath;
  fullPath = CProperties::btsDirectory + "/" + FileName;
  cout << "BTS full directory: " << fullPath << endl;
  return fullPath;
}

void CBaseTransceiver::read( const string& FileName )
{
  string   dummyString;
  string   inputFileName;
  ifstream cinput;
  char     c;
  CDiagram DiagramObject;
  int      hertzianDipole;
  int      cornerDiffraction;
  int      randomWalk;
  string   fullPath;

  fullPath = fullDirectoryPath( FileName );
  cerr << "CBaseTransceiver filename (read): " << fullPath << endl;
  cinput.open( fullPath.c_str() );

  if ( !cinput.good() )
  {
    cerr << "CBaseTransceiver filename: " << fullPath
         << " does not exist" << endl;
    exit(1);
  }

  sourceType_ = LINE_SOURCE;

  cinput >> name_;
  cinput >> referenceNumber_;
  cinput >> regionName_;
  
  cinput >> dummyString;
  cerr << dummyString;
  cinput >> position_;
  cout << position_ << endl;

  cinput >> dummyString;
  cerr << dummyString;
  cinput >> power_;

  //cerr << "=============================================" << endl;
  //cerr << "For BTS:       " << name_ << endl;
  //cerr << "=============================================" << endl;
  //cerr << "  Reference:   " << referenceNumber_ << endl;
  //cerr << "  Region Name: " << regionName_ << endl;
  //cerr << "  Position:    " << position_ << endl;
  //cerr << "  Power:       " << power_ << endl;

  // next read frequency wide band
  cinput >> dummyString;
  cinput >> c;
  if ( c == '[' )
    cinput >> lowerFrequency_;

  cinput >> c;
  if ( c == '-' )
    cinput >> upperFrequency_;

  cinput >> c;

  // read in the antenna orientation
  cinput >> dummyString;
  cinput >> c;
  if ( c == '<' )
    cinput >> tilt_;
  tilt_ = tilt_ / 180.0 * Pi;
  //cerr << "  Tilt:        " << tilt_ << endl;

  cinput >> c;
  if ( c == ',' )
    cinput >> azimult_;
  azimult_ = azimult_ / 180.0 * Pi;
  //cerr << "  Azimult:     " << azimult_ << endl;

  cinput >> c;

  // read other variables
  cinput >> dummyString;
  cinput >> cost_;

  cinput >> dummyString;
  cinput >> numberOfChannels_;
  //cerr << "  Cost:        " << cost_ << endl;
  //cerr << "  Channels:    " << numberOfChannels_ << endl;

  cinput >> dummyString;
  cinput >> receiverNoiseFactorBTS_;
  cerr << "  Noise Factor (dBm):    " << receiverNoiseFactorBTS_  << endl;
  receiverNoiseFactorBTS_ = pow( 10.0, receiverNoiseFactorBTS_ / 10.0 );
  cerr << "  Noise Factor (mW):    " << receiverNoiseFactorBTS_  << endl;

  cinput >> dummyString;
  cinput >> receiverNoiseFactorUE_;
  cerr << "  Noise Factor (dBm):    " << receiverNoiseFactorUE_  << endl;
  receiverNoiseFactorUE_ = pow( 10.0, receiverNoiseFactorUE_ / 10.0 );
  cerr << "  Noise Factor (mW):    " << receiverNoiseFactorUE_  << endl;

  // read in the number of reflections in ray-tracing
  cinput >> dummyString;
  cinput >> numberOfReflections_;

  if ( numberOfReflections_ < 0 || numberOfReflections_ > 8 )
  {
    cerr << "either your BTS input file is wrong or you tried more than 8"
         << " reflections" << endl;
    exit(1);
  }

  cinput >> dummyString;
  cinput >> numberOfTransmissions_;

  if ( numberOfTransmissions_ < 0 || numberOfTransmissions_ > 8 )
  {
    cerr << "either your BTS input file is wrong or you tried more than 8"
         << " transmissions" << endl;
    exit(1);
  }

  cinput >> dummyString;
  cinput >> numberOfDiffractions_;

  cinput >> dummyString;
  cinput >> hertzianDipole;

  if ( hertzianDipole == 1 )
    hertzianDipole_ = true;
  else
    hertzianDipole_ = false;

  cinput >> dummyString;
  cinput >> randomWalk;

  if ( randomWalk == 1 )
    randomWalk_ = true;
  else
    randomWalk_ = false;

  cinput >> dummyString;
  cinput >> cornerDiffraction;

  if ( cornerDiffraction == 1 )
    cornerDiffraction_ = true;
  else
    cornerDiffraction_ = false;

  cinput >> dummyString;
  cinput >> serverCapacity_;

  cinput >> diagramName_;
  antennaDiagram_ = DiagramObject.read( diagramName_ );
  //cerr << "  Diagram:     " << diagramName_ << endl;

  //cinput >> polarizationName_;

  //polarization_ = polarizationObject.read( polarizationName_.c_str() );
  //cerr << "  Polarization:" << polarizcationName_ << endl;

  //cerr << "=============================================" << endl;
}

void CBaseTransceiver::setValues( 
               char*   name,         char*  regionName, 
               double  x,            double y,          double z, 
               double  power,        double lowerFreq,  double upperFreq,
               double  tilt,         double azimult,    double cost,
               int     reference,    int    channels,
               char*   diagramName,  double maximumGain,
	       int     numberOfHorizontalPts, int     numberOfVerticalPts, 
	       double* gain )
{
  name_             = name;
  regionName_       = regionName;
  diagramName_      = diagramName;
  position_         = CPoint3d( x, y, z );
  power_            = power;
  lowerFrequency_   = lowerFreq;
  upperFrequency_   = upperFreq;
  tilt_             = tilt / 180.0 * Pi;
  azimult_          = azimult / 180.0 * Pi;
  cost_             = cost;
  referenceNumber_  = reference;
  numberOfChannels_ = channels;

  antennaDiagram_.setValues( maximumGain, numberOfVerticalPts, 
                             numberOfHorizontalPts,  gain );
} 

void  CBaseTransceiver::setName( const string& name )
{
  name_ = name; 
}

void  CBaseTransceiver::setRegionName( const string& regionName )
{ 
  regionName_ = regionName;
}

void  CBaseTransceiver::setDiagramName( const string& diagramName )
{ 
  diagramName_ = diagramName; 
}

void  CBaseTransceiver::setPosition( const CPoint3d& position )   
{ 
  position_ = position; 
}

void  CBaseTransceiver::setPosition( double x1, double y1, double z1 )   
{ 
  position_ = CPoint3d( x1, y1, z1 ); 
}

void  CBaseTransceiver::setPower( double power )      
{
  power_ = power;
}

void  CBaseTransceiver::setSourceType( int sourceType )      
{
  sourceType_ = sourceType;
}

void  CBaseTransceiver::setNumberOfReflections( int numberOfReflections )      
{
  numberOfReflections_ = numberOfReflections;
}

void  CBaseTransceiver::setNumberOfTransmissions( int numberOfTransmissions )      
{
  numberOfTransmissions_ = numberOfTransmissions;
}

void  CBaseTransceiver::setLowerFrequency( double frequency )  
{
  lowerFrequency_ = frequency;
}

void  CBaseTransceiver::setUpperFrequency( double frequency )  
{
  upperFrequency_ = frequency;
}

void  CBaseTransceiver::setCost( double cost )  
{
  cost_ = cost;
}


void  CBaseTransceiver::setTilt( double tilt )       
{
  tilt_ = tilt;
}

void  CBaseTransceiver::setAzimult( double azimult )    
{
  azimult_ = azimult;
}

void  CBaseTransceiver::setRandomWalk( boolean onOrOff )  
{
  randomWalk_ = onOrOff;
}
void  CBaseTransceiver::setServerCapacity( double serverCapacity )    
{
  serverCapacity_ = serverCapacity;
}

void  CBaseTransceiver::setNumberOfChannels( int channels )  
{
  numberOfChannels_=channels;
}

void  CBaseTransceiver::setReferenceNumber( int reference ) 
{
  referenceNumber_ = reference;
}

void  CBaseTransceiver::setGaussianRandomVariance( double variance ) 
{
  gaussianRandomVariance_ = variance;
  srand(5);
}

CComplexVector CBaseTransceiver::incidentField( const double& r, 
                                                const CPoint3d& x_l,
                                                const CPoint3d& y_l,
                                                const CPoint3d& z_l,
                                                const double& theta,
                                                const double& phi ) const
{
  return antennaDiagram_.omniDirectional( r, x_l, y_l, z_l, 
                                          theta - tilt(), phi - azimult(), 
                                          wavenumber() );
}

CComplexVector CBaseTransceiver::incidentFieldHertzian( const double& r, 
                                                const CPoint3d& x_l,
                                                const CPoint3d& y_l,
                                                const CPoint3d& z_l,
                                                const double& theta,
                                                const double& phi ) const
{
  return antennaDiagram_.hertzianDipole( r, x_l, y_l, z_l, 
                                         theta - tilt(), phi - azimult(), 
                                         wavenumber() );
}

CComplexVector CBaseTransceiver::imageField( const double& r, 
                                             const CPoint3d& x_l,
                                             const CPoint3d& y_l,
                                             const CPoint3d& z_l,
                                             const double& theta,
                                             const double& phi ) const
{
  return antennaDiagram_.imageDipole( r, x_l, y_l, z_l, theta - tilt(), 
                                      phi - azimult(), wavenumber() );
}

double CBaseTransceiver::receiverSensitivityBTS() const
{
  double boltzmanConstant, temperature, bandwidth;

  boltzmanConstant = 1.380658e-23;
  temperature      = 290;
  //bandwidth        = 3.84e+6 * 1000.0;
  bandwidth        = 3.84e+6;

  return ( boltzmanConstant * temperature * bandwidth * receiverNoiseFactorBTS_ );
}

double CBaseTransceiver::receiverSensitivityUE() const
{
  double boltzmanConstant, temperature, bandwidth;

  boltzmanConstant = 1.380658e-23;
  temperature      = 290;
  //bandwidth        = 3.84e+6 * 1000.0;
  bandwidth        = 3.84e+6;

  return (boltzmanConstant * temperature * bandwidth * receiverNoiseFactorUE_ );
}
