function [way_points] = planner(state_goal, state_start)
    step_dist = .2;
    max_V = 1;
    max_A = 0.2;
    % comes up with plan
    % just cutting it up into small pieces
    delta = state_goal - state_start;
    max_travel = sqrt(delta(1)^2 + delta(2)^2);
    steps = ceil(max_travel / step_dist);
    way_points = zeros(steps, length(state_goal));
    way_points(:,1) = linspace(state_start(1), state_goal(1), steps);
    way_points(:,2) = linspace(state_start(2), state_goal(2), steps);
    way_points(:,3) = linspace(state_start(3), state_goal(3), steps);
    flag = true;
    c = max_A;
    while flag
        v = - c * (linspace(0,max_travel,steps) - (max_travel/2)).^2 + max_V;
        if min(v) < 0
            c = c/2;
        elseif min(v) > 0.001
            c = c * 3;
        else
            flag = false;
        end
    end
    way_points(:,3) = v;
%     way_points(end-1,3) = way_points(2,3);
%     way_points(2:end-1,3) = max_V; %all but the start and end go at max velocity
    way_points(:,4) = linspace(state_start(4), state_goal(4), steps);

end

