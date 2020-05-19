clear all;
close all;
s = load( 'diffractionfield.txt');

figure
plot( s(:,1), s(:,2), 'b-' );
title( 'Diffracted field from corner diffraction' );
xlabel( '\theta/degrees' );
ylabel( 'E_\theta (dB)' );
axis tight;
