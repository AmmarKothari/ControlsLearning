function [LEoBlade_pos, REoBlade_pos] = BullDozerKinematics(GPS_pos, arm_angle, roll_angle, yaw_angle)

% Transformation from GPS to Base of Arm
T_GPS_arm = groupSE3([1, 0, -10, 0, 0, 0]);

% Transformation from Base of Arm to Ball-Socket
T_arm_bs = groupSE3([3, 0, 0, 0, arm_angle, 0]);

% Trasnformation from BS to Center of Blade
T_bs_CoBlade = groupSE3([1, 0, 1, roll_angle, 0, yaw_angle]);

% Transformation from Center of Blade to Edges of Blade
T_CoBlade_LEoBlade = groupSE3([0, 4, -1, 0, 0, 0]);
T_CoBlade_REoBlade = groupSE3([0, -4, -1, 0, 0, 0]);

T_GPS_CoBlade = T_bs_CoBlade * T_arm_bs * T_GPS_arm;

LEoBlade = T_CoBlade_LEoBlade * T_GPS_CoBlade * groupSE3([GPS_pos;0;0;0]);
REoBlade = T_CoBlade_REoBlade * T_GPS_CoBlade * groupSE3([GPS_pos;0;0;0]);

LEoBlade_pos = LEoBlade(1:3, 4);
REoBlade_pos = REoBlade(1:3, 4);
end


function T = groupSE3(pose, inv)
    if nargin < 2
        inv = 'n';
    end
    x = pose(1);
    y = pose(2);
    z = pose(3);
    gamma = pose(4);
    beta = pose(5);
    alpha = pose(6);
    
	Rx = RotX(gamma);
	Ry = RotY(beta);
	Rz = RotZ(alpha);
	t = Tl(x, y, z);
    if inv == 'y'
        % this is the version from Murray Li and Sastry
        % weirdness happens when alpha = pi/2
%         R = Rz * Ry * Rx;
%         p = -R.'*[x;y;z;-1];
%         T = R.';
%         T(:,4) = p;
        T = RotX(-gamma) * RotY(-beta) * RotZ(-alpha) * Tl(-x,-y,-z);
    else
        T = t * Rz * Ry * Rx;
    end
end

function Rx = RotX(gamma)
    s = sin(gamma);
    c = cos(gamma);
	Rx = [
	1,	0,	0,	0;
	0,	c,	-s,	0;
	0,	s,	c,	0;
	0,	0,	0,	1;
	];
end

function Ry = RotY(beta)
    s = sin(beta);
    c = cos(beta);
	Ry = [
	c,		0,	s,	0;
	0,		1,	0,	0;
	-s,	0,	c,	0;
	0,		0,	0,	1
	];
end

function Rz = RotZ(alpha)
    s = sin(alpha);
    c = cos(alpha);
	Rz = [
	c,	-s,	0,	0;
	s,	c,	0,	0;
	0,	0,	1,	0;
	0,	0,	0,	1;
	];
end

function T = Tl(x,y,z)
	T = [
	1,	0,	0,	x;
	0,	1,	0,	y;
	0,	0,	1,	z;
	0,	0,	0,	1;
	];
end