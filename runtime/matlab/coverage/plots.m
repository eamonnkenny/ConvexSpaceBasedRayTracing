% PLOTS plots all the interesting graphics of this trasmission sistem
% It requires to be executed after MIMOCh, thus all variables needed are stored in memory

close all;
clear all;

% -------------------------------------------------------------------------------------------------------------------
% Figure(1): All the arrays are plotted inside the building where we are simulating the trasmission
% -------------------------------------------------------------------------------------------------------------------

figure (1)
load BaseStations.mat;

plot3(points(:,1), points(:,2), points(:,3), 'bo', data(:,1), data(:,2), data(:,3), 'bx'),
grid on, view(77, 76),
title ('All the arrays employed by the network'); 

% This function draws the builing where the indoor propagation occurs; 
% it requires two files: story1.txt and story1.txta 
story3d( 'story1.txt', 'story1.txta' );
   
% -------------------------------------------------------------------------------------------------------------------
% Figure(2): Array 1 -> 2. All the path loaded in 'paths' structure and the Tx and Rx arrays are plotted
% -------------------------------------------------------------------------------------------------------------------

load channel_1_2;

figure (2);

% Trasmitter
plot3(tx(1,1), tx(1,2), tx(1,3), 'ro'), hold on,

% Receiver
plot3(rx(1,1), rx(1,2), rx(1,3), 'bo'), hold on,

% Direct ray: if it exists, it is plotted in a different colour
if ( paths(1).points == 0 )
    tmp=[tx; rx];
    plot3(tmp(:,1), tmp(:,2), tmp(:,3), 'k-'), hold on,
end
    
% Indirect rays
if ( paths(1).points == 0 ) a=2;
else a=1;
end
for n=a:multipaths
    tmp=[tx; paths(n).coord; rx];
    plot3(tmp(:,1), tmp(:,2), tmp(:,3), 'g-'), hold on,
end   
grid on, view(0, 86),

plot3(tx(:,1), tx(:,2),tx(:,3), 'ro', ArrTx(:,1), ArrTx(:,2),ArrTx(:,3), 'rx'), hold on
plot3(rx(:,1), rx(:,2),rx(:,3), 'bo', ArrRx(:,1), ArrRx(:,2),ArrRx(:,3), 'bx'), 
hold on, grid on, view(77, 76),
title ('Array 1 -> 2. Multipaths between the central point of the Transmitting and the Receiving Array.'); 

story3d( 'story1.txt', 'story1.txta' );
   

% -------------------------------------------------------------------------------------------------------------------
% Figure(3): Array 1 -> 2. Plot of the amplitude of the channel impulse response from Tx 1 (pol.Ez) to Rx 1 (pol.Ez)  
% -------------------------------------------------------------------------------------------------------------------

load channel_1_2;
 
figure(3)

t3=0:DeltaT:maxdel;
stem(t3, abs(channelZZ(1,:)), '^'),
title ('Array 1 -> 2. Amplitude of the channel impulse response from Tx 1 (pol.Ez) to Rx 1 (pol.Ez).'),
xlabel('Time'), ylabel('Amplitude');
grid on;

% -------------------------------------------------------------------------------------------------------------------



% -------------------------------------------------------------------------------------------------------------------
% Figure(4): Array 1 -> 2.  Plot of the amplitude of the channel impulse response from Tx 2 (pol.Ez) to Rx N (pol.Ey) 
% -------------------------------------------------------------------------------------------------------------------

load channel_1_2;
 
figure(4)

t3=0:DeltaT:maxdel;
stem(t3, abs(channelZY(2*N,:)), '^'),
title ('Array 1 -> 2. Amplitude of the channel impulse response from Tx 2 (pol.Ez) to Rx N (pol.Ey)'),
xlabel('Time'), ylabel('Amplitude');
grid on;

% -------------------------------------------------------------------------------------------------------------------


% ----------------------------------------------------------------------------------------------------------------------------------
% Figure(5): Array 1 -> 2.  Difference of amplitude of the channel impulse response from Tx i (pol.Ez) to Rx j (pol.Ez)
% ----------------------------------------------------------------------------------------------------------------------------------

load channel_1_2;
 
figure(5)

t3=0:DeltaT:maxdel;
plot(t3, abs(channelZY(2,:))-abs(channelZY(1,:)), 'r', t3, abs(channelZY(3,:))-abs(channelZY(1,:)), 'b'), hold on,
plot(t3, abs(channelZY(N+2,:))-abs(channelZY(1,:)), 'g', t3, abs(channelZY(N+3,:))-abs(channelZY(1,:)), 'm'),
title ('Array 1 -> 2. Difference of amplitude of the channel impulse response from Tx 1 (pol.Ez) to Rx j (pol.Ez)'),
xlabel('Time'), ylabel('Amplitude'), grid on,
legend ('TX1&RX2 - TX1&RX1', 'TX1&RX3 - TX1&RX1', 'TX2&RX2 - TX1&RX1', 'TX2&RX3 - TX1&RX1'), 

% -------------------------------------------------------------------------------------------------------------------

