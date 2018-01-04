%% Example from http://matlab.cheme.cmu.edu/2011/08/09/phase-portraits-of-a-system-of-odes/

% y'' + sin(y) = 0 --> Pendulum with no damping
% Y(1) = y
% Y(2) = y'
f = @(t,Y) [Y(2); -sin(Y(1))]; % standard state space form

% y'' + sin(y) - y'*mu = 0 
% mu = 2;
% f = @(t, Y) [Y(2); -sin(Y(1)) + -sign(Y(2))*Y(2)*mu];

drawPhasePortrait(f);


%% Section 2.9, Pg 39
%% 2.2 (a)
% theta'' + theta' + 0.5*theta = 0
f = @(t,Y) [Y(2); -Y(2) - 0.5*Y(1)];
drawPhasePortrait(f);
%% 2.2 (b)
% theta'' + theta' + 0.5*theta = 1
f = @(t,Y) [Y(2); -Y(2) - 0.5*Y(1) - 1];
drawPhasePortrait(f);
%% 2.2 (c)
% theta'' + theta'^2 + 0.5*theta = 0
f = @(t,Y) [Y(2); -Y(2)^2 - 0.5*Y(1)];
drawPhasePortrait(f);