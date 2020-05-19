//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    11th Janueary 2001
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef CANTENNA_DIAGRAM_HH
#define CANTENNA_DIAGRAM_HH

#include <iostream>
#include <cmath>
#include <stdlib.h>

#include "maths-utilities.hh"
#include "userconst.hh"
#include <string>
#include "ComplexVector.hh"
#include "configuration.hh"

#ifndef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport) 
#else
  #define EXPORT_CLASS_DECLSPEC class
#endif

EXPORT_CLASS_DECLSPEC CDiagram
//=========================================================================
// = FUNCTION 
//     bts.hh 
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Antenna Diagram class for horizontal and vertical directions
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//=========================================================================
{
  public:
    double    horizontalStep_;
    double    verticalStep_;
    int       numberOfHorizontalSteps_;
    int       numberOfVerticalSteps_;
    double    maximumGain_;
    double*   gain_;

  public:
    CDiagram();

    string     fullDirectoryPath( const string& fileName );
    //
    //  Obtain the full path from CProperties 
    //

    CDiagram& read( const string& FileName );

    void      setValues( const double& maximumGain, 
                         const int&    numberOfVerticalSteps,
			 const int&    numberOfHorizontalSteps,
                               double* gain );
    //
    // Read in the name of the diagram, the stepping rate in the horizontal
    // and vertical directions, and the associated gains at each step.
    //

    double gain( const double& theta, const double& phi ) const;
    //
    // calculate the vertical or horizontal gain as a specific angle. 
    // Note that interpolation is required because a discrete stepping is 
    // used. Also there is a wraparound at the last point which must be 
    // accounted for.
    //

    inline double gain( const int& i, const int& j ) const
    { return gain_[i*numberOfHorizontalSteps_+j]; }
    //
    // return the gain at a discretization point
    //

    inline double  maximumGain()      const { return maximumGain_; }
    inline double  horizontalStep()   const { return horizontalStep_; }
    inline int     horizontalNumber() const { return numberOfHorizontalSteps_; }
    inline double  verticalStep()     const { return verticalStep_; }
    inline int     verticalNumber()   const { return numberOfVerticalSteps_; }
    inline int     totalPoints()      const   
    { return numberOfVerticalSteps_ * numberOfHorizontalSteps_; }
    //
    // Return private variable information
    //

    void clear()
    {
      if ( numberOfHorizontalSteps_ > 0 && numberOfVerticalSteps_ > 0 )
      {
        delete [] gain_;
        numberOfHorizontalSteps_ = 0;
        numberOfVerticalSteps_ = 0;
      }
    }
    //
    // Clear the Diagram vectors to clear all memory allocations
    //

    CDiagram& operator=( const CDiagram& otherDiagram );
    //
    // assignment operator overload
    //

    CComplexVector hertzianDipole( const double& r,     const CPoint3d& x_l,
                                    const CPoint3d& y_l, const CPoint3d& z_l,
                                    const double& theta, const double& phi, 
				    const double& ki ) const;
    CComplexVector imageDipole( const double& r,     const CPoint3d& x_l,
                                    const CPoint3d& y_l, const CPoint3d& z_l,
                                    const double& theta, const double& phi, 
				    const double& ki ) const;
    CComplexVector omniDirectional( const double& r,     const CPoint3d& x_l,
                                    const CPoint3d& y_l, const CPoint3d& z_l,
                                    const double& theta, const double& phi, 
				    const double& ki ) const;
    CComplexVector imageOmni( const double& r,   const double& theta,
                              const double& phi, const double& ki ) const;
    //
    // define a field using a hertzian dipole
    //

    complex gaussianBeam( const double& theta, const double& phi ) const;
    //
    // create a gaussian beam 
    //

    double random( const int& i ) const;
    //
    // returns a random number between 0 and 1 using i to generating the seed
    //

    ~CDiagram()
    {
      clear();
    }
};

#endif 
