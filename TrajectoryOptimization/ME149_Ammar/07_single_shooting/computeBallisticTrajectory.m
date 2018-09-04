function soln = computeBallisticTrajectory(param, nGrid, method, nlpOpt)
%  soln = computeBallisticTrajectory(param, nGrid, method, nlpOpt)
%
% Computes a ballistic trajectory, given a model of
% the projectile motion and the start and target locations.
%
% INPUTS:
%   param = parameters for the simulation and ballistic model:
%       .gravity = scalar = gravity constant
%       .wind = [3,1] = [w1; w2; w3] = wind velocity
%       .drag = scalar = quadratic drag constant
%       .mass = scalar = mass of the projectile
%       .start = [3,1] = initial position of the trajectory
%       .target = [3,1] = target final position on the trajectory
%   nGrid = scalar = number of gridpoints for the integration method
%           --> use a uniform time grid
%   method = string = name of the desired method
%        'euler' = Euler's method (first-order)
%        'heun' = Heun's method (second-order)
%        'midpoint' = the midpoint method (second-order)
%        'ralston' = Ralston's method (second-order)
%        'rk4' = "The" Runge--Kutta method (forth-order)
%   nlpOpt = solver options object, created by:
%        >> nlpOpt = optimset('fmincon')
%       Useful options (set using "." operator, eg: nlpOpt.Display = 'off')
%           --> Display = {'iter', 'final', 'off'}
%           --> OptimalityTolerance = {1e-3, 1e-6}
%           --> ConstraintTolerance = {1e-3, 1e-5, 1e-10}
%
% OUTPUT:
%   soln = struct with the solution to the ballistic trajectory:
%     .tGrid = [1, nTime] = time grid for the ballistic trajectory
%     .zGrid = [6, nTime] = [x1;x2;x3;  dx1; dx2; dx3] = state trajectory
%
% NOTES:
%
%   Method: Single Shooting with uniform time grid
%
%   NLP Solver:  fmincon()
%
%   Simulation Method:  runSimulation()
%       - explicit Runge--Kutta methods, order 1, 2, or 4.
%
%   Physics Model:  projectileDynamics()
%

%%%% TODO:  implement this function

dynFun = @(t,z) projectileDynamics(z, param);

% Initial Guess -- taken from solution
xDel = param.target - param.start;
dist = norm(xDel);  % distance to the target
xDel(3) = xDel(3) + 0.5*dist;  % aim above the target
guess.T = sqrt(2*param.gravity*xDel(3));  % time to fall
guess.v0 = xDel / guess.T;  % rough guess at the vector

P1.objective = @objFun;
P1.x0 = packDecVar(guess.T, guess.v0);
P1.Aineq = [];
P1.bineq = [];
P1.Aeq = [];
P1.beq = [];
P1.lb = [];
P1.ub = [];
P1.nonlcon = @(D) constraintFunc(D, param, dynFun, nGrid, method);
P1.options = nlpOpt;
P1.solver = 'fmincon';

[S1,fval,exitflag,output] = fmincon(P1); % solves the trajectory problem

[~,~,tGrid, zGrid] = P1.nonlcon(S1);

%%%% HINT:  a good implementation will have at least one sub-function
soln.zGrid = zGrid;
soln.tGrid = tGrid;
soln.output = output;
end

function D = packDecVar(T, v0)
    D = [T;v0];
end

function [T,v0] = unpackDecVar(D)
    T = D(1);
    v0 = D(2:end);
end

function J = objFun(D)
    [~,v0] = unpackDecVar(D);
    J = sum(v0.^2);
end

function [c, ceq, tGrid, zGrid] = constraintFunc(D, param, dynFun, nGrid, method)
    [T,v0] = unpackDecVar(D);
    tGrid = linspace(0,T,nGrid);
    [zGrid, ~] = runSimulation(dynFun, tGrid, [param.start; v0], method);
    c = [];
    ceq = zGrid(1:3, end) - param.target; % only constraint is to have this be zero

end
