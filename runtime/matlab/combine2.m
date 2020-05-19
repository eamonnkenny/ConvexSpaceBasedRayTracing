close all;
M = 46;
N = 46;
Ei = zeros( M, N, 6 );
Er = zeros( M, N, 6 );
Ed = zeros( M, N, 6 );
Es = zeros( M, N, 6 );
ErrorRT      = zeros( M, N, 6 );
Eexact       = zeros( M, N, 6 );

normedError  = zeros( M, N );
normedEexact = zeros( M, N );
normedEs     = zeros( M, N );

Ei     = componentsRT( 'incidentfield.txt',    M, N, 21 );
Er     = componentsRT( 'reflectionfield.txt',  M, N, 21 );
Ed     = componentsRT( 'diffractionfield.txt', M, N, 21 );
Eexact = componentsRT( 'hemi_fields20.res',    M, N, 9  );

% change to only give exact scattered field
Eexact  = Eexact - Ei;

x       = ((1:M)-1) * (90 / ( M-1) );
y       = ((1:N)-1) * (360 / ( N-1) );
Es      = Er + Ed;
ErrorRT = ( Es - Eexact );

for m = 1:M
  for n = 1:N
    normedEs( m, n )     = sqrt( Es(m, n, 5 )^2 + Es( m, n, 6 )^2 );
    normedEr( m, n )     = sqrt( Er(m, n, 5 )^2 + Er( m, n, 6 )^2 );
    normedEi( m, n )     = sqrt( Ei(m, n, 5 )^2 + Ei( m, n, 6 )^2 );
    %normedError( m, n )  = sqrt( ErrorRT(m, n, 5 )^2 + ErrorRT( m, n, 6 )^2 );
    normedEexact( m, n ) = sqrt( Eexact(m, n, 5 )^2 + Eexact( m, n, 6 )^2 );
    normedError( m, n )  = sqrt( ErrorRT(m, n, 5 )^2 + ErrorRT( m, n, 6 )^2 ) / normedEexact( m, n ); 

    %normedError( m, n )  = sqrt( normedError( m, n ) );
    %normedEs( m, n )     = sqrt( normedEs( m, n) );
    %normedEexact( m, n ) = sqrt( normedEexact( m, n) );
  end;
end;

figure
surface( x, y, normedError, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Exact Total field Versus Ray tracing' );
xlabel( '\theta/degrees' );
ylabel( '\phi/degrees' );
axis tight;

figure;
surface( x, y, normedEi, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Incident field in Ray tracing' );
xlabel( '\theta/degrees' );
ylabel( '\phi/degrees' );
axis tight;

figure;
surface( x, y, normedEr, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Reflected field in ray-tracing' );
xlabel( '\theta/degrees' );
ylabel( '\phi/degrees' );
axis tight;

figure;
surface( x, y, normedEs, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Total field in Ray tracing' );
xlabel( '\theta/degrees' );
ylabel( '\phi/degrees' );
axis tight;

figure;
surface( x, y, normedEexact, 'FaceColor', 'interp', 'EdgeColor', 'none' );
title( 'Exact Total Field' );
xlabel( '\theta/degrees' );
ylabel( '\phi/degrees' );
axis tight;
