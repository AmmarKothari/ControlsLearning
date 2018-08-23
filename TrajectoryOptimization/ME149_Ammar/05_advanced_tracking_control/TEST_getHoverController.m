
%       .m = mass of the quadrotor
%       .w = distance between the rotors (width)
%       .g = gravity acceleration
param.m = 1;
param.w = 0.1;
param.g = 9.8;

z = [0; 0; 0; 0; 0; 0];
u = [1; 1] * (param.m * param.g)/2;
[A, B, dzRef] = planarQuadrotorLinDyn(z, u, param)

% hoverController = getHoverController(0, 0, param);