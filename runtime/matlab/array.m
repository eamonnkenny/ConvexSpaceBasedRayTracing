% ARRAY defines all the properties of all the TX/RX
% Version 3: for P generic arrays 

% The input file is "Array.txt". Format required for the input file:
% - first row: P = number of arrays of the network;
% - M1 = the number of transmitting elements of the first array (min 2);
% - M1 rows with 3 columns each: 3 coordinates: (x, y, z) of that points;  
% - empty row (optional);
% - M2 = the number of trasmitting elements of the second array (min 2);
% - M2 rows with 3 columns each: 3 coordinates: (x, y, z) of that points; 
% - empty row (optional);
% - ....
% - MP = the number of trasmitting elements of the last array (min 2);
% - MP rows with 3 columns each: 3 coordinates: (x, y, z) of that points. 

% Other parameters than can be used in future:
% Transmitted Power [W]: this is the power trasmitted by all the transmitter array; thus,
% if the power launched by each transmitter element is inversely proportional to Mi, the
% total radiated power is equal to Pow, apart from Mi.
Pow=1;
% Antenna Gain Pattern of each array element (function of theta and phi):
G=1;

% IMPORTANT: here we assume that there is no interference between all the antennas used
% in each array. This means that we make an approximation, ignoring their mutual coupling.

% -----------------------------------------------------------------------------------------------
% Routine used to read data from file: 
% -----------------------------------------------------------------------------------------------

fid=fopen('Array.txt', 'r');
if (fid==-1) error ('Input file "Array.txt" missing.'); end

% This is the number of the arrays of the system:
P=fscanf( fid, '%d\n', 1 );

data=[];

for a=1:P
    
    % Vector Nelem contains the number of elements of each array.
    Nelem(a,1)=fscanf( fid, '%d\n', 1 );
    tmp=fscanf( fid, '%lf\n', [3, Nelem(a,1)] );
    tmp=tmp';

    % Here the central point of each array is computed; it may be either 
    % a geometric point or it may contain one element of the array.
    points(a,:)=mean(tmp);

    % All the points of each array are stored is the following matrix:
    data=[data; tmp];
    
end

save ('BaseStations.mat', 'points', 'data', 'P');
fclose(fid);
