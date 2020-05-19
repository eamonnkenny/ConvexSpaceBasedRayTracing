//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// Last Modification:     November 19th 2000
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "matlab.hh"

void CMatlab::Story( int j, const CBuilding& buildingObject, const string& fileName )
{
  int      i, k, N;
  ofstream filePtr;
  string  apertureName;

  if ( j < buildingObject.numberOfStories_ )
  {
    N = 0;
    OpenWrite( filePtr, "building", fileName );
  
    for ( i = 0; i < buildingObject.storyArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.storyArray_[j].convexSpaceIndex_[i];
      if ( k < 0 )
      {
        cout << "Problem in CMatlab::Story, k < 0, occurrence 1" << endl;
        exit(-1);
      }
      if ( buildingObject.convexArray_[k].conductivity_ != 0.0
         && buildingObject.convexArray_[k].permittivity_ != 2.32 ) N++;
    }
  
    filePtr << N << endl;

    for ( i = 0; i < buildingObject.storyArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.storyArray_[j].convexSpaceIndex_[i];
      if (k < 0) 
      {
        cout << "Problem in CMatlab::Story, k < 0, occurrence 2" << endl;
        exit(-1);
      }
      if ( buildingObject.convexArray_[k].conductivity_ != 0.0
         && buildingObject.convexArray_[k].permittivity_ != 2.32 ) 
        filePtr << buildingObject.convexArray_[k];
    }
    cout << "N=        " << N << endl;
  
    filePtr.close();
  
    apertureName = fileName;
    apertureName += "a";
  
    N = 0;
    OpenWrite( filePtr, "building", apertureName );
  
    for ( i = 0; i < buildingObject.storyArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.storyArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ != 0
         && buildingObject.convexArray_[k].permittivity_ == 2.32 ) N++;
    }
  
    filePtr << N << endl;
  
    for ( i = 0; i < buildingObject.storyArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.storyArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ != 0
         && buildingObject.convexArray_[k].permittivity_ == 2.32 )
        filePtr << buildingObject.convexArray_[k];
    }
  
    filePtr.close();

    apertureName = fileName;
    apertureName += "b";
  
    N = 0;
    OpenWrite( filePtr, "building", apertureName );
  
    for ( i = 0; i < buildingObject.storyArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.storyArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ == 0.0 ) N++;
    }
  
    filePtr << N << endl;
  
    for ( i = 0; i < buildingObject.storyArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.storyArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ == 0.0 )
        filePtr << buildingObject.convexArray_[k].centre() << endl;
    }
  
    filePtr.close();

    apertureName = "convexs.txt";
  
    N = 0;
    OpenWrite( filePtr, "building", apertureName );
  
    for ( i = 0; i < buildingObject.storyArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.storyArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ == 0.0 ) N++;
    }
  
    filePtr << N << endl;
  
    for ( i = 0; i < buildingObject.storyArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.storyArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ == 0.0 )
        filePtr << buildingObject.convexArray_[k];
    }
  
    filePtr.close();
  }
}
void CMatlab::OpenRead( ifstream& filePtr, const string& fileName )
{
  string fullPathToFile;

  fullPathToFile = CProperties::matlabBuildingDirectory + "/" + fileName;
  cout << "Opening File for Reading: " << fullPathToFile << endl;
  filePtr.open( fullPathToFile.c_str() );
}

void CMatlab::OpenWrite( ofstream& filePtr, const string& typeOfWrite, const string& fileName )
{
  string fullPathToFile;

  if ( typeOfWrite.compare("building" ) == 0 )
  {
    fullPathToFile = CProperties::matlabBuildingDirectory + "/" + fileName;
  }
  else if ( typeOfWrite.compare("raypaths" ) == 0 )
  {
    fullPathToFile = CProperties::matlabRayPathsDirectory + "/" + fileName;
  }
  else if ( typeOfWrite.compare("coverage" ) == 0 )
  {
    fullPathToFile = CProperties::matlabCoverageDirectory + "/" + fileName;
  }
  else if ( typeOfWrite.compare("optimisation" ) == 0 )
  {
    fullPathToFile = CProperties::matlabOptimisationDirectory + "/" + fileName;
  }
  else if ( typeOfWrite.compare("thesis" ) == 0 )
  {
    fullPathToFile = CProperties::matlabThesisDirectory + "/" + fileName;
  }
  else
  {
    if ( typeOfWrite.compare("" ) == 0 )
      fullPathToFile = CProperties::matlabDirectory + "/" + fileName;
    else
      fullPathToFile = CProperties::matlabDirectory + "/" + typeOfWrite + "/" + fileName;
  }

  //cout << "Opening File for Write: " << fullPathToFile << endl;
  filePtr.open( fullPathToFile.c_str() );
}

void CMatlab::Floor( int j, const CBuilding& buildingObject, const string& fileName )
{
  int      i, k, N;
  ofstream filePtr;
  string  apertureName;

  if ( j < buildingObject.numberOfFloors_+1 )
  {
    N = 0;
    OpenWrite( filePtr, "building", fileName );
  
    for ( i = 0; i < buildingObject.floorArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.floorArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ != 0
         && buildingObject.convexArray_[k].permittivity_ != 2.32 ) N++;
    }
  
    filePtr << N << endl;
  
    for ( i = 0; i < buildingObject.floorArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.floorArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ != 0
         && buildingObject.convexArray_[k].permittivity_ != 2.32 ) 
        filePtr << buildingObject.convexArray_[k];
    }
  
    filePtr.close();
  
    apertureName = fileName;
    apertureName += "a";
  
    N = 0;
    OpenWrite( filePtr, "building", apertureName );
  
    for ( i = 0; i < buildingObject.floorArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.floorArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ != 0
         && buildingObject.convexArray_[k].permittivity_ == 2.32 ) N++;
    }
  
    filePtr << N << endl;
  
    for ( i = 0; i < buildingObject.floorArray_[j].numberOfConvexs_; i++ )
    {
      k = buildingObject.floorArray_[j].convexSpaceIndex_[i];
      if ( buildingObject.convexArray_[k].conductivity_ != 0
         && buildingObject.convexArray_[k].permittivity_ == 2.32 )
        filePtr << buildingObject.convexArray_[k];
    }
  
    filePtr.close();
  }
}

void CMatlab::images( const CPoint3d& Tx, List<CImage>& imagelist )
{
  int      i;
  ofstream filePtr;

  OpenWrite( filePtr, "raypaths", "images.txt" );

  filePtr << Tx << endl;

  filePtr << imagelist.size() << endl;
  for ( i = 0; i < imagelist.size(); i++ )
  {
    filePtr << imagelist.GetItem(i).point_ << endl;
  }
  filePtr.close();
}

void CMatlab::edgeImages( CEdgeList& edgeList )
{
  int      i;
  ofstream filePtr;

  OpenWrite( filePtr, "raypaths", "edges.txt" );

  filePtr << edgeList.listOfEdges.size() << endl;

  for ( i = 0; i < edgeList.listOfEdges.size(); i++ )
  {
    filePtr << "[ " << edgeList.listOfEdges.GetItem(i).edge_.start() << " ] [ " 
                    << edgeList.listOfEdges.GetItem(i).edge_.end()   << " ]"  << endl;
  }

  filePtr.close();
}

void CMatlab::reflections( List<CRayPath>& rayPathList, const string& fileName )
{
  int              i, j;
  ofstream         filePtr;
  CRayPath         rayPath;
  List<CPointInfo> pointsInRayPath; 

  OpenWrite( filePtr, "raypaths", fileName );

  filePtr << rayPathList.size() << endl;
  for ( i = 0; i < rayPathList.size(); i++ )
  {
    rayPath = rayPathList.GetItem(i);
    filePtr << rayPath.numberOfReflections() + 2 << endl;

    pointsInRayPath = rayPath.pointList;

    filePtr << pointsInRayPath.first().node << endl;

    for ( j = 1; j < pointsInRayPath.size() - 1; j++ )
    {
      if ( pointsInRayPath.GetItem(j).contact == REFLECTION )
        filePtr << pointsInRayPath.GetItem(j).node << endl;
    }

    filePtr << pointsInRayPath.last().node << endl;
  }
  filePtr.close();
  pointsInRayPath.clear();
}

void CMatlab::fullRayPaths( List<CRayPath>& rayPathList )
{
  int              i, j;
  ofstream         filePtr;
  CRayPath         rayPath;
  List<CPointInfo> pointsInRayPath; 

  OpenWrite( filePtr, "raypaths", "fullrays.txt" );

  filePtr << rayPathList.size() << endl;
  for ( i = 0; i < rayPathList.size(); i++ )
  {
    rayPath         = rayPathList.GetItem(i);
    pointsInRayPath = rayPath.pointList;

    if ( rayPath.numberOfReflections() <= 2 )
    {
      filePtr << pointsInRayPath.size() << endl;

      for ( j = 0; j < pointsInRayPath.size(); j++ )
        filePtr << pointsInRayPath.GetItem(j).node << endl;
    }
  }
  filePtr.close();
  pointsInRayPath.clear();
}

void CMatlab::diffractions( List<CRayPath>& rayPathList, int N )
{
  int              i, j;
  ofstream         filePtr;
  CRayPath         rayPath;
  List<CPointInfo> pointsInRayPath; 

  // added: 10/12/01
  OpenWrite( filePtr, "raypaths", "diffraction.txt" );

  filePtr << rayPathList.size() - N << endl;
  for ( i = N; i < rayPathList.size(); i++ )
  {
    rayPath = rayPathList.GetItem(i);

    //if ( rayPath.numberOfDiffractions() > 0 )
    {
      pointsInRayPath = rayPath.pointList;
      filePtr << pointsInRayPath.size() << endl;
  
      filePtr << pointsInRayPath.first().node << endl;
  
      for ( j = 1; j < pointsInRayPath.size() - 1; j++ )
      {
        filePtr << pointsInRayPath.GetItem(j).node << endl;
      }

      filePtr << pointsInRayPath.last().node << endl;
    }
  }
  filePtr.close();
  pointsInRayPath.clear();
}

void CMatlab::printRays( const CBuilding&      buildingObject,
                               List<CRayPath>& rayPathList,
                               const string&   fileName )
{
  ofstream filePtr;

  OpenWrite( filePtr, "raypaths", fileName );
  printRays( filePtr, buildingObject, rayPathList, fileName );
  filePtr.close();
}

void CMatlab::printRays(       ofstream&       filePtr,
                         const CBuilding&      buildingObject,
                               List<CRayPath>& rayPathList,
                               const string&   fileName )
{
  int              i, j, b1, b2;
  CRayPath         rayPath;
  CConvex          *aConvex;
  List<CPointInfo> pointsInRayPath; 

  filePtr << "NumberOfRays: " << rayPathList.size() << endl;
  for ( i = 0; i < rayPathList.size(); i++ )
  {
    rayPath         = rayPathList.GetItem(i);
    pointsInRayPath = rayPath.pointList;

    filePtr << "ray number: " << i << " T: " << rayPath.numberOfTransmissions()
            << " R: " << rayPath.numberOfReflections() 
            << " D: " << rayPath.numberOfDiffractions() << endl;

    for ( j = 0; j < pointsInRayPath.size(); j++ )
    {
      aConvex =
        &buildingObject.convexArray_[pointsInRayPath.GetItem(j).convexNumber];

      if ( pointsInRayPath.GetItem(j).contact == DIFFRACTION )
      {
        filePtr << " DIFFRACTION " << endl;
        filePtr << pointsInRayPath.GetItem(j).node << endl;
        b1 = pointsInRayPath.GetItem(j).boundaryNumber1;
        b2 = pointsInRayPath.GetItem(j).boundaryNumber2;
	filePtr << aConvex->facet[b1].equation_.outwardNormal_ << endl;
	filePtr << aConvex->facet[b2].equation_.outwardNormal_ << endl;
        //filePtr << aConvex->permittivity_ << " " << aConvex->permeability_ 
        //        << " " << aConvex->conductivity_ << endl;
      }
      else if ( pointsInRayPath.GetItem(j).contact == REFLECTION )
      {
        filePtr << " REFLECTION " << endl;
        filePtr << pointsInRayPath.GetItem(j).node << endl;
        b1 = pointsInRayPath.GetItem(j).boundaryNumber1;
	filePtr << aConvex->facet[b1].equation_.outwardNormal_ << endl;
        //filePtr << aConvex->permittivity_ << " " << aConvex->permeability_ 
        //        << " " << aConvex->conductivity_ << endl;
      }
      else if ( pointsInRayPath.GetItem(j).contact == TRANSMISSION )
      {
        filePtr << " TRANSMISSION " << endl;
        filePtr << pointsInRayPath.GetItem(j).node << endl;
        b1 = pointsInRayPath.GetItem(j).boundaryNumber1;
	filePtr << aConvex->facet[b1].equation_.outwardNormal_ << endl;
        //filePtr << aConvex->permittivity_ << " " << aConvex->permeability_ 
        //        << " " << aConvex->conductivity_ << endl;
      }
      else if ( pointsInRayPath.GetItem(j).contact == TRANSMITTER )
      {
        filePtr << " TRANSMITTER " << endl;
        filePtr << pointsInRayPath.GetItem(j).node << endl;
      }
      else if ( pointsInRayPath.GetItem(j).contact == RECEIVER )
      {
        filePtr << " RECEIVER " << endl;
        filePtr << pointsInRayPath.GetItem(j).node << endl;
      }


    }
    filePtr << rayPath.fieldStrength_[0] << endl;
    filePtr << rayPath.fieldStrength_[1] << endl;
    filePtr << rayPath.fieldStrength_[2] << endl;
    pointsInRayPath.clear();
  }

}

void CMatlab::readMimoLocations( const string& fileName,
                                 CPoint3d *transmitter,
                                 int&      numberOfLocations )
{
  ifstream filePtr;
  double   x, y, z;
  int      i;

  OpenRead( filePtr, fileName );

  filePtr >> numberOfLocations;

  for ( i = 0; i < numberOfLocations; i++ )
  {
    filePtr >> x;
    filePtr >> y;
    filePtr >> z;
    transmitter[i] = CPoint3d( x, y, z );
  }

  filePtr.close();
}

void CMatlab::fieldRayPathLoss( CComplexVector *fieldStrength,
                                CPathLoss&     pathLossObject, 
                                const string&  fileName,
                                int            m )
{
  int      i, j, k;
  double   x, y, z;
  ofstream filePtr;
  string  fileString;
  CIntrinsic intrinsicObject;
  char     s[5];

  intrinsicObject.itoa( m, s );
  s[1]        = '\0';
  fileString  = fileName;
  fileString += s;

  OpenWrite( filePtr, "coverage", fileName );

  filePtr << pathLossObject.xSouthWest() << endl;
  filePtr << pathLossObject.ySouthWest() << endl;
  filePtr << pathLossObject.xStepSize() << endl;
  filePtr << pathLossObject.yStepSize() << endl;
  filePtr << pathLossObject.xNumberOfPoints() << endl;
  filePtr << pathLossObject.yNumberOfPoints() << endl;

  k = 0;
  for ( j = 0; j < pathLossObject.yNumberOfPoints(); j++ )
  {
    for ( i = 0; i < pathLossObject.xNumberOfPoints(); i++ )
    {
      x = pathLossObject.xSouthWest() + double(i) * pathLossObject.xStepSize();
      y = pathLossObject.ySouthWest() + double(j) * pathLossObject.yStepSize();
      z = 2.0;
      pathLossObject.setPathLoss( fieldStrength[k].euclideanNorm(), i, j );
      filePtr << setprecision(8) 
              << x << "  " << y << "  " << z << "  " 
              << cabs(fieldStrength[k].z) << endl;
      k++;
    }
  }

  filePtr.close();
}

void CMatlab::fieldRayPathLoss( CComplexVector **fieldStrength,
                                CPathLoss&     pathLossObject, 
                                const string&  fileName,
                                CPoint3d*      receiverPoints )
{
  int      i, j, k, l;
  ofstream filePtr;

  OpenWrite( filePtr, "coverage", fileName );

  filePtr << pathLossObject.xSouthWest() << endl;
  filePtr << pathLossObject.ySouthWest() << endl;
  filePtr << pathLossObject.xStepSize() << endl;
  filePtr << pathLossObject.yStepSize() << endl;
  filePtr << pathLossObject.xNumberOfPoints() << endl;
  filePtr << pathLossObject.yNumberOfPoints() << endl;

  k = 0;
  for ( j = 0; j < pathLossObject.yNumberOfPoints(); j++ )
  {
    for ( i = 0; i < pathLossObject.xNumberOfPoints(); i++ )
    {
      pathLossObject.setPathLoss( fieldStrength[k][2].euclideanNorm(), i, j );
      filePtr << setprecision(8) << receiverPoints[k] << "  ";
      for ( l = 0; l < 3; l++ )
        filePtr << fieldStrength[k][l].x << "  "
                << fieldStrength[k][l].y << "  "
                << fieldStrength[k][l].z << "  ";
      filePtr << endl;
      k++;
    }
  }

  filePtr.close();
}

void CMatlab::pathLoss( CPathLoss& pathLossObject, const string& fileName )
                        
{
  int        i, j;
  ofstream   filePtr;

  OpenWrite( filePtr, "coverage", fileName );

  filePtr << pathLossObject.xSouthWest() << endl;
  filePtr << pathLossObject.ySouthWest() << endl;
  filePtr << pathLossObject.xStepSize() << endl;
  filePtr << pathLossObject.yStepSize() << endl;
  filePtr << pathLossObject.xNumberOfPoints() << endl;
  filePtr << pathLossObject.yNumberOfPoints() << endl;

  for ( j = 0; j < pathLossObject.yNumberOfPoints(); j++ )
    for ( i = 0; i < pathLossObject.xNumberOfPoints(); i++ )
      filePtr << setprecision(8) << pathLossObject.getPathLoss(i,j) << endl;

  filePtr.close();
}


void CMatlab::Cells( List<CBoundingBox>& cells, List<CPoint3d>& centres,
                     const string& cellOutputFile )
{
  int       i;
  ofstream  filePtr;

  // added: 21/05/02
  OpenWrite( filePtr, "", cellOutputFile );

  filePtr << cells.size() << endl;
  for ( i = 0; i < cells.size(); i++ )
  {
    filePtr << cells.GetItem(i) << endl;
    filePtr << centres.GetItem(i) << endl << endl;
  }
  filePtr.close();
}

