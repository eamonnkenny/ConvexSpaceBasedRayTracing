//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     November 2001
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "building.hh"

CBuilding::CBuilding()
{
  numberOfConvexs_   = 0;
  numberOfRooms_     = 0;
  numberOfFloors_    = 0;
  numberOfStories_   = 0;
  numberOfBuildings_ = 0;
  boundingBox_       = CBoundingBox();
}

void CBuilding::getNumberOfConvexs( const string& buildingFileName )
{
  CIntrinsic intrinsicObject;
  string    aString;

  if ( intrinsicObject.CheckFileExists( buildingFileName ) == false )
  {
    cerr << "Error: Building file: " << buildingFileName << " does not exist" << endl;
    exit(-1);
  }

  // look for occurrences of string=Convex to allocate convex list size
  aString = "Convex\0";
  numberOfConvexs_ = intrinsicObject.occurrencesInFile( aString, buildingFileName );
}

void CBuilding::getNumberOfRooms( const string& buildingFileName )
{
  string    aString;
  CIntrinsic intrinsicObject;

  // look for occurrences of string=Room to allocate room list size
  aString = "Room\0";
  numberOfRooms_ = intrinsicObject.occurrencesInFile( aString, buildingFileName );
}

void CBuilding::connectConvexsAndRooms( string buildingFileName, string roomFile )
{
  ifstream       filePtr;
  boolean        moreConvexsToRead;
  string        convexString, buildingString, floorString, storyString;
  CMemoryStorage MemoryObject;
  CConvex        convexObject;
  CIntrinsic	 intrinsicObject;
  int            i;

  // define the sizes for the dynamic allocation of the building
  getNumberOfConvexs( buildingFileName );
  getNumberOfRooms( buildingFileName );
  cout << "Found ( " << numberOfConvexs_ << " Convexs, "; 
  cout << numberOfRooms_   << " Rooms, ";

  // now reread the file to read in the convexs and rooms
  convexArray_ = MemoryObject.CreateConvexVector( numberOfConvexs_ );
  roomArray_   = MemoryObject.CreateRoomVector( numberOfRooms_ );

  filePtr.open( buildingFileName.c_str() );

  filePtr >> buildingString;       // read number of buildings
  filePtr >> numberOfBuildings_;
  cout << numberOfBuildings_ << " Buildings, "; 
  filePtr >> floorString;          // read number of floors
  filePtr >> numberOfFloors_;
  cout << numberOfFloors_ << " Floors, "; 
  filePtr >> storyString;         // read number of stories
  filePtr >> numberOfStories_;
  cout << numberOfStories_ << " Stories )" << endl; 

  storyArray_ = MemoryObject.CreateStoryVector( numberOfStories_ );
  floorArray_ = MemoryObject.CreateFloorVector( numberOfFloors_ );
  
  filePtr >> floorString;          // read first floor 

  convexString     = "Convex\0";

  i = 0;
  do 
  {
    moreConvexsToRead = intrinsicObject.readToString( convexString, filePtr );
    convexArray_[i]   = CConvex( filePtr );
    updateBoundingBox( convexArray_[i] );
    //cout << i << " convex read to array" << endl;
    i++;
  } while (moreConvexsToRead && i < numberOfConvexs_ );

  filePtr.close();

  linkToConvexsInAllRooms( buildingFileName );
  linkToConvexsInAllFloors( buildingFileName );

  if ( numberOfStories_ > 0 )
  {
    linkToConvexsInAllStories( buildingFileName );
    linkToRoomsInAllStories( buildingFileName );
    linkAllRoomsToOtherRooms( roomFile );
  }

  linkAllConvexsToOtherConvexs();
  cout << "Finished Connectivity" << endl;
}

void CBuilding::linkToConvexsInAllRooms( const string& buildingFileName )
{
  int      i=0, j, k, numberOfConvexsInRoom;
  CIntrinsic intrinsicObject;
  ifstream filePtr;
  boolean  moreConvexsToRead, moreRoomsToRead;
  string  roomString, roomConvexString, fullString, convexString;

  roomString       = "Room\0";
  roomConvexString = "ConvexRoom\0";
  convexString     = "Convex\0";

  filePtr.open( buildingFileName.c_str() );

  // find the first occurrence of the Room String in building database
  moreRoomsToRead = intrinsicObject.readToString( roomString, filePtr );

  // for all rooms do the following
  do
  {
    // finds the Room string
    roomArray_[i].numberOfConvexs_ = 0;
    do 
    {
      // until we find another Room string or eof() find Convexs in Rooms
      moreConvexsToRead = intrinsicObject.readToString( roomConvexString, filePtr, roomString );
      roomArray_[i].numberOfConvexs_++;
    } while (moreConvexsToRead);
    roomArray_[i].numberOfConvexs_--;
    // We now know how many convexs exist in room i

    i++;
    cout << "Room counter: " << i << endl;
  } while ( moreRoomsToRead && i < numberOfRooms_ );

  filePtr.close();


  // dynamically allocate the number of convexs for each room pointer
  for ( i = 0; i < numberOfRooms_; i++ )
  {
    numberOfConvexsInRoom = roomArray_[i].numberOfConvexs_;
    roomArray_[i].convexSpaceIndex_ = new int [numberOfConvexsInRoom+1];
  }

  // find the addresses of the convex spaces that each room contains
  filePtr.open( buildingFileName.c_str() );

  k = 0;
  i = 0;
  for (j = 0; j < numberOfConvexs_; j++ )
  {
    fullString = intrinsicObject.readFullString( convexString, filePtr );

    if ( fullString.compare(roomConvexString) == 0 )
    {
      roomArray_[i].convexSpaceIndex_[k] = j;
      k++;
      if ( k == roomArray_[i].numberOfConvexs_ )
      {
        k = 0; // reset the convex room number for the next room
        i++;   // go to the next room and make it's links
      }
    }
  }
   
  filePtr.close();

  // print first point in convex of a room as a test
  for ( i = 0; i < numberOfRooms_; i++ )
    for ( j = 0; j < roomArray_[i].numberOfConvexs_; j++ )
    {
      cout << "Room: " << i << " contains " << roomArray_[i].convexSpaceIndex_[j] << endl;
    }
} 

void CBuilding::linkToConvexsInAllFloors( const string& buildingFileName )
{
  int      i, j, k, numberOfConvexsInFloor;
  ifstream filePtr;
  boolean  moreConvexsToRead, moreFloorsToRead;
  string  floorString, storyString, aString, convexString;
  CIntrinsic intrinsicObject;
  char     oneCharacter;

  floorString  = "Floor\0";
  storyString  = "Story\0";
  convexString = "Convex\0";

  filePtr.open( buildingFileName.c_str() );


  // for all floors do the following
  i = 0;
  do
  {
    // find the first occurrence of the Floor String in building database
    moreFloorsToRead = intrinsicObject.readToString( floorString, filePtr );

    // finds the floor string
    if ( i == numberOfFloors_ )
      roof.numberOfConvexs_ = 0;
    else
      floorArray_[i].numberOfConvexs_ = 0;
    do 
    {
      // until we find a Story string or eof() find Convexs in Floor
      moreConvexsToRead = intrinsicObject.readToString( convexString, filePtr, storyString );
      if ( i == numberOfFloors_ )
        roof.numberOfConvexs_++;
      else
        floorArray_[i].numberOfConvexs_++;
    } while (moreConvexsToRead);

    if ( i == numberOfFloors_ )
      roof.numberOfConvexs_--;
    else
      floorArray_[i].numberOfConvexs_--;

    // We now know how many convexs exist in floor i

    /*if ( i == numberOfFloors_ )
      cerr << "Found " << roof.numberOfConvexs_ 
           << " convex space in Roof " << endl;
    else
      cerr << "Found " << floorArray_[i].numberOfConvexs_ 
           << " convex space in Floor " << i << endl;*/
    i++;
  } while ( moreFloorsToRead && i < numberOfFloors_+1 );

  filePtr.close();

  // dynamically allocate the number of convexs for each floor pointer
  for ( i = 0; i < numberOfFloors_; i++ )
  {
    numberOfConvexsInFloor           = floorArray_[i].numberOfConvexs_;
    floorArray_[i].convexSpaceIndex_ = new int [numberOfConvexsInFloor+1];
  }

  // dynamically allocate the number of convexs for each floor pointer
  roof.convexSpaceIndex_ = new int [roof.numberOfConvexs_+1];

  // find the addresses of the convex spaces that each floor contains
  filePtr.open( buildingFileName.c_str() );

  for ( i = 0; i < 6; i++ )
    filePtr >> aString; // read redundant information at start of file

  j = 0;
  i = 0;
  oneCharacter = '0';
  do
  {
    k = 0;
    do
    {
      oneCharacter = readABuildingMaterialType( filePtr );
      if ( oneCharacter == 'C' ) j++;
    } while ( oneCharacter != 'F' );
    //cerr << "got floor" << endl;

    do
    {
      oneCharacter = readABuildingMaterialType( filePtr );
      if ( i == numberOfFloors_ )
        roof.convexSpaceIndex_[k] = j;
      else
        floorArray_[i].convexSpaceIndex_[k] = j;
      if ( oneCharacter == 'C' ) j++;
      k++;
    } while ( oneCharacter != 'S' && oneCharacter != '0' );
    //cerr << "got story" << endl;

    i++;
  } while ( !filePtr.eof() && oneCharacter != '0' );
   
  filePtr.close();
  

  // print all convexs of a floor as a test
  for ( i = 0; i < numberOfFloors_; i++ )
    for ( j = 0; j < floorArray_[i].numberOfConvexs_; j++ )
    {
      cout << "Floor: " << i << " contains " <<
      floorArray_[i].convexSpaceIndex_[j] << endl;
    }
  for ( j = 0; j < roof.numberOfConvexs_; j++ )
    cout << "Roof: " << " contains " << roof.convexSpaceIndex_[j] << endl;
} 

void CBuilding::linkToConvexsInAllStories( const string& buildingFileName )
{
  int      i=0, j, k, numberOfConvexsInStory;
  ifstream filePtr, filePtr2;
  boolean  moreConvexsToRead, moreStoriesToRead;
  string  floorString, storyString, aString, convexString;
  CIntrinsic intrinsicObject;
  char     oneCharacter;

  floorString  = "Floor\0";
  storyString  = "Story\0";
  convexString = "Convex\0";

  filePtr.open( buildingFileName.c_str() );

  // for all stories do the following
  do
  {
    // find the first occurrence of the Story String in building database
    moreStoriesToRead = intrinsicObject.readToString( storyString, filePtr );

    // finds the floor string
    storyArray_[i].numberOfConvexs_ = 0;
    do 
    {
      // until we find a Floor string or eof() find Convexs in Story
      moreConvexsToRead = intrinsicObject.readToString( convexString, filePtr, floorString );
      storyArray_[i].numberOfConvexs_++;
    } while (moreConvexsToRead);
    storyArray_[i].numberOfConvexs_--;
    // We now know how many convexs exist in story i

    cerr << "Found " << storyArray_[i].numberOfConvexs_
         << " convex space in Story " << i << endl;
    i++;
  } while ( moreStoriesToRead && i < numberOfStories_ );

  filePtr.close();

  // dynamically allocate the number of convexs for each floor pointer
  for ( i = 0; i < numberOfStories_; i++ )
  {
    numberOfConvexsInStory = storyArray_[i].numberOfConvexs_;
    storyArray_[i].convexSpaceIndex_ = new int [numberOfConvexsInStory+1];
  }

  // find the addresses of the convex spaces that each story contains
  filePtr2.open( buildingFileName.c_str() );

  for ( i = 0; i < 6; i++ )
    filePtr2 >> aString; // read redundant information at start of file
  //cout << aString << endl;

  j = 0;
  i = 0;
  oneCharacter = '0';
  do
  {
    k = 0;

    do
    {
      oneCharacter = readABuildingMaterialType( filePtr2 );
      if ( oneCharacter == 'C' ) j++;
    } while ( oneCharacter != 'S' && oneCharacter != '0' );
    cerr << "got story" << endl;

    do
    {
      oneCharacter = readABuildingMaterialType( filePtr2 );
      storyArray_[i].convexSpaceIndex_[k] = j;
      cerr << "story " << i << " index " << k << " contains " << j << endl;
      if ( oneCharacter == 'C' ) // find a convex space
      { 
        j++;
        k++; // only update counter if not a room, story, floor
      }
    } while ( oneCharacter != 'F' && oneCharacter != '0' );
    cerr << "got floor" << endl;

    i++;
  } while ( !filePtr2.eof() && oneCharacter != '0' && i < numberOfStories_ );
   

  // print all convexs of a story as a test
  for ( i = 0; i < numberOfStories_; i++ )
  {
    for ( j = 0; j < storyArray_[i].numberOfConvexs_; j++ )
    {
      cout << "Story: " << i << " at index " << j << " contains " << 
      storyArray_[i].convexSpaceIndex_[j] << endl;
    }
  }
  filePtr2.close();
} 

void CBuilding::linkToRoomsInAllStories( const string& buildingFileName )
{
  int      i=0, j, k, numberOfRoomsInStory;
  ifstream filePtr;
  boolean  moreRoomsToRead, moreStoriesToRead;
  string  floorString, storyString, aString, roomString;
  CIntrinsic intrinsicObject;
  char     oneCharacter;

  floorString  = "Floor\0";
  storyString  = "Story\0";
  roomString = "Room\0";

  filePtr.open( buildingFileName.c_str() );


  // for all stories do the following
  do
  {
    // find the first occurrence of the Story String in building database
    moreStoriesToRead = intrinsicObject.readToString( storyString, filePtr );

    // finds the floor string
    storyArray_[i].numberOfRooms_ = 0;
    do 
    {
      // until we find a Floor string or eof() find Rooms in Story
      moreRoomsToRead = intrinsicObject.readToString( roomString, filePtr, floorString );
      storyArray_[i].numberOfRooms_++;
    } while (moreRoomsToRead);
    storyArray_[i].numberOfRooms_--;
    // We now know how many rooms exist in story i

    cout << "****" << storyArray_[i].numberOfRooms_ << " in story no. " << i << endl;
    i++;
  } while ( moreStoriesToRead && i < numberOfStories_ );


  filePtr.close();

  // dynamically allocate the number of convexs for each floor pointer
  for ( i = 0; i < numberOfStories_; i++ )
  {
    numberOfRoomsInStory = storyArray_[i].numberOfRooms_;
    storyArray_[i].roomIndex_ = new int [numberOfRoomsInStory+1];
  }

  // find the indices of the room spaces that each story contains
  filePtr.open( buildingFileName.c_str() );

  for ( i = 0; i < 6; i++ )
    filePtr >> aString; // read redundant information at start of file

  // initialisers
  j = 0;
  i = 0;
  oneCharacter = '0';
  do
  {
    k = 0;

    // read until you find the Story or end of file
    do
    {
      oneCharacter = readABuildingMaterialType( filePtr );
      if ( oneCharacter == 'R' ) j++;
    } while ( oneCharacter != 'S' && oneCharacter != '0' );
    //cerr << "got story" << endl;

    // now that we have the Story find all rooms until we hit a Floor 
    // or end of file.
    do
    {
      oneCharacter = readABuildingMaterialType( filePtr );
      storyArray_[i].roomIndex_[k] = j;
      //cerr << "story " << i << " index " << k << " contains room" << j << endl;
      if ( oneCharacter == 'R' ) // find a room number
      { 
        j++;
        k++; // only update counter if not a convex space, story, floor
      }
    } while ( oneCharacter != 'F' && oneCharacter != '0' );
    //cerr << "got floor" << endl;

    i++;
  } while ( !filePtr.eof() && oneCharacter != '0' && i < numberOfStories_ );
   
  filePtr.close();
  

  // print all room indices of a story as a test
  for ( i = 0; i < numberOfStories_; i++ )
    for ( j = 0; j < storyArray_[i].numberOfRooms_; j++ )
    {
      cout << "Story: " << i << " at index " << j << " contains room " << 
      storyArray_[i].roomIndex_[j] << endl;
    }
  cout << "Finished Story Output" << endl;
} 

char CBuilding::readABuildingMaterialType( ifstream& filePtr )
{
  string materialType;
  CConvex aConvex;
  double  xyzPoint;
  char    c;

  filePtr >> materialType; 
  if ( filePtr.eof() || materialType.at(0) == EOF ) return '0';

  c = materialType.at(0);

  if ( c == 'C' )
  {
    // read to the end of the current convex space including 
    // constitutive parameters. Changed 12/12/01 due to bug in release
    // mode version of VC6++ code,
    aConvex = CConvex( filePtr );
  }
  else if ( c == 'R' )
  {
    // do nothing if a room identifier is found
  }
  else 
  {
    // else read in one number (Story,Floor or building)
    filePtr >> xyzPoint; 
  }
  
  return materialType.at(0);
}

void CBuilding::linkAllConvexsToOtherConvexs()
{
  int     i, j, k, l, m;
  int     noOfAdjacentConvexs;
  boolean isContained, isContained2, isOppositeNormal;

  cout << "linking Convex....please wait about 5 seconds" << endl;
  cout << "The building connectivity information will be saved to a file, and reloaded"
       << endl << "in the next computation, resulting in very fast computational times." 
       << endl << "(this assumes that the transmitter does not move in next computation)" 
	   << endl;

  // forall convexs in building
  for ( i = 0; i < numberOfConvexs_; i++ )
  {
    // forall boundaries in the current convex
    for ( k = 0; k < 6; k++ )
    {
      (convexArray_[i].facet[k]).numberOfAdjacentConvexs_ = 0;
      // for all other convexs in building
      for ( j = 0; j < numberOfConvexs_; j++ )
      {
        if ( i != j )
        {
          // forall boundaries in the other convex
          for ( l = 0; l < 6; l++ )
          {
            // test to see if they are connected 
            //cout << "Testing: " << i << " " << k << " " << j << " " << l <<
            //endl;
            isContained = 
            (convexArray_[i].facet[k]).intersection(convexArray_[j].facet[l]);
            //cout << "Testing: " << j << " " << l << " " << i << " " << k <<
            //endl;
            isContained2 = 
            (convexArray_[j].facet[l]).intersection(convexArray_[i].facet[k]);
            
            isOppositeNormal =
            (convexArray_[i].facet[k]).oppositeNormal(convexArray_[j].facet[l]);
            // if so, increment counter for required memory allocation 

            if ( isOppositeNormal )
              if ( isContained || isContained2 ) 
            {
              (convexArray_[i].facet[k]).numberOfAdjacentConvexs_++;
            }
          }
        }
      }
      noOfAdjacentConvexs = (convexArray_[i].facet[k]).numberOfAdjacentConvexs_;
      (convexArray_[i].facet[k]).adjacentConvexIndex_ 
                     = new int [noOfAdjacentConvexs+1];
    }
  }

  for ( i = 0; i < numberOfConvexs_; i++ )
  {
     for ( k = 0; k < 6; k++ )
    {
      m = 0;
      for ( j = 0; j < numberOfConvexs_; j++ )
      {
        if ( i != j )
        {
          for ( l = 0; l < 6; l++ )
          {
            isContained = 
            (convexArray_[i].facet[k]).intersection(convexArray_[j].facet[l]);
            isContained2 = 
            (convexArray_[j].facet[l]).intersection(convexArray_[i].facet[k]);

            isOppositeNormal =
            (convexArray_[i].facet[k]).oppositeNormal(convexArray_[j].facet[l]);
            // this time we have the memory allocated so we just insert the 
            // index of the connecting convex space

            if ( isOppositeNormal )
              if ( isContained || isContained2 ) 
            {
              (convexArray_[i].facet[k]).adjacentConvexIndex_[m] = j;
              m++;
            }
          }
        }
      }
    }
  }

  // the following code should be uncommented if having problems with your
  // database connectivity
 /* do
  {
    cerr << "Type convex number in range to see adjacent convex numbers" 
         << endl << "Otherwise type a number outside the range to quit"
         << endl;
    cerr << "Type convex in range to see adjacent convex numbers" << endl;
    cerr << "convex number: (0-" << numberOfConvexs_-1 << ")";
    cin  >> i;
    if ( i < numberOfConvexs_ && i >= 0 )
    {
      cerr << "boundary number: (0-5)";
      cin  >> k;
  
      m = convexArray_[i].facet[k].numberOfAdjacentConvexs_;
      cout << convexArray_[i].facet[k].vertex_[0] << endl;
      cout << convexArray_[i].facet[k].vertex_[1] << endl;
      cout << convexArray_[i].facet[k].vertex_[2] << endl;
      cout << convexArray_[i].facet[k].vertex_[3] << endl;
      cout << convexArray_[i].facet[k].equation_.outwardNormal_ << endl;
      cout << convexArray_[i].facet[k].equation_.D_ << endl;
      cout << "connected to: " 
           << m << " convexs" << endl << "       ";
      for ( l = 0; l < m; l++ )
        cout << convexArray_[i].facet[k].adjacentConvexIndex_[l] << " ";
      cout << endl;
    }
  } while ( i < numberOfConvexs_ && i >= 0 );*/
}

void CBuilding::linkAllRoomsToOtherRooms( const string& buildingFileName )
{
  ifstream filePtr;
  string  roomString;
  char     c;
  int      roomNumber;
  int      belowCounter, adjacentCounter, aboveCounter, storyCounter;
  int      i, j;

  // scan the file to get the memory allocation required for storage
  // of index into other rooms

  filePtr.open( buildingFileName.c_str() );

  for ( i = 0; i < numberOfRooms_; i++ )
  {
    // read the next room string, room number and carriage return
    filePtr >> roomString;
    cout << "Room String Value" << roomString << endl;
    filePtr >> roomNumber;
    cout << "Room Number Value" << roomNumber << endl;
    filePtr.get(c); // read end of line character 

    belowCounter    = 0;
    adjacentCounter = 0;
    aboveCounter    = 0;

    // get connected rooms below, on same story and above 
    storyCounter = 0;
    do 
    {
      j = 0;
      // find number of adjoining rooms on one line of the file
      do
      {
        filePtr.get(c);
        j++;
      } while ( c != ' ' && c != '\n' && c != '\r' );

      // j > 1 required since line may contain only a carriage return
      if ( storyCounter == 0 && j > 1 ) 
      {
        belowCounter++;
        //cout << "below" << endl;
      }
      else if ( storyCounter == 1 && j > 1 ) 
      {
        adjacentCounter++;
        //cout << "adjacent" << endl;
      }
      else if ( storyCounter == 2 && j > 1 )  
      {
        aboveCounter++;
        //cout << "above" << endl;
      }

      // when we hit the end of line in the file, go to next story level
      if ( c == '\n' || c == '\r' )
        storyCounter++;
    } while ( storyCounter <= 2 );

    // now we allocate the memory dynamically
    roomArray_[i].numberOfRoomsBelow_    = belowCounter;
    roomArray_[i].numberOfRoomsAdjacent_ = adjacentCounter;
    roomArray_[i].numberOfRoomsAbove_    = aboveCounter;
    roomArray_[i].roomsBelow_            = new int [belowCounter+1];
    roomArray_[i].roomsAdjacent_         = new int [adjacentCounter+1];
    roomArray_[i].roomsAbove_            = new int [aboveCounter+1];
  }
  filePtr.close();

  // this time we read in the indexs

  filePtr.open( buildingFileName.c_str() );

  for ( i = 0; i < numberOfRooms_; i++ )
  {
    filePtr >> roomString;
    filePtr >> roomNumber;

    // get connected rooms below, on same story and above 
    // note that room index is one less than room number
    // since it is a reference in an array from [0,1,2,...]
    for ( j = 0; j < roomArray_[i].numberOfRoomsBelow_; j++ )
    {
      filePtr >> roomNumber;
      roomArray_[i].roomsBelow_[j] = roomNumber-1;
    }

    for ( j = 0; j < roomArray_[i].numberOfRoomsAdjacent_; j++ )
    {
      filePtr >> roomNumber;
      roomArray_[i].roomsAdjacent_[j] = roomNumber-1;
    }

    for ( j = 0; j < roomArray_[i].numberOfRoomsAbove_; j++ )
    {
      filePtr >> roomNumber;
      roomArray_[i].roomsAbove_[j] = roomNumber-1;
    }
  }
  filePtr.close();

  // to test the results print all room connectivity
  /*for ( i = 0; i < numberOfRooms_; i++ )
  {
    cout << "Room index: " << i << endl;
    cout << " connected below to: " << roomArray_[i].numberOfRoomsBelow_ 
         << " rooms: "; 
    for ( j = 0; j < roomArray_[i].numberOfRoomsBelow_; j++ )
      cout << roomArray_[i].roomsBelow_[j] << " ";
    cout << endl;

    cout << " connected adjacent to: " << roomArray_[i].numberOfRoomsAdjacent_ 
         << " rooms: ";  
    for ( j = 0; j < roomArray_[i].numberOfRoomsAdjacent_; j++ )
      cout << roomArray_[i].roomsAdjacent_[j] << " ";
    cout << endl;

    cout << " connected above to:" << roomArray_[i].numberOfRoomsAbove_ 
         << " rooms: "; 
    for ( j = 0; j < roomArray_[i].numberOfRoomsAbove_; j++ )
      cout << roomArray_[i].roomsAbove_[j] << " ";
    cout << endl;
  }*/
}

boolean CBuilding::locateConvexNumber( const CPoint3d& aPoint, 
                                       int&  convexNumber ) const
{
  int     i, floorBelow, floorAbove, k, l, roomNumber;
  CRoom   *aRoom;

  for ( i = 0; i < numberOfFloors_-1; i++ )
  {
    floorBelow = floorArray_[i].convexSpaceIndex_[0];
    floorAbove = floorArray_[i+1].convexSpaceIndex_[0];
    if ( aPoint.z >= (convexArray_[floorBelow].facet[5]).vertex_[0].z - TOL 
      && aPoint.z <= (convexArray_[floorAbove].facet[0]).vertex_[0].z + TOL )
    {
      for ( k = 0; k < storyArray_[i].numberOfRooms_; k++ )
      {
        roomNumber = storyArray_[i].roomIndex_[k];
        aRoom      = &roomArray_[roomNumber];
        for ( l = 0; l < aRoom->numberOfConvexs_; l++ )
        {
          convexNumber = aRoom->convexSpaceIndex_[l];
          if ( convexArray_[convexNumber].intersection( aPoint ) == true )
            return true;
        }
      }

      for ( k = 0; k < storyArray_[i].numberOfConvexs_; k++ )
      {
        convexNumber = storyArray_[i].convexSpaceIndex_[k];
        if ( convexArray_[convexNumber].intersection( aPoint ) == true )
            return true;
      }
    }
  } 

  floorBelow = floorArray_[i].convexSpaceIndex_[0];
  floorAbove = roof.convexSpaceIndex_[0];
  if ( aPoint.z >= convexArray_[floorBelow].facet[5].vertex_[0].z 
    && aPoint.z <= convexArray_[floorAbove].facet[0].vertex_[0].z )
  {
    for ( k = 0; k < storyArray_[i].numberOfRooms_; k++ )
    {
      roomNumber = storyArray_[i].roomIndex_[k];
      aRoom      = &roomArray_[roomNumber];

      for ( l = 0; l < aRoom->numberOfConvexs_; l++ )
      {
        convexNumber = aRoom->convexSpaceIndex_[l];
        if ( convexArray_[convexNumber].intersection( aPoint ) == true )
          return true;
      }
    }

    for ( k = 0; k < storyArray_[i].numberOfConvexs_; k++ )
    {
      convexNumber = storyArray_[i].convexSpaceIndex_[k];
      if ( convexArray_[convexNumber].intersection( aPoint ) == true )
          return true;
    }
  }

  convexNumber = -1;
  return false;
}

boolean CBuilding::locateConvexNumber2( const CPoint3d& aPoint, 
                                        int&  convexNumber ) const
{
  int     i, floorBelow, floorAbove, k, l, roomNumber;
  int     j = 0;
  CRoom   *aRoom;
  CConvex *aConvex;

  for ( i = 0; i < numberOfFloors_-1; i++ )
  {
    floorBelow = floorArray_[i].convexSpaceIndex_[0];
    floorAbove = floorArray_[i+1].convexSpaceIndex_[0];
    if ( aPoint.z >= (convexArray_[floorBelow].facet[5]).vertex_[0].z - TOL 
      && aPoint.z <= (convexArray_[floorAbove].facet[0]).vertex_[0].z + TOL )
    {
      for ( k = 0; k < storyArray_[i].numberOfRooms_; k++ )
      {
        roomNumber = storyArray_[i].roomIndex_[k];
        aRoom      = &roomArray_[roomNumber];
        for ( l = 0; l < aRoom->numberOfConvexs_; l++ )
        {
          convexNumber = aRoom->convexSpaceIndex_[l];
          if ( convexArray_[convexNumber].intersection( aPoint ) == true )
            j++;
	  if ( j == 2 )
            return true;
        }
      }
    }

    if ( aPoint.z >= (convexArray_[floorAbove].facet[0]).vertex_[0].z - TOL 
      && aPoint.z <= (convexArray_[floorAbove].facet[5]).vertex_[0].z + TOL )
    {
      for ( k = 0; k < floorArray_[i+1].numberOfConvexs_; k++ )
      {
        convexNumber = floorArray_[i+1].convexSpaceIndex_[k];
	aConvex      = &convexArray_[convexNumber];
	if ( aConvex->conductivity_ == 0.0 )
          if ( aConvex->intersection( aPoint ) == true )
            j++;
	if ( j == 2 )
          return true;
      }
    }
  } 

  floorBelow = floorArray_[i].convexSpaceIndex_[0];
  floorAbove = roof.convexSpaceIndex_[0];
  if ( aPoint.z >= convexArray_[floorBelow].facet[5].vertex_[0].z 
    && aPoint.z <= convexArray_[floorAbove].facet[0].vertex_[0].z )
  {
    for ( k = 0; k < storyArray_[i].numberOfRooms_; k++ )
    {
      roomNumber = storyArray_[i].roomIndex_[k];
      aRoom      = &roomArray_[roomNumber];

      for ( l = 0; l < aRoom->numberOfConvexs_; l++ )
      {
        convexNumber = aRoom->convexSpaceIndex_[l];
        if ( convexArray_[convexNumber].intersection( aPoint ) == true )
          j++;
	    if ( j == 2)
          return true;
      }
    }
  }

  convexNumber = -1;
  return false;
}

void CBuilding::locateConvexNumber( List<CEdge>& diffractionEdges, 
                                    int         *diffractionEdgeIndex ) const
{
  int            i, N;
  CSegment3d     aSegment;
  CPoint3d       aPoint;

  N = diffractionEdges.size();

  for ( i = 0; i < N; i++ )
  {
    aSegment = diffractionEdges.GetItem(i).edge_;
    aPoint   = ( aSegment.start() + aSegment.end() ) / 2.0;
    locateConvexNumber( aPoint, diffractionEdgeIndex[i] );
  }
}

void CBuilding::locateConvexNumber2( List<CEdge>& diffractionEdges, 
                                     int         *diffractionEdgeIndex ) const
{
  int            i, N;
  CSegment3d     aSegment;
  CPoint3d       aPoint;

  N = diffractionEdges.size();

  for ( i = 0; i < N; i++ )
  {
    aSegment = diffractionEdges.GetItem(i).edge_;
    aPoint   = ( aSegment.start() + aSegment.end() ) / 2.0;
    locateConvexNumber2( aPoint, diffractionEdgeIndex[i] );
  }
}

//List<CPlane>& CBuilding::calculateReflectionPlanes() const
void CBuilding::calculateReflectionPlanes( List<CPlane>& planeList ) const
{
  int          i, j, k, m;
  //List<CPlane> *planeList;
  CConvex      *currentConvex;
  CBoundary    *currentBoundary;
  boolean      isContained, canSeeRoom;

  //planeList = new List<CPlane>;

  for ( i = 0; i < numberOfConvexs_; i++ )
  {
    currentConvex = &convexArray_[i];

    // only continue if we have encountered a filled convex space.
    if ( currentConvex->conductivity_ != 0.0 )
    {
      for ( j = 0; j < 6; j++ )
      {  
        isContained = false;
        currentBoundary = &currentConvex->facet[j];
  
        canSeeRoom = false;  // default value
        // we want to see if a plane of contact is obtainable through 
        // free space. If it is adjoined to a filled space, then it is 
        // not defined to be a reflectable plane

        for ( k = 0; k < currentBoundary->numberOfAdjacentConvexs_; k++ )
        {
          m = currentBoundary->adjacentConvexIndex_[k];
          if ( convexArray_[m].conductivity_ == 0.0 )
          {
            canSeeRoom = true; 
            break;
          }
        }
  
        if ( canSeeRoom == true )
        {
          for ( k = 0; k < planeList.size(); k++ )
          {
            if ( currentBoundary->equation_ == planeList.GetItem(k) )
            {
              isContained = true;
              break;
            }
          }
    
          if ( isContained == false )
          {
            planeList.append( currentBoundary->equation_ );
          }
        }
      }
    }
  }
  //cout << "Number of possibly reflectable Boundary: " 
       //<< planeList.size() << endl;

  //return *planeList;
}

void CBuilding::updateBoundingBox( const CConvex& convexObject )
{
  if ( convexObject.conductivity_ == 0.0 )
  {
    boundingBox_.updateBox( convexObject.facet[0] );
    boundingBox_.updateBox( convexObject.facet[5] );
  }
}
