close all;
%M = 91;
M = 1201;
N = 1;
Ei = zeros( M, N, 6 );
Er = zeros( M, N, 6 );
Ed = zeros( M, N, 6 );
Edc= zeros( M, N, 6 );
Es = zeros( M, N, 6 );
ErrorRT      = zeros( M, N, 6 );
Eexact       = zeros( M, N, 6 );

normedError  = zeros( M, N );
normedEexact = zeros( M, N );
normedEs     = zeros( M, N );
normedEd1     = zeros( M, N );
normedEd2     = zeros( M, N );
normedEd3     = zeros( M, N );
normedEd      = zeros( M, N );
%normedEdc    = zeros( M, N );

Ei      = componentsRT( 'incidentfield.txt',    M, N, 21 );
Er      = componentsRT( 'reflectionfield.txt',  M, N, 21 );
Ed1     = componentsRT( 'diffractionfield1.txt', M, N, 21 );
Ed2     = componentsRT( 'diffractionfield2.txt', M, N, 21 );
Ed3     = componentsRT( 'diffractionfield3.txt', M, N, 21 );
%Edc    = componentsRT( 'cornerdiffraction.txt', M, N, 21 );
Edabs   = zeros( 6, 1 );
Eexact = componentsRT( 'linex.res',    M, N, 9  );

% change to only give exact scattered field
%Eexact  = Eexact - Ei;

x       = (-100:0.1:20);
%x       = (0:1:90);
y       = (-5:0.1:5);
Es      = Ed + Er;
ErrorRT = ( Es - Eexact );

for m = 1:M
  for n = 1:N
    normedEs( m, n )     = sqrt( Es(m, n, 5 )^2 + Es( m, n, 6 )^2 );
    normedEsx( m, n )     = sqrt( Es(m, n, 1 )^2 + Es( m, n, 2 )^2 );
    normedEsy( m, n )     = sqrt( Es(m, n, 3 )^2 + Es( m, n, 4 )^2 );
    normedEr( m, n )     = sqrt( Er(m, n, 5 )^2 + Er( m, n, 6 )^2 );
    normedEd1( m, n )     = sqrt( Ed1(m, n, 5 )^2 + Ed1( m, n, 6 )^2 );
    normedEd2( m, n )     = sqrt( Ed2(m, n, 5 )^2 + Ed2( m, n, 6 )^2 );
    normedEd3( m, n )     = sqrt( Ed3(m, n, 5 )^2 + Ed3( m, n, 6 )^2 );
    %Ed = sqrt( (Ed1( m, n, 5 ) + Ed2( m, n, 5 ) +Ed3( m, n, 5 ))^2 + (Ed1( m, n, 6 ) +Ed2( m, n, 6 )^2 + Ed3( m, n, 6 ))^2 );
    Edabs = Ed1(m,n,:) + Ed2(m,n,:) + Ed3(m,n,:) + Er(m,n,:);
    normedEd( m, n )     = sqrt( Edabs( 5 )^2 + Edabs( 6 )^2 );

    %normedEdc( m, n )    = sqrt( Edc(m, n, 5 )^2 + Edc( m, n, 6 )^2 );
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
plot( x, normedError( :, 1 ) );
title( 'Relative Error in Ray-tracing' );
xlabel( 'x/metres' );
ylabel( 'scattered field' );
axis tight;

figure;
plot( x, normedEi( :, 1 ) );
title( 'Incident field in Ray tracing' );
xlabel( 'x/metres' );
ylabel( 'scattered field' );
axis tight;

figure;
plot( x, normedEr( :, 1 ) );
title( 'Reflected field in ray-tracing' );
xlabel( 'x/metres' );
ylabel( 'scattered field' );
axis tight;

figure;
plot( x, normedEs( :, 1 ) );
title( 'Scattered field in Ray tracing' );
xlabel( 'x/metres' );
ylabel( 'z component scattered field' );
axis tight;

figure;
plot( x, normedEsx( :, 1 ) );
title( 'Scattered field in Ray tracing' );
xlabel( 'x/metres' );
ylabel( 'x component scattered field' );
axis tight;

figure;
plot( x, normedEsy( :, 1 ) );
title( 'Scattered field in Ray tracing' );
xlabel( 'x/metres' );
ylabel( 'y component scattered field' );
axis tight;

figure;
plot( x,normedEd1(:,1), 'r-', x,normedEd2(:,1),'g-', x,normedEd3(:,1),'b-', x, normedEr(:,1), 'k-', x, normedEd(:,1), 'm-' );
title( 'Diffracted fields in Ray tracing' );
xlabel( 'x/metres' );
ylabel( 'scattered field' );
legend( 'Side A Diffraction', 'Side C Diffraction', 'Side B+D diffraction', 'Reflected field', 'Scattered Field' );
axis tight;

figure;
%plot( x, normedEexact( :, 1 ), 'r-', x, 20.0*log10(normedEs( :, 1 )), 'g-.' );
plot( x, normedEexact( :, 1 ), 'r-', x, normedEd( :, 1 ), 'g-.' );
title( 'Exact Scattered Field versus UTD Diffracted field, line = (x,0,20)' );
xlabel( 'x/metres' );
ylabel( 'scattered field' );
legend( 'Exact Solution', 'UTD' );
axis tight;

%figure;
%plot( x, normedEd( :, 1 ), 'r-', x, normedEdc( :, 1 ), 'g-.' );
%title( 'Diffracted Field versus UTD Corner Diffracted field, line = (x,0,20)' );
%xlabel( 'x/metres' );
%ylabel( 'diffracted field' );
%legend( 'UTD diff', 'UTD corner diff' );
%axis tight;
