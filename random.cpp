double meanAbsSum( List<CRayPath>& allRayPaths )
{
  double   R;
  CRayPath *rayPath;
  complex  fieldStrength, expjtest, temp;
  int      i, l, numberOfIterations = 100;

  for ( i = 0; i < numberOfIterations; i++ )
  {
    temp = ComplexZero;
    for ( l = 0; l < allRayPaths.size(); l++ )
    {
      rayPath        = &allRayPaths.GetItem(l);
      fieldStrength  = rayPath->fieldStrength_[2].z;

      expjtest = expj( TwoPi * ( double(rand()) / double(RAND_MAX) ) );
      temp += fieldStrength * expjtest;
    }
    R += fabs( temp );
  }

  R = R / numberOfIterations;
  return R;
}
