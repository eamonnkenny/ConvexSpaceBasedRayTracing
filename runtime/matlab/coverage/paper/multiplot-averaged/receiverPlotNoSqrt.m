function y331=receiverPlot()
% read in the story from the file

fid = fopen( 'receiverFields-conorshouse-r4t4d1-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y441 = xyArray';

%fid = fopen( 'receiverFields-conorshouse-r4t3d1-sorted.txt', 'r' );
%xyArray = fscanf( fid, '%lf\n', [2, 28] );
%fclose( fid );
%y431 = xyArray';

fid = fopen( 'receiverFields-conorshouse-r3t3d1-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y331 = xyArray';

fid = fopen( 'receiverFields-conorshouse-r2t3d1-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y231 = xyArray';

fid = fopen( 'receiverFields-conorshouse-r1t3d1-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y131 = xyArray';

fid = fopen( 'receiverFields-conorshouse-r1t3d0-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y130 = xyArray';

fid = fopen( 'vefie.csv', 'r' );
vefie = fscanf( fid, '%lf\n', [1, 28] );
fclose( fid );

fid = fopen( 'measurements.csv', 'r' );
measurements = fscanf( fid, '%lf\n', [1, 28] );
fclose( fid );


m = measurements';
v = vefie';

plot( y441(:,1),y441(:,2),'r:o', y331(:,1),y331(:,2),'m:o', y231(:,1),y231(:,2),'g:o', y131(:,1),y131(:,2),'c:o', y130(:,1),y130(:,2),'c-o',y331(:,1),m(:,1),'-k.',y331(:,1),v(:,1),'b--*');

title('Raytracing Results for 28 points');
%subtitle('3 reflections, 1 diffraction and 3 transmission' );
xlabel('Distance from T_x to R_x^i / metres');
ylabel('Signal Power/dBm');
legend('r4-t4-d1','r3-t3-d1','r2-t3-d1','r1-t3-d1','r1-t3-d0','Measurements','VEFIE')
legend('show')
