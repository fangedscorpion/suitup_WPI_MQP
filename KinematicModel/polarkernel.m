clear; close all; clc;

% motor locations must be between -pi and pi
motor_locations = [-180,-120,-60,0,60,120]*pi/180;

% kernel function takes a current angle and a list of motor locations 
% (adjusted to be within pi radians of current angle) and returns a 
% list of "motor speeds"
scale_normal = 0.5;
stdev = 0.6;
kernel_normal = @(ml,ca) scale_normal*normpdf(ml,ca,stdev);

scale_window = 0.5;
width = 0.6;
kernel_window = @(ml,ca) scale_window*(abs(ml-ca) < width);

% which kernel to use
kernel = kernel_normal;

radius = 1;
theta = linspace(-pi,pi,50);

f = figure();

while ishandle(f)
    for current_angle = theta
        % break if the figure is closed
        if ~ishandle(f), break, end
        
        % shift the motor locations to be within pi radians of the current
        % angle
        motor_locations_adj = motor_locations;
        motor_locations_adj(motor_locations - current_angle > pi) = motor_locations(motor_locations - current_angle > pi) - 2*pi;
        motor_locations_adj(motor_locations - current_angle < -pi) = motor_locations(motor_locations - current_angle < -pi) + 2*pi;
        
        % plot
        polar(motor_locations,radius + kernel(motor_locations_adj,current_angle),'o')
        hold on;
        polar([0 current_angle],[0 radius])
        polar(theta,radius*ones(size(theta)))
        hold off;
        
        pause(0.1)
    end
end