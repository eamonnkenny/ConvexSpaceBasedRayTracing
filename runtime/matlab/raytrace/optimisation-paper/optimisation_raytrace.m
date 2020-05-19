function y=story3d(storyfile,storyfilea,storyfileb)
% read in the story from the file
figure(1);
hold;
fid = fopen( storyfile );
N = fscanf( fid, '%lf\n', 1 );
story = fscanf( fid, '%lf\n', [3, N*8] );
fclose( fid );
for k=1:N*8
  story(3,k) = story(3,k) - 5.0;
end;

faceindex = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];

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
% shift so visualisation is easier
for k=1:Na*8
  storysa(3,k) = storysa(3,k) - 5.0;
end;

faceindex2 = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];

for m=1:Na
  index = (1:8) + (m-1)*8;
  x = storysa(:,index)';
  patch('Vertices',x,'Faces',faceindex2,'FaceColor',[0.2 0.2 1.0],'EdgeColor','none' );
end;
alpha(0.5)


samplepoints = load( 'receivers.txt');
size(samplepoints)
plot3( samplepoints(:,1), samplepoints(:,2), samplepoints(:,3),  'b.' );
plot3( samplepoints(1,1), samplepoints(1,2), samplepoints(1,3),  'g+' );
plot3( samplepoints(1,1), samplepoints(1,2), samplepoints(1,3),  'go' );
N = round(size(samplepoints-1,1)/2)
text( samplepoints(1,1)-0.2, samplepoints(1,2)+0.7, samplepoints(N,3)+0.5,  'R_x^j' );


fid = fopen( 'images.txt' );
transmitter = fscanf( fid, '%lf\n', [1, 3] );
fclose( fid );

plot3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2,  'b.' );
plot3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2,  'g+' );
plot3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2,  'go' );

text( transmitter(1,1)-0.5, transmitter(1,2)+0.5, transmitter(1,3)+2,  'T_x^i' );
q = quiver3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2, -0.8, 0.0, 0.0, 1.0,...
           'LineWidth',2.0, 'MaxHeadSize',5.0,'Color','black');
q = quiver3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2, 0.8, 0.0, 0.0, 1.0,...
           'LineWidth',2.0, 'MaxHeadSize',5.0,'Color','black');
q = quiver3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2, 0.0, -0.8, 0.0,  1.0,...
           'LineWidth',2.0, 'MaxHeadSize',5.0,'Color','black');
q = quiver3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2, 0.0, 0.8, 0.0, 1.0,...
           'LineWidth',2.0, 'MaxHeadSize',5.0,'Color','black');
q = quiver3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2, 0.0, 0.0, -0.8, 1.0,...
           'LineWidth',2.0, 'MaxHeadSize',5.0,'Color','black');
q = quiver3( transmitter(1,1), transmitter(1,2), transmitter(1,3)+2, 0.0, 0.0, 0.8, 1.0,...
           'LineWidth',2.0, 'MaxHeadSize',5.0,'Color','black');

set(1,'color','white');
axis equal;
axis tight;

%images;
%story3d('story1-measurements.txt', 'story1-measurements.txta');
reflect3( 'reflection.txt' );
reflect2( 'reflection.txt' );
reflect1( 'reflection.txt' );
reflect0( 'reflection.txt' );
%title('0^{th}-3^{th} order reflections, colour code- magneta, red, green, blue' );
title('0^{th}-3^{th} order reflections - no diffractions' );
xlabel('x-axis');
ylabel('y-axis');

hold 

