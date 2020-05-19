clear all
hold

floor3d
story3d( 'story1.txt', 'story1.txta' );
%fullrays
diffraction2(3)
reflect0('reflection.txt');
reflect1('reflection.txt');

title('Diffraction paths including reflection and transmission points');
axis tight;
xlabel('x-axis/metres');
ylabel('y-axis/metres');
zlabel('z-axis/metres');

hold
