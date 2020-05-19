function meshOfAbsolutes = components2( fileName, M1, N1, N ) 

x = load( fileName );

size(x)
meshOfAbsolutes = zeros( M1, N1, 6 );

k = 1;

for m = 1:M1
  for n = 1:N1
      meshOfAbsolutes( m, n, 5 ) = x( k, 2 );
    k = k + 1;
  end;
end;
