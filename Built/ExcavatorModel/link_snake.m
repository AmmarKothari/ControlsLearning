classdef link_snake
    properties
        h
        a
        index
        h_poi
        zero_pose
        alpha
        alpha_dot
        pose
        proximal
        distal
        c
        % input desired velocity and output acceleration
        alpha_dot_desired
        kp
        ki
        alpha_dot_dot
        e_total
    end
    
    methods
        function obj = link_snake(a, h, c, h_poi)
            addpath(fullfile('..','/GeoOps2D'));
            obj.h = GeoOps2D.group(h);
            obj.h_poi = GeoOps2D.group(h_poi);
            obj.a = a;
            obj.pose = GeoOps2D.group([0,0,0]);
            obj.zero_pose = GeoOps2D.group([0,0,0]);
            obj.alpha = 0;
            obj.c = c;
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
            obj.proximal = GeoOps2D.group(GeoOps2D.RightAction(obj.zero_pose, obj.a * obj.alpha));
            obj.pose = GeoOps2D.poseFromMatrix(GeoOps2D.RightAction(obj.proximal, obj.h));
            obj.distal = poseFromMatrix(RightAction(obj.pose, obj.h));
        end
        function obj = linkPosBackward(obj, alpha_)
            obj.alpha = alpha_;
            % figure out how the base pose changes
            obj.distal = poseFromMatrix(RightAction(obj.zero_pose, obj.a * obj.alpha));
            obj.pose = poseFromMatrix(RightAction(obj.distal, inverseGroup(obj.h)));
            obj.proximal = poseFromMatrix(RightAction(obj.pose, inverseGroup(obj.h)));
        end
        
        function obj = drawLink(obj, ax)
            X = [obj.proximal(1), obj.distal(1)];
            Y = [obj.proximal(2), obj.distal(2)];
            hold on
            % plots line
            plot(ax, X, Y, obj.c, 'LineWidth', 8);
            % plots center
            plot(ax, obj.pose(1), obj.pose(2), '*k');
            % plot proximal
            plot(ax, obj.proximal(1), obj.proximal(2), '^k');
            % plots distal
            plot(ax, obj.distal(1), obj.distal(2), 'sk');
            hold off
        end
        function obj = drawDistalPose(obj, ax)
            plotPose(ax, obj.distal, 1);
        end
        
        function obj = drawPose(obj, ax, h)
            % plots a cosys at some point on link
            plotPose(ax, poseFromMatrix(rightAction(obj.pose, h)), 1);
        end
        
        function obj = drawArrow(obj, ax, arrow_params)
            poi = poseFromMatrix(rightAction(obj.pose, obj.h_poi));
            hold on;
            quiver3(ax, poi(1), poi(2), poi(3), arrow_params(1), arrow_params(2), arrow_params(3), 'linewidth',5);
            hold off;
        end
    end
    
    
    
end