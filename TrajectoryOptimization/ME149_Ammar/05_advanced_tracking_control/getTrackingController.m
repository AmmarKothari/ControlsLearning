function [controller, gains] = getTrackingController(ref, param)
% [controller, gains] = getTrackingController(ref, param)
%
% This function designs a controller that will track a specified reference
% trajectory using finite-horizon LQR.
%
% INPUT REQUIREMENTS:
%   - the reference trajectory must be feasible
%   - the reference trajectory must end in a fixed-point (hover)
%
% CONTROLLER REQUIREMENTS:
%   - let [tLow, tUpp] be the time domain of the reference trajectory
%   - the behavior is undefined if input time t < tLow
%   - the controller must regulate the final state on the trajectory for any
%     input time t > tUpp.
%
% INPUTS:
%   ref = struct with a reference trajectory
%     .state = Matlab PP struct = state reference trajectory
%     .control = Matlab PP struct = nominal (feed-forward) control
%   param = struct with constant scalar parameters:
%       .m = mass of the quadrotor
%       .w = distance between the rotors (width)
%       .g = gravity acceleration
%
% OUTPUTS:
%   controller = function handle:  u = controller(t, z)
%       IN: z = [6, n] = [x; y; q; dx; dy; dq] = state
%             x = horizontal position
%             y = vertical position
%             q = absolute angle (zero for hover)
%             dx = time-derivative of horizontal position
%             dy = time-derivative of vertical position
%             dq = time-derivative of absolute angle (zero for hover)
%       OUT: u = [2, n] = [u1; u2] = control
%             u1 = left rotor force
%             u2 = right rotor force
%   gains = Matlab PP struct = LQR gains along the trajectory
%       K(t) = reshape(ppval(gains, t), nState, nControl);
%
% NOTES:
%   The nominal system dynamics for the quadrotor are defined in:
%        ME149/codeLibrary/modelSystems/planarQuadrotor/
%

%%%% TODO: specify the maximum "tolerable" state perturbation and control effort
% Set the tolerable errors in state and values in actuation
zTol = [0.1;  % tolerable error in horizontal position (m)
    0.1;  % tolerable error in vertical position (m)
    0.8;  % tolerable error in angle (rad)
    0.3;  % tol. error in horiz. vel. (m/s)
    0.3;  % tol. error in vert. vel. (m/s)
    0.7];  % tol. error in ang. vel. (m/s)
uTol = 0.6 * [1;1];  % tolerable actuation effort above nominal


%%%% TODO: set the (constant) state (Q) and actuator (R) cost matricies
% Cost terms for the LQR controller:
Q = diag(1./(zTol.^2));  % cost on state errors
R = diag(1./(uTol.^2));  % cost on actuator effort
% Q = eye(ref.state.dim);
% R = 10*eye(ref.control.dim);

%%%% TODO: call trajectoryLqr() to compute gains along the trajectory
% u_des = planarQuadrotorHoverThrust(param);
linSys = @(t) planarQuadrotorLinDyn(ppval(ref.state, t), ppval(ref.control,t), param);
kGrid = trajectoryLqr(ref.time, linSys, Q, R);

%%%% TODO: fit a cubic spline to the gains using pchip()
gains = pchip (ref.time, kGrid);

%%%% TODO: return the function handle for the controller
controller = @(t, z) quadrotorTrackingController(t, z, ref, gains);
end

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%

function u = quadrotorTrackingController(t, z, ref, gains)
%
%%%% TODO: document this function if you choose to use it (delete it otherwise)
%

%%%% TODO: compute reference state and control
t = min(ref.time(end), max(ref.time(1), t));
z_des = ppval(ref.state, t);
u_des = ppval(ref.control, t);

%%%% TODO: compute the feed-back gain matrix K
k = reshape(ppval(gains, t), ref.control.dim, ref.state.dim);

%%%% TODO: compute the output control (nominal + feedback)
u = u_des - k*(z - z_des);


end
