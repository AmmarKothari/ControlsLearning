function dz = cannonDynamics(~,z,c)
% this function simulates a point mass flying through the air with drag
%
%INPUTS:
%   ~ = [1 x N] time vector (not used)
%   z = [4 x N] state vector
%   z(1,:) = x
%   z(2,:) = y
%   z(3,:) = dx
%   z(4,:) = dy
%   c = drag coefficient
%
%
% OUTPUTS:
%   dz = d(x)/dt -- state derivative
%
% NOTES:
%   gravity is 1 -- could be 9.8, but basically everything normalized by
%   gravity

dz = zeros(size(z));

dz(1:2, :) = z(3:4,:);

dx = z(3,:);
dy = z(4,:);
v = sqrt(dx^2 + dy^2);

% fx = -c*dx*v;
% fy = -c*dy*v;
theta = atan2(dy, dx);
f = c*v^2;
fx = -f*cos(theta);
fy = -f*sin(theta);

dz(3) = fx;
dz(4) = fy;

end