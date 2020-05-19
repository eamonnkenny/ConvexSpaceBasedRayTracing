x1 = 1:0.2:3; 
y1 = 1:0.2:3;
x2 = 5:0.2:7;
y2 = 1:0.2:3;

hold on;
scatter(x1,y1,x2,y2);
hMarkers = findobj(hLegend,'type','patch');
set(hMarkers, 'MarkerEdgeColor','k', 'MarkerFaceColor','k');
legend('red','blue');
hold off;

