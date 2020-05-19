clear all
hold

floor3d( 'thesis/floor3.txt', 'thesis/floor3.txta')
story3d( 'thesis/story3.txt', 'thesis/story3.txta' );
%fullrays
diffraction2(3,'thesis/diffraction.txt')
reflect0('thesis/reflection.txt');
reflect1('thesis/reflection.txt');

title('Diffraction paths including reflection and transmission points');
axis tight;
xlabel('x-axis/metres');
ylabel('y-axis/metres');
zlabel('z-axis/metres');

hold
