clear all;
close all;
s = load( 'meanInfinitePS.txt' );
figure(1);
hold;
plot( s(:,1)./pi.*180, 20.0*log10(s(:,3)), 'g-' );
plot( s(:,1)./pi.*180, 20.0*log10(s(:,2)), 'r.-' );
plot( s(:,1)./pi.*180, 20.0*log10(s(:,6)), 'm:' );
hold
legend( 'Single Reflection', 'Ray Tracing', 'Burnside');
xlabel( 'Angle \theta^o' );
ylabel( 'Pathloss/ dB' );
title('r = 10, source = (-10r cos\theta,0,10r sin\theta), receiver(r,0,r)');

axis tight;
