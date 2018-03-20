function statep_out = dynamics(statep_in, stated_in, dt)
    % statep = x, y, theta
    % stated = v, theta_dot
    statep_out = statep_in;
    statep_out(1) = statep_in(1) + stated_in(1) * cos(statep_in(3)) * dt;
    statep_out(2) = statep_in(2) + stated_in(1) * sin(statep_in(3)) * dt;
    statep_out(3) = statep_in(3) + stated_in(2) * dt;
end