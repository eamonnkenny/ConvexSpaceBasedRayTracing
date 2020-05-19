function y=octtree(storyfile,storyfilea,storyfileb,lines)

maxXvalue = 0.0;
maxYvalue = 0.0;
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

for m=1:N
  index = (1:8) + (m-1)*8;
  x = story(:,index)';
  patch('Vertices',x,'Faces',faceindex,'FaceColor',[0.7 0.7 0.7] );
  
end;

if ( lines == 1 )
  alpha(0.5)
  xmin = min(story(1,:));
  xmax = max(story(1,:));
  ymin = min(story(2,:));
  ymax = max(story(2,:));

  horizontal = zeros(3,2);
  vertical   = zeros(3,2);
  horizontal = [ xmin, xmax ; ymax/2.0, ymax/2.0; 0.0, 0.0]
  vertical   = [ xmax/2.0, xmax/2.0 ; ymin, ymax; 0.0, 0.0]
  plot3( horizontal(1,:), horizontal(2,:), horizontal(3,:), 'r-' );
  plot3( vertical(1,:), vertical(2,:), vertical(3,:), 'r-' );
  
  strcross = sprintf( '%f,%f,%f,%f\n', xmin, xmax, ymin, ymax )
end;

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
alpha(0.5);

% now print convex spaces centres
fid = fopen( storyfileb );
Na = fscanf( fid, '%lf\n', 1 );
storysb = fscanf( fid, '%lf\n', [3, Na] );
size(storysb);
fclose( fid );

plot3(storysb(1,:),storysb(2,:),storysb(3,:),'r.');
for k=1:Na
  text(storysb(1,k)+0.25, storysb(2,k), storysb(3,k)-25, num2str(k));
end;
alpha(0.5);

samplepoints = load( 'receivers-octtree.txt');
plot3( samplepoints(:,1), samplepoints(:,2), samplepoints(:,3),  'b.' );
plot3( samplepoints(:,1), samplepoints(:,2), samplepoints(:,3),  'g+' );
plot3( samplepoints(:,1), samplepoints(:,2), samplepoints(:,3),  'go' );
text( samplepoints(1,1)+0.25, samplepoints(1,2), samplepoints(1,3)-25,  'T_x' );
N = round(size(samplepoints-1,1)/2);
text( samplepoints(2,1)+0.15, samplepoints(2,2)+0.15, samplepoints(2,3)-25,  'R_x' );

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
