hold

fileName = 'path1.txt';
fid   = fopen( fileName );
M = fscanf( fid, '%lf\n', 1 )
N = fscanf( fid, '%lf\n', 1 )
z = zeros( N, M );

for n = 1:N
  for m = 1:M
    z(n,m) = fscanf( fid, '%lf\n', 1 );
    if ( z(n,m) ~= 0.0 )
      z(n,m) = 20.0 * log10( z(n,m) );
    else
      z(n,m) = -140;
    end;

    if ( z(n,m) < -140 | z(n,m) > 50 )
      z(n,m) = -140;
    end;
  end;
end;
fclose( fid );

X = (1:M);
Y = (1:N);
surf(X,Y,z, 'Facecolor', 'interp', 'Edgecolor', 'none' );
title('Field Strength on x-y grid at 1metre resolution (z=5m). Tx=(50,50,10)')
xlabel('x-axis/m')
ylabel('y-axis/m')

hold
