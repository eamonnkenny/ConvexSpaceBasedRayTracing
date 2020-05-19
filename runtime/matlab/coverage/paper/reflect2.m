% read in the rays from the file
function y = reflect2( fileName )

fid   = fopen( fileName );
Nrays = fscanf( fid, '%lf\n', 1 );
z     = zeros( 3, 5 );

for n = 1:Nrays
  Norder = fscanf( fid, '%lf\n', 1 );

  for m = 1:Norder
    z(1,m) = fscanf( fid, '%lf\n', 1 );
    z(2,m) = fscanf( fid, '%lf\n', 1 );
    z(3,m) = fscanf( fid, '%lf\n', 1 );
  end;

  if ( Norder == 4 )
    plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder)+0.01, 'g-' );
  end;

end;

fclose( fid );

