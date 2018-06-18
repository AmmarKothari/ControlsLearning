M = csvread('State_recording.csv');
M2 = csvread('NoisyState_recording.csv');
t = cumsum(M(:,1));
ax1 = subplot(3,1,1);
ax2 = subplot(3,1,2);
ax3 = subplot(3,1,3);


plot(ax1, t, M(:,2), 'rx', t, M2(:,2), 'b^'); legend('Position');
plot(ax2, t, M(:,3), 'rx', t, M2(:,3), 'b^'); legend('Velocity');
plot(ax3, t, M(:,4), 'rx', t, M2(:,4), 'b^');
legend('Actual', 'Noise');