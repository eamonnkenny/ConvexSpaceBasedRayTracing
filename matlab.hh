//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// Last Modification:     November 19th 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef MATLAB_PLOT_HH
#define MATLAB_PLOT_HH

#include <iostream>
#include "building.hh"
#include "edgelist.hh"
#include "raypath.hh"
#include "path-loss.hh"

class CMatlab
//==========================================================================
// = FUNCTION 
//     matlab.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     plotting tool for generating matlab images to show building results
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
    void Story( int j, const CBuilding& buildingObject, const string& fileName );
    //
    // used to plot story j of a building.
    //

    void Floor( int j, const CBuilding& buildingObject, const string& fileName );
    //
    // used to plot floor j of a building.
    //

    void images( const CPoint3d& Tx, List<CImage>& imagelist );
    //
    // used to plot all images of a transmitter Tx.
    //

    void edgeImages( CEdgeList& edgeList );
    //
    // Added: 04/12/01 
    // used to plot all images of possible diffraction edges 
    //

    void reflections( List<CRayPath>& rayPathList, const string& fileName );
    //
    // plot of all reflection paths in a list, added 20/11/01
    //

    void fullRayPaths( List<CRayPath>& rayPathList );
    //
    // plot of full ray paths in a list, added 27/11/01
    //

    void diffractions( List<CRayPath>& rayPathList, int N );
    //
    // plot of all diffraction paths in a list, added 10/12/01
    //

    void printRays( const CBuilding&      buildingObject,
                    List<CRayPath>& rayPathList, const string& fileName );
    void printRays( ofstream& filePtr, const CBuilding& buildingObject,
                    List<CRayPath>& rayPathList, const string& fileName );
    void appendRays( const CBuilding& buildingObject,
                     List<CRayPath>& rayPathList, const string& fileName );
    //
    // complete set of information of the raypahts
    // plot of full ray paths in a list, added 12/12/01
    //

    void fieldRayPathLoss( CComplexVector **fieldStrengthArray,
                           CPathLoss&     pathLossObject,
                           const string&  fileName,
                           CPoint3d       *receiverPoints );
    void fieldRayPathLoss( CComplexVector *fieldStrengthArray,
                           CPathLoss&     pathLossObject,
                           const string&  fileName,
                           int            m );
    //
    // Added 04/01/02
    // calculate a norm for the field strength array and include the result
    // in the path-loss grid object for future plotting in matlab
    //

    void readMimoLocations( const string& fileName, CPoint3d *transmitter,
                            int& numberOfLocations );
    //
    // Added 16/04/02
    // Input the number of points and transmitter locations from a file
    //

    void Cells( List<CBoundingBox>& cells, List<CPoint3d>& centres,
                const string& cellOutputFile );
    //
    // Added 21/05/02
    // Output the extremities of the cell and the centre of capacity also
    //

    void pathLoss( CPathLoss& pathLossObject, const string& outputFileName );
    //
    // Added 22/05/02
    // the path-loss grid object can plotted in matlab
    //


    void OpenWrite( ofstream& filePtr, const string& typeOfWrite, const string& fileName );
    //
    // Added 28/08/2015
    // The output stream is opened to a file in the raypaths, optimisation, coverage or buildings
    // directory by default, or to a subdirectory of the Matlab Directory if this fails.
    //

    void OpenRead( ifstream& filePtr, const string& fileName );
    //
    // Added 28/08/2015
    // Read files from the Matlab Directory
    //
};

#endif
