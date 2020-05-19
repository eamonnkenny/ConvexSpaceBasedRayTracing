clear all;
close all;
s = load( 'meanInfinitePS.txt' );
figure(1);
hold;
plot( s(:,1)./pi.*180, (s(:,2)), 'r.-' );
plot( s(:,1)./pi.*180, (s(:,3)), 'm:' );
hold
legend( 'RT Complex Permittivity', 'Single Reflection');
xlabel( 'Angle \theta^o' );
ylabel( 'Pathloss/ dB' );
title('r = 10, source = (-r cos\theta,0,r sin\theta), receiver(r,0,r)');

axis tight;
