%% Author: Jacob Busfield
%% Date: 10/05/2016

%% MAIN FILE

% Graph fitness over subsequent generations.
evoProgression('4WJX4_durations_populations.txt');

% Graph comparison of both generations (box plot and stacked bar chart).
compareDurations('4WJX4_durations_unimproved.txt','4WJX4_durations_evolved.txt')


