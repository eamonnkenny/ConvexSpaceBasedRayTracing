function meshOfAbsolutes = componentsRT( fileName, M1, N1, N ) 

x = load( fileName );

res             = zeros( M1, N1, N );
meshOfAbsolutes = zeros( M1, N1, 6 );

k = 1;

for m = 1:M1
  for n = 1:N1
    for l = 1:N
      res( m, n, l ) = x( k, l );
      if ( l > N-6 )
        meshOfAbsolutes( m, n, l-N+6 ) = x( k, l );
      end;
    end;
    k = k + 1;
  end;
end;
