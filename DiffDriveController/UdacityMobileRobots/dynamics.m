function state_out = dynamics(v, w, state, dt)
    x = state(1); y = state(2); phi = state(3);
    phidot = w;
    xdot = v * cos(phi);
    ydot = v * sin(phi);
    
    state_out = state + [xdot, ydot, phidot] * dt;
end
