%% Assignment 4

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
    fprintf("%s = %s \n", names{i}, s.(names{i}));
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
fprintf("J(x(t)) = %s \n", simplify(J(t)));
%% Part 2
clear all;
run('../../ME149_Spring2018/codeLibrary/addLibraryToPath.m');

% simple pendulum
theta_ref = [0, pi/2, pi];
t = linspace(0,1,3);

ref_q_pp = spline(t, theta_ref);
ref_dq_pp = ppDer(ref_q_pp);
ref_ddq_pp = ppDer(ref_dq_pp);


% u_ref = simplePendulumInvDyn(