//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     October 2001
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "intrinsic.hh"

//=========================================================================
// = FUNCTION 
// 	antenna_position
//
// = DESCRIPTION 
//=========================================================================

complex CIntrinsic::AntennaPosition( double x, double y, double BaseHeight )
{
  complex za;

  za = complex( x, y + BaseHeight );
  return( za );
}

//========================================================================
// = FUNCTION
//	Round
// = DESCRIPTION
// Round a double to nearest integer.
//========================================================================

int CIntrinsic::Round( double d )
{
  return( (int) (d + 0.5) );
}


//========================================================================
// = FUNCTION
//	Minimum
// = DESCRIPTION
// Calculate the minimum of two doubles.
//========================================================================

int CIntrinsic::Minimum( int M, int N )
{
  if ( M < N ) return M; 
  else return N;
}

double CIntrinsic::Minimum( double M, double N )
{
  if ( M < N ) return M; 
  else return N;
}

double CIntrinsic::Minimum( double M, double N, double M1 )
{
  double temp = Minimum( M, N ) ;
  if ( M1 < temp ) return M1;
  return temp ;
}

double CIntrinsic::Minimum( double p1, double p2, double p3, double p4 )
{
  double MinimumValue;
  
  MinimumValue = Minimum( p1, p2 );
  MinimumValue = Minimum( p3, MinimumValue );
  MinimumValue = Minimum( p4, MinimumValue );
 
  return( MinimumValue );
}

//========================================================================
// = FUNCTION
//	Maximum
// = DESCRIPTION
// Calculate the maximum of two doubles.
//========================================================================

int CIntrinsic::Maximum( int M, int N )
{
  if ( M > N ) return M; 
  else return N;
}

double CIntrinsic::Maximum( double M, double N )
{
  if ( M > N ) return M; 
  else return N;
}

double CIntrinsic::Maximum( double M, double N, double M1 )
{
  double temp = Maximum( M, N ) ;
  if ( M1 > temp ) return M1;
  return temp ;
}

//========================================================================
// = FUNCTION
//	itoa
// = DESCRIPTION
// Convert integer to character equivalent.
//========================================================================

void CIntrinsic::itoa( int n, char* s )
{
  int i, j, c, sign;

  if ( ( sign = n ) < 0 ) n = -n;
  i = 0;

  do
  {
    s[i++] = n % 10 + '0';
  } while ( ( n /= 10 ) > 0 );

  if ( sign < 0 )
    s[i++] = '-';

  s[i] = '\0';

  for ( i = 0, j = strlen(s) - 1; i < j; i++, j-- )
  {
    c    = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

bool CIntrinsic::CheckFileExists( const string& InputFile )
{
  FILE *fp;

  if ( (fp = fopen( InputFile.c_str(), "r" )) == NULL )
  {
    cout << "File: " << InputFile << " does not exist." << endl;
    return false;
  }
  else
  {
    cout << "File: " << InputFile << " exists." << endl;
    fclose( fp );
    return true;
  }
}

int CIntrinsic::occurrencesInFile( const string& str, const string& fileName )
{
  int      stringCounter = 0;
  ifstream filePtr;
  string  stringInFile;

  filePtr.open( fileName.c_str() );

  do
  {
    filePtr >> stringInFile;
    size_t found = stringInFile.find(str);
    if ( found == 0 )
    {
      stringCounter++;
    }
  } while ( !filePtr.eof() );

  filePtr.close();

  return stringCounter;
}

bool CIntrinsic::readToString( string str, ifstream& filePtr )
{
  string  stringInFile;
  size_t  found;

  do
  {
    filePtr >> stringInFile;

    found = stringInFile.find(str);

    if ( found == 0 )
    {
      cout << "readToString is working: " << stringInFile << " " << str << endl;
      return true;
    }
  } while ( !filePtr.eof() );

  return false;
}

string CIntrinsic::readFullString( string str, ifstream& filePtr )
{
  string  stringInFile;
  string  newString;
  size_t  found;

  do
  {
    filePtr >> stringInFile;

    found = stringInFile.find( str );

    if ( found != string::npos )
    {
      break;
    }
  } while ( !filePtr.eof() );

  newString = stringInFile;

  return newString;
}

bool CIntrinsic::readToString( string str, ifstream& filePtr, const string& terminationString )
{
  string  stringInFile;
  size_t  found;

  do
  {
    filePtr >> stringInFile;

    found = stringInFile.find(str);

    if ( found == 0 )
    {
    	cout << "Read to termination: " << stringInFile << "," << str << "," << terminationString << endl;
    	return true;
    }
  } while ( !filePtr.eof() && stringInFile != terminationString );

  return false;
}


void CIntrinsic::CheckParameterFileExists( char* InputFile )
{
  FILE *fp;

  if ( (fp = fopen( InputFile, "r" )) == NULL )
  {
    cout << "Parameter File: " << InputFile << " does not exist." << endl;
    exit(-1);
  }
  else
  {
    cout << "File: " << InputFile << " exists." << endl;
    fclose( fp );
  }
}

void CIntrinsic::CheckTerrainFileExists( char* InputFile )
{
  FILE *fp;

  if ( (fp = fopen( InputFile, "r" )) == NULL )
  {
    cout << "File: " << InputFile << " does not exist." << endl;
    exit(-1);
  }
  else
  {
    cout << "File: " << InputFile << " exists." << endl;
    fclose( fp );
  }
}

void CIntrinsic::PrintTen( int i )
{
  cout << "   " << i;
  if ( fmod( double(i), 10.0 ) == 0.0 ) cout << endl;
}

void CIntrinsic::swap( complex& x, complex& y )
{
  complex temp = x;

  x = y;
  y = temp;
}
 
void CIntrinsic::swap( double& x, double& y )
{
  double temp = x;

  x = y;
  y = temp;
}

void CIntrinsic::swap( long& i, long& j )
{
  long temp = i;

  i = j;
  j = temp;
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
int CIntrinsic::StartClock()
{
  return( clock() );
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void CIntrinsic::ClockTime( int StartTime )
{
  double ClockTime;

  ClockTime = double( clock() - StartTime ) / double(CLOCKS_PER_SEC); 
  cout << "Elapsed Time: " << ClockTime << endl;
}

void CIntrinsic::ClockTime( int StartTime, const string& str )
{
  double ClockTime;

  ClockTime = double( clock() - StartTime ) / double(CLOCKS_PER_SEC);
  cout << str << ": " << ClockTime << "seconds" << endl << endl;
}

void CIntrinsic::IntegerToHexidecimal( int i, string& str )
{
  int     FirstDigit, SecondDigit;
  char    FirstHex,   SecondHex;

  if ( i == 256 || i < 0 )
    cout << i << " Error " << endl;
  FirstDigit  = int( double(i) / 16.0 );
  SecondDigit = int( fmod( double(i),  16.0 ) );
  FirstHex    = OneIntegerToHexidecimal( FirstDigit );
  SecondHex   = OneIntegerToHexidecimal( SecondDigit );
  str  = FirstHex + SecondHex;
}

char CIntrinsic::OneIntegerToHexidecimal( int i )
{
  char str;

  switch( i )
  {
    case 0: str = '0';
            break;
    case 1: str = '1';
            break;
    case 2: str = '2';
            break;
    case 3: str = '3';
            break;
    case 4: str = '4';
            break;
    case 5: str = '5';
            break;
    case 6: str = '6';
            break;
    case 7: str = '7';
            break;
    case 8: str = '8';
            break;
    case 9: str = '9';
            break;
    case 10: str = 'a';
            break;
    case 11: str = 'b';
            break;
    case 12: str = 'c';
            break;
    case 13: str = 'd';
            break;
    case 14: str = 'e';
            break;
    case 15: str = 'f';
            break;
    default:
            cout << "Error in Hexidecimal conversion: " << i << endl;
            str = '0';
  }

  return( str );
}

double CIntrinsic::mantissa( double FullDoubleNumber )
{
  return( FullDoubleNumber - int( FullDoubleNumber ) );
}

double CIntrinsic::RadiansToDegrees( double AngleInRadians )
{
  return( 180.0 * AngleInRadians / Pi );
}

double CIntrinsic::DegreesToRadians( double AngleInDegrees )
{
  return( Pi * AngleInDegrees / 180.0 );
}
