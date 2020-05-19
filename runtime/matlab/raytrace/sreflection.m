clear all;
dos( 'Release\singleHardcoded.exe building' );
hold

%images;
floor3d;
story3d('story1.txt', 'story1.txta');
reflect3( 'reflection.txt' );
reflect2( 'reflection.txt' );
reflect1( 'reflection.txt' );
reflect0( 'reflection.txt' );
%title('0^{th}-3^{th} order reflections, colour code- magneta, red, green, blue' );
title('0^{th}-3^{th} order reflections - no diffractions' );
xlabel('x-axis');
ylabel('y-axis');

hold 

