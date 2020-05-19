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

#include "storage.hh"

int*  CMemoryStorage::CreateIntegerVector(int Num )
{
  int* p;
  int  i;

  p = new int[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = 0;

  return p;
}
 

boolean*  CMemoryStorage::CreateBoolVector(int Num )
{
  boolean* p;
  int      i;

  p = new boolean[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = 0;

  return p;
}

double*  CMemoryStorage::CreateDoubleVector(int Num )
{
  double* p;
  int     i;

  p = new double[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = 0.0;

  return p;
}
 
CComplexVector*  CMemoryStorage::CreateComplex3dVector(int Num )
{
  CComplexVector* p;
  int             i;

  p = new CComplexVector[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CComplexVector();

  return p;
}

complex*  CMemoryStorage::CreateComplexVector(int Num )
{
  complex* p;
  int      i;

  p = new complex[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = complex( 0.0, 0.0 );

  return p;
}

CConvex*  CMemoryStorage::CreateConvexVector(int Num )
{
  CConvex* p;
  int      i;

  p = new CConvex[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CConvex();

  return p;
}

CRoom*  CMemoryStorage::CreateRoomVector(int Num )
{
  CRoom* p;
  int    i;

  p = new CRoom[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CRoom();

  return p;
}

CFloor*  CMemoryStorage::CreateFloorVector(int Num )
{
  CFloor* p;
  int    i;

  p = new CFloor[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CFloor();

  return p;
}

CStory*  CMemoryStorage::CreateStoryVector(int Num )
{
  CStory* p;
  int    i;

  p = new CStory[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CStory();

  return p;
}

CPoint3d*  CMemoryStorage::CreatePoint3dVector( int Num )
{
  CPoint3d* p;
  int        i;

  p = new CPoint3d[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CPoint3d( 0.0, 0.0, 0.0 );

  return p;
}

CImage*  CMemoryStorage::CreateImageVector(int Num )
{
  CImage* p;
  int    i;

  p = new CImage[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CImage();

  return p;
}

CEdgeList*  CMemoryStorage::CreateEdgeList(int Num )
{
  CEdgeList* p;
  int    i;

  p = new CEdgeList[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CEdgeList();

  return p;
}

List<CImage>*  CMemoryStorage::CreateImageList(int Num )
{
  List<CImage>* p;
  //int    i;

  p = new List<CImage>[Num];
  //for ( i = 0; i < Num; i++ )
    //p[i] = List<CImage>;

  return p;
}

CEdge*  CMemoryStorage::CreateEdgeVector(int Num )
{
  CEdge* p;
  int    i;

  p = new CEdge[Num];
  for ( i = 0; i < Num; i++ )
    p[i] = CEdge();

  return p;
}

int**  CMemoryStorage::CreateIntegerMatrix(int NumRows, int* NumCols )
{
  int  i, j;
  int** p;

  p = new int* [NumRows];

  for ( i = 0; i < NumRows; i++ )
  {
    j    = NumCols[i];
    p[i] = new int [j];
  }

  for( i = 0; i < NumRows; i++ )
    for ( j = 0; j < NumCols[i]; j++ )
      p[i][j] = 0;

  return (p);
}
 
int**  CMemoryStorage::CreateIntegerMatrix(int NumRows, int NumCols )
{ 
  int  i, j;
  int** p;
  p = new int* [NumRows] ;

  for(i=0;i<NumRows;i++) 
  {
    p[i] = new int [NumCols];
  }
  for(i=0;i<NumRows;i++) 
  {
    for(j=0;j<NumCols;j++) 
    {
      p[i][j] = 0 ;
    }
  }

  return (p);
}

double**  CMemoryStorage::CreateDoubleMatrix( int NumRows, int NumCols )
{ 
  int  i, j;
  double** p ;

  p = new double* [NumRows] ;
  for(i=0;i<NumRows;i++) 
  {
    p[i] = new double [NumCols];
  }

  for(i=0;i<NumRows;i++) 
  {
    for(j=0;j<NumCols;j++) 
    {
      p[i][j] = 0.0 ;
    }
  }

  return (p);
}

boolean**  CMemoryStorage::CreateBoolMatrix( int NumRows, int NumCols )
{ 
  int  i, j;
  boolean** p ;

  p = new boolean* [NumRows] ;
  for(i=0;i<NumRows;i++) 
  {
    p[i] = new boolean [NumCols];
  }

  for(i=0;i<NumRows;i++) 
  {
    for(j=0;j<NumCols;j++) 
    {
      p[i][j] = false;
    }
  }

  return (p);
}

CComplexVector** CMemoryStorage::CreateComplexVectorMatrix( int NumRows, int NumCols )
{ 
  int  i, j;
  CComplexVector  **p ;
 
  p = new CComplexVector* [NumRows] ;
  for ( i = 0; i < NumRows; i++ ) 
  {
    p[i] = new CComplexVector [NumCols];
  }

  for ( i = 0; i < NumRows; i++ ) 
  {
    for ( j = 0; j < NumCols; j++ ) 
      p[i][j] = CComplexVector();
  }
  return (p);
} 

complex** CMemoryStorage::CreateComplexMatrix( int NumRows, int NumCols )
{ 
  int  i, j;
  complex  **p ;
 
  p = new complex* [NumRows] ;
  for ( i = 0; i < NumRows; i++ ) 
  {
    p[i] = new complex [NumCols];
  }

  for ( i = 0; i < NumRows; i++ ) 
  {
    for ( j = 0; j < NumCols; j++ ) 
      p[i][j] = complex( 0.0, 0.0 );
  }
  return (p);
} 


complex** CMemoryStorage::CreateComplexvarMatrix( int NumRows, int* Columns )
{ 
  int  i, j, NumCols;
  complex  **p ;
 
  p = new complex* [NumRows];

  for ( i = 0; i < NumRows; i++ ) 
  {
    NumCols = Columns[i];
    p[i] = new complex [NumCols];
  }

  for ( i = 0; i < NumRows; i++ ) 
  {
    for ( j = 0; j < (Columns[i] + 1); j++ ) 
      p[i][j] = complex( 0.0, 0.0 );
  }

  return (p);
} 


CPoint3d** CMemoryStorage::CreatePoint3dMatrix( int NumRows, int NumCols )
{ 
  int        i;
  CPoint3d  **p;
 
  p = new CPoint3d* [NumRows];
  for(i=0;i<NumRows;i++) 
  {
    p[i] = new CPoint3d [NumCols];
  }

  return (p);
} 


CPoint3d*** CMemoryStorage::CreatePoint3dtripleMatrix( int NumRows, int NumCols,
                                                       int NumIndex3 )
{ 
  int  i, j ;
  CPoint3d  ***p ;
 
  p = new CPoint3d** [NumRows] ;
  for(i=0;i<NumRows;i++) 
  {
     p[i] = new CPoint3d* [NumCols];
     for(j=0;j<NumCols;j++)
     {
      p[i][j] = new CPoint3d [NumIndex3] ;
     }
  } 

  return (p);
} 

double*** CMemoryStorage::CreateDouble3dMatrix( int NumRows, int NumCols,
                                                int NumIndex3 )
{ 
  int    i, j, k;
  double ***p;
 
  p = new double** [NumRows];
  for ( i = 0; i < NumRows; i++ ) 
  {
    p[i] = new double* [NumCols];
    for ( j = 0; j < NumCols; j++ )
    {
      p[i][j] = new double [NumIndex3];
        for ( k = 0; k < NumIndex3; k++ )
          p[i][j][k] = 0.0;
    }
  } 

  return (p);
} 

complex*** CMemoryStorage::CreateComplex3dMatrix( int NumRows, int NumCols,
                                                  int NumIndex3 )
{ 
  int     i, j, k;
  complex ***p;
 
  p = new complex** [NumRows];
  for ( i = 0; i < NumRows; i++ ) 
  {
    p[i] = new complex* [NumCols];
    for ( j = 0; j < NumCols; j++ )
    {
      p[i][j] = new complex [NumIndex3];
        for ( k = 0; k < NumIndex3; k++ )
          p[i][j][k] = 0.0;
    }
  } 

  return (p);
} 


void  CMemoryStorage::DeleteIntegerMatrix(int** p, int NumRows )
{ 
  int  i ;

  for ( i = NumRows; i > 0; i-- ) 
  {
    delete [] p[i-1];
  }
  delete [] p;
}

void  CMemoryStorage::DeleteDoubleMatrix(double** p, int NumRows )
{ 
  int  i;

  for(i=NumRows;i>0;i--) 
  {
    delete [] p[i-1];
  }
  delete [] p;
}

void  CMemoryStorage::DeleteBoolMatrix( boolean** p, int NumRows )
{ 
  int  i;

  for(i=NumRows;i>0;i--) 
  {
    delete [] p[i-1];
  }
  delete [] p;
}


void  CMemoryStorage::DeleteComplexMatrix( complex** p, int NumRows )
{ 
  int  i ;

  for(i=NumRows;i>0;i--)
  {
    delete [] p[i-1];
  }
    delete [] p;
}

void  CMemoryStorage::DeleteComplexVectorMatrix( CComplexVector** p, 
                                                 int              NumRows )
{ 
  int  i ;

  for(i=NumRows;i>0;i--) 
  {
    delete [] p[i-1];
  }
  delete [] p;
}

void  CMemoryStorage::DeleteComplexvarMatrix( complex** p, int NumRows )
{ 
  int  i;

  for(i=NumRows;i>0;i--) 
  {
    delete [] p[i-1];
  }
  delete [] p;
}

void  CMemoryStorage::DeletePoint3dMatrix( CPoint3d** p, int NumRows )
{ 
  int  i ;

  for(i=NumRows;i>0;i--) 
  {
    delete [] p[i-1];
  }
  delete [] p;
}

void  CMemoryStorage::DeleteComplex3dMatrix( complex*** p, int NumRows,
                                                         int NumCols  )
{ 
  int i, j;

  for(i=NumRows;i>0;i--) 
  {
    for(j=NumCols;j>0;j--)
    {
     delete [] p[i-1][j-1];
    }
    delete [] p[i-1];
  }
  delete [] p;
}

void  CMemoryStorage::DeleteDouble3dMatrix( double*** p, int NumRows,
                                                         int NumCols  )
{ 
  int i, j;

  for(i=NumRows;i>0;i--) 
  {
    for(j=NumCols;j>0;j--)
    {
     delete [] p[i-1][j-1];
    }
    delete [] p[i-1];
  }
  delete [] p;
}


void  CMemoryStorage::DeletePoint3dtripleMatrix( CPoint3d*** p, int NumRows,
                                                            int NumCols  )
{ 
  int  i, j ;

  for(i=NumRows;i>0;i--) 
  {
    for(j=NumCols;j>0;j--)
    {
     delete [] p[i-1][j-1];
    }
    delete [] p[i-1];
  }
  delete [] p;
}

void CMemoryStorage::DeleteIntegerVector( int* p )
{
 delete [] p;
} 


void CMemoryStorage::DeleteBoolVector( boolean* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteComplex3dVector( CComplexVector* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteConvexVector( CConvex* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteRoomVector( CRoom* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteStoryVector( CStory* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteFloorVector( CFloor* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteComplexVector( complex* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteDoubleVector( double* p )
{
 delete [] p;
} 

void CMemoryStorage::DeletePoint3dVector( CPoint3d* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteImageVector( CImage* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteEdgeVector( CEdge* p )
{
 delete [] p;
} 

void CMemoryStorage::DeleteEdgeList( CEdgeList* p )
{
  delete [] p;
} 

void CMemoryStorage::DeleteImageList( List<CImage>* p )
{
  delete [] p;
} 
