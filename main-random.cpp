#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include "complex.hh"

int main()
{
  time_t ltime;

  srand( (unsigned) time( NULL ) );
  time( &ltime );
  //seed( ltime );
  cout << expj( 2.0 * 3.1412 * double( rand() ) / double( RAND_MAX ) ) << endl;
}
