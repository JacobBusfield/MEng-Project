%% Author: Jacob Busfield
%% Date: 10/05/2016

function [ o, n ] = compareDurations( oPath, nPath )
% Graph comparison of both generations (box plot and stacked bar chart).

%% Read old durations
o = [];

fid = fopen(oPath); % open the file
while ~feof(fid) % loop over the following until the end of the file is reached.
      line = fgets(fid); % read in one line
      o = [ o ; str2num(line)];
end

%% Read new durations
n = [];

fid = fopen(nPath); % open the file
while ~feof(fid) % loop over the following until the end of the file is reached.
      line = fgets(fid); % read in one line
      n = [ n ; str2num(line)];
end

%% Display data
figure
a1 = subplot(1,2,1);
bar(o, 'stacked');
title('Car durations for no controller');
xlabel('Tests');
ylabel('Duration / Seconds');

a2 = subplot(1,2,2);
bar(n, 'stacked');
title('Car durations for evolved controller');
xlabel('Tests');
ylabel('Duration / Seconds');
linkaxes([a1,a2],'xy')

%% BOXPLOT
% figure
oSum = sum(o,2);
Omin = min(oSum)
Omedian = median(oSum)
Omax = max(oSum)
nSum = sum(n,2);
nMin = min(nSum)
nmedian = median(nSum)
nMax = max(nSum)


% BOXPLOT ONLY ON 2015
figure
boxplot([nSum oSum],{'Evolved' 'Round-Robin'},'orientation','horizontal')
title({'Distribution of Car Durations for Round-robin vs';'Four-way EpiNet Controller on Three-way Junction'});
xlabel('Duration / Seconds');
ylabel('Controller');


end

