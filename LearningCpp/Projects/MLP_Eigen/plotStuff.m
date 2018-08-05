M = csvread('Overall.csv');

ax1 = subplot(2,1,1);
ax2 = subplot(2,1,2);

% plot(ax1, M(:,1), M(:,2), 'rx-');
% ylim([0,1]);
% plot(ax2, M(:,1), M(:,3), 'bo-');

plot(ax1, M(:,3), 'rx-');
title(ax1, 'Loss');
plot(ax2, M(:,2), 'rx');
title(ax2, 'Accuracy');