function soln = dirColBvpHermiteSimpson(problem)
% soln = dirColBvpHermiteSimpson(problem)
%
% This function computes the solution to a simple trajectory optimization
% problem using the Hermite--Simpson method for direct collocation.
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
%         .time = [1, nGuess] = time grid for the transcription (constant)
%         .state = [nState, nGuess] = guess for state at gridpoints
%         .control = [nControl, nGuess] = guess for control at gridpoints
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
P_spec.gridPoints.original = size(problem.guess.time,2);
P_spec.gridPoints.resampled = size(problem.guess.time,2)*2-1; % uncompressed so need midpoints of each spline
P_spec.stateDims = size(problem.guess.state,1);
P_spec.controlDims = size(problem.guess.control,1);

% define the dynamics
dynFunc = @(t,x,u) problem.func.dynamics(t,x,u);

% define an initial guess
guess = insert_MidKnots(problem.guess, P_spec);
P_spec.hStep = diff(guess.time);

% define the cost function
pathObj = @(t,x,u) problem.func.pathObj(t,x,u);

% define the constraints

% setup problem
unpackFunc = @(DV) unpackDecVar(DV, P_spec);
P1.x0 = packDecVar(guess.state, guess.control);
P1.objective = @(DV) objFunc(DV, unpackFunc, P_spec.hStep, pathObj);
P1.Aineq = [];
P1.bineq = [];
% [P1.Aeq, P1.beq] = boundaryConstraints(guess, problem);
P1.Aeq = []; P1.beq = [];
P1.lb = [];
P1.ub = [];
P1.nonlcon = @(DV) dynamicsConstraints(DV, dynFunc, unpackFunc, P_spec.hStep, problem.func.bndCst);
P1.options = problem.nlpOpt;
P1.solver = 'fmincon';

% run the minimization
tic;
[S1,fval,exitflag,output] = fmincon(P1); % solves the trajectory problem
soln.info.nlpTime = toc;
soln.info.exitFlag = exitflag;
soln.info.objVal = fval;
output_fields = fields(output);
for i = 1:length(output_fields)
    soln.info.(output_fields{i}) = output.(output_fields{i});
end

soln.grid.time = guess.time;
[soln.grid.state, soln.grid.control] = unpackFunc(S1);
soln.grid.dState = dynFunc(guess.time, soln.grid.state, soln.grid.control);

% linear piece-wise spline -- this is linked to the integration method when
% solving
soln.spline.state = ppSpline3a(soln.grid.time, soln.grid.state, soln.grid.dState);
soln.spline.control = ppSpline2b(soln.grid.time, soln.grid.control);
end 


function DV = packDecVar(x,u)
    DV = [x(:); u(:)];
end

function [x,u] = unpackDecVar(DV, P_spec)
    stateDims = P_spec.stateDims;
    controlDims = P_spec.controlDims;
    gridPoints = P_spec.gridPoints.resampled;
    sepIndex = stateDims*gridPoints;
    x = reshape(DV(1:sepIndex), stateDims, []);
    u = reshape(DV(sepIndex+1:end), controlDims, []);
end

function J = objFunc(DV, unpackFunc, hStep, pathObj)
    % Hermite-Simpson integration
    [~,u] = unpackFunc(DV);
    b = 0*u(1:2:end-2);
    u_lower = pathObj(b,b,u(1:2:end-2));
    u_mid = pathObj(b,b,u(2:2:end-1));
    u_upper = pathObj(b,b,u(3:2:end));
    J = sum(hStep(1:2:end)./6.*( u_lower +  4.*u_mid + u_upper ) );
end

function [c, ceq] = dynamicsConstraints(DV, dynFunc, unpackFunc, hStep, boundaryFunc)
    % Hermite-Simpson integration
    [x,u] = unpackFunc(DV);
    x_lower = x(:,1:2:end-2);
    x_upper = x(:,3:2:end);
    xd = dynFunc(0,x,u);
    xd_lower = xd(:, 1:2:end-1);
    xd_mid = xd(:, 2:2:end-1);
    xd_upper = xd(:,3:2:end);
    x_bar = x_lower + hStep(1:2:end)./6.*(xd_lower + 4.*xd_mid + xd_upper);
%     x_bar = x_lower + hStep(1:2:end)./2.*(xd_lower + xd_upper); %trap rule
    defects = (x_upper - x_bar)';
    
    % Boundary Conditions
    t0 = 0;
    tF = 0;
    x0 = x(:, 1);
    xF = x(:, end);
    [bndCstIneq, bndCstEq] = boundaryFunc(t0, tF, x0, xF);
    
    % combine everything
    c = bndCstIneq;
    ceq = [defects(:); bndCstEq];
end

function [Aeq, beq] = boundaryConstraints(guess, problem) % this should use the provided boundary function
    [nStates, tGridCt] = size(guess.state);
    [nControl, ~] = size(guess.control);
    Aeq_dim = tGridCt*(nStates + nControl);
    Aeq = zeros(2*nStates,Aeq_dim);
    boundary_idx = [1:nStates,... % boundary condition at t = 0
                    nStates*(tGridCt-1)+1:nStates*(tGridCt)];
    for i =  1:length(boundary_idx)
        Aeq(i,boundary_idx(i)) = 1;
    end
    [A1,A2] = problem.func.bndCst(guess.time(1), guess.time(end), guess.state(:,1), guess.state(:,end));
    beq = A2 + [guess.state(:,1); guess.state(:,end)];
end

function guess = insert_MidKnots(guess_original, P_spec)
% this can be done with a matrix operation!
    guess.state = [];
    guess.control = [];
    for i = 1:P_spec.stateDims
        guess.state = [guess.state;
                    interp(guess_original.state(i,:), 2)];
    end
    for i = 1:P_spec.controlDims
        guess.control = [guess.control;
                    interp(guess_original.control(i,:), 2)];
    end
	guess.time = interp(guess_original.time(1,:), 2);
    
    guess.state = guess.state(:,1:end-1); % removing last point because outside of range
    guess.control = guess.control(:,1:end-1); % removing last point because outside of range
    guess.time = guess.time(:,1:end-1);
end
