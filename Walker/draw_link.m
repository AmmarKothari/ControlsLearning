classdef draw_link
    properties
        c
    end
    
    methods
        function obj = draw_link(c)
            obj.c = c;
        end
        
        function drawLink(obj, link, ax)
            proximal = link.proximal;
            distal = link.distal;
            pose = link.pose;
            X = [proximal(1), distal(1)];
            Y = [proximal(2), distal(2)];
            hold on
            % plots line
            plot(ax, X, Y, obj.c, 'LineWidth', 8);
            % plots center
            plot(ax, pose(1), pose(2), '*k');
            % plot proximal
            plot(ax, proximal(1), proximal(2), '^k');
            % plots distal
            plot(ax, distal(1), distal(2), 'sk');
            hold off
        end
        function drawDistalPose(~, link, ax)
            distal = link.distal;
            plotPose(ax, distal, 1);
        end
        
        function obj = drawPose(obj, ax, h)
            % plots a cosys at some point on link
            plotPose(ax, poseFromMatrix(rightAction(pose, h)), 1);
        end
        
        function obj = drawArrow(obj, ax, arrow_params)
            poi = poseFromMatrix(rightAction(pose, obj.h_poi));
            hold on;
            quiver3(ax, poi(1), poi(2), poi(3), arrow_params(1), arrow_params(2), arrow_params(3), 'linewidth',5);
            hold off;
        end
    end
    
end