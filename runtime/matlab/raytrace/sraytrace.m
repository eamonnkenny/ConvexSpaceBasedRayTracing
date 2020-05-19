clear all;
%dos( 'Release\single.exe building' );
hold

%images;
floor3d;
story3d( 'thesis/story1.txt', 'thesis/story1.txta' );
reflect3( 'reflection.txt' );
reflect2( 'reflection.txt' );
reflect1( 'reflection.txt' );
reflect0( 'reflection.txt' );
title('0^{th}-3^{th} order reflections, colour code- magneta, red, green, blue' );
%axis( [-0.5 31.5 -0.5 21.5 0 9.6] )
axis tight

hold 

