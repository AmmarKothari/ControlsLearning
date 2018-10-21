TrueKin = struct('l', 1, 'theta_zero', 0.1);



getPosition = @(kin, theta) kin.l * [cos(kin.theta_zero + theta), sin(kin.theta_zero + theta)];

measurement_pts = 100;
test_thetas = linspace(0,2*pi,measurement_pts);
measured_position = zeros(measurement_pts, 2);
for i = 1:measurement_pts
    measured_position(i,:) = getPosition(TrueKin, test_thetas(i));
end