% read in the direct rays from the file reflection.txt
function y = reflect0( fileName )

fid   = fopen( fileName );
Nrays = fscanf( fid, '%lf\n', 1 );
z     = zeros( 3, 4 );

for n = 1:Nrays
  Norder = fscanf( fid, '%lf\n', 1 );

  for m = 1:Norder
    z(1,m) = fscanf( fid, '%lf\n', 1 );
    z(2,m) = fscanf( fid, '%lf\n', 1 );
    z(3,m) = fscanf( fid, '%lf\n', 1 );
  end;

  if ( Norder == 2 )
    plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder)+0.03, 'm-' );
  end;

end;

fclose( fid );

