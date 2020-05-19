% RESP builds the impulse-response of this channel, using the following information
% (avaliable for each path): paths(n).fieldloss, paths(n).maindel,
%                            paths(n).ArrTxPhase and paths(n).ArrRxPhase. 

% The real impulse-response of this channel is:
%
% H(t)=Sumj Aj * Delta(t-tauj)
%
% We work with a discrete model of that channel that is:
%
% H(t)=Sumj Aj * Delta(t-DeltaT*j)
%
% where DeltaT is a constant number and represents the resolution of the model.

% We choose a time resolution bigger than the other signals (i. e. a smaller step) with gain beta, 
% because this resolution determines the accuracy of this model:
beta=20;
DeltaT=tres/beta;

% This means that 2 rays arriving at the receiver are considered as different rays only if
% the difference of ther delay is bigger than DeltaT; otherwise, they contribute is summed. 

% Than, the channel of every polarization can be decribed with a matrix 'channel', 
% with (M x N) rows and T columns, where T is the number of DeltaT considered; 
% it is given by:    
T=ceil(maxdel/DeltaT);

% Every row of 'channel' contains the complex time responce between one of the transmitting  
% and one of the receiving element: rows 1 to N are the responses from Tx1 to all Rx, rows   
% N+1 to 2N from Tx2 to all the Rx, and so on.

% In every output file some useful variables, as well as the channel responses, are stored:
str=sprintf('channel_%d_%d.mat', count1, count2);
save (str, 'multipaths', 'tspread', 'mindel', 'maxdel', 'DeltaT', 'T', 'paths', 'tx', 'ArrTx', 'rx', 'ArrRx', 'M', 'N');

% We repeat the same operations for all the channels due to different polarizations:
for c=1:3
   for d=1:3

       % We initialize the channel response as zero over every link and in every time: 
       channel=zeros(M*N,T);      
       
       for n=1:multipaths
    
          % The following line computes the "temporal position" of each received ray: 
          index=floor(paths(n).maindel/DeltaT);
   
          for a=1:M
               for b=1:N
                  % This is the contribute to add:
                  tmp=paths(n).fieldloss(c,d) * exp( i* (paths(n).ArrTxPhase(1,a)+paths(n).ArrRxPhase(1,b) ) );
                  channel((a-1)*N+b,index)=channel((a-1)*N+b,index)+tmp;
                end
          end   
       % P.S: The absolute values of all the lines of 'channel' are equal when different 
       % rays are not summed, due to a very high time resolution or a small number of rays.
       end
          
       e=c+3*(d-1);
       switch e 
          case 1, channelXX=channel; save (str, 'channelXX', '-append'); 
                  fprintf('\n      channelXX done');  channelXX=[];
          case 2, channelXY=channel; save (str, 'channelXY', '-append');
                  fprintf('\n      channelXY done');  channelXY=[];
          case 3, channelXZ=channel; save (str, 'channelXZ', '-append');
                  fprintf('\n      channelXZ done');  channelXZ=[];
          case 4, channelYX=channel; save (str, 'channelYX', '-append');
                  fprintf('\n      channelYX done');  channelYX=[];
          case 5, channelYY=channel; save (str, 'channelYY', '-append');
                  fprintf('\n      channelYY done');  channelYY=[];
          case 6, channelYZ=channel; save (str, 'channelYZ', '-append');
                  fprintf('\n      channelYZ done');  channelYZ=[];
          case 7, channelZX=channel; save (str, 'channelZX', '-append');
                  fprintf('\n      channelZX done');  channelZX=[];
          case 8, channelZY=channel; save (str, 'channelZY', '-append');
                  fprintf('\n      channelZY done');  channelZY=[];
          case 9, channelZZ=channel; save (str, 'channelZZ', '-append');
                  fprintf('\n      channelZZ done');  channelZZ=[];
       end
       
       tmp=[];
      
   end   
end

clear channel;
clear paths;

