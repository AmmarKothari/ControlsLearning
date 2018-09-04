function soln = simplePendulumOptimBvp(config, param, nlpOpt)
% soln = simplePendulumOptimBvp(config, param, nlpOpt)
%
% Compute the minimum-torque solution to move the simple pendulum between
% two prescribed states in a specified duration. The transcription of the
% trajectory optimiatization problem is performed using Euler's method
% and multiple shooting with one step per segment. The duration of each
% step is the same (uniform time grid).
%
% INPUTS:
%   config = struct = configuration options for the trajectory optimization
%       .nStep = scalar = number of simulation steps
%       .beginState = [2, 1] = initial [angle; rate]
%       .finalState = [2, 1] = final [angle; rate]
%       .duration = scalar = duration of the trajectory
%   param = struct = parameters of the simple pendulum
%     .freq = scalar = undamped natural frequency squared
%                    = (gravity / length) for a point mass pendulum
%     .damp = scalar = normalized linear viscous friction term
%   nlpOpt = solver options object, created by:
%        >> nlpOpt = optimset('fmincon')
%       Useful options (set using "." operator, eg: nlpOpt.Display = 'off')
%           --> Display = {'iter', 'final', 'off'}
%           --> OptimalityTolerance = {1e-3, 1e-6}
%           --> ConstraintTolerance = {1e-3, 1e-5, 1e-10}
%
% OUTPUTS:
%   soln = struct = solution data
%    .grid = struct = values of the trajectory at the grid points
%       .time = [1, nStep+1] = knot points
%       .state = [2, nStep+1] = state at the knot points
%       .control = [1, nStep] = control over each step (constant)
%   .info = information about the optimization run
%       .nlpTime = time (seconds) spent in fmincon
%       .exitFlag = fmincon exit flag
%       .objVal = value of the objective function
%       .[all fields in the fmincon "output" struct]
%
% NOTES:
%
%   Minimize: integral of actuator torque along the entire trajectory.
%   J = \int_0^T u^2(t) dt
%
%   Subject to: system dynamics and boundary conditions
%

%%%% TODO:  Implement this function
dynFun = @(t,z,u) simplePendulumDynamics(z, u, param);
tGrid = linspace(0,config.duration, config.nStep+1);
h = tGrid(2) - tGrid(1);
unpackDecVar = getFunc_unpackDecVar(config.nStep+1);

% Initial Guess
guess = linearInitialGuess(config);
% guess = springBasedGuess(config);
% guess = fallDownGuess(config, dynFun, tGrid, param);


% Setup Constraints
P1.objective = @(D) objFun(D, unpackDecVar, tGrid);
P1.x0 = packDecVar(guess.x, guess.u);
P1.Aineq = [];
P1.bineq = [];
P1.Aeq = [];
P1.beq = [];
P1.lb = [];
P1.ub = [];
P1.nonlcon = @(D) constraintFunc(D, config, dynFun, tGrid, nlpOpt, unpackDecVar, h);
P1.nonlcon(P1.x0);
P1.options = nlpOpt;
P1.solver = 'fmincon';
tic;
[S1,fval,exitflag,output] = fmincon(P1); % solves the trajectory problem
soln.info.nlpTime = toc;

[soln.grid.state, soln.grid.control] = unpackDecVar(S1);
soln.grid.time = tGrid;
soln.info.exitFlag = exitflag;
soln.info.objVal = fval;
output_fields = fields(output);
for i = 1:length(output_fields)
    soln.info.(output_fields{i}) = output.(output_fields{i});
end

%%%% HINTS:   (not required, but will make things easier)
%
% Write sub-functions! I suggest at least four:
%   - objective function evaluation
%   - dynamics constraints
%   - unpack decision variables
%   - pack decision variables
%
% Write functions to convert between the decision variable vector (which is
% easy for fmincon to use, but incomprehensible to humans) and the state and
% control at the grid points (hard for fmincon, but easy for humans). This
% will make debugging MUCH easier. Trust me.
%
% Feel free to write a fancy initialization if you want, but I suggest starting
% with the following simple initialization routine:
%   - the state at the grid points varies linearly between the boundaries
%   - the control effort is set to zero
%
% Notice that the control value is constant over each segment - this comes from
% the definition of Euler's method. As a result there is one fewer columns in
% the control grid than the state grid.
%
%%%%%

end

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%
function D = packDecVar(x,u)
    D = [x(:);u(:)];
end
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%
function f = getFunc_unpackDecVar(nStep)
    f = @(D) nStep_unpackDecVar(D,nStep);
end
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%
function [x,u] = nStep_unpackDecVar(D,nStep)
    x = reshape(D(1:2*nStep), 2, []);
    u = reshape(D(2*nStep+1:end), 1, []);
end
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%
function J = objFun(D, unpackDecVar, tGrid)
    % u^2 along trajectory
    [~,u] = unpackDecVar(D);
    J = sum(u.^2);

%     % build a piecewise polynomial
%     pp_u = pchip(tGrid(1:end-1), u);
%     % integrate over polynomial
%     J = integral(@(t) ppval(pp_u,t), tGrid(1), tGrid(end));
end
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%
function [c, ceq] = constraintFunc(D, config, dynFun, tGrid, nlpOpt, unpackDecVar, h)
    c = []; %unused
    % run the simulation forward
    [x,u] = unpackDecVar(D);
    x(:,1) = config.beginState; % these can be passed as boundary constraints
    x(:,end) = config.finalState; % these can be passed as boundary constraints
    stateSize = size(x,1);
    % at each point in the traj run ODE45 to simulate that segment
    nGrid = length(tGrid)-1;
    ceq = zeros(nGrid,stateSize);
    for i = 1:nGrid
        % Very slow to use ODE45 -- implent by hand a simulation method
        % (RK4, Eulers, ...)
%         [T,X] = ode45(@(t,z) dynFun(t,z,u(i)), tGrid(i:i+1), x(:,i));
%         ceq(i,:) = transpose( X(end,:)' - x(:,i+1) );
        ceq(i,:) = transpose( ( x(:,i) + h*dynFun(0,x(:,i),u(i)) ) - x(:,i+1) );
    end
end
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%
function guess = linearInitialGuess(config)
    s = size(config.beginState, 1);
    guess.x = [];
    for i = 1:s
        guess.x = [guess.x; linspace(config.beginState(i), config.finalState(i), config.nStep+1)];
    end
    guess.u = zeros(1,config.nStep);
end

% function guess = guess = springBasedGuess(config)
%     s = size(config.beginState, 1);
%     cycles = floor(config.freq/2); % number of swings
%     guess.x = [];
%     for i = 1:s
%                 
%     end
%     
% end

function guess = fallDownGuess(config, dynFun, tGrid, param)
    s = size(config.beginState, 1);
    if all(config.finalState == [pi; 0])
        config.finalState = config.finalState + [1e-10; 0];
    end
    soln = ode45(@(t,z) dynFun(t,z,0), [tGrid(1), 1e5], config.finalState);
    guess.x = zeros(s, config.nStep+1);
    guess.u = zeros(1, config.nStep);
    i = 1;
    for t = tGrid(end:-1:1)
        guess.x(:,i) = deval(soln, t);
        
        guess.u(i) = param.freq * sin(guess.x(1,i));
        i = i + 1;
    end
    
end