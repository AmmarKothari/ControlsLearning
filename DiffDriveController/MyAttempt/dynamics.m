function [stateL, stateR, state] = dynamics(stateL, stateR, state, dt)
    l = 10;
    % state are pos,vel,acc of each wheel
    xL = stateL(1); vL = stateL(2); aL = stateL(3);
    xR = stateR(1); vR = stateR(2); aR = stateR(3);
    x = state(1); y = state(2);
    v = state(3); theta = state(4);
    omega = (vR - vL)/l;
    theta = theta + omega * dt;
    % fix wrap around
    if theta < -pi
        theta = theta + 2*pi;
    elseif theta > pi
        theta = theta - 2*pi;
    end
    if (vR - vL) == 0
        R = inf; % when it is going straight
    elseif vR == -vL
        R = 0; % spinning in place
    else
        R = l/2 * (vL + vR)/(vR - vL);
    end
    stateL(1) = xL + vL * dt + 1/2 * aL*dt^2;
    stateR(1) = xR + vR * dt + 1/2 * aR*dt^2;
    stateL(2) = vL + aL * dt;
    stateR(2) = vR + aR * dt;
    v = 1/2 * (vL + vR);
    v = clampV(v);
    if R ~= inf
        x_ICC = x - R * sin(theta);
        y_ICC = y + R * cos(theta);
        xp = cos(omega*dt) * (x-x_ICC) + -sin(omega*dt) * (y-y_ICC) + x_ICC;
        yp = sin(omega*dt) * (x-x_ICC) + cos(omega*dt) *  (y-y_ICC) + y_ICC;
    else
        xp = x + v * dt * cos(theta);
        yp = y + v * dt * sin(theta);
    end
    state(1) = xp; state(2) = yp; state(3)  = v; state(4) = theta;
    
end

function v_clamp = clampV(v)
vMax = 1; vMin = 0;
v_clamp = sign(v) * min(max(abs(v),vMin), vMax);

end