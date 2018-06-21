M = csvread('Overall.csv');

plot(M(:,1), M(:,2), 'rx-');
ylim([0,1]);