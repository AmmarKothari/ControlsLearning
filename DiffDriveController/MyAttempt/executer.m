function result = executer(way_points)
    state_start = way_points(1,:);

    goal_flag = false;
    i_w = 1;
    state_current = state_start;
    stateL = state_start;
    stateR = state_start;
    dt = 0.1;
    step_count = 0;
    step_limit = 10^3/3;
    error_limit = 0.1;
    state_all = [];
    error_all = [];
    while ~goal_flag && step_count < step_limit
        step_count = step_count + 1;
        errorTotal = 0;
        errorLast = 0;
       % send first way point to controller
       [pwm, errorLast, errorTotal] = controller(way_points(i_w,:), state_current, errorTotal, errorLast);
       % convert pwm to acceleration
       [aL, aR] = pwm2torque(pwm);
       % acceleration to dynamics
       stateR(3) = aR;
       stateL(3) = aL;
       [stateL, stateR, state_current] = dynamics(stateL, stateR, state_current, dt);
       state_all = [state_all; state_current];
       error_all = [error_all; errorLast];
%        plot(state_all(:,1), state_all(:,2), 'ro');
       figure(1);
       plot(state_all(end,1), state_all(end,2), 'ro');
       hold on;
       plot(way_points(1,1), way_points(1,2), 'kx');
       plot(way_points(end,1), way_points(end,2), 'bx');
       xlim([min(way_points(:,1)), max(way_points(:,1)) + 1]);
       hold off;
       figure(2); plot(error_all,'r');hold on; plot(state_all(:,3),'b');hold off;
       pause(0.001);
       way_point_check = sum(abs(state_current(:)' - way_points(i_w,:)));
       % check state against goal and update as necessary
%        if i_w < size(way_points,1)
%            % only care about position
%            way_point_check = sum(abs(state_current(1:2) - way_points(i_w,1:2)));
%        else
%            way_point_check = sum(abs(state_current(:) - way_points(i_w,:)));
%        end
       if way_point_check < error_limit
           i_w = i_w + 1;
           fprintf('Waypoint %d Started \n', i_w);
           if i_w > size(way_points,1) %stop when you have reached goal
               goal_flag = true;
           end
       end
    end
    
    if ~(step_count < step_limit)
        result = 0;
        disp('Steps Exceeded');
    else
        result = goal_flag;
    end    
end