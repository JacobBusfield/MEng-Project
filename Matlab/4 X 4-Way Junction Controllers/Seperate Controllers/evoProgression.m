%% Author: Jacob Busfield
%% Date: 10/05/2016

function [ ] = evoProgression( gpath, tpath )
% Plot progression of GA over generations.


%% Read generations
g = [];

fid = fopen( gpath); % open the file
while ~feof(fid) % loop over the following until the end of the file is reached.
      line = fgets(fid); % read in one line
      g = [ g ; str2num(line)];
end

% gens, ind. var.
x = 1 : size(g);
x = x';

%% Just plot
figure
h_d = plot(x, g);

title({'Car Durations for Champion Over Subsequent Generations';'Where Each Controller is Evolved Uniquely and Seperatly'});
xlabel('Generation');
ylabel('Duration / seconds');

%% Moving average filter
hold on
MAF = tsmovavg(g,'s',50,1);
plot(x, MAF);
legend('Champion`s Mean Simulation Duration', 'Moving Average (50 Coeffecients)');
hold off

%% Envelope
% hold on
% [envHigh, envLow] = envelope(g,16,'peak');
% envMean = (envHigh+envLow)/2;
% plot(x, envHigh, ...
%      x, envMean, ...
%      x, envLow);
% hold off

%% Savitzky-Golay Filters
% hold on
% cubicMA   = sgolayfilt(g, 3, 7);
% quarticMA = sgolayfilt(g, 4, 7);
% quinticMA = sgolayfilt(g, 5, 9);
% plot(x, [cubicMA quarticMA quinticMA]);
% hold off

end

