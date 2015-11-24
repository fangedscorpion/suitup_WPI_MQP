%% Known information
q = Quaternion([1 0 0 0]);
q_axis = Quaternion([0,1,0,0]);
q_goal = Quaternion([0.8924,0.2391,0.3696,0.0990]);

%% Derived information
q_err = q_goal*inv(q);
[swing,twist] = swingTwist(q_err,q_axis);
% the important quantities that come out of all of this
swing_axis = swing.v/norm(swing.v)
swing_angle = acos(swing.w)*360/pi
twist_axis = twist.v/norm(twist.v)
twist_angle = acos(twist.w)*360/pi

%% Rotate coordinate system axes and plot
% Coordinate system axes
cs_x = Quaternion([0 1 0 0]);
cs_y = Quaternion([0 0 1 0]);
cs_z = Quaternion([0 0 0 1]);

% Rotate coord system by s
cs_x_s = swing.rotPure(cs_x);
cs_y_s = swing.rotPure(cs_y);
cs_z_s = swing.rotPure(cs_z);

% Rotate coord system by s then t (q_err)
cs_x_q = q_err.rotPure(cs_x);
cs_y_q = q_err.rotPure(cs_y);
cs_z_q = q_err.rotPure(cs_z);

% Populate vector heads array
uvw = [];
uvw = [uvw, cs_x.v cs_y.v cs_z.v]; 
uvw = [uvw, cs_x_s.v cs_y_s.v cs_z_s.v]; 
uvw = [uvw, 1.2*swing.v/norm(swing.v) nan(3,2)];
uvw = [uvw, cs_x_q.v cs_y_q.v cs_z_q.v];
t_in_s = swing.rotPure(Quaternion([0;twist.v]));
uvw = [uvw, 1.2*t_in_s.v/norm(t_in_s.v) nan(3,2)];

% Vector tails at origin
xyz = zeros(size(uvw));

% Plot coordinate systems and vectors
figure;
hold on;
num_quiv = ceil(size(uvw,2)/3);
for i = 1:num_quiv
    ind_start = (i-1)*3 + 1;
    if i < num_quiv
        ind_end = ind_start + 2;
        u = uvw(1,ind_start:ind_end);
        v = uvw(2,ind_start:ind_end);
        w = uvw(3,ind_start:ind_end);
        x = xyz(1,ind_start:ind_end);
        y = xyz(2,ind_start:ind_end);
        z = xyz(3,ind_start:ind_end);
    else
        u = uvw(1,ind_start:end);
        v = uvw(2,ind_start:end);
        w = uvw(3,ind_start:end);
        x = xyz(1,ind_start:end);
        y = xyz(2,ind_start:end);
        z = xyz(3,ind_start:end);
    end
    quiver3(x,y,z,u,v,w);
end
xlabel('x')
ylabel('y')
zlabel('z')
axis equal