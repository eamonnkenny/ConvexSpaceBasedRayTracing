% read in the rays from the file
fid   = fopen( 'fullrays.txt' );
Nrays = fscanf( fid, '%lf\n', 1 );

%l = 1;
for n = 1:Nrays
  Norder = fscanf( fid, '%lf\n', 1 );
  z      = zeros( 3, Norder );

  for m = 1:Norder
    z(1,m) = fscanf( fid, '%lf\n', 1 );
    z(2,m) = fscanf( fid, '%lf\n', 1 );
    z(3,m) = fscanf( fid, '%lf\n', 1 );
  end;

  %if ( k == Norder )
    plot( z(1,1:Norder), z(2,1:Norder), 'r-', z(1,1:Norder), z(2,1:Norder), 'bo');
    M(n) = getframe;
    %l = l+1;
  %end;

end;

fclose( fid );

