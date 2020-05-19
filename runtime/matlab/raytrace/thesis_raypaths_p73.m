clear all;
%dos( 'Release\singleHardcoded.exe building' );
hold

%images;
floor3d('thesis/floor2.txt', 'thesis/floor1.txta');
story3d('thesis/story1.txt', 'thesis/story1.txta');
reflect3( 'thesis/reflection.txt' );
reflect2( 'thesis/reflection.txt' );
reflect1( 'thesis/reflection.txt' );
reflect0( 'thesis/reflection.txt' );
%title('0^{th}-3^{th} order reflections, colour code- magneta, red, green, blue' );
title('0^{th}-3^{th} order reflections - no diffractions' );
xlabel('x-axis');
ylabel('y-axis');

hold 

