% read in the rays from the file
function M=fullrays(k, l)

antenna
fid   = fopen( 'fullrays.txt' );
Nrays = fscanf( fid, '%lf\n', 1 );

for n = 1:Nrays
  Norder = fscanf( fid, '%lf\n', 1 );
  z      = zeros( 3, Norder );

  for m = 1:Norder
    z(1,m) = fscanf( fid, '%lf\n', 1 );
    z(2,m) = fscanf( fid, '%lf\n', 1 );
    z(3,m) = fscanf( fid, '%lf\n', 1 );
  end;

  if ( k == Norder )
    l = l + 1;
    plot( z(1,1:Norder), z(2,1:Norder), 'r-','EraseMode','background');
    plot( x1, y1, 'b+', 'EraseMode','background');
    plot( x2, y2, 'b+', 'EraseMode','background');
    M(l) = getframe;
    l = l + 1;
    plot( z(1,1:Norder), z(2,1:Norder), 'w-','EraseMode','background');
    plot( x1, y1, 'b+', 'EraseMode','background');
    plot( x2, y2, 'b+', 'EraseMode','background');
    M(l) = getframe;
  end;

end;

fclose( fid );

