//=========================================================================
//  COPYRIGHT (c) 1995-2000 by Trinity College Dublin, Dublin 2, IRELAND
//  All rights reserved  
//=========================================================================

#ifndef BASE_TRANSCEIVER_HH
#define BASE_TRANSCEIVER_HH

#include <iostream>
#include <cmath>

#include "point3d.hh"
#include <string>
#include "diagram.hh"
#include "configuration.hh"

#ifndef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport) 
#else
  #define EXPORT_CLASS_DECLSPEC class
#endif

EXPORT_CLASS_DECLSPEC CBaseTransceiver
//=========================================================================
// = FUNCTION 
//     bts.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Base Transceiver station class
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-2000 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//=========================================================================
{
  public:
    string        name_;
    int            referenceNumber_;
    string        regionName_;
    CPoint3d       position_;
    double         power_;
    double         lowerFrequency_;
    double         upperFrequency_;
    double         tilt_;
    double         azimult_;
    double         cost_;
    double         gaussianRandomVariance_;
    int            numberOfChannels_;
    string        diagramName_;
    CDiagram       antennaDiagram_;
    string        polarizationName_;
    double         receiverNoiseFactorBTS_;
    double         receiverNoiseFactorUE_;
    int            numberOfReflections_;
    int            numberOfTransmissions_;
    int            numberOfDiffractions_;
    double         serverCapacity_;
    boolean        hertzianDipole_;
    boolean        randomWalk_;
    boolean        cornerDiffraction_;
    //CPolarization  polarization_;
    int            sourceType_;

  public:
    CBaseTransceiver();
    //
    // Base Transceiver constructor for initialisation
    //

    string fullDirectoryPath( const string& FileName );
    //
    // Obtain the full directory path using CProperties
    //

    void read( const string& FileName );
    //
    // read in the contents of a BTS file, finding the diagram also
    //

    void setValues( 
               char*   name,         char*  regionName, 
               double  x,            double y,          double z, 
               double  power,        double lowerFreq,  double upperFreq,
               double  tilt,         double azimult,    double cost,
               int     reference,    int    channels,
               char*   diagramName,  double maximumGain,
	       int     numberOfHorizontalPts, int     numberOfVerticalPts, 
	       double* gain );
    //
    // set the values for the current BTS.
    //

    string   name() const        { return name_; }
    string   regionName() const  { return regionName_; }
    string   diagramName() const { return diagramName_; }
    CPoint3d  position() const    { return position_; }
    double    power() const       { return power_; }
    double    frequency() const   { return ( lowerFrequency_ + upperFrequency_ )
                                            * 0.5 * HERTZ; }
    double    tilt() const        { return tilt_; }
    double    azimult() const     { return azimult_; }
    double    cost() const        { return cost_; }
    int       numberOfChannels() const { return numberOfChannels_; }
    int       referenceNumber()  const { return referenceNumber_; }
    int       sourceType()       const { return sourceType_; }
    int       numberOfReflections() const { return numberOfReflections_; }
    int       numberOfTransmissions() const { return numberOfTransmissions_; }
    int       numberOfDiffractions() const { return numberOfDiffractions_; }
    boolean   hertzianDipole() const { return hertzianDipole_; }
    CDiagram  antennaDiagram()   const { return antennaDiagram_; }
    double    randomVariance()   const { return gaussianRandomVariance_; }
    double    serverCapacity()   const { return serverCapacity_; }
    boolean   randomWalk()       const { return randomWalk_; }
    boolean   cornerDiffraction()       const { return cornerDiffraction_; }

    void   setName( const string& name );
    void   setRegionName( const string& regionName );
    void   setDiagramName( const string& diagramName );
    void   setPosition( const CPoint3d& position );
    void   setPosition( double x1, double y1, double z1 );
    void   setPower( double power );
    void   setLowerFrequency( double frequency );
    void   setUpperFrequency( double frequency );
    void   setTilt( double tilt );
    void   setCost( double cost );
    void   setAzimult( double azimult );
    void   setPolarization( char polar );
    void   setNumberOfChannels( int channels );
    void   setNumberOfReflections( int numberOfReflections );
    void   setNumberOfTransmissions( int numberOfTransmissions );
    void   setReferenceNumber( int reference );
    void   setSourceType( int sourceType );
    void   setGaussianRandomVariance( double variance );
    void   setRandomWalk( boolean randomWalkOnOrOff );
    void   setServerCapacity( double serverCapacity );

    double wavenumber() const { return (TwoPi*frequency()/SpeedOfLight); }

    CComplexVector incidentField( const double&   r,     const CPoint3d& x_l,
                                  const CPoint3d& y_l,   const CPoint3d& z_l, 
				  const double&   theta, const double& phi )
				  const;
    CComplexVector incidentFieldHertzian( 
                                  const double&   r,     const CPoint3d& x_l,
                                  const CPoint3d& y_l,   const CPoint3d& z_l, 
				  const double&   theta, const double& phi )
				  const;
    CComplexVector imageField( const double& r, const CPoint3d& x_l,
                               const CPoint3d& y_l, const CPoint3d& z_l,
			       const double& theta, const double& phi ) const;
    //
    // returns the incident field for an incident field with a given 
    // polarization which varies in x, y or z. Usually we assume on
    // direction of polarization resulting in a field which is complex. 
    // Since it is now has a more general nature it must be stored as 
    // a complex triple.
    //

    double receiverSensitivityBTS() const;
    double receiverSensitivityUE() const;
    //
    // calculate the receiver sensitivity in the base station
    //

    ~CBaseTransceiver()
    {
      // default destructor
      // string, diagram and polarization classes have their own destructors
    }
};

#endif 
