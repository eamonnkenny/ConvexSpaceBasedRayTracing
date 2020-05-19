clear all
hold

%floor3d( 'floor1-conorshouse.txt', 'floor1-conorshouse.txta')
story3d( 'story1-conorshouse.txt', 'story1-conorshouse.txta' );
%fullrays
diffraction2(3,'diffraction.txt')
%reflect0('reflection.txt');
%reflect1('reflection.txt');

title('Diffraction paths including reflection and transmission points');
axis tight;
xlabel('x-axis/metres');
ylabel('y-axis/metres');
zlabel('z-axis/metres');

hold
