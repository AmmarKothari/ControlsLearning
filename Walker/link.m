classdef link
    properties
        h
        a
        h_poi
        zero_pose
        pose
        % positions in space
        proximal
        distal
        c % move this to drawing class
        alpha
        alpha_dot
        % input desired velocity and output acceleration
        alpha_dot_desired
        kp
        ki
        alpha_dot_dot
        e_total
    end
    
    methods
        function obj = link(a, h, h_poi)
            addpath('/Users/Ammar/Documents/ROB541_GeometricMecahnics/Random/GeoOps2D')
            obj.h = group(h);
            obj.h_poi = group(h_poi);
            obj.a = a;
            obj.pose = group([0,0,0]);
            obj.zero_pose = group([0,0,0]);
            obj.alpha = 0;
            obj.alpha_dot_desired = 0;
            obj.kp = 10;
            obj.ki = 1;
            obj.alpha_dot = 0;
            obj.alpha_dot_dot = 0;
            obj.e_total = 0;
        end
        function obj = setZero(obj, zero_pose)
            obj.zero_pose = zero_pose;
        end
        function obj = setProximal(obj, zero_pose)
            obj.zero_pose = zero_pose;
        end
        function obj = setDistal(obj, zero_pose)
            obj.zero_pose = zero_pose;
        end
        function obj = setAlphaDotDesired(obj, alpha_dot_desired)
            obj.alpha_dot_desired = alpha_dot_desired;
        end
        function obj = calcAlphaDD(obj)
            % calculates alpha_dd for a given error in alpha_d
            e = obj.alpha_dot_desired - obj.alpha_dot;
            obj.e_total = obj.e_total + e;
            obj.alpha_dot_dot = obj.kp * e;
            if sign(e) == sign(obj.e_total)
                obj.alpha_dot_dot = obj.alpha_dot_dot + obj.ki * obj.e_total;
%             else
%                 obj.alpha_dot_dot = obj.alpha_dot_dot + obj.ki * sign(e)*abs(obj.e_total);
            end
        end
        function obj = clearError(obj)
            obj.e_total = 0;
        end
        function obj = step(obj, dt)
            % runs system forward one time step
            obj.alpha_dot = obj.alpha_dot + dt*obj.alpha_dot_dot;
            obj.alpha_ = obj.alpha_ + dt*obj.alpha_dot;
        end
        function obj = linkPosForward(obj, alpha_)
            obj.alpha = alpha_;
            % figure out how the base pose changes
            obj.proximal = poseFromMatrix(RightAction(obj.zero_pose, obj.a * obj.alpha));
            obj.pose = poseFromMatrix(RightAction(obj.proximal, obj.h));
            obj.distal = poseFromMatrix(RightAction(obj.pose, obj.h));
        end
        function obj = linkPosBackward(obj, alpha_)
            obj.alpha = alpha_;
            % figure out how the base pose changes
            obj.distal = poseFromMatrix(RightAction(obj.zero_pose, obj.a * obj.alpha));
            obj.pose = poseFromMatrix(RightAction(obj.distal, inverseGroup(obj.h)));
            obj.proximal = poseFromMatrix(RightAction(obj.pose, inverseGroup(obj.h)));
        end
    end
end