classdef Quaternion
    % Quaternion: [w,x,y,z]
    
    properties
        quat = [0,0,0,0].'; 
    end
    
    methods
        function q = Quaternion(arg1,arg2)
            switch nargin
                case 1
                    % constructor from 4-element array. No restriction on
                    % classification of output quaternion (e.g. unit, pure)
                    if size(arg1,1) < size(arg1,2)
                        arg1 = arg1.';
                    end
                    if any(size(arg1) ~= [4 1])
                        error('            q_arr must be a 4-element vector');
                    end
                    q.quat = arg1;
                case 2 
                    % constructor from axis,angle. always returns unit
                    % quaternion
                    q.quat(1) = cos(arg2/2);
                    q.quat(2:4) = arg1.*sin(arg2/2)/norm(arg1);
            end
        end
        function q_out = unit(q)
            % unit quaternion
            q_out = Quaternion(q.quat/norm(q));
        end
        function m = norm(q)
            % vector norm of quaternion
            m = norm(q.quat);
        end
        function q_out = inv(q)
            % quaternion inversion
            q_out = Quaternion(q.quat .* [1 -1 -1 -1].');
        end
        function t = mtimes(q,r)
            % quaternion multiplication
            t_vec = [0 0 0 0].';
            r_vec = r.quat;
            q_vec = q.quat;
            t_vec(1) = r_vec(1)*q_vec(1) - r_vec(2)*q_vec(2) - r_vec(3)*q_vec(3) - r_vec(4)*q_vec(4);
            t_vec(2) = r_vec(1)*q_vec(2) + r_vec(2)*q_vec(1) - r_vec(3)*q_vec(4) + r_vec(4)*q_vec(3);
            t_vec(3) = r_vec(1)*q_vec(3) + r_vec(2)*q_vec(4) + r_vec(3)*q_vec(1) - r_vec(4)*q_vec(2);
            t_vec(4) = r_vec(1)*q_vec(4) - r_vec(2)*q_vec(3) + r_vec(3)*q_vec(2) + r_vec(4)*q_vec(1);
            t = Quaternion(t_vec);
        end
        function t = mrdivide(q,r)
            % right quaternion division
            t_vec = [0 0 0 0].';
            r_vec = r.quat;
            q_vec = q.quat;
            t_vec(1) = r_vec(1)*q_vec(1) + r_vec(2)*q_vec(2) + r_vec(3)*q_vec(3) + r_vec(4)*q_vec(4);
            t_vec(2) = r_vec(1)*q_vec(2) - r_vec(2)*q_vec(1) - r_vec(3)*q_vec(4) + r_vec(4)*q_vec(3);
            t_vec(3) = r_vec(1)*q_vec(3) + r_vec(2)*q_vec(4) - r_vec(3)*q_vec(1) - r_vec(4)*q_vec(2);
            t_vec(4) = r_vec(1)*q_vec(4) - r_vec(2)*q_vec(3) + r_vec(3)*q_vec(2) - r_vec(4)*q_vec(1);
            t = Quaternion(t_vec/norm(r_vec)^2);
        end
        function t = mldivide(q,r)
            % left quaternion division
            t = mrdivide(r,q);
        end
        function out = ne(q,r)
            % not equal
            out = any(q.quat ~= r.quat);
        end
        function out = eq(q,r)
            % equal
            out = all(q.quat == r.quat);
        end
        function t = plus(q,r)
            % add
            t = Quaternion(q.quat + r.quat);
        end
        function t = minus(q,r)
            % subtract
            t = Quaternion(q.quat - r.quat);
        end
        function v = v(q,ind)
            % returns the xyz vector components of a quaternion
            switch nargin
                case 1
                    v = q.quat(2:4);
                case 2
                    v = q.quat(1+ind);
            end
        end
        function w = w(q)
            % returns the w component of a quaternion
            w = q.quat(1);
        end
        function [swing,twist] = swingTwist(q,axis)
            % returns swing and twist components of this quaternion about
            % axis pure quaternion axis
            % q = twist * swing
            axis = axis.v/norm(axis.v);
            p = dot(q.v, axis)*axis;
            twist = Quaternion([q.w; p]).unit();
            swing = inv(twist) * q;
        end
        function pq_rot = rotPure(q,pq)
            % rotates pure quaternion pq by unit rotation quaternion q
            pq_rot = inv(q)*pq*q;
        end
    end
end

