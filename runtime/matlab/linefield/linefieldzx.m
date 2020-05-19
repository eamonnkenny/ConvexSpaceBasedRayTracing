close all;
M = 1201;
N = 1;
Ei = zeros( M, N, 6 );
Er = zeros( M, N, 6 );
Ed = zeros( M, N, 6 );
Es = zeros( M, N, 6 );
ErrorRT      = zeros( M, N, 6 );
Eexact       = zeros( M, N, 6 );

normedError  = zeros( M, N );
normedEexact = zeros( M, N );
normedEs     = zeros( M, N );

Ei     = componentsRT( 'incidentfieldx.txt',    M, N, 21 );
Er     = componentsRT( 'reflectionfieldx.txt',  M, N, 21 );
Ed     = componentsRT( 'diffractionfieldx.txt', M, N, 21 );
Ercon  = componentsRT( 'reflx.res',    M, N, 9 );
Eexact = componentsRT( 'linex.res',    M, N, 9  );

% change to only give exact scattered field
%Eexact  = Eexact - Ei;

x       = (-100:0.1:20);
y       = (-5:0.1:5);
Es      = Er + Ed;
ErrorRT = ( Es - Eexact );

for m = 1:M
  for n = 1:N
    normedEs( m, n )     = sqrt( Es(m, n, 5 )^2 );
    normedEr( m, n )     = sqrt( Er(m, n, 5 )^2  );
    normedEi( m, n )     = sqrt( Ei(m, n, 5 )^2  );
    %normedError( m, n )  = sqrt( ErrorRT(m, n, 5 )^2 );
    normedEexact( m, n ) = sqrt( Eexact(m, n, 5 )^2 );
    normedErcon( m, n ) = sqrt( Ercon(m, n, 5 )^2 );
    normedError( m, n )  = sqrt( ErrorRT(m, n, 5 )^2 ) / normedEexact( m, n ); 

    %normedError( m, n )  = sqrt( normedError( m, n ) );
    %normedEs( m, n )     = sqrt( normedEs( m, n) );
    %normedEexact( m, n ) = sqrt( normedEexact( m, n) );
  end;
end;

figure
plot( x, normedError( :, 1 ) );
title( 'Relative Error in Ray-tracing' );
xlabel( 'x/metres' );
ylabel( '\phi/degrees' );
axis tight;

figure;
plot( x, normedEi( :, 1 ) );
title( 'Incident field in Ray tracing' );
xlabel( 'x/degrees' );
ylabel( '\phi/degrees' );
axis tight;

figure;
plot( x, normedEr( :, 1 ) );
title( 'Reflected field in ray-tracing' );
xlabel( 'x/metres' );
ylabel( 'scattered field' );
axis tight;

figure;
plot( x, normedErcon( :, 1 ) );
title( 'Reflected field in Conors code' );
xlabel( 'x/metres' );
ylabel( 'scattered field' );
axis tight;

figure;
plot( x, normedEs( :, 1 ) );
title( 'Scattered field in Ray tracing' );
xlabel( '\theta/degrees' );
ylabel( '\phi/degrees' );
axis tight;

figure;
plot( x, normedEexact( :, 1 ), 'r-.', x, normedEs( :, 1 ), 'g-.' );
title( 'Exact Scattered Field versus UTD Diffracted field, line = (x,0,20)' );
xlabel( 'x/metres' );
ylabel( 'scattered field' );
legend( 'Exact Conor', 'UTD' );
axis tight;
