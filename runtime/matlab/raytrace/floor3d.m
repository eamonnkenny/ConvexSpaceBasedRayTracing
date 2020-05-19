function y=floor3d( floorFile, flooraFile )
% read in the floor from the file

fid = fopen( floorFile );
N = fscanf( fid, '%lf\n', 1 );
afloor = fscanf( fid, '%lf\n', [3, N*8] );
fclose( fid );
for k=1:N*8
  afloor(3,k) = afloor(3,k);
end;

faceindex = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];
for m=1:N
  index = (1:8) + (m-1)*8;
  x = afloor(:,index)';
  patch('Vertices',x,'Faces',faceindex,'FaceColor',[0.7 0.7 0.7] );
end;
%alpha(0.5)

% now print windows and doors
fid = fopen( flooraFile );
Na = fscanf( fid, '%lf\n', 1 );
afloor = fscanf( fid, '%lf\n', [3, Na*8] );
fclose( fid );

for k=1:Na*8
  afloor(3,k) = afloor(3,k);
end;
faceindex2 = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];
faceindex2 = [1 4 6 5; 3 2 8 7; 2 1 5 8];

for m=1:Na
  index = (1:8) + (m-1)*8;
  x = afloor(:,index)';
  patch('Vertices',x,'Faces',faceindex2,'FaceColor',[0.1 0.1 1.0],'EdgeColor','none' );
end;
%axis( [-1 32 -1 22] );
%alpha(0.5)
