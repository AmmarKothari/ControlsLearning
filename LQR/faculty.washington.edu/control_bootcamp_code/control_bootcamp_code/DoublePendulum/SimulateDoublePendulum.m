function SimulateDoublePendulum(t, y, l1, l2)

X1 = @(theta1) l1*sin(theta1);
Y1 = @(theta1) -l1*cos(theta1);

X2 = @(theta1, theta2) X1(theta1) + l2 * sin(theta1 + theta2);
Y2 = @(theta1, theta2) Y1(theta1) - l2 * cos(theta1 + theta2);

figure(1); hold on;
l12 = l1 + l2;
xlim([-l12, l12]);
ylim([-l12, l12]);
p0 = plot(0, 0, 'bo');
p1 = plot(X1(y(1,1)), Y1(y(1,1)), 'ro');
p2 = plot(X2(y(1,1), y(1,3)), Y2(y(1,1), y(1,3)), 'go');

l1 = plot([0, X1(y(1,1))],...
        [0, Y1(y(1,1))], 'b-');
l2 = plot([X1(y(1,1)), X2(y(1,1), y(1,3))],...
        [Y1(y(1,1)), Y2(y(1,1), y(1,3))], 'b-');
path = plot(X2(y(1,1), y(1,3)), Y2(y(1,1), y(1,3)), 'b-');
drawnow
for i = 1:length(t)
    x1 = X1(y(i,1));
    y1 = Y1(y(i,1));
    x2 = X2(y(i,1), y(i,3));
    y2 = Y2(y(i,1), y(i,3));
    
    p1.XData = x1; p1.YData = y1;
    p2.XData = x2; p2.YData = y2;
    l1.XData = [0, x1]; l1.YData = [0, y1];
    l2.XData = [x1, x2]; l2.YData = [y1, y2];
    path.XData = [path.XData, x2]; path.YData = [path.YData, y2];
    drawnow
end

end