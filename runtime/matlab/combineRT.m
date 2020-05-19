close all;
M = 46;
N = 46;
Ei = zeros( M, N, 6 );
Er = zeros( M, N, 6 );
Ed = zeros( M, N, 6 );
Et = zeros( M, N, 6 );
ErrorRT      = zeros( M, N, 6 );
Eexact       = zeros( M, N, 6 );

normedError  = zeros( M, N );
normedEexact = zeros( M, N );
normedEt     = zeros( M, N );

Ei     = componentsRT( 'incidentfield.txt',    M, N, 21 );
Er     = componentsRT( 'reflectionfield.txt',  M, N, 21 );
Ed     = componentsRT( 'diffractionfield.txt', M, N, 21 );
Eexact = componentsRT( 'hemi_fields20.res',    M, N, 9  );

x       = ((1:M)-1) * (90 / ( M-1) );
y       = ((1:N)-1) * (360 / ( N-1) );
Et      = Ei + Er + Ed;
ErrorRT = ( Et - Eexact ) ./ Eexact;

for m = 1:M
  for n = 1:N
    normedError( m, n )  = ErrorRT(m, n, 5 );
    normedEt( m, n )     = Et(m, n, 5 );
    normedEr( m, n )     = Er(m, n, 5 );
    normedEi( m, n )     = Ei(m, n, 5 );
    normedEexact( m, n ) = Eexact(m, n, 5 );

    %normedError( m, n )  = sqrt( normedError( m, n ) );
    %normedEt( m, n )     = sqrt( normedEt( m, n) );
    %normedEexact( m, n ) = sqrt( normedEexact( m, n) );
  end;
end;

figure
surface( y, x, normedError, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Exact Total field Versus Ray tracing' );
ylabel( '\theta/degrees' );
xlabel( '\phi/degrees' );
axis tight;

figure;
surface( y, x, normedEi, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Incident field in Ray tracing' );
ylabel( '\theta/degrees' );
xlabel( '\phi/degrees' );
axis tight;

figure;
surface( y, x, normedEr, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Reflected field in ray-tracing' );
ylabel( '\theta/degrees' );
xlabel( '\phi/degrees' );
axis tight;

figure;
surface( y, x, normedEt, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Total field in Ray tracing' );
ylabel( '\theta/degrees' );
xlabel( '\phi/degrees' );
axis tight;

figure;
surface( y, x, normedEexact, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Exact Total Field' );
ylabel( '\theta/degrees' );
xlabel( '\phi/degrees' );
axis tight;
