% read in the story from the file
figure(4);
hold;
lambda = 300.0 / 1000.0;
C = (lambda / 8.0 / pi);% / sqrt(2.0);
fid = fopen( '../../measurements/measureacross.dat' );
N = fscanf( fid, '%lf\n', 1 );
t = fscanf( fid, '%lf\n', [4, N] );
fclose( fid );

s1  = load( 'corridorclosedR1.txt' );
s1RW = load( 'corridorclosedR1RW.txt' );
s3 = load( 'corridorclosedR3.txt' );
s3RW = load( 'corridorclosedR3RW.txt' );

plot( s1(:,1), 20.0*log10(s1(:,2) * C), 'r-', s1(:,1), 20.0*log10(s3(:,2) * C), 'g-', s1(:,1), 20.0*log10(s1RW(:,2) * C), 'b-', s1(:,1), 20.0*log10(s3RW(:,2) * C), 'k-', t(2,:), -t(4,:), 'm-');
xlabel('R_x - Receiver Location/units');
ylabel('Signal Strength/dB');
hold;
meant    = mean( -t(4,:) );
means3RW = mean( 20*log10(s3RW(:,2) * C) );
means3   = mean( 20*log10(s3(:,2) * C) );
means1RW = mean( 20*log10(s1RW(:,2) * C) );
means1   = mean( 20*log10(s1(:,2) * C) );

fprintf( 'mean diff3RW = %f\n', abs(meant - means3RW) )
fprintf( 'mean diff3   = %f\n', abs(meant - means3 ) );
fprintf( 'mean diff1RW = %f\n', abs(meant - means1RW) );
fprintf( 'mean diff1   = %f\n', abs(meant - means1) );

set(4,'color','white');
legend('RT Reflections=1', 'RT Reflections=3', 'RW Reflections=1', 'RW Reflections=3', 'measurements corridor' );
title('Measurement Set 2 - Ray-trace with/without Random Mean');
%axis equal;
axis tight;
