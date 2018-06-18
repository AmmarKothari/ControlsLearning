classdef Reibert
    %UNTITLED3 Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        % physical system
        mass = 1.0;
        g = 9.81;
        l0 = 0.5; l; ld
        body_moi = 1;
        leg_moi = 0.1;
        leg_k = 200;
        leg_damping = 1.0;
        
        % parameters that change
        leg_contact = 0; % in the air
        spring_force = 0;
        z_floor = 0;
        theta = 0; theta_d = 0;
        xd; x; % body velocity
        xd_last;
        xd_foot; x_foot;
        
        xd_all; x_all; % keeping records
        x_foot_all;
        theta_all;
        u_all;
        t_all;
        leg_contact_all;
        
        
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
            % % % %
            % Calculate foot location
            % % % % %
            foot = obj.x - obj.l0 * [sin(obj.theta); cos(obj.theta)];
            if foot(2) > 0 % in the air
                obj.leg_contact = 0;
                obj.l = obj.l0;
                obj.x_foot = obj.x - obj.l * [sin(obj.theta); cos(obj.theta)];
            else
                if obj.leg_contact == 0 % just touched the ground
                    obj.x_foot = [obj.x(1) - obj.l0*sin(obj.theta); 0]; % full length of leg; foot touching ground
                    obj.leg_contact = 1;
                end
                obj.l = norm(obj.x - obj.x_foot);
                obj.ld = (obj.x - obj.x_foot)'*obj.xd / obj.l;
                
            end            
            
            % Forces at foot
            F_total = (obj.l0 - obj.l)*obj.leg_k+u(1);
            
            % move point based on COM acceleration
            a = F_total/obj.mass*[sin(obj.theta); cos(obj.theta)] - [0; obj.g];
            obj.xd_last = obj.xd;
            obj.xd = obj.xd + obj.dt_sim*a;
            obj.x = obj.x + obj.dt_sim*obj.xd;
            
            if obj.leg_contact == 0
                % rotate leg based on torque
                obj.theta_d = obj.theta_d + u(2)*obj.dt_sim;
                obj.theta = obj.theta + obj.theta_d*obj.dt_sim;
            elseif obj.leg_contact == 1% determine angle to body if foot is fixed to ground
                d = obj.x - obj.x_foot;
                if abs(obj.theta - atan2(d(1), d(2))) > 1e-2
                    balh = 1;
                end
                obj.theta = atan2(d(1), d(2));                
            end
            
            obj.x_foot_all = [obj.x_foot_all; obj.x_foot'];
            obj.x_all = [obj.x_all; obj.x'];
            obj.u_all = [obj.u_all; u'];
            obj.theta_all = [obj.theta_all; obj.theta];
            obj.leg_contact_all = [obj.leg_contact_all; obj.leg_contact];
        end
    end
end

