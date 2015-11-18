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
        
        function pts = getPoints(kt,parent_quat,parent_offset)
            % return an array of pure quaternion positions in the tree
            
            % set default argument for parent_quat to identity unit quaternion
            % set default argument for parent_offset to zero pure quaternion
            if nargin == 1
                parent_quat = Quaternion([1,0,0,0]);
                parent_offset = Quaternion([0,0,0,0]);
            end
            
            trans_norot = inv(parent_quat)*kt.t*parent_quat + parent_offset;
            trans_rot = inv(kt.r)*kt.rt*kt.r;
            trans_total = trans_rot + trans_norot;
            
            pts = [trans_total; trans_norot];
            
            % generate the array of points from each subtree
            for i = 1:numel(kt.sub)
                pts = [pts; kt.sub(i).getPoints(kt.r,trans_total)];
            end
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