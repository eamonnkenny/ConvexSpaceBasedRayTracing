//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 2001
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "diagram.hh"

CDiagram::CDiagram()
{
  numberOfHorizontalSteps_ = 0;
  numberOfVerticalSteps_   = 0;
  maximumGain_             = 0.0;
  horizontalStep_          = 0.0;
  verticalStep_            = 0.0;
}

string CDiagram::fullDirectoryPath( const string& FileName )
{
  string fullPath;
  const char* separator = "/";
  fullPath = CProperties::diagramDirectory + separator + FileName;
  return fullPath;
}


CDiagram& CDiagram::read( const string& FileName )
{
  int      i;
  ifstream cinput;
  string  dummyName, totalPathToFileName;

  cinput.open( fullDirectoryPath(FileName).c_str() );
 
  if ( !cinput.good() )
  {
    cerr << "CDiagram filename: " << totalPathToFileName 
         << " does not exist" << endl;
    exit(1);
  }

  cinput >> dummyName;
  cout << "Name: " << dummyName << endl;
  cinput >> maximumGain_;
  cout << "Maximum Gain: " << maximumGain_ << endl;
  cinput >> dummyName;
  cinput >> numberOfVerticalSteps_;
  cout << "Num Vertical: " << numberOfVerticalSteps_ << endl;
  cinput >> dummyName;
  cinput >> numberOfHorizontalSteps_;
  cout << "Num Horizontal: " << numberOfHorizontalSteps_ << endl;

  verticalStep_   = 360.0 / double( numberOfVerticalSteps_ );
  horizontalStep_ = 360.0 / double( numberOfHorizontalSteps_ );

  gain_ = new double [totalPoints()+1];
  cout << "Total points in diagram: " << totalPoints() << endl;
  
  for ( i = 0; i < totalPoints(); i++ )
    cinput >> gain_[i];

  return *this;
}

void CDiagram::setValues( const double& maximumGain, 
                          const int&    numberOfVerticalSteps,
                          const int&    numberOfHorizontalSteps,
                                double* gain )
{
  int i;

  maximumGain_             = maximumGain;
  numberOfVerticalSteps_   = numberOfVerticalSteps;
  numberOfHorizontalSteps_ = numberOfHorizontalSteps;
  verticalStep_            = 360.0 / double( numberOfVerticalSteps );
  horizontalStep_          = 360.0 / double( numberOfHorizontalSteps );

  gain_ = new double [totalPoints()+1];
  
  for ( i = 0; i < totalPoints(); i++ )
    gain_[i] = gain[i];
}

double CDiagram::gain( const double& theta, const double& phi ) const
{
  int    i, j; 
  double interpolant, w_i, w_j, thetaStep, phiStep;
  
  thetaStep = theta / verticalStep_;
  phiStep   = phi   / horizontalStep_;
  i          = int( thetaStep + TOL/2.0 );
  j          = int( phiStep   + TOL/2.0 );

  if ( fabs( thetaStep - double(i) ) < TOL 
    && fabs( phiStep   - double(j) ) < TOL )
  {
    return ( gain( i, j ) );
  }
  else if ( fabs( thetaStep - double(i) ) < TOL )
  {
    w_j = double( j + 1 - phiStep );
    interpolant = w_j * gain( i, j ) + ( 1.0 - w_j ) * gain( i, j+1 );
    return (interpolant);
  }
  else if ( fabs( phiStep - double(j) ) < TOL )
  {
    w_i = double( i + 1 - thetaStep );
    interpolant = w_i * gain( i, j ) + ( 1.0 - w_i ) * gain( i+1, j );
    return (interpolant);
  }
  else
  {
    w_i = double( i + 1 - thetaStep );
    w_j = double( j + 1 - phiStep );

    if ( i < numberOfVerticalSteps_ && j < numberOfHorizontalSteps_ 
      && i >= 0 && j >= 0 )
    {
      interpolant =           w_i *           w_j * gain( i, j )
                  + ( 1.0 - w_i ) *           w_j * gain( i+1, j )
                  +           w_i * ( 1.0 - w_j ) * gain( i, j+1 )
                  + ( 1.0 - w_i ) * ( 1.0 - w_j ) * gain( i+1, j+1 );
      return interpolant;
    }
    else
    {
      cerr << "CDiagram: double in gain(x,y) outside the poincare sphere." 
           << endl;
      cerr << "( theta, phi ): ( " << theta << ", " << phi << ")" << endl;
      cerr << "(i,j):          ( " << i     << ", " << j   << ")" << endl;
      exit(2);
    }
  }
}

CDiagram& CDiagram::operator=( const CDiagram& otherDiagram )
{
  int i;

  if ( this != &otherDiagram )
  {
    clear();

    numberOfVerticalSteps_   = otherDiagram.numberOfVerticalSteps_;
    numberOfHorizontalSteps_ = otherDiagram.numberOfHorizontalSteps_;
    verticalStep_           = otherDiagram.verticalStep_;
    horizontalStep_         = otherDiagram.horizontalStep_;
    maximumGain_             = otherDiagram.maximumGain_;
  
    gain_ = new double [totalPoints()+1];
  
    for ( i = 0; i < totalPoints(); i++ )
      gain_[i] = otherDiagram.gain_[i]; 
  }

  return *this;
}

/*CComplexVector CDiagram::hertzianDipole( 
                                        const double& r,   const double& theta, 
                                        const double& phi, const double& ki )
                                        const
{
  CComplexVector Eipolar;
  double         Io = 1.0, l1, l=0.1;

  l1        = ki * l / 2.0;
  Eipolar.x = ComplexZero;
  Eipolar.y = complex( 0.0, Impedance * Io ) 
            * qsqr( ( cos( l1 * cos( theta ) ) - cos( l1 ) ) / sin( theta ) )
            * CMaths::expminusj( ki * r )
            * gaussianBeam( theta, phi ) / ( TwoPi * r );
  Eipolar.z = ComplexZero;

  return Eipolar.sphericalToRectangular( theta, phi );
}*/

CComplexVector CDiagram::hertzianDipole( const double& r, 
                                         const CPoint3d& x_l, 
                                         const CPoint3d& y_l, 
                                         const CPoint3d& z_l,  
                                         const double& theta,
                                         const double& phi, const double& ki )
                                         const
{
  CComplexVector Eipolar;
  double         Io = 1.0, l1;
  ofstream       coutput;

  //coutput.open( "pathloss.txt", ios::app );

  l1        = Pi / ki;  // lambda / 2 = pi / ki = length of dipole
  Eipolar.x = ComplexZero;
  Eipolar.y = complex( 0.0, ki * Impedance * Io / (4.0 * Pi * r) ) 
            * l1 * CMaths::expminusj( ki * r ) * sin( theta );
  //Eipolar.y = complex( 0.0,  1.0 / r) * CMaths::expminusj( ki * r ) * sin( theta );
  Eipolar.z = ComplexZero;
  //cout << r << "  " << Eipolar.y << "  " << sin( theta ) << endl;

  //coutput << Eipolar.sphericalToRectangular( x_l, y_l, z_l, theta, phi )<<endl;

  return Eipolar.sphericalToRectangular( x_l, y_l, z_l, theta, phi );
}

CComplexVector CDiagram::imageDipole( const double& r,     const CPoint3d& x_l,
                                      const CPoint3d& y_l, const CPoint3d& z_l, 
                                      const double& theta,
                                      const double& phi, const double& ki )
                                      const
{
  CComplexVector Eipolar;
  double         Io = -1.0, l1;

  l1        = Pi / ki;  // lambda / 2 = pi / ki = length of dipole
  Eipolar.x = ComplexZero;
  Eipolar.y = complex( 0.0, ki * Impedance * Io / (4.0 * Pi * r) ) 
            * l1 * CMaths::expminusj( ki * r ) * sin( theta );
  Eipolar.z = ComplexZero;

  return Eipolar.sphericalToRectangular( x_l, y_l, z_l, theta, phi );
}

complex CDiagram::gaussianBeam( const double& theta, const double& phi ) const
{
  complex G, C;
  int     i;

  G = ComplexZero;

  for ( i = 0; i < 10; i++ )
  {
    C  = complex( random(i), random(i+10) );
    G += C * CMaths::expminusj( Pi * random(i) + Pi * (i-1) * cos( phi ) )
       * 1.5 * sin( theta ) * sin( theta );
  }
  return G;
}

CComplexVector CDiagram::omniDirectional(const double& r,  
                                         const CPoint3d& x_l, 
                                         const CPoint3d& y_l, 
                                         const CPoint3d& z_l, 
                                         const double& theta,
                                         const double& phi, const double& ki )
                                         const
{
  CComplexVector Eipolar;
  Eipolar.x = ComplexZero;
  Eipolar.y = CMaths::expminusj( ki * r ) / r;
  Eipolar.z = ComplexZero;

  return Eipolar.sphericalToRectangular( x_l, y_l, z_l, theta, phi );
}

CComplexVector CDiagram::imageOmni(const double& r,   const double& theta,
                                   const double& phi, const double& ki ) const
{
  CComplexVector Eipolar;
  Eipolar.x = ComplexZero;
  Eipolar.y = CMaths::expminusj( ki * r ) / r;
  Eipolar.z = ComplexZero;

  return Eipolar.sphericalToRectangular( theta, phi );
}

double CDiagram::random( const int& i ) const
{
  srand(i); // generate a seed
  return ( double( rand() ) / double( RAND_MAX ) );
}
