//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    December 4th 2001
// Previous Projects:     None
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef RAY_TRACE_HH
#define RAY_TRACE_HH

#include "reflection.hh"
#include "diffraction.hh"
#include "diffcoeff.hh"
#include "ComplexVector.hh"
#include "path-loss.hh"
#include "contact.hh"
#include "corners.hh"
#include "cornerdiff.hh"

class CRayTracing
//==========================================================================
// = FUNCTION 
//        raytracing.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      Raytracing algorithm for reflections, diffractions and transmissions.
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
    CRayTracing() 
    {
      // default constructor
    }

    CRayTracing( const CBuilding&        buildingObject,
                 const CBaseTransceiver& BTSobject,
                 const CPoint3d&         receiverPoint,
                 const mediaType&        media );
    void MimoRayTracing(
                       List<CRayPath>&   nonZeroRays,
                 const CBuilding&        buildingObject,
                       CBaseTransceiver& BTSobject,
                 const CPoint3d&         receiverPoint,
                 const mediaType&        media );
    CRayTracing( const CBuilding&        buildingObject,
                 const CBaseTransceiver& BTSobject,
                       CPathLoss&        pathLossObject,
                       CPoint3d*         receiverPoints,
                       int               i,
                 const mediaType&        media );
    //
    // given a building transmitter and receiver point, calculate a complete
    // set of 3rd order reflections and transmissions for the given points.
    //

    void geometricalOptics( const CBuilding&        buildingObject,
                            const CBaseTransceiver& BTSobject,
                            const CPoint3d&         receiverPoint );
    void geometricalOptics( const CBuilding&        buildingObject,
                            const CBaseTransceiver& BTSobject,
                                  CPathLoss&        pathLossObject,
                                  CPoint3d*         receiverPoints,
                                  int               i );
    void geometricalOptics(       CComplexVector&   Et,
                                  CRayPath&         rayPath,
                            const CBuilding&        buildingObject,
                            const CBaseTransceiver& btsObject,
                            const mediaType&        media  );
    //
    // Geometrical Optics (GO) for many rays and for one particular ray path
    //

    void areAllValidDiffractions(      List<CRayPath>& rayPathList,
                                 const CBuilding&      buildingObject,
                                       List<CEdge>&    diffractionEdges );
    //
    // check to see if the diffraction rays are really valid?
    //

    CComplexVector hertzianDipole( const double& r,   const double& theta, 
                                   const double& phi, const double& ki );
    //
    // Added 18/12/01
    // calcular the incident field vector from a hertzian dipole emitted
    // at angle theta, phi and a distance r away with a wavenumber ki
    //

    void totalField(  int j,      CComplexVector&   Et,
                           CEdgeList&        diffractionImages,
                           CCorners&         cornerDiffractionPoints,
                     const CBuilding&        buildingObject,
                     const CBaseTransceiver& btsObject,
                     const mediaType&        media,
		     const CPoint3d&         receiverPoint,
                     const polarisationType& polarisation  );
    //
    // Added 20/12/01 Changed 02/01/02
    // calculate the total field for a given ray-path
    //
   
    boolean transmissionCoefficient(    CComplexVector&   Ei,
                                        List<CPointInfo>& pointList,
                                        CSegment3d&       si,
                                  const CBaseTransceiver& btsObject,
                                  const CBuilding&        buildingObject,
                                  const int&              firstTransmission,
                                  const int&              lastTransmission,
                                  const mediaType&        media );
    //
    // Added 21/12/01 Amended 03/01/02
    // calculate the transmission coefficient through many dielectric slabs
    // taking into account the bending of the ray inside that slab.
    // This calculation is performed at transmission points between two
    // two points (segment si). The indices of the raypath list are given by
    // firstTransmission and lastTransmission
    // Routine Burnside formulation 
    //

    void randomVariable( const CBaseTransceiver& btsObject,
                               CComplexVector&   Ei );
    //
    // add a gaussian random value to the complex field
    //

    void transmissionThroughDielectric( CComplexVector&   Ei,
                                  const CSegment3d&       si,
                                  const CConvex&          convexSpace,
                                  const CBoundary&        boundary,
                                  const CBaseTransceiver& btsObject,
                                        complex           Pd,
                                        complex           Pt,
                                  const mediaType&        media );
    //
    // Added 12/09/02  (added due to for simplicity)
    // calculate the change in the incident field due to travelling
    // from free space to a dielectric material along s_i
    // This is a much simplier formula than before since we note that
    // T_1 * T_2 = 1 - sqr( R_1 ) for both electric and magnetic cases
    //


    void transmissionFreeSpaceToDielectric(
                                        CComplexVector&   Ei,
                                  const CSegment3d&       si,
                                  const CConvex&          convexSpace,
                                  const CBoundary&        boundary,
                                  const CBaseTransceiver& btsObject,
                                  const mediaType&        media );
    //
    // Added 21/12/01 Amended 03/01/02
    // calculate the change in the incident field due to travelling
    // from free space to a dielectric material along s_i
    //

    void transmissionDielectricToFreeSpace(
                                        CComplexVector&   Ei,
                                  const CSegment3d&       si,
                                  const CConvex&          convexSpace,
                                  const CBoundary&        boundary,
                                  const CBaseTransceiver& btsObject,
                                  const mediaType&        media );
    //
    // Added 21/12/01 Amended 03/01/02
    // calculate the change in the incident field due to travelling
    // from a dielectirc material to free space along s_i
    //

    void divideOutReflectionTerms(      double            slabWidth,
                                        CComplexVector&   Ei,
                                  const CSegment3d&       si,
                                  const CConvex&          convexSpace,
                                  const CBoundary&        boundary,
                                  const CBaseTransceiver& btsObject,
                                  const mediaType&        media );
    //
    // When calculating the transmission through a slab it is important to 
    // remember to divide out the product term of reflection coefficients
    // as presented in Born, Wolf, "Principles of Optics", pp.64-66
    //

    boolean reflectionCoefficient(      CComplexVector&   Ei,
                                  const CSegment3d&       si,
                                  const CSegment3d&       sr,
                                  const double&           pathLength,
                                  const CPointInfo&       reflectionPoint,
                                  const CBuilding&        buildingObject,
                                  const CBaseTransceiver& btsObject,
                                  const mediaType&        media );
    //
    // Added 20/12/01 Amended 03/01/02
    // calculate the change in the incident field due to travelling
    // in free space along s_i to hit a refleciton point on a dielectric 
    // material and then travel along s_r. This calculation creates the 
    // reflection coefficient
    //

    void localReflectionCoefficient(       double     cosTheta_i,
                                     const complex&   permittivity,
                                     const mediaType& media,
                                           complex&   Rparallel,
                                           complex&   Rperpendicular );
    //
    // separated the localised reflection coefficient (along the ray)
    // so that we can use it in the diffraction coefficients without
    // rewritting the code. Added 23/01/02
    //

    boolean diffractionCoefficient(      CComplexVector&   Ei,
                                   const CSegment3d&       si,
                                   const CSegment3d&       sr,
                                   const double&           pathLength,
                                   const double&           totalPathLength,
                                   const CPointInfo&       diffractionPoint,
                                   const CSegment3d&       diffractingEdge,
                                   const CBuilding&        buildingObject,
                                   const CBaseTransceiver& btsObject,
                                   const mediaType&        media );
    //
    // Added 23/01/02 
    // calculate the change in the field due to travelling
    // in free space along s_i to hit a diffraction point on a dielectric 
    // material and then travel along s_r. This calculation creates the 
    // diffraction coefficient, and updates the field Ei.
    //

    boolean cornerDiffraction(       CComplexVector&   Ei,
                               const CPoint3d&         sihat,
                               const CPoint3d&         srhat,
                               const double&           pathLength,
                               const double&           totalPathLength,
                               const CPoint3d&         diffractionPt,
                               const CSegment3d&       diffractionEdge,
                               const CBuilding&        buildingObject,
                               const CBaseTransceiver& btsObject,
                               const mediaType&        media,
                                     CCorners&         corners,
                               const polarisationType& polarisation );
    //
    // Added 10/12/02 
    // calculate the change in the field due corner diffraction, 
    // and updates the field Ei.
    //


    void    calculateAnglesAndLocalCoordinates(
                                        const CPoint3d&   cornerPoint,
					const CPoint3d&   ehat,
					const CPoint3d&   t0_hat,
					const CPoint3d&   n0_hat,
					const CPoint3d& si,
                                        const CPoint3d& sr,
					const CSegment3d& diffractingEdge,
                                              CPoint3d&   betaC_dash_hat, 
					      CPoint3d&   phi_dash_hat, 
					      CPoint3d&   betaC_hat,
					      CPoint3d&   phi_hat, 
					      double&     betaC,
					      double&     beta0c, 
					      double&     Lc, 
					      CSegment3d& s_ic,
					      CSegment3d& s_rc,
					      double&     absS_ic,
					      double&     absS_rc );
    //
    // Added 12/12/02 
    // calculate the angles and local coordinate system unit vectors
    // required to complete a corner diffraction calculation
    //

    CComplexVector incidentField( CSegment3d& si, 
                                  const polarisationType& polarisation,
                                  const CBaseTransceiver& btsObject );
    CComplexVector incidentFieldHertzian( CSegment3d& si, 
                                  const polarisationType& polarisation,
                                  const CBaseTransceiver& btsObject );
    //
    // Added 16/04/02
    // Calculate the incident field for a given polarisation
    //
};

#endif
