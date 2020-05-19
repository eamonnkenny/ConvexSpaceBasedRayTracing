% read in the rays from the file

fid   = fopen( 'rays.txt' );
Nrays = fscanf( fid, '%lf\n', 1 );
z     = zeros( 3, 5 );

for n = 1:Nrays
  Norder = fscanf( fid, '%lf\n', 1 );

  for m = 1:Norder
    z(1,m) = fscanf( fid, '%lf\n', 1 );
    z(2,m) = fscanf( fid, '%lf\n', 1 );
    z(3,m) = fscanf( fid, '%lf\n', 1 );
  end;


  if ( Norder == 3 )
    plot( z(1,1:Norder), z(2,1:Norder), 'r-' );
  end;

  if ( Norder == 4 )
    plot( z(1,1:Norder), z(2,1:Norder), 'g-' );
  end;

  if ( Norder == 5 )
    plot( z(1,1:Norder), z(2,1:Norder), 'b-' );
  end;

end;

fclose( fid );

