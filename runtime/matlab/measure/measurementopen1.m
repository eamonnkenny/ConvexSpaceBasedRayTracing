% read in the story from the file
figure(1);
hold;
fid = fopen( '../../measurements/measurementopen1.dat' );
N = fscanf( fid, '%lf\n', 1 );
t = fscanf( fid, '%lf\n', [4, N] );
fclose( fid );

s = load( 'corridoropen1.txt' );

plot( s(:,1), 20.0*log10(s(:,2)), 'r-', t(1,:), -t(4,:), 'b-.' );
xlabel('x-axis');
ylabel('Power Path Loss/dB');
hold;

set(1,'color','white');
legend('ray-trace','measurements');
title('Measurement Set 1 - Door Open');
%axis equal;
axis tight;
