% MIMO CHANNEL Model (for Linear Memoryless Channels)
% 
%       - multipath data loaded from raytracing;
%       - scatterers effect modelled as a phase shift;
%       - a network with more than 2 arrays can be employed;
%
%   For details, see Readme.txt

close all;
clear all;


% ------------------------------------------------------------------------------
%  Variable definition
% ------------------------------------------------------------------------------
% Carrier Frequency [Hz]
Fc=900e6;
% Light speed [m/s]
C=3e8;
% Wavelength
lambda=C/Fc;
% Bandwidth [Hz]
B=10e6;
% Temporal resolution:
tres=inv(B);


% ------------------------------------------------------------------------------------
%  Loading of system characteristics
% ------------------------------------------------------------------------------------ 
  array;


% ------------------------------------------------------------------------------------
%  Channel model: - it computes the channel response of every path from Tx to Rx using 
%                   raytracing data (for each path, 3 reflections, 4 transmissions 
%                   and 1 corner diffraction are allowed).
%                 - it adds uniform random phase shift to the received signal.
% ------------------------------------------------------------------------------------

% Version 1: (OLD)
% Now we might have already executed the Raytracing TCD Software, with input the central 
% point of both the Transmitter and the Receiver (i.e.: the two vectors 'tx' and 'rx' 
% computed by array.m).

% Version 2: (NEW)
% We call the Raytracing TCD Software, giving as input a file named "points.dat":
fid=fopen('points.dat', 'w');
fprintf (fid, '%d\n', P);
for a=1:P 
    fprintf (fid, '%f %f %f\n', points(a,1), points(a,2), points(a,3));
end
fclose(fid);
% We call that program with the following syntax:
dos('Release\Mimo building');
fprintf ('\n\n Wait, I''m working...')

% Considering that the number of not-redundant links between P points is: L=(P^2-P)/2,
% the raytrace Software produces L files with the following names: 
% raypaths_1_2.txt, raypaths_1_3.txt, ..., raypaths_(P-1)_P.txt , where
% that numbers represent the transmitter and receiver of every link.

% count1 and count2 are 2 index used to consider every possible link between the arrays 
for count1=1:P
 for count2=(count1+1):P   
    
    %--------------------------------------------------------------------------------------------
    % The following lines set the data of the 2 arrays considered in each loop
    % as "current data"; then they are processed.
    
    M=Nelem(count2,1);
    N=Nelem(count1,1);
    tx=points(count2,:);
    rx=points(count1,:);
    
    % ArrTx is the matrix contaning what is now considered as a trasmitting array
    ind=1;
    for a=1:(count2-1)
        ind=ind+Nelem(a,1);
    end
    ArrTx=[];
    for a=0:(M-1)
        ArrTx=[ArrTx; data(ind+a,:)];
    end
    
    % Here is built ArrTxDiff, that is a matrix containig the relative position 
    % (respect to the central point of the array) of all transmitting elements
    ArrTxDiff=ArrTx-ones(M,1)*tx;
    
    % ArrRx is the matrix contaning what is now considered as a receiving array
    ind=1;
    for a=1:(count1-1)
        ind=ind+Nelem(a,1);
    end
    ArrRx=[];
    for a=0:(N-1) 
        ArrRx=[ArrRx; data(ind+a,:)];
    end
    
    % Here is built ArrRxDiff, that is a matrix containig the relative position 
    % (respect to the central point of the array) of all receiving elements
    ArrRxDiff=ArrRx-ones(N,1)*rx;
    %--------------------------------------------------------------------------------------------
    
    
    % Thus, now we can load all the information about the multipath between tx and rx with 
    % the following line:
    loadpaths;

    % Delay and phase shift in the array: we compute them calling the following function:
    delay;


    % MAIN ipothesis: there is a great number of scatterers in the propagation environment;
    % we model this phenomenon supposing that the rays are not attenuated by the scatterers,
    % but that it is applied to them a random phase shift, uniformly distibuited over [0,2pi]. 

    for n=1:multipaths       
      paths(n).fieldloss=paths(n).fieldloss.*exp(i*2*pi*rand(3,3));
    end    


    % IMPORTANT: we ignore the different values of amplitude of the path-loss between 
    % each point of the tx (& rx) array and its central point; therefore, we suppose
    % that the amplitude of the electric field is slowly changing.
    % The same assumption is made for the time delay. 

    % The following function builds the impulse-response of this channel, using all the 
    % information built up to now, and stores it in output files.
    resp;
        
 end    
end



% ------------------------------------------------------------------------------------
%  Received signal (considerations): 
% ------------------------------------------------------------------------------------

% If the tranmitted signal is tra(t),
%
% with the channel model built:  H(t)=Sumj Aj * Delta(t-DeltaT*j)
%
% then the received signal is:
%
% rec(t) = (tra(t) convol H(t)) + n(t) = Sumj Aj(t) * tra(t-DeltaT*j) + n(t).
% 
% Where:
%
% tra(t) is a vector of M signals, rec(t) is a vector of N signals,
%
% Aj(t) is the matrix (M x N) that describes the channel, in function  
%
% of time, during all the time spread.
%
%
% This is the model built for each of the 9 different polarization channels
%
% even if only few orthogonal polarizations are exploitable channels.
