boolean CRayTracing::diffractionCoefficient( CComplexVector&   Ei,
                                       const CSegment3d&       si,
                                       const CSegment3d&       sr,
                                       const double&           pathLength,
                                       const double&           totalPathLength,
                                       const CPointInfo&       diffractionPt,
                                       const CSegment3d&       diffractingEdge,
                                       const CBuilding&        buildingObject,
                                       const CBaseTransceiver& btsObject,
                                       const mediaType&        media )
{
  CConvex   *convexSpace;
  CBoundary *boundary0, *boundary1;
  CPoint3d  n0_hat, n1_hat, sihat, srhat, ehat, si_cross_e, sr_cross_e;
  CPoint3d  e_cross_si, e_cross_sr;
  CPoint3d  phi_dash_hat, phi_hat, beta0_dash_hat, beta0_hat;
  double    k1, p, s, n, attenuation, sinBeta0, anAngle, phi, phi_dash;
  double    cosTheta_i0, cosTheta_i1, angle2, astigmatic;
  complex   Rparallel1, Rperpendicular1, Rparallel2, Rperpendicular2;
  complex   Dsoft, Dhard, phase, permittivity;
  CPoint3d  t0_hat, t1_hat, minusSt_dash_hat, st_hat, st_dash, st;
  complex   Ei_beta0_dash, Ei_phi_dash, Ed_beta0, Ed_phi;

  CDiffractionCoefficient diffractionObject;

  // define the boundaries of the convex space containing the diffraction edge
  convexSpace = &buildingObject.convexArray_[diffractionPt.convexNumber];
  boundary0   = &convexSpace->facet[diffractionPt.boundaryNumber1];
  boundary1   = &convexSpace->facet[diffractionPt.boundaryNumber2];

  // define unit vector of all vectors (needed below)
  n0_hat = boundary0->equation_.outwardNormal_;
  n1_hat = boundary1->equation_.outwardNormal_;
  if ( n0_hat != CPoint3d( 0.0, 0.0, 1.0 )  )
  {
    cout << "n0_hat: " << n0_hat << "n1_hat: " << n1_hat << endl;
  }

  if ( n0_hat != CPoint3d( 0.0, 0.0, 1.0 )
    && n1_hat != CPoint3d( 0.0, 0.0, 1.0 ) )
  {
    Ei = CComplexVector();
    return false;
  }

  if ( n0_hat == CPoint3d( 0.0, 0.0, 1.0 )  )
    n1_hat = CPoint3d( 0.0, 0.0, -1.0 );
  sihat  = si.unitVector();
  srhat  = sr.unitVector();
  ehat   = diffractingEdge.unitVector();
  t0_hat = n0_hat ^ ehat;

  // check just inside the boundary to see if t0_hat is contained.
  // if not, then we must ensure that it will be
  if ( boundary0->intersection( diffractionPt.node + t0_hat * TOL3 ) == false )
  {
    ehat   = ehat * -1.0;
    t0_hat = n0_hat ^ ehat; 
    //cout << "Got t0 the wrong way around in this instance" << endl;
  }
  if ( boundary0->intersection( diffractionPt.node + t0_hat * TOL3 ) == false )
  {
    cout << "t0 not contained in plane" << endl;
    cout << "t0: " << t0_hat << "  n0: " << n0_hat << endl;
    cout << "edge: " << ehat << endl;
    cout << "boundary: " << *boundary0 << endl;
    exit(1);
  }

  t1_hat = n1_hat ^ ehat;

  //if ( boundary1->intersection( diffractionPt.node + t1_hat * TOL3 ) == false )
  if ( boundary0->intersection( diffractionPt.node + t1_hat * TOL3 ) == false )
  {
    t1_hat = t1_hat * -1.0;
  }

  ///if ( boundary1->intersection(diffractionPt.node + t1_hat * TOL3 ) == false)
  if ( boundary0->intersection( diffractionPt.node + t1_hat * TOL3 ) == false )
  {
    cout << "t1 not contained in plane" << endl;
    exit(1);
  }

  // pre-define some vectors to speed up the computation
  si_cross_e = sihat ^ ehat;
  e_cross_si = ehat ^ sihat;

  if ( si_cross_e == CPoint3d( 0.0, 0.0, 0.0 ) )
    cout << "Tried to get a diffraction point along edge" << endl;

  sr_cross_e = srhat ^ ehat;
  e_cross_sr = ehat ^ srhat;
  //sinBeta0   = si_cross_e.abs();
  sinBeta0   = e_cross_si.abs();

  // define ray-based orthogonal basis
  /*phi_dash_hat   = si_cross_e / sinBeta0;
  beta0_dash_hat = sihat ^ phi_dash_hat;
  phi_hat        = sr_cross_e / sinBeta0;
  beta0_hat      = srhat ^ phi_hat;*/
  phi_dash_hat   = e_cross_si / (-sinBeta0);
  beta0_dash_hat = phi_dash_hat ^ sihat;
  phi_hat        = e_cross_sr / sinBeta0;
  beta0_hat      = phi_hat ^ srhat;

  // define vector lengths along the ray
  //p  = si.length();
  //s  = sr.length();
  p  = pathLength;
  s  = totalPathLength - pathLength;
  k1 = btsObject.wavenumber();

  // calculation of phi and phi' angles is difficult but follows. Requires a 
  // projection along both wedge faces in the direction perpendicular to the 
  // edge. This help us find the angles using that axis and the outward normal.


  st_dash          = ( sihat - ehat * ( sihat * ehat ));
  minusSt_dash_hat = st_dash / -sinBeta0;
  st               = ( srhat - ehat * ( srhat * ehat )); 
  st_hat           = st / st.abs();
  n                = 2.0 - t0_hat.angle( t1_hat ) / Pi;

  // found this in McNamara pp.268. Obviously he found a trick for getting the 
  // correct angle every time. Otherwise you would need to use if statements.
  // angles must be in the range [0,Pi] from Point3d class otherwise the
  // angles returned would be wrong. sign(*) acts as the if statement.
  anAngle     = minusSt_dash_hat.angle(t0_hat);
  angle2      = minusSt_dash_hat.angle(n0_hat);
  cosTheta_i0 = cos( angle2 );
  phi_dash    = Pi - ( Pi - anAngle ) * sign( minusSt_dash_hat * n0_hat );
  anAngle     = st_hat.angle(t0_hat);
  angle2      = st_hat.angle(n0_hat);
  cosTheta_i1 = cos( angle2 );
  phi         = Pi - ( Pi - anAngle ) * sign( st_hat * n0_hat );

  // convert incident field into the local ray-based coordinate system
  Ei_beta0_dash = Ei * beta0_dash_hat;
  Ei_phi_dash   = Ei * phi_dash_hat;

  // define the permittivity of the interfacing media for different types
  if ( media == PEC )
    permittivity  = 0.0;
  else if ( media == LOSSLESS )
    permittivity  = convexSpace->permittivity_;
  else if ( media == LOSSY )
    permittivity = convexSpace->complexPermittivity( btsObject );
    
  // components of reflection coefficient along the incoming ray
  // from both side of the wedge
  localReflectionCoefficient( cosTheta_i0, permittivity, media, 
                              Rparallel1, Rperpendicular1 );

  localReflectionCoefficient( cosTheta_i1, permittivity, media, 
                              Rparallel2, Rperpendicular2 );

  astigmatic     = sqrt( p / s / ( p + s ) );
  //attenuation    = ( p * s * qsqr( sinBeta0 ) ) / ( p + s );
  attenuation    = ( p * s ) / ( p + s );
  phase          = expminusj( k1 * s );

  //cout << "---------------------------------------------" << endl;
  //cout << "Ei = " << Ei << endl;
  // build soft (E-polarized) are hard (H-polarized) diffraction coefficients
  Dsoft = diffractionObject.softDiffraction( phi, phi_dash, n, k1, sinBeta0, 
                                             attenuation, Rperpendicular1, 
                                             Rperpendicular2 );

  Dhard = diffractionObject.hardDiffraction( phi, phi_dash, n, k1, sinBeta0,
                                             attenuation, Rparallel1, 
                                             Rparallel2 );
  //cout << "Dsoft: " << Dsoft << "  Dhard: " << Dhard << endl;

  // define the reflection matrix for different dielectric types
  // multiply out all terms to give the electric field
  Ed_beta0 = Dsoft * Ei_beta0_dash * astigmatic * phase;
  Ed_phi   = Dhard * Ei_phi_dash   * astigmatic * phase;

  // convert back into global coordinate system
  /*if ( fabs( sinBeta0 - 1.0 ) > TOL )
  {
    Ei = CComplexVector();
    cout << "diffraction coefficients off line" << endl;
    return true;
  }*/

  Ei = ( CComplexVector( beta0_hat ) * Ed_beta0 * -1.0
     - CComplexVector( phi_hat   ) * Ed_phi );
  //randomVariable( btsObject, Ei );

  /*cout << "si    = " << si << endl;
  cout << "sihat = " << sihat << endl;
  cout << "sr    = " << sr << endl;
  cout << "srhat = " << srhat << endl;
  cout << "t0hat = " << t0_hat << endl;
  cout << "t1hat = " << t1_hat << endl;
  cout << "ehat  = " << ehat  << endl;
  cout << "k1 (beta) = " << k1  << endl;
  cout << "beta0_hat = " << beta0_hat  << endl;
  cout << "phi_hat  = " << phi_hat  << endl;
  cout << "beta0_dash_hat = " << beta0_dash_hat  << endl;
  cout << "phi_dash_hat   = " << phi_dash_hat  << endl;
  cout << "sinBeta0 = " << sinBeta0 << endl;
  cout << "attenuation = " << attenuation << endl;
  cout << "edge  = " << diffractingEdge << endl;
  cout << "phi\'  = " << phi_dash << endl;
  cout << "phi   = " << phi << endl;
  cout << "Dsoft = " << Dsoft << endl;
  cout << "Dhard = " << Dhard << endl;
  cout << "Ed_beta0 = " << Ed_beta0 << endl;
  cout << "Ed_phi   = " << Ed_phi << endl;
  cout << "Es       = " << Ei << endl;*/

  return true;
}

