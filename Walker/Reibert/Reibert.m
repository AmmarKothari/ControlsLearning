classdef Reibert
    %UNTITLED3 Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        % physical system
        mass = 1.0;
        g = 9.81;
        l0 = 1.5; l;
        body_moi = 1;
        leg_moi = 0.1;
        leg_k = 200;
        leg_damping = 1.0;
        
        % parameters that change
        leg_contact = 0; % in the air
        spring_force = 0;
        z_floor = 0;
        theta = 0;
        xd; x; % body velocity
        xd_last;
        x_foot;
        
        xd_all; x_all; % keeping records
        x_foot_all; 
        u_all;
        t_all;
        
        
        % control system
        dt = 1e-2;

        % simulate
        dt_sim = 1e-3;
    end
    
    methods
        function obj = Reibert()
            
            obj.l = obj.l0;
            obj.x = [0; obj.l];
            obj.xd = [0;0];
            obj.x_foot(1) = obj.x(1)-obj.l*sin(obj.theta);
            obj.x_foot(2) = obj.x(2)-obj.l*cos(obj.theta);
            
        end
        
        function obj = step(obj, u)
            % u is input force and angular torque
            % Forces at foot
            F_total = (obj.l0 - obj.l)*obj.leg_k+u(1);
            a = F_total/obj.mass*[sin(obj.theta); cos(obj.theta)] - [0; obj.g];
            
            obj.xd_last = obj.xd;
            obj.xd = obj.xd + obj.dt_sim*a;
            obj.x = obj.x + obj.dt_sim*obj.xd;
            
            % only extend leg if it is shorter than resting length
            foot = obj.x-obj.l*[sin(obj.theta); cos(obj.theta)];
            if foot(2) <= 0 % below the ground
                obj.leg_contact = 1;
                obj.l = norm([obj.x_foot(1); 0] - obj.x);
            else
                obj.leg_contact = 0;
                obj.l = obj.l0;
            end
            
            obj.x_foot = obj.x-obj.l*[sin(obj.theta); cos(obj.theta)];
            obj.x_foot_all = [obj.x_foot_all; obj.x_foot];
            obj.x_all = [obj.x_all; obj.x'];
            obj.u_all = [obj.u_all; u'];
        end
    end
end

