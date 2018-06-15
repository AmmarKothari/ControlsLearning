classdef ReibertDraw
    %UNTITLED5 Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        ax; animation;
        scrsz = get(0,'ScreenSize');
        h_axes, body, leg, ground, goal
        min_x = -1.0; % display size in robot units
        max_x = 1.0;
        Hdes; % desired height
        

        color0 = [0 0 0];
        color1 = [1 0 0];
        color2 = [0 0 1];
        color3 = [0 1 0];
        color4 = [1 0 1];
        color5 = [1 1 0];
        
    end
    
    methods
        function obj = ReibertDraw(animation)
            % give it a figure to draw in
            obj.animation = animation;
            obj.ax = gca(obj.animation);
%             set(obj.animation,'name','My Robot','Position',[20 100 500 500]);
            set(obj.animation,'name','My Robot');
            obj.h_axes = axes('Parent',obj.animation,'Units','Pixels','Position',[0 0 500 500],'Ylim',[0 (obj.max_x - obj.min_x)],'Xlim',[obj.min_x obj.max_x]);
            obj.body = line('Parent',obj.h_axes,'Color',obj.color0,'Visible','off','LineWidth',10);
            obj.leg = line('Parent',obj.h_axes,'Color',obj.color1,'Visible','off','LineWidth',10);
            obj.ground = line('Parent',obj.h_axes,'Color',obj.color2,'Visible','on','LineWidth',10);
            obj.goal = line('Parent',obj.h_axes,'Color',obj.color3,'Visible','on','LineWidth',10);
            set(obj.ground, 'Parent', obj.h_axes, 'YData', [0 0],'XData',[obj.min_x obj.max_x])
            set(obj.h_axes,'visible','on');
        end
        
        function draw(obj,Reibert, Hdes)
            body_angle = Reibert.theta;
            x = Reibert.x(1);
            z = Reibert.x(2);
            foot_x = Reibert.x_foot(1);
            foot_z = Reibert.x_foot(2);
            
            body_length = 0.2;
            bdx = 0.5*body_length*cos( body_angle );
            bdz = 0.5*body_length*sin( body_angle );

            set(obj.body,'Parent',obj.h_axes,'Xdata',[x - bdx x + bdx], ...
            'Ydata',[z - bdz z + bdz],'visible','on');
            set(obj.leg,'Parent',obj.h_axes,'Xdata',[x foot_x], ...
            'Ydata',[z foot_z],'visible','on');
            set(obj.goal, 'Parent', obj.h_axes, 'YData', [Hdes, Hdes],'XData',[obj.min_x obj.max_x]);

            drawnow
            
            
        end
    end
    methods (Static)
    end
end

