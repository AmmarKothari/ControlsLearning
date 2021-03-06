%% Assignment 4
run('../../ME149_Spring2018/codeLibrary/addLibraryToPath.m');
%% Part 1
% part 1a
syms x0 xh v0 vh t x(t) v(t) a(t) h
syms c0 c1 c2 c3

x(t) =  c0 + c1*t + c2*t^2 + c3*t^3;
v(t) = diff(x(t), t);

eq1 = x0 == x(0);
eq2 = v0 == v(0);
eq3 = xh == x(h);
eq4 = vh == v(h);

s = solve([eq1; eq2; eq3; eq4], [c0; c1; c2; c3]);
names = fieldnames(s);
for i = 1:length(names)
    fprintf('%s = %s \n', names{i}, s.(names{i}));
end
% c0 = x0 
% c1 = v0 
% c2 = -(3*x0 - 3*xh + 2*h*v0 + h*vh)/h^2 
% c3 = (2*x0 - 2*xh + h*v0 + h*vh)/h^3 

% part 1b
c = [c0, c1, c2, c3];
for i = 1:length(names)
    x = subs(x, c(i), s.(names{i}));
end
a(t) = diff(x, t, 2);
fprintf("a(0) = %s \n", simplify(a(0)));
fprintf("a(h) = %s \n", simplify(a(h)));

% part 1c
syms J(t)
J(t) = int(diff(x,t,3)^2, t, 0, h);
fprintf('J(x(t)) = %s \n', simplify(J(t)));
%% Part 2
clear all;

%  % % % % % % % % %  simple pendulum % % % % % % % % % % % % % % 
x0 = 0; v0 = 0;
xF = pi; vF = 0;
t0 = 0; tF = 3; tDuration = 1.5*tF;
sim_points = 250;

q_mid = [0.5]*xF;

q_mid = [0.3, 0.8]*xF;


q_knot = [x0, q_mid, xF];
t = linspace(t0,tF, length(q_knot));
ref_q_pp = spline(t, [v0, q_knot, vF]);
ref_dq_pp = ppDer(ref_q_pp);
ref_ddq_pp = ppDer(ref_dq_pp);

g = 9.8;
l = 1;
param.freq = 1.0;
param.damp = 0;
z = [0;0];

u_ref = @(t,z) simplePendulumInvDyn([ppval(ref_q_pp,t); ppval(ref_dq_pp,t)], ...
    ppval(ref_ddq_pp,t),...
    param);

Kp = -5.4;
Kd = -0.01;

u = @(t, z) u_ref(t, z) + Kp.*(z(1,:) - ppval(ref_q_pp,t)) + Kd.*(z(2,:)-ppval(ref_dq_pp,t));

% J = @(t, z) integral( 

t_sim = [t(1), t(end)];
q0 = [ppval(ref_q_pp, 0) + 0.1; ppval(ref_q_pp, 0)];
soln = ode45(@(t,z) simplePendulumDynamics(z, u(t,z), param), t_sim, q0);

t_out = linspace(t_sim(1), t_sim(2), sim_points);
y_out = deval(soln, t_out);
u_out = zeros(size(t_out));
u_ref_out = zeros(size(t_out));
for i_t = 1:length(t_out)
    u_out(i_t) = u(t_out(i_t), y_out(:, i_t));
    u_ref_out(i_t) = u_ref(t_out(i_t), y_out(:,i_t));
end
J = sum(u_out.^2);


y_des = [ppval(ref_q_pp, t_out); ppval(ref_dq_pp, t_out)];
figure(1)
subplot(3,1,1)
plot(t_out, y_out(1,:), 'rx-')
hold on;
plot(t_out, y_des(1,:), 'go-')
hold off;
title('Position')
subplot(3,1,2)
plot(t_out, y_out(2,:), 'rx-')
hold on;
plot(t_out, y_des(2,:), 'go-')
hold off;
title('Velocity')
subplot(3,1,3)
plot(t_out, u_out, 'rx-')
hold on;
plot(t_out, u_ref_out, 'bo-')
hold off;
title(sprintf('Torque: %0.2f', J))

%%
clear all;
%  % % % % % % % % %  double pendulum % % % % % % % % % % % % % % 
x0 = [0; 0]; v0 = [0; 0];
xF = [pi; pi]; vF = [0; 0];
t0 = 0; tF = 5; tDuration = 1.5*tF;
sim_points = 250;
m = 2; %inputs

t = [t0, tF, tDuration];
q_knot = [x0, xF, xF];
qd_knot = [v0, vF, vF];

q_mid = [0.5].*xF;

q_mid = [0.3, 0.8].*xF;


q_knot = [x0, q_mid, xF];
t = linspace(t0,tF, size(q_knot,2));

ref_q_pp = spline(t, [v0, q_knot, vF]);
ref_dq_pp = ppDer(ref_q_pp);
ref_ddq_pp = ppDer(ref_dq_pp);

param.m1 = 1.0;
param.m2 = 1.0;
param.d1 = 1.0;
param.d2 = 1.0;
param.g = -1;
z = [0;0];

u_ref = @(t,z) doublePendulumInvDyn([ppval(ref_q_pp,t); ppval(ref_dq_pp,t)], ...
    ppval(ref_ddq_pp,t),...
    param);

Kp = -50.5;
Kd = -20.0;

u = @(t, z) u_ref(t, z) + Kp.*(z(1:m) - ppval(ref_q_pp,t)) + Kd.*(z(m+1:end)-ppval(ref_dq_pp,t));

t_sim = [t(1), t(end)];
q0 = [ppval(ref_q_pp, 0) + [0.1;0.1] ; ppval(ref_q_pp, 0)];
soln = ode45(@(t,z) doublePendulumDynamics(z, u(t,z), param), t_sim, q0);

t_out = linspace(t_sim(1), t_sim(2), sim_points);
y_out = deval(soln, t_out);
u_out = zeros(m, length(t_out));
u_ref_out = zeros(size(u_out));
for i_t = 1:length(t_out)
    u_out(:,i_t) = u(t_out(i_t), y_out(:, i_t));
    u_ref_out(:,i_t) = u_ref(t_out(i_t), y_out(:,i_t));
end
J = sum(u_out.^2, 2);


y_des = [ppval(ref_q_pp, t_out); ppval(ref_dq_pp, t_out)];
figure(2); clf;
% Theta 1
subplot(3,m,1)
plot(t_out, y_out(1,:), 'rx-')
hold on;
plot(t_out, y_des(1,:), 'go-')
hold off;
title('Position')
subplot(3,m,3)
plot(t_out, y_out(3,:), 'rx-')
hold on;
plot(t_out, y_des(3,:), 'go-')
hold off;
title('Velocity')
subplot(3,m,5)
plot(t_out, u_out(1,:), 'rx-')
hold on;
plot(t_out, u_ref_out(1,:), 'ro-')
hold off;
title(sprintf('Torque: %0.2f -> %0.2f', J(1), sum(J)))

% Theta 2
subplot(3,m,2)
plot(t_out, y_out(2,:), 'bx-')
hold on;
plot(t_out, y_des(2,:), 'go-')
hold off;
title('Position')
subplot(3,m,4)
plot(t_out, y_out(4,:), 'bx-')
hold on;
plot(t_out, y_des(4,:), 'go-')
hold off;
title('Velocity')
subplot(3,m,6)
plot(t_out, u_out(2,:), 'bx-')
hold on;
plot(t_out, u_ref_out(2,:), 'bo-')
hold off;
title(sprintf('Torque: %0.2f -> %0.2f', J(2), sum(J)))