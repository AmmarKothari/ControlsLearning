points = 200;
%test 1
stateL = [0, 1.0, 0];
stateR = [0, 1.0, 0];
state  = [0, 0.0, 0, pi/2];

%test 2
% stateL = [0, 10.0, 0];
% stateR = [0, 5, 0];
% state  = [0, 0.0, 0, pi/8];

state_array = zeros( points, length(state));
state_t = state_array;
dt = 0.1;
t = dt:dt:points*dt';
for i = 1:points
    [sL, sR, s] = dynamics(stateL, stateR, state, dt);
    state_array(i,:) = s;
    state_t(i, :) = state_track(s);
    stateL = sL; stateR = sR; state = s;
end
% center point
plot(state_array(:,1), state_array(:,2), 'c*'); %end
hold on;
% plot(t, state_array(:,1 ),'b*')
% plot(t, state_array(:,2),'r*')
% plot(t, state_array(:,4), 'k*')
% tracking point
plot(state_t(:,1), state_t(:,2), 'go')
% plot(t, state_t(:,1 ),'bo')
% plot(t, state_t(:,2),'ro')
% plot(t, state_t(:,4), 'ko')
% axis([0,1,0,1])
hold off;

%% Controller Test
state_current  = [0, 0.0, 0, pi/2];
% state_goal = [1, 0, 0, pi/2];
state_goal = [1, 1, 0, 0];
errorTotal = 0;
errorLast = 0;
pwm = controller(state_goal, state_current, errorTotal, errorLast);
[aL, aR] = pwm2torque(pwm)

%% Planner Test
state_current  = [0, 0.0, 0, pi/2];
% state_goal = [1, 0, 0, pi/2];
% state_goal = [10, 10, 0, 0];
state_goal = [10, 10, 0, 0];
way_points = planner(state_goal, state_current);

%% Executer Test
state_current  = [0, 0.0, 0, pi/2];
state_goal = [0, 10, 0, pi/2];
way_points = planner(state_goal, state_current);
executer(way_points)
