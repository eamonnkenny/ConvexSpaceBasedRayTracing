//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    June 13th 2002
// Last Modification:     Dated below "Added dd/mm/yy"
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef DOWNLINK_HH
#define DOWNLINK_HH

#include <iostream>
#include "storage.hh"
#include "list.hh"
#include "capacity.hh"
#include "path-loss.hh"
#include "parameters.hh"
#include "vector.hh"
#include "matrix.hh"
#include "linear-program.hh"

class CDownlink
//==========================================================================
// = FUNCTION 
//     downlink.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Implementation of convex and non-linear Optimisation for multiple
//     transmitters in a building
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
    CDownlink()
    {
      // default constructor 
    };

    double objective( boolean& isCapacityExceeded, 
                      int&     l, double   t, 
		      int   numberOfSectors,
                      const CVector<double>&  vcentres,     
                      const CVector<double>&  oldcentres,     
                      CBaseTransceiver& btsObject,
                      const CBuilding&  buildingObject,
                      const CCapacity&  capacityGrid,
		            double*     targetSIR,
                      const SystemParameters& parameterObject,
                      const boolean&    writeBestServer,
		            complex     ***fieldStrength );
    double objective( boolean& isCapacityExceeded, 
                      int&     l, double   t, 
		      int   numberOfSectors,
                      const CVector<double>&  vcentres,     
                      const CVector<double>&  oldcentres,     
                      const CVector<double>&  minimisedPower,     
                      CBaseTransceiver& btsObject,
                      const CBuilding&  buildingObject,
                      const CCapacity&  capacityGrid,
		            double*     targetSIR,
                      const SystemParameters& parameterObject,
                      const boolean&    writeBestServer,
		            complex     ***fieldStrength );
    //
    // generate the objective function for the non-linear optimisation
    // or given that we know the minimised power calculate the objective
    // function to see if the same number of SIR values are still obtained
    //

    void useObjectiveToCreateConvexConstraintMatrix( 
                      CMatrix<double>& A,  CVector<double>& b,
                      CVector<double>& p0, boolean&         isCapacityExceeded, 
                      int&   l,         double t, 
		      int   numberOfSectors,
                      const CVector<double>&  vcentres,     
                      CBaseTransceiver& btsObject,
                      const CBuilding&  buildingObject,
                      const CCapacity&  capacityGrid,
		            double*     targetSIR,
                      const SystemParameters& parameterObject,
                      const boolean&    writeBestServer );
    //
    // generate the constraint matrix for the convex optimisation
    //

    CVector<double> gradient(       double            j, 
                                    double            t, 
				    int               numberOfSectors,
                                    CVector<double>&  vcentres,     
                                    CBaseTransceiver& btsObject,
                              const CBuilding&        buildingObject,
                              const CCapacity&        capacityGrid,
			            double*           targetSIR,
                              const SystemParameters& parameterObject,
			            complex           ***fieldStrength );
    boolean          annealing(     double            j, 
                                    double            t,
				    int               numberOfSectors,
                                    CVector<double>&  vcentres,     
                                    CBaseTransceiver& btsObject,
                              const CBuilding&        buildingObject,
                              const CCapacity&        capacityGrid,
			            double*           targetSIR,
                              const SystemParameters& parameterObject,
			            complex           ***fieldStrength );
    //
    // calculate the positions of the base stations which meet the
    // Signal to interference ratio (SIR) everywhere (if possible).
    // the base station locations are stored as a vector of doubles
    // bs1 = (v[1],v[2],v[3]) bs2 = (v[4],v[5],v[6]), etc...
    //

    void bestServers( List<CPoint3d>&   centres,
                      CBaseTransceiver& btsObject,
                      const CBuilding&  buildingObject,
                      const CCapacity&  capacityGrid );
    //
    // Generate the best servers for the current sent of transmitters
    //

    double objectiveFunction( 
               boolean& isCapacityExceeded,
               int&   l,                double  t,
               double *maximumPower,    complex **gain,      
               int    NumberOfServers,  int     numberOfSectors, 
                      List<CPoint3d>&   centres,
                      List<CPoint3d>&   oldcentres,     
               const  CBuilding&        buildingObject,
                      CBaseTransceiver& btsObject,
               const  SystemParameters& parameterObject,
               const  CCapacity&        capacityGrid, 
	              double*           targetSIR,
               const  mediaType&        media,
               const  boolean&          writeBestServer,
	              complex           ***fieldStrength );

    void formConvexOptimisationMatrix( 
               CMatrix<double>& A, CVector<double>& b,
               boolean& isCapacityExceeded,
               int&   l,                double  t,
               double *maximumPower,    complex **gain,      
               int    NumberOfServers,  int     numberOfSectors, 
                      List<CPoint3d>&   centres,
               const  CBuilding&        buildingObject,
                      CBaseTransceiver& btsObject,
               const  SystemParameters& parameterObject,
               const  CCapacity&        capacityGrid, 
	              double*           targetSIR,
               const  mediaType&        media,
               const  boolean&          writeBestServer );
    //
    // define the objectiveFunction that needs to be minimised
    // or define the matrix for the convex optimisation constraints.
    //

    CVector<double> convertCentresToVector( List<CPoint3d>& centres );
    List<CPoint3d>& vectorToCentres( const CVector<double>& vectorcentres );
    //
    // change the list of centres into a vector of variables for the
    // optimisation routine (or vice versa).
    //

    CVector<double> steepestDescentSUMT(
                                    List<CPoint3d>&     centres,     
                                    List<CBoundingBox>& cells,
                                    CBaseTransceiver&   btsObject,
				    int                 numberOfSectors,
                              const CBuilding&          buildingObject,
                              const CCapacity&          capacityGrid,
                              const SystemParameters&   parameterObject,
                                    double              stoppingPercentage );
    //
    // define a non-linear steepest descent SUMT method which will not 
    // allow a point to cross the boundary of it's valid area
    //

    void updateLargestSIR( const CBaseTransceiver& btsObject, int i, CVector<double>& x );
    //
    // update the largest SIR value to a file
    //

    boolean testFeasibility( CVector<double>& x, int cellIndex,
                             CBoundingBox& area, CPoint3d& pt );
    //
    // test the feasibility of a point by seeing if it is still in it's cell
    // of influence (area).
    //

    void meanAbsSum( List<CRayPath>& allRayPaths, int      numberOfSectors, 
                     double          sectorAngle, complex* Rsectors );
    void meanAbsSum( List<CRayPath>& allRayPaths, int      numberOfSectors, 
                     double          sectorAngle, complex* gain,
		     complex*        Rsectors );
    void absSum( List<CRayPath>& allRayPaths, int      numberOfSectors, 
                 double          sectorAngle, complex* Rsectors );
    void absSum( List<CRayPath>& allRayPaths, int      numberOfSectors, 
                 double          sectorAngle, complex* gain,
		 complex*        Rsectors );
    void sumAbs( List<CRayPath>& allRayPaths, int      numberOfSectors, 
                 double          sectorAngle, complex* Rsectors );
    void sumAbs( List<CRayPath>& allRayPaths, int      numberOfSectors, 
                 double          sectorAngle, complex* gain,
		 complex*        Rsectors );
    //
    // multiple each ray-path by a random phase component and sum all
    // components. Then do this a number of times, and then get the mean.
    //

    double gaussianQuadrature4( double* E, int    N,       double r, 
                                double a,  double stepSize );
    //
    // gaussian quadrature rule of order 4 to evaluate the integral at a 
    // discrete set of points E[0]..E[N-1] with variable r and starting at a
    // and finishing at a+stepSize 
    //

    double trapezoidal( double* E, int    N,       double r, 
                        double a,  double stepSize );
    //
    // newton-cotes quadrature rule of order 2. This is adaptive in that
    // it keeps subdividing the interval [a,b] until an appropriate error
    // bound is reached, also it integrates from [0,+inf] and stops when
    // the integral is converged to some tolerance
    //

    double WnFunction( double* E, int N, double r, double a, double stepSize );
    //
    // probability density function for random walk
    //

    void   randomWalkMean( List<CRayPath>& allRayPaths, int numberOfSectors,
                           double sectorAngle, complex* gains, 
			   complex* Rsectors );
    void   randomWalkMean( List<CRayPath>& allRayPaths, int numberOfSectors,
                           double sectorAngle, complex* Rsectors );
    //
    // calculate a probabilty density function of a random walk. This takes
    // the incoming ray-path values at a point and adds a random phase to
    // them which is then averaged to give the mean amplitude which can then
    // be used as a better measure of the field strength (note there is an 
    // ungained and gained version of this routine)
    //

    void sort( complex* totalField, int* orderedIndices, int numberOfServers );
    //
    // the servers total field indices are reordered in descending terms
    // this is very useful for the best server calculation
    //

    double SIR( complex* totalField, int numberOfServers, int bestServer,
                double   sensitivity, double& pathLoss, double& totalInter );
    //
    // find the SIR for the current set of servers using the bestserver 
    // index specified
    //

    void setTargetSIR( const CCapacity& capacityGrid, 
                       const SystemParameters& parameterObject, 
		       double* targetSIR);
    //
    // define the target SIR at all user capacity points, this must be done
    // to see up code since it would otherwise be calculated from a file
    // many times
    //

    double besselJ0( double x );
    //
    // bessel function j0(x) including far field term for x > 3
    //

    boolean offloadServer( int **bestServerMatrix, 
                           const CCapacity& capacityGrid, double  sensitivity,
			   double *totalCapacity,  
                           int *currentBestServer, complex **totalField,
			   double *targetSIR,      double serverCapacity,
			   int numberOfServers,    int numberOfCapacityPoints,
			   int bestServer );
    //
    // offload the server if it exceeds it's capacity requirements
    //
};

#endif

