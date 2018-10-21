classdef body
    properties
        links
        base_link % which link to start from -- lets ignore even length snakes for now
        base_pose
        num_links
        zero_link
    end
    
    methods
        function obj = body(links, zero_link)
            obj.links = links;
            obj.num_links = length(links);
            obj.zero_link = zero_link;
            obj.base_pose = zeros(1,3);
        end
        
        function poses = get_poses(obj)
            poses = zeros(obj.num_links, 3);
            for i = 1:obj.num_links
                poses(i,:) = obj.links(i).pose;
            end
        end
        
        function obj = calc_poses(obj, alphas)
            if nargin < 2
                alphas = [obj.links.alpha];
            else
                alphas = [alphas(1:obj.zero_link-1), 0, alphas(obj.zero_link:end)];
                obj = obj.set_joints(alphas);
            end
            %start at '0' link
            forward_range = obj.zero_link+1:1:obj.num_links;
            backward_range = obj.zero_link-1:-1:1;
            obj.links(obj.zero_link) = obj.links(obj.zero_link).linkPosForward(alphas(obj.zero_link));
            for i = forward_range
                obj.links(i) = obj.links(i).setProximal(obj.links(i-1).distal);
                obj.links(i) = obj.links(i).linkPosForward(alphas(i));
            end
            for i = backward_range
                obj.links(i) = obj.links(i).setDistal(obj.links(i+1).proximal);
                obj.links(i) = obj.links(i).linkPosBackward(alphas(i));
            end
            
        end
        
        function obj = set_joints(obj, alpha_)
            for i = 1:obj.num_links
                obj.links(i).alpha = alpha_(i);
            end
        end
    end
    
    
    
    
end
    
    
    