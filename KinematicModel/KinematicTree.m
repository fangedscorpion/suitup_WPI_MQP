classdef KinematicTree < handle
    % Tree of kinematic relationships
    % r: rotation (Quaternion object: unit quaternion)
    % rt: rotated translation (Quaternion object: pure quaternion)
    % t: unrotated translation (Quaternion object: pure quaternion)
    % sub: vector of subtrees
    
    properties 
        r; % orientation in *world frame*
        rt; % rotated vector from parent to this
        t; % unrotated vector from parent to this
        sub; % subtrees
    end
    
    methods
        function kt = KinematicTree(rot,rot_trans,trans,subtrees)
            % constructor
            kt.r = rot;
            kt.rt = rot_trans;
            kt.t = trans;
            kt.sub = subtrees; 
        end
        function pts = getPoints(kt)
            % return an array of pure quaternion positions in the tree
            subpts = [];
            % generate the array of points from each subtree
            for i = 1:numel(kt.sub)
                subpts = [subpts; kt.sub(i).getPoints()];
            end
            % add this tree's rotated vector and unrotated vector components to
            % each subpoint
            for i=1:numel(subpts)
                subpts(i) = subpts(i) + inv(kt.r)*kt.rt*kt.r + kt.t;
            end
            % append this tree's point to the array of points
            pts = [kt.r\kt.rt*kt.r + kt.t;subpts];
            
            % uncomment the following lines to append additional points at the end of
            % each unrotated translation
%             for i = 1:numel(kt.sub)
%                 pts = [pts;kt.sub(i).t + kt.r\kt.rt*kt.r + kt.t];
%             end
        end
        function rotateAll(kt,quat)
            % rotate all of the subtree orientations by the given unit quaternion
            kt.r = kt.r*quat;
            for i = 1:numel(kt.sub)
                kt.sub(i).rotateAll(quat);
            end
        end
    end
end