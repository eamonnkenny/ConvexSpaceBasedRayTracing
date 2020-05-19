//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Derek Bell
// Project Manager:       Dr. Peter J. Cullen
// First Modification:    January 2002
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "reloader.hh"

string CReloadBuilding::fullDirectoryPath( const string& FileName )
{
  string fullPath;
  fullPath = CProperties::buildingDirectory + "/" + FileName;
  cout << fullPath << endl;
  return fullPath;
}


CReloadBuilding::CReloadBuilding( char* buildingFileName, 
                                  char* buildingSaveFileName )
                                  
{
  boolean    mustConnectBuilding = true;
  ifstream   savedBuildingFile;
  string     fullBuildingFileName, roomFileName;
  CIntrinsic intrinsicObject;
  string     savedBuildingFileName;
  char       c;

  fullBuildingFileName  = fullDirectoryPath( buildingFileName );
  roomFileName          = fullDirectoryPath( buildingFileName );
  savedBuildingFileName = fullDirectoryPath( buildingSaveFileName );

  fullBuildingFileName += ".dat";
  roomFileName += ".roo";
  savedBuildingFileName += ".sav";

  if ( intrinsicObject.CheckFileExists( savedBuildingFileName ) == true )
  {
    savedBuildingFile.open( savedBuildingFileName.c_str() );
    savedBuildingFile.get(c);

    if ( c != EOF  )
    {
      cout << "Reading Building: " << savedBuildingFile << endl;
      readTotalConvexes( savedBuildingFile ); 
      savedBuildingFile.close();
  
      building_.getNumberOfConvexs( fullBuildingFileName );
      cout << "Convex count: " << totalConvexes_ << building_.numberOfConvexs_ << endl;
      if ( totalConvexes_ == building_.numberOfConvexs_ )
      {
        cout << "Number Of Convexes are same, read from saved file" << endl;
        readBuilding( savedBuildingFileName );
        mustConnectBuilding = false;
      }
      else
      {
        cout << "Building connection previous formed, but Convexes changed" 
             << endl;
      }
    }
    
    if ( mustConnectBuilding == true )
    {
      building_.connectConvexsAndRooms( fullBuildingFileName, roomFileName );
      setTotalConvexes( building_.numberOfConvexs_ );
      //setBuilding( build );
  
      cerr << "Writing save file for quicker computations on next run" << endl;
      writeBuilding( savedBuildingFileName );
    }
  }
  else
  {
    cerr << "Save File does not exist so creating it now" << endl;
    building_.connectConvexsAndRooms( fullBuildingFileName, roomFileName );
    setTotalConvexes( building_.numberOfConvexs_ );
    //setBuilding( build );
  
    cerr << "Writing save file for quicker computations on next run" << endl;
    writeBuilding( savedBuildingFileName );
  }
  
  
}

CReloadBuilding::CReloadBuilding( const string& buildingFileName ) 
{
  boolean  mustConnectBuilding = true;
  ifstream savedBuildingFile;
  string   fullBuildingFileName, roomFileName;
  string   savedBuildingFileName;
  CIntrinsic intrinsicObject;
  char c;

  cout << "Developed in Informatic 2000 Programme Project: STIWRO" << endl;
  cout << "COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, "
       << "IRELAND." << endl << "All rights reserved" << endl;
  fullBuildingFileName  = fullDirectoryPath( buildingFileName ) + ".dat";
  roomFileName          = fullDirectoryPath( buildingFileName ) + ".roo";
  savedBuildingFileName = fullDirectoryPath( buildingFileName ) + ".sav";


  savedBuildingFile.open( savedBuildingFileName.c_str() );
  savedBuildingFile.get(c);

  if ( intrinsicObject.CheckFileExists( savedBuildingFileName ) == true )
  {
    if ( c != EOF  )
    {
      readTotalConvexes( savedBuildingFile ); 
      savedBuildingFile.close();
  
      building_.getNumberOfConvexs(fullBuildingFileName);
      if ( totalConvexes_ == building_.numberOfConvexs_ )
      {
        cout << "Total Convexes remained the same, read from saved file" << endl;
        readBuilding( savedBuildingFileName );
        mustConnectBuilding = false;
      }
      else
      {
        cout << "Building connection previous formed, but transmitter moved" 
             << endl;
      }
    }
  
    if ( mustConnectBuilding == true )
    {
      building_.connectConvexsAndRooms( fullBuildingFileName, roomFileName );
      setTotalConvexes( building_.numberOfConvexs_ );
      //setBuilding( build );
  
      cerr << "Writing save file for quicker computations on next run" << endl;
      writeBuilding( savedBuildingFileName );
    }
  }
  else
  {
    cerr << "Building connectivity does not yet exist, create a .sav file" << endl;
    building_.connectConvexsAndRooms( fullBuildingFileName, roomFileName );
    setTotalConvexes( building_.numberOfConvexs_ );
    //setBuilding( build );
  
    cerr << "Writing save file for quicker computations on next run" << endl;
    writeBuilding( savedBuildingFileName );
  }
}

CReloadBuilding::CReloadBuilding()
{
  totalConvexes_ = 0.0;
  building_ = CBuilding();
}

CReloadBuilding::CReloadBuilding( const double& total, const CBuilding& b)
{
  totalConvexes_ = total;
  building_ = CBuilding();
  building_ = b;
}

void CReloadBuilding::setTotalConvexes( const double& total )
{
  totalConvexes_ = total;
}

double CReloadBuilding::getTotalConvexes()
{
  return totalConvexes_;
}

void CReloadBuilding::setBuilding( const CBuilding& b )
{
  building_ = CBuilding();
  building_ = b;
}

CBuilding CReloadBuilding::getBuilding()
{
  return building_;
}

void CReloadBuilding::writeTotalConvexes(ofstream& ostr)
{
  ostr << "TotalConvexes" << endl;
  ostr << totalConvexes_ << endl;
}

void CReloadBuilding::readTotalConvexes(ifstream& ifstr)
{
  string temp = "\0";
  ifstr >> temp;
  ifstr >> totalConvexes_;
  cout << "read total convexs: " << totalConvexes_ << endl;
}

void CReloadBuilding::writeCBoundary(ostream& ostr, int convex, int facet)
{
  int i;
  ostr << "CBoundary" << endl;
  // Write out corners
  for(i=0;i<4;i++)
  {
    ostr << building_.convexArray_[convex].facet[facet].vertex_[i].x << endl;
    ostr << building_.convexArray_[convex].facet[facet].vertex_[i].y << endl;
    ostr << building_.convexArray_[convex].facet[facet].vertex_[i].z << endl;
  }
  // Equation of plane
  ostr << building_.convexArray_[convex].facet[facet].equation_.outwardNormal_.x << endl;
  ostr << building_.convexArray_[convex].facet[facet].equation_.outwardNormal_.y << endl;
  ostr << building_.convexArray_[convex].facet[facet].equation_.outwardNormal_.z << endl;

  ostr << building_.convexArray_[convex].facet[facet].equation_.D_ << endl;

  ostr << building_.convexArray_[convex].facet[facet].numberOfAdjacentConvexs_ << endl;

  for(i=0;i<building_.convexArray_[convex].facet[facet].numberOfAdjacentConvexs_;i++)
  {
    ostr << building_.convexArray_[convex].facet[facet].adjacentConvexIndex_[i] << endl;
  }
}

void CReloadBuilding::readCBoundary(ifstream& ifstr,int convex,int facet)
{
  string temp = "\0";
  int i;

  ifstr >> temp; // Read "CBoundary" string

  for(i=0;i<4;i++)
  {
    ifstr >> building_.convexArray_[convex].facet[facet].vertex_[i].x;
    ifstr >> building_.convexArray_[convex].facet[facet].vertex_[i].y;
    ifstr >> building_.convexArray_[convex].facet[facet].vertex_[i].z;
    //ifstr.get(c);
  }
  ifstr >> building_.convexArray_[convex].facet[facet].equation_.outwardNormal_.x;
  ifstr >> building_.convexArray_[convex].facet[facet].equation_.outwardNormal_.y;
  ifstr >> building_.convexArray_[convex].facet[facet].equation_.outwardNormal_.z;

  ifstr >> building_.convexArray_[convex].facet[facet].equation_.D_;


  ifstr >> building_.convexArray_[convex].facet[facet].numberOfAdjacentConvexs_;
  // Allocate space

  building_.convexArray_[convex].facet[facet].adjacentConvexIndex_=objectStorage_.CreateIntegerVector(building_.convexArray_[convex].facet[facet].numberOfAdjacentConvexs_);
  
  for(i=0;i<building_.convexArray_[convex].facet[facet].numberOfAdjacentConvexs_;i++)
  {
    ifstr >> building_.convexArray_[convex].facet[facet].adjacentConvexIndex_[i];
  }
}

void CReloadBuilding::writeCPlane( ostream& ostr, const CPlane& p)
{
  ostr << "CPlane" << endl;

  writeCPoint3d(ostr,p.outwardNormal_);
  ostr << p.D_ << endl;
}

void CReloadBuilding::readCPlane( ifstream& ifstr, CPlane& cp )
{
  string temp = "\0";

  ifstr >> temp;
  //ifstr >> result.outwardNormal_;
  readCPoint3d(ifstr,cp.outwardNormal_);
  ifstr >> cp.D_;
}

void CReloadBuilding::writeCRoom( ostream& ostr, int roomnum )
{
  int i;
  ostr << "CRoom" << endl;
  // Write Convex space indices
  ostr << building_.roomArray_[roomnum].numberOfConvexs_ << endl;

  for(i=0;i<building_.roomArray_[roomnum].numberOfConvexs_;i++)
    ostr << building_.roomArray_[roomnum].convexSpaceIndex_[i] << endl;

  // Write indices of rooms below
  ostr << building_.roomArray_[roomnum].numberOfRoomsBelow_ << endl;
  for(i=0;i<building_.roomArray_[roomnum].numberOfRoomsBelow_;i++)
    ostr << building_.roomArray_[roomnum].roomsBelow_[i] << endl;

  // Write indices of rooms adjacent
  ostr << building_.roomArray_[roomnum].numberOfRoomsAdjacent_ << endl;
  for(i=0;i<building_.roomArray_[roomnum].numberOfRoomsAdjacent_;i++)
    ostr << building_.roomArray_[roomnum].roomsAdjacent_[i] << endl;

  // Write indices of rooms above
  ostr << building_.roomArray_[roomnum].numberOfRoomsAbove_ << endl;
  for(i=0;i<building_.roomArray_[roomnum].numberOfRoomsAbove_;i++)
    ostr << building_.roomArray_[roomnum].roomsAbove_[i] << endl;

  // Centroid
  
  ostr << building_.roomArray_[roomnum].centroid_.x << endl;
  ostr << building_.roomArray_[roomnum].centroid_.y << endl;
  ostr << building_.roomArray_[roomnum].centroid_.z << endl;
}

void CReloadBuilding::readCRoom( ifstream& ifstr,int roomnum )
{
  string temp = "\0";
  int i;

  ifstr >> temp;
  // Get Convex space indices
  ifstr >> building_.roomArray_[roomnum].numberOfConvexs_;

  if(building_.roomArray_[roomnum].numberOfConvexs_>0)
    building_.roomArray_[roomnum].convexSpaceIndex_=objectStorage_.CreateIntegerVector(building_.roomArray_[roomnum].numberOfConvexs_);

  for(i=0;i<building_.roomArray_[roomnum].numberOfConvexs_;i++)
  {
    ifstr >> building_.roomArray_[roomnum].convexSpaceIndex_[i];
  }

  ifstr >> building_.roomArray_[roomnum].numberOfRoomsBelow_;

  if ( building_.roomArray_[roomnum].numberOfRoomsBelow_ > 0 )
    building_.roomArray_[roomnum].roomsBelow_=objectStorage_.CreateIntegerVector(building_.roomArray_[roomnum].numberOfRoomsBelow_);

  for(i=0;i<building_.roomArray_[roomnum].numberOfRoomsBelow_;i++)
  {
    ifstr >> building_.roomArray_[roomnum].roomsBelow_[i];
  }

  ifstr >> building_.roomArray_[roomnum].numberOfRoomsAdjacent_;

  if(building_.roomArray_[roomnum].numberOfRoomsAdjacent_>0)
    building_.roomArray_[roomnum].roomsAdjacent_ = objectStorage_.CreateIntegerVector(building_.roomArray_[roomnum].numberOfRoomsAdjacent_);

  for(i=0;i<building_.roomArray_[roomnum].numberOfRoomsAdjacent_;i++)
  {
    ifstr >> building_.roomArray_[roomnum].roomsAdjacent_[i];
  }

  ifstr >> building_.roomArray_[roomnum].numberOfRoomsAbove_;

  if(building_.roomArray_[roomnum].numberOfRoomsAbove_>0)
    building_.roomArray_[roomnum].roomsAbove_ = objectStorage_.CreateIntegerVector(building_.roomArray_[roomnum].numberOfRoomsAbove_);

  for(i=0;i<building_.roomArray_[roomnum].numberOfRoomsAbove_;i++)
  {
    ifstr >> building_.roomArray_[roomnum].roomsAbove_[i];
  }

  ifstr >> building_.roomArray_[roomnum].centroid_.x;
  ifstr >> building_.roomArray_[roomnum].centroid_.y;
  ifstr >> building_.roomArray_[roomnum].centroid_.z;
}

void CReloadBuilding::writeCStory( ostream& ostr, int story )
{
  int i;

  ostr << "CStory" << endl;
  ostr << building_.storyArray_[story].numberOfRooms_ << endl;
  for(i=0;i<building_.storyArray_[story].numberOfRooms_;i++)
    ostr << building_.storyArray_[story].roomIndex_[i] << endl;
  ostr << building_.storyArray_[story].numberOfConvexs_ << endl;
  for(i=0;i<building_.storyArray_[story].numberOfConvexs_;i++)
    ostr << building_.storyArray_[story].convexSpaceIndex_[i] << endl;
}

void CReloadBuilding::writeCFloor( ostream& ostr,int floor )
{
  int i;

  ostr << "CFloor" << endl;
  ostr << (building_.floorArray_[floor]).numberOfConvexs_ << endl;
  for(i=0;i<(building_.floorArray_[floor]).numberOfConvexs_;i++)
    ostr << (building_.floorArray_[floor]).convexSpaceIndex_[i] << endl;
}

void CReloadBuilding::readCFloor( ifstream& ifstr,int floor )
{
  string temp="\0";
  int i;

  ifstr >> temp;
  ifstr >> building_.floorArray_[floor].numberOfConvexs_;
  building_.floorArray_[floor].convexSpaceIndex_ = objectStorage_.CreateIntegerVector(building_.floorArray_[floor].numberOfConvexs_);

  for(i=0;i < building_.floorArray_[floor].numberOfConvexs_;i++)
  {
    ifstr >> building_.floorArray_[floor].convexSpaceIndex_[i];

  }

}

void CReloadBuilding::writeRoof( ostream& ostr )
{
    int i;
   ostr << "CFloor" << endl;
  ostr << (building_.roof).numberOfConvexs_ << endl;
  for(i=0;i<(building_.roof).numberOfConvexs_;i++)
    ostr << (building_.roof).convexSpaceIndex_[i] << endl;
}

void CReloadBuilding::readRoof( ifstream& ifstr )
{
  int i;
  string temp="\0";

  ifstr >> temp;
  ifstr >> building_.roof.numberOfConvexs_;
  building_.roof.convexSpaceIndex_ = objectStorage_.CreateIntegerVector(building_.roof.numberOfConvexs_);

  for(i=0;i < building_.roof.numberOfConvexs_;i++)
  {
    ifstr >> building_.roof.convexSpaceIndex_[i];
  }
}

void CReloadBuilding::readCStory( ifstream& ifstr,int story )
{
  int i;
  string temp="\0";

  building_.storyArray_[story]=CStory();
  ifstr >> temp;
  ifstr >> building_.storyArray_[story].numberOfRooms_;
  building_.storyArray_[story].roomIndex_=objectStorage_.CreateIntegerVector((building_.storyArray_[story]).numberOfRooms_);
  for(i=0;i<building_.storyArray_[story].numberOfRooms_;i++)
  {
    ifstr >> building_.storyArray_[story].roomIndex_[i];
  }
  
    ifstr >> building_.storyArray_[story].numberOfConvexs_;
  building_.storyArray_[story].convexSpaceIndex_=objectStorage_.CreateIntegerVector(building_.storyArray_[story].numberOfConvexs_);
  for(i=0;i<building_.storyArray_[story].numberOfConvexs_;i++)
  {
    ifstr >> building_.storyArray_[story].convexSpaceIndex_[i];
  }
}

void CReloadBuilding::writeBuilding(string outputFileName)
{
  ofstream oFileStream;
  int i;

  oFileStream.open(outputFileName.c_str());

  writeTotalConvexes(oFileStream);
  // Write number of buildings out
  oFileStream << building_.numberOfBuildings_ << endl;
  // Write storeys out
  oFileStream << building_.numberOfStories_ << endl;
  for(i=0;i<building_.numberOfStories_;i++)
    writeCStory(oFileStream,i);

  // Write floors out
  oFileStream << building_.numberOfFloors_ << endl;
  for(i=0;i<building_.numberOfFloors_;i++)
    writeCFloor(oFileStream,i);
  // Write roof out
  writeRoof(oFileStream);
  // Write convex space details
  oFileStream << building_.numberOfConvexs_ << endl;
  for(i=0;i<building_.numberOfConvexs_;i++)
    writeCConvex(oFileStream,i);
  // Write room details
  oFileStream << building_.numberOfRooms_ << endl;
  for(i=0;i<building_.numberOfRooms_;i++)
    writeCRoom(oFileStream,i);

  writeBoundingBox( oFileStream );
  oFileStream.close();
}

void CReloadBuilding::readBuilding(const string& inputFileName)
{
  ifstream fileStream;
  int i;

  fileStream.open(inputFileName.c_str());
  // Read the transmitter in
  readTotalConvexes(fileStream);
  fileStream >> building_.numberOfBuildings_;
  
  // Read Stories in
  fileStream >> building_.numberOfStories_;
  building_.storyArray_=objectStorage_.CreateStoryVector(building_.numberOfStories_);
  for(i=0;i<building_.numberOfStories_;i++)
  {
    readCStory(fileStream,i);
  }
  // Read Floors in
  fileStream >> building_.numberOfFloors_;

  building_.floorArray_=objectStorage_.CreateFloorVector(building_.numberOfFloors_);
  for(i=0;i<building_.numberOfFloors_;i++)
  {
    readCFloor(fileStream,i);
  }

  readRoof(fileStream);
  // Read Convex space details in
  fileStream >> building_.numberOfConvexs_;
  building_.convexArray_=objectStorage_.CreateConvexVector(building_.numberOfConvexs_);

  for(i=0;i<building_.numberOfConvexs_;i++)
  {
    readCConvex(fileStream,i);
  }
  // Read room details
  fileStream >> building_.numberOfRooms_;
  building_.roomArray_=objectStorage_.CreateRoomVector(building_.numberOfRooms_);
  for(i=0;i<building_.numberOfRooms_;i++)
  {
    readCRoom(fileStream,i);
  }
  readBoundingBox( fileStream );
}

void CReloadBuilding::writeCConvex(ostream& ofstr,int convexnum)
{
  int i;
  ofstr << "CConvex" << endl;
  
  for(i=0;i<6;i++)
    writeCBoundary(ofstr,convexnum,i);

  ofstr << "Parameters" << endl;
  ofstr << building_.convexArray_[convexnum].permittivity_ << endl;
  ofstr << building_.convexArray_[convexnum].permeability_ << endl;
  ofstr << building_.convexArray_[convexnum].conductivity_ << endl;
}

void CReloadBuilding::readCConvex(ifstream& ifstr,int convex)
{
  string temp="";
  int i;

  ifstr >> temp;

  for(i=0;i<6;i++)
  {
  readCBoundary(ifstr,convex,i);
  }
  ifstr >> temp;
  ifstr >> building_.convexArray_[convex].permittivity_;
  ifstr >> building_.convexArray_[convex].permeability_;
  ifstr >> building_.convexArray_[convex].conductivity_;
}

void CReloadBuilding::readBoundingBox(ifstream& ifstr)
{
  string temp;

  ifstr >> temp;
  ifstr >> building_.boundingBox_.xSouthWest_;
  ifstr >> building_.boundingBox_.ySouthWest_;
  ifstr >> building_.boundingBox_.zSouthWest_;
  ifstr >> building_.boundingBox_.xNorthEast_;
  ifstr >> building_.boundingBox_.yNorthEast_;
  ifstr >> building_.boundingBox_.zNorthEast_;
}

void CReloadBuilding::writeBoundingBox(ofstream& ofstr)
{
  string temp="BoundingBox";

  ofstr << temp << endl;
  ofstr << building_.boundingBox_.xSouthWest_ << " ";
  ofstr << building_.boundingBox_.ySouthWest_ << " ";
  ofstr << building_.boundingBox_.zSouthWest_ << endl;
  ofstr << building_.boundingBox_.xNorthEast_ << " ";
  ofstr << building_.boundingBox_.yNorthEast_ << " ";
  ofstr << building_.boundingBox_.zNorthEast_ << endl;
}

void CReloadBuilding::writeCPoint3d(ostream& ostr, const CPoint3d& p )
{
  ostr << p.x << " " << p.y << " " << p.z << endl;
}

void CReloadBuilding::readCPoint3d(ifstream& ifstr, CPoint3d& p )
{
  double x,y,z;
  ifstr >> x;
  ifstr >> y;
  ifstr >> z;
  p.x=x;
  p.y=y;
  p.z=z;
}

void CReloadBuilding::compareBuildings( const CBuilding& b1, 
                                        const CBuilding& b2 )
{
  int i,j;


  if(b1.numberOfBuildings_ == b2.numberOfBuildings_)
    cout << "Agree on number of buildings" << endl;
  else
  {
    cout << "Disagree on number of buildings!" << endl;
    return;
  }

  if(b1.numberOfStories_ == b2.numberOfStories_)
    cout << "Agree on number of stories" << endl;
  else
  {
    cout << "Disagree on number of stories!" << endl;
    return;
  }

  for(i=0;i<b1.numberOfStories_;i++)
  {
    if(b1.storyArray_[i].numberOfRooms_==b2.storyArray_[i].numberOfRooms_)
      cout << "Agree on number of rooms in story " << i << endl;
    else
    {
      cout << "Disagree on number of rooms in story " << i << "!" << endl;
      return;
    }
    for(j=0;j<b1.storyArray_[i].numberOfRooms_;j++)
    {
      if(b1.storyArray_[i].roomIndex_[j]!=b2.storyArray_[i].roomIndex_[j])
      {
        cout << "Disagree on room index " << j << " of story " << i << endl;
        return;
      }
    }

    if(b1.storyArray_[i].numberOfConvexs_==b2.storyArray_[i].numberOfConvexs_)
      cout << "Agree on number of convexes in story " << i << endl;
    else
    {
      cout << "Disagree on number of convexes in story " << i << "!" << endl;
      return;
    }
    for(j=0;j<b1.storyArray_[i].numberOfConvexs_;j++)
    {
      if(b1.storyArray_[i].convexSpaceIndex_[j]!=b2.storyArray_[i].convexSpaceIndex_[j])
      {
        cout << "Disagree on room convex " << j << " of story " << i << endl;
        return;
      }
    }
  }
  
  if(b1.numberOfFloors_ == b2.numberOfFloors_)
    cout << "Agree on number of floors" << endl;
  else
  {
    cout << "Disagree on number of floors!" << endl;
    return;
  }

  for(i=0;i<b1.numberOfFloors_;i++)
  {
    if(b1.floorArray_[i].numberOfConvexs_==b2.floorArray_[i].numberOfConvexs_)
      cout << "Agree on number of convexs in floor " << i << endl;
    else
    {
      cout << "Disagree on number of convexes in floor " << i << "!" << endl;
      return;
    }
    for(j=0;j<b1.floorArray_[i].numberOfConvexs_;j++)
    {
      if(b1.floorArray_[i].convexSpaceIndex_[j]!=b2.floorArray_[i].convexSpaceIndex_[j])
      {
        cout << "Disagree on convex space index " << j << " of floor " << i << endl;
        return;
      }
    }
  }

  
  if(b1.roof.numberOfConvexs_==b2.roof.numberOfConvexs_)
    cout << "Agree on number of convexs in roof " << endl;
  else
  {
    cout << "Disagree on number of convexes in roof " << endl;
    return;
  }
  for(j=0;j<b1.roof.numberOfConvexs_;j++)
  {
    if(b1.roof.convexSpaceIndex_[j]!=b2.roof.convexSpaceIndex_[j])
    {
      cout << "Disagree on convex space index " << j << " of roof " << endl;
      return;
    }
  }

  if(b1.numberOfConvexs_ == b2.numberOfConvexs_)
    cout << "Agree on number of convexes" << endl;
  else
  {
    cout << "Disagree on number of convexes" << endl;
    return;
  }

  for(i=0;i<b1.numberOfConvexs_;i++)
  {
    if(b1.convexArray_[i].permeability_!=b2.convexArray_[i].permeability_)
    {
      cout << "Disagree on permeability of convex array element " << i << endl;
      return;
    }
    if(b1.convexArray_[i].permittivity_!=b2.convexArray_[i].permittivity_)
    {
      cout << "Disagree on permittivity of convex array element " << i << endl;
      return;
    }
    if(b1.convexArray_[i].conductivity_!=b2.convexArray_[i].conductivity_)
    {
      cout << "Disagree on conductivity of convex array element " << i << endl;
      return;
    }

    for(j=0;j<6;j++)
    {
      if(b1.convexArray_[i].facet[j].numberOfAdjacentConvexs_!=b2.convexArray_[i].facet[j].numberOfAdjacentConvexs_)
      {
        cout << "Disagreement on number of convexes neighbouring facet " << j << " of convex space " << i << endl;
        return;
      }

      if(b1.convexArray_[i].facet[j].equation_.D_!=b2.convexArray_[i].facet[j].equation_.D_)
      {
        cout << "Disagreement on D coefficient of equation of facet " << j << " of convex " << i << endl;
      }
      if(b1.convexArray_[i].facet[j].equation_.outwardNormal_.x!=b2.convexArray_[i].facet[j].equation_.outwardNormal_.x)
      {
        cout << "Disagreement on x coord of outward normal of equation of facet " << j << " of convex " << i <<endl;
      }
      if(b1.convexArray_[i].facet[j].equation_.outwardNormal_.y!=b2.convexArray_[i].facet[j].equation_.outwardNormal_.y)
      {
        cout << "Disagreement on y coord of outward normal of equation of facet " << j << " of convex " << i <<endl;
      }
      if(b1.convexArray_[i].facet[j].equation_.outwardNormal_.z!=b2.convexArray_[i].facet[j].equation_.outwardNormal_.z)
      {
        cout << "Disagreement on z coord of outward normal of equation of facet " << j << " of convex " << i <<endl;
      }
    }
  }
  if(b1.numberOfRooms_!=b2.numberOfRooms_)
  {
    cout << "Disagreement on number of rooms" << endl;
    return;
  }
  for(i=0;i<b1.numberOfRooms_;i++)
  {
    if(b1.roomArray_[i].numberOfConvexs_!=b2.roomArray_[i].numberOfConvexs_)
    {
      cout << "Disagreement on number of convexes in room " << i << endl;
      return;
    }
    for(j=0;j<b1.roomArray_[i].numberOfConvexs_;j++)
    {
      if(b1.roomArray_[i].convexSpaceIndex_[j]!=b2.roomArray_[i].convexSpaceIndex_[j])
      {
        cout << "Disagreement on convex space " << j << " of room " << i << endl;
        return;
      }
    }
    if(b1.roomArray_[i].numberOfRoomsBelow_!=b2.roomArray_[i].numberOfRoomsBelow_)
    {
      cout << "Disagreement on number of rooms below room " << i << endl;
      return;
    }
    for(j=0;j<b1.roomArray_[i].numberOfRoomsBelow_;j++)
    {
      if(b1.roomArray_[i].roomsBelow_[j]!=b2.roomArray_[i].roomsBelow_[j])
      {
        cout << "Disagreement on index of room below " << j << " of room " << i << endl;
        return;
      }
    }

    if(b1.roomArray_[i].numberOfRoomsAdjacent_!=b2.roomArray_[i].numberOfRoomsAdjacent_)
    {
      cout << "Disagreement on number of rooms adjacent to room " << i << endl;
      cout << "b1 gives " << b1.roomArray_[i].numberOfRoomsAdjacent_ << endl;
      cout << "b2 gives " << b2.roomArray_[i].numberOfRoomsAdjacent_ << endl;
      return;
    }
    for(j=0;j<b1.roomArray_[i].numberOfRoomsAdjacent_;j++)
    {
      if(b1.roomArray_[i].roomsAdjacent_[j]!=b2.roomArray_[i].roomsAdjacent_[j])
      {
        cout << "Disagreement on index of room adjacent " << j << " of room " << i << endl;
        return;
      }
    }

    if(b1.roomArray_[i].numberOfRoomsAbove_!=b2.roomArray_[i].numberOfRoomsAbove_)
    {
      cout << "Disagreement on number of rooms above room " << i << endl;
      return;
    }
    for(j=0;j<b1.roomArray_[i].numberOfRoomsAbove_;j++)
    {
      if(b1.roomArray_[i].roomsAbove_[j]!=b2.roomArray_[i].roomsAbove_[j])
      {
        cout << "Disagreement on index of room above " << j << " of room " << i << endl;
        return;
      }
    }
  }
}
