figure(1)

fprintf('1: Foyer, 2: printing 44p, 3: printing 152p\n');
plotType = input( 'Type 1,2 or 3: ' );

if ( plotType == 1 )
  name = 'foyer';
elseif ( plotType == 2 )
  name = 'print44p';
elseif ( plotType == 3 )
  name = 'print152p';
else
  fprintf('I do not understand this option, exit.');
  return;
end;  


fprintf('1: Cell Start Points, 2: Best Servers Plot, 3: Capacity Requirements, 4: Cell End Points \n');
plotNumber = input( 'Type 1,2,3,4 or combination such as [1 2 3 4]: ' )

sizeOfPlot = size(plotNumber,2);
if ( sizeOfPlot == 4 )
  rowsInSubplot = 2;
  columnsInSubplot = 2;
else
  rowsInSubplot = 1;
  columnsInSubplot = 1;
end;


storyFileName       = ['../coverage/thesis/story1-' name '.txt'];
storyFileNameA      = [ storyFileName 'a' ];
capacityFileName    = ['capacity-' name '.txt' ];
coverageMapFileName = [ '../coverage/bestserver-' name '-r1.txt' ];
locationFileName    = [ '../../linux/locations-' name '.txt' ];
cellCentresFileName = [ '../cells-' name '-start.txt' ];

% plot user capacity requirements for full building
if ( sizeOfPlot == 4 )
  subplot( 2, 2, 1 );
end;
if ( sizeOfPlot == 4 || ( plotNumber == 3 ) )
  hold;
  capacityBubbles( capacityFileName, storyFileName )
  if ( plotType == 1 )
    title( 'Foyer - (93 voice, 28 data)' );
  elseif ( plotType == 2 )
    title( 'Printing House - (19 voice, 25 data)' );
  elseif ( plotType == 3 )
    title( 'Printing House - (92 voice, 60 data)' );
  else
    title( '' );
  end;
  colormap(flag);
  xlabel('x-axis/m')
  ylabel('y-axis/m')
  axis tight;
  axis equal;
  view( 0, 90 );
  hold;
end;

% plot building
if ( sizeOfPlot == 4 )
  subplot( 2, 2, 2 );
end;
if ( sizeOfPlot == 4 || ( plotNumber == 1 ) )
  hold;
  story3d( storyFileName, storyFileNameA );
  cellcentres( cellCentresFileName );
  if ( plotType == 1 )
    title( '3 Hyper-rectangles + capacity centres' );
  elseif ( plotType == 2 )
    title( '2 Hyper-rectangles + capacity centres' );
  elseif ( plotType == 3 )
    title( '8 Hyper-rectangles + capacity centres' );
  else
    title( '' );
  end;
  xlabel('x-axis/m')
  ylabel('y-axis/m')
  view( 0, 90 );
  axis tight;
  axis equal;
  hold;
end;

% plot best server positions
if ( sizeOfPlot == 4 )
  subplot( 2, 2, 3 );
end;
if ( sizeOfPlot == 4 || ( plotNumber == 2 ) )
  hold;
  story3d( storyFileName, storyFileNameA );
  [x y server] = readcoverage( coverageMapFileName );
  [x1 y1 c1 r1] = gridToCircles(x,y,server);
  %surf(x, y, server)
  dataCircles  = scatter( x1,  y1,  r1,  c1,  'filled', 'MarkerEdgeColor', 'k', 'DisplayName', 'Best Servers' );
  if ( plotType == 1 )
    title('User requirements met (112/121)')
  elseif ( plotType == 2 )
    title('User requirements met (43/44)')
  elseif ( plotType == 3 )
    title('User requirements met (80/152)')
  else
    title( '' );
  end;
  xlabel('x-axis/m')
  ylabel('y-axis/m')
  view( 0, 90 );
  axis tight;
  axis equal;
  colormap(summer);
  hold;
end;


if ( sizeOfPlot == 4 )
  subplot( 2, 2, 4 );
end;
if ( sizeOfPlot == 4 || ( plotNumber == 4 ) )
  hold;
  story3d( storyFileName, storyFileNameA );
  cellcentres( cellCentresFileName );
  endlocations( locationFileName );
  title('Initial/Final Transmitter Points')
  xlabel('x-axis/m')
  ylabel('y-axis/m')
  view( 0, 90 );
  axis tight;
  axis equal;
  hold;
end;
  
colormap(autumn) ;
