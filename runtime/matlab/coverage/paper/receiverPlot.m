function y331=receiverPlot()
% read in the story from the file
fid = fopen( 'receiverFields-conorshouse-r3t3d1-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y331 = xyArray';

fid = fopen( 'receiverFields-conorshouse-r4t4d1-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y441 = xyArray';

fid = fopen( 'receiverFields-conorshouse-r3t1d1-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y311 = xyArray';

fid = fopen( 'receiverFields-conorshouse-r3t3d0-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y330 = xyArray';

fid = fopen( 'receiverFields-conorshouse-r3t1d0-sorted.txt', 'r' );
xyArray = fscanf( fid, '%lf\n', [2, 28] );
fclose( fid );
y310 = xyArray';

fid = fopen( 'vefie.csv', 'r' );
vefie = fscanf( fid, '%lf\n', [1, 28] );
fclose( fid );

fid = fopen( 'measurements.csv', 'r' );
measurements = fscanf( fid, '%lf\n', [1, 28] );
fclose( fid );


m = measurements';
v = vefie';

plot( y331(:,1),y331(:,2),'r:o', y441(:,1),y441(:,2),'m:o', y311(:,1),y311(:,2),'g:o', y330(:,1),y330(:,2),'c:o', y310(:,1),y310(:,2),'c-o',y331(:,1),m(:,1),'-k.',y331(:,1),v(:,1),'b--*');

title('Raytracing Results for 28 points');
%subtitle('3 reflections, 1 diffraction and 3 transmission' );
xlabel('Distance from T_x to R_x^i / metres');
ylabel('Signal Power/dBm');
legend('r3-t3-d1','r4-t4-d1','r3-t1-d1','r3-t3-d0','r3-t1-d0','Measurements','VEFIE')
legend('show')
