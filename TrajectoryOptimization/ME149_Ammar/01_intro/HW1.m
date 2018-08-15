%% HW1

%% problem 1 -- fun with random sequences
% Started at 1:00 PM
% Finished at 1:20 PM
% Part 1

pt_circ_func = @(r,theta) [r*cos(theta), r*sin(theta)];

r = 1;
init_pts_ct = 3;
thetas = linspace(0,2*pi, init_pts_ct+1)';
init_pts = pt_circ_func(r, thetas(1:init_pts_ct));

thetas = 0:0.01:2*pi;
circ = pt_circ_func(r, thetas');

N = 5000;
alpha = 1/2;

P = zeros(N+1, 2);
P(1,:) = init_pts(1,:);

for i = 1:N
    P(i+1,:) = alpha*P(i,:) + (1-alpha)*init_pts(randi(init_pts_ct),:); 
end

subplot(2,1,1);
plot(circ(:,1), circ(:,2), 'b-');
hold on
plot(init_pts(:,1), init_pts(:,2), 'ro');
plot(init_pts(1,1), init_pts(1,2), 'gx');
plot(P(2:end,1), P(2:end, 2), 'b.');
hold off

% part 2
init_pts_ct = 4;
thetas = linspace(0,2*pi, init_pts_ct+1)';
init_pts = pt_circ_func(r, thetas(1:init_pts_ct));

thetas = 0:0.01:2*pi;
circ = pt_circ_func(r, thetas');

N = 5000;
alpha = 1/3;

P = zeros(N+1, 2);
P(1,:) = init_pts(1,:);

for i = 1:N
    P(i+1,:) = alpha*P(i,:) + (1-alpha)*init_pts(randi(init_pts_ct),:); 
end

subplot(2,1,2);
plot(circ(:,1), circ(:,2), 'b-');
hold on
plot(init_pts(:,1), init_pts(:,2), 'ro');
plot(init_pts(1,1), init_pts(1,2), 'gx');
plot(P(2:end,1), P(2:end, 2), 'b.');
hold off

%% Problem 2
% Start time at 1:20 PM
% End time at 1:38 PM
syms t
x = (1 + (t-2)^2)*sin(3*t);
dx = diff(x,t); 
ddx = diff(x,t,2);

y = t^3 - 6*t^2 + 2*t + 5;
dy = diff(y,t);
ddy = diff(y,t,2);


t_eval = 0:0.1:5;
subplot(3,2,1)
plot(t_eval, subs(x, t, t_eval), 'b-');

subplot(3,2,2)
[~,max_ind] = max(subs(y, t, t_eval));
[~,min_ind] = min(subs(y, t, t_eval));
y_vals = subs(y, t, t_eval);
plot(t_eval, y_vals, 'b-');
hold on;
plot(t_eval(max_ind), y_vals(max_ind), 'bo');
plot(t_eval(min_ind), y_vals(min_ind), 'rx');

subplot(3,2,3)
plot(t_eval, subs(dx, t, t_eval), 'b-');

subplot(3,2,4)
plot(t_eval, subs(dy, t, t_eval), 'b-');


subplot(3,2,5)
plot(t_eval, subs(ddx, t, t_eval), 'b-');

subplot(3,2,6)
plot(t_eval, subs(ddy, t, t_eval), 'b-');




