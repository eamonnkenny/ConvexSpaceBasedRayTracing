%=========================================================================
% COPYRIGHT (c) 2000-2003 by Trinity College Dublin, Dublin 2, IRELAND
% All rights reserved  
%=========================================================================
% Author:                Eamonn M. Kenny
% Principal Researcher:  Dr. Peter J. Cullen
% First Modification:    May 2002
% Last Modification:     Dated Below "Added dd/mm/yy"
% Previous Projects:     Storms, FMW/ARG
% Current  Project:      IP2000 STIWRO
% Software Tool Version: 1.0
%=========================================================================
function [x y capacity capacityScaled xData yData colorData radiusData xVoice yVoice colorVoice radiusVoice ]  = plotcapacity(fileName)

fout = fopen(fileName,'r');

% print size of matrix to file
dummy = fscanf( fout, '%s', 1 );
xStart = fscanf( fout, '%f', 1 );
yStart = fscanf( fout, '%f', 1 );
zStart = fscanf( fout, '%f', 1 );
dummy = fscanf( fout, '%s', 1 );
xStep = fscanf( fout, '%f', 1 );
yStep = fscanf( fout, '%f', 1 );
dummy = fscanf( fout, '%s', 1 );
M = fscanf( fout, '%f', 1 );
N = fscanf( fout, '%f', 1 );

%fprintf( 1, 'StartPoint: %f %f %f \n', xStart, yStart, zStart );
%fprintf( 1, 'StepSize: %f %f\n', xStep, yStep );
%fprintf( 1, 'Dimensions: %d %d\n', M, N );

% set capacity requirements
capacity       = zeros(N,M);
capacityScaled = zeros(N,M);

x = zeros(M,1);
y = zeros(N,1);

dataSize = 0;
voiceSize = 0;

for n = 1:N;
  for m = 1:M;
    value = fscanf( fout, '%f', 1 );
    if ( value == 0.128 )
      dataSize = dataSize + 1;
    elseif ( value == 0.0122 )
      voiceSize = voiceSize + 1;
    end;
    capacity(n,m) = value;
  end;
end;

% Required for bubble plots
xData       = size(dataSize,1);
yData       = size(dataSize,1);
colorData   = size(dataSize,1);
radiusData  = size(dataSize,1);
xVoice      = size(voiceSize,1);
yVoice      = size(voiceSize,1);
colorVoice  = size(voiceSize,1);
radiusVoice = size(voiceSize,1);


for n = 1:N;
  y(n) = yStart + (n-1) * yStep;
end;

dataCount = 1;
voiceCount = 1;
capacitySum = 0;

for n = 1:N;
  for m = 1:M;
    x(m) = xStart + (m-1) * xStep;
    
    a = capacity(n,m);
    if ( a == 0.128 )
      capacityScaled(n,m) = 3;
      xData(dataCount) = x(m);
      yData(dataCount) = y(n);
      radiusData(dataCount) = a * 500.0;
      colorData(dataCount) = capacityScaled(n,m);
      dataCount = dataCount + 1;
    elseif ( a == 0.0122 )
      capacityScaled(n,m) = 2;
      xVoice(voiceCount) = x(m);
      yVoice(voiceCount) = y(n);
      radiusVoice(voiceCount) = a * 2000.0;
      colorVoice(voiceCount) = capacityScaled(n,m);
      voiceCount = voiceCount + 1;
    else
      capacityScaled(n,m) = 0;
    end;
      
    capacitySum = capacitySum + a;
  end;
end;

%fprintf( 'Start: (%fx%f), End: (%fx%f)\n', xStart, yStart, xStart+(M-1)*xStep, yStart+(N-1)*yStep );
fprintf( 'Total Capacity in Building: %f\n', capacitySum );

% plot the capacity in the grid and the story in which it is defined
%title( 'User Capacity Requirements' );
