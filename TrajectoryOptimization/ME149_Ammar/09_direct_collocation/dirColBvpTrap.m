function soln = dirColBvpTrap(problem)
% soln = dirColBvpTrap(problem)
%
% This function computes the solution to a simple trajectory optimization
% problem using the trapezoid method for direct collocation.
%
% minimize: J = integral(pathObj(t, x, u))
%
% subject to:
%
%       dynamics:  dx = dynamics(t, x, u)
%
%       boundary constraints:  [c, ceq] = bndCst(x0, xF)
%
% given: time grid and initialization for state and control
%
%
% INPUT: "problem" -- struct with fields:
%
%     func -- struct for user-defined functions, passed as function handles
%
%         Input Notes:  square braces show size:  [a,b] = size()
%                 t = [1, nTime] = time vector (grid points)
%                 x = [nState, nTime] = state vector at each grid point
%                 u = [nControl, nTime] = control vector at each grid point
%                 x0 = [nState, 1] = initial state
%                 xF = [nState, 1] = final state
%
%         dx = dynamics(t, x, u)
%                 dx = [nState, nTime] = dx/dt = derivative of state wrt time
%
%         dObj = pathObj(t, x, u)
%                 dObj = [1, nTime] = integrand from the cost function
%
%         [c, ceq] = bndCst(t0, tF, x0, xF)
%                 c = column vector of inequality constraints  ( c <= 0 )
%                 ceq = column vector of equality constraints ( c == 0 )
%
%     guess - struct with an initial guess at the trajectory
%
%         .time = [1, nGrid] = time grid for the transcription (constant)
%         .state = [nState, nGrid] = guess for state at gridpoints
%         .control = [nControl, nGrid] = guess for control at gridpoints
%
%     nlpOpt = solver options object, created by:
%           >> nlpOpt = optimset('fmincon')
%       Useful options (set using "." operator, eg: nlpOpt.Display = 'off')
%           --> Display = {'iter', 'final', 'off'}
%           --> OptimalityTolerance = {1e-3, 1e-6}
%           --> ConstraintTolerance = {1e-3, 1e-5, 1e-10}
%
%
%   OUTPUT: "soln"  --  struct with fields:
%
%     .grid = solution at the grid-points
%         .time = [1, nTime]
%         .state = [nState, nTime] = state at each grid point
%         .control = [nControl, nTime] = control at each grid point
%         .dState = [nState, nTime] = derivative of state at grid points
%
%     .spline = method-consistent spline interpolation of the trajectory
%          .state = matlab PP struct = contains state trajectory
%          .control = matlab PP struct = contains control trajectory
%
%     .info = information about the optimization run
%         .nlpTime = time (seconds) spent in fmincon
%         .exitFlag = fmincon exit flag
%         .objVal = value of the objective function
%         .[all fields in the fmincon "output" struct]
%
%
% NOTES:
%
%   guess.time is used as the time grid for the transcription
%

% Constants
gridPoints = size(problem.guess.time,2);
stateDims = size(problem.guess.state,1);
controlDims = size(problem.guess.control,1);
hStep = problem.guess.time(2) - problem.guess.time(1);

% define the dynamics
dynFunc = @(t,x,u) problem.func.dynamics(t,x,u);

% define an initial guess
guess = problem.guess;

% define the cost function
pathObj = @(t,x,u) problem.func.pathObj(t,x,u);

% define the constraints

% define the packing function

% define an unpacking function

% setup problem
unpackFunc = @(DV) unpackDecVar(DV, stateDims, controlDims, gridPoints);
P1.x0 = packDecVar(guess.state, guess.control);
[x,u] = unpackFunc(P1.x0);
P1.objective = @(DV) objFunc(DV, unpackFunc, hStep, pathObj);
P1.Aineq = [];
P1.bineq = [];
P1.Aeq = [];
P1.beq = [];
P1.lb = [];
P1.ub = [];
P1.nonlcon = @(DV) dynamicsConstraints(DV, dynFunc, unpackFunc, hStep);
P1.nonlcon(P1.x0);
P1.options = problem.nlpOpt;
P1.solver = 'fmincon';

% run the minimization
[S1,fval,exitflag,output] = fmincon(P1); % solves the trajectory problem

end


function DV = packDecVar(x,u)
    DV = [x(:); u(:)];
end

function [x,u] = unpackDecVar(DV, stateDims, controlDims, gridPoints)
    sepIndex = stateDims*gridPoints;
    x = reshape(DV(1:sepIndex), stateDims, []);
    u = reshape(DV(sepIndex+1:end), controlDims, []);
end

function J = objFunc(DV, unpackFunc, hStep, pathObj)
    % trapezoidal integration
    [~,u] = unpackFunc(DV);
    u_upper = pathObj(u(1:end-1));
    u_lower = pathObj(u(2:end));
    J = hStep/2*sum(u_upper + u_lower);
end

function [c, ceq] = dynamicsConstraints(DV, dynFunc, unpackFunc, hStep)
    % trapezoidal integration
    c = [];
    [x,u] = unpackFunc(DV);
    x_lower = x(:,1:end-1);
    x_upper = x(:,2:end);
    xd = dynFunc(0,x,u);
    xd_lower = xd(:, 1:end-1);
    xd_upper = xd(:, 2:end);
    x_bar = x_lower + hStep/2*(xd_lower + xd_upper);
    ceq = x_upper - x_bar;
end

% function [Aeq, beq] = boundaryConstraints(