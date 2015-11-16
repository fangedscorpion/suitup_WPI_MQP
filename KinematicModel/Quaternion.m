classdef Quaternion
    % Quaternion: [w,x,y,z]
    
    properties
        quat = [1 0 0 0].'; % identity
    end
    
    methods
        function q = Quaternion(q_arr)
            % constructor from 4-element array
            if size(q_arr,1) < size(q_arr,2)
                q_arr = q_arr.';
            end
            if any(size(q_arr) ~= [4 1])
                error('            q_arr must be a 4-element vector');
            end
            q.quat = q_arr;
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
    end
end

