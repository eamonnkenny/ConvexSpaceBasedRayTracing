function y=story3d(storyfile,storyfilea,storyfileb)
% read in the story from the file
figure(1);
hold;
fid = fopen( storyfile );
N = fscanf( fid, '%lf\n', 1 );
story = fscanf( fid, '%lf\n', [3, N*8] );
fclose( fid );
for k=1:N*8
  story(3,k) = story(3,k);
end;

faceindex = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];
N
for m=1:N
  index = (1:8) + (m-1)*8;
  x = story(:,index)';
  patch('Vertices',x,'Faces',faceindex,'FaceColor',[0.7 0.7 0.7] );
end;
alpha(0.5)

% now print windows and doors
fid = fopen( storyfilea );
Na = fscanf( fid, '%lf\n', 1 );
storysa = fscanf( fid, '%lf\n', [3, Na*8] );
fclose( fid );

for k=1:Na*8
  storysa(3,k) = storysa(3,k);
end;
faceindex2 = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];
%faceindex2 = [1 4 6 5; 3 2 8 7; 2 1 5 8];

for m=1:Na
  index = (1:8) + (m-1)*8;
  x = storysa(:,index)';
  patch('Vertices',x,'Faces',faceindex2,'FaceColor',[0.2 0.2 1.0],'EdgeColor','none' );
end;
alpha(0.5)

% now print convex spaces centres
fid = fopen( storyfileb );
Na = fscanf( fid, '%lf\n', 1 );
storysb = fscanf( fid, '%lf\n', [3, Na] );
size(storysb)
fclose( fid );

plot3(storysb(1,:),storysb(2,:),storysb(3,:),'r.');
for k=1:Na
  text(storysb(1,k)+0.25, storysb(2,k), storysb(3,k)-25, num2str(k));
end;
alpha(0.5)

samplepoints = load( 'receivers.txt');
size(samplepoints)
plot3( samplepoints(:,1), samplepoints(:,2), samplepoints(:,3),  'b.' );
plot3( samplepoints(1,1), samplepoints(1,2), samplepoints(1,3),  'g+' );
plot3( samplepoints(1,1), samplepoints(1,2), samplepoints(1,3),  'go' );
N = round(size(samplepoints-1,1)/2)
text( samplepoints(1,1)+0.25, samplepoints(1,2), samplepoints(1,3)-25,  'T_x' );
text( samplepoints(N,1)+0.25, samplepoints(N,2)+0.25, samplepoints(N,3)-25,  'R_x Positions' );

%fid = fopen( 'convexs.txt' );
%Na = fscanf( fid, '%lf\n', 1 );
%storysa = fscanf( fid, '%lf\n', [3, Na*8] );
%fclose( fid );
%
%for k=1:Na*8
  %storysa(3,k) = storysa(3,k);
%end;
%faceindex2 = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];

%for m=1:Na
  %index = (1:8) + (m-1)*8;
  %x = storysa(:,index)';
  %patch('Vertices',x,'Faces',faceindex2,'FaceColor',[1.0 0.0 0.0],'EdgeColor','none' );
%end;
%alpha(0.5)

set(1,'color','white');
axis equal;
axis tight;
hold;
