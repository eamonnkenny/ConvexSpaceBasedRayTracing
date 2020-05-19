%=========================================================================
% COPYRIGHT (c) 2000-2016 by Trinity College Dublin, Dublin 2, IRELAND
% All rights reserved  
%=========================================================================
% Author:                Eamonn M. Kenny
% Principal Researcher:  
% First Modification:    May 2016
% Last Modification:     4th May 2016
% Previous Projects:     Storms, FMW/ARG, IP2000 STIWRO
% Current  Project:      ADAPT
% Software Tool Version: 1.0
%=========================================================================

function capacityBubbles(capacityFileName, storyFileName )
[ x y capacity capacityScaled xData yData colorData radiusData xVoice yVoice colorVoice radiusVoice ] = readcapacity(capacityFileName);

fprintf( 'Data size:   %d\n', size(xData,1) );
fprintf( 'Voice size:  %d\n', size(xVoice,1) );


storyFileNameA = [ storyFileName 'a' ];
% plot the capacity in the grid and the story in which it is defined
hold;
title('User Capacity Requirements' );
story3d( storyFileName, storyFileNameA ); hold on;
dataCircles  = scatter( xData,  yData,  radiusData,  colorData,  'filled', 'MarkerEdgeColor', 'k', 'DisplayName', 'Data links' ); hold on;
voiceCircles = scatter( xVoice, yVoice, radiusVoice, colorVoice, 'filled', 'MarkerEdgeColor', 'k', 'DisplayName', 'Voice links' );hold on;

colormap('autumn');
%legend([dataCircles,voiceCircles,buildingConvexes], {'Data links', 'Voice links', 'Building boxes'})
axis tight;
axis equal;
hold;
