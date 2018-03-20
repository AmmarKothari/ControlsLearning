function st = state_track(state_center)
    d = 1; % distance of tracking point from center point
    x = state_center(1);
    y = state_center(2);
    v = state_center(3);
    theta = state_center(4);
    
    st = state_center;
    st(1) = x + d * cos(theta);
    st(2) = y + d * sin(theta);
end