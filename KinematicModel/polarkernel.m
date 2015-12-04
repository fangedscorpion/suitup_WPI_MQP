clear all; close all; clc;

%% Video Recording Settings
% set true to record video
RECORD_VIDEO = true;
video_filename = 'polarkernel2.avi';
    
%% Useful Functions
% returns: 0 if a<x<b or b<x<a; 1 if x>a and x>b; -1 if x<a and x<b
rngcmp = @(x,a,b) (x>a) + (x>b) - 1;
% adjusts angles a to be within pi radians of the reference angle ref
adjust_angles = @(a,ref) a - rngcmp(a-ref,-pi,pi)*2*pi;

%% Angular Motor Positions
% motor locations must be between -pi and pi
motor_loc = [-180,-120,-60,0,60,120]*pi/180;

%% Kernel Functions
% kernel function takes a current angle and a list of motor locations 
% (adjusted to be within pi radians of current angle) and returns a 
% list of "motor speeds"
stdev = 0.5;
scale_normal = 1/normpdf(0,0,stdev);
kernel_normal = @(ml,ca) scale_normal*normpdf(ml,ca,stdev);

width = 60 * pi/180; % 60 degrees sets max of 2 motors running at a time
scale_window = 1;
kernel_window = @(ml,ca) scale_window*(abs(ml-ca) < width);

% choose which kernel to use
kernel = @(ml,ca) kernel_normal(ml,ca) .* kernel_window(ml,ca);

%% Error Behavior
% "saturation point": error at which a motor could be at its maximum value
max_err = 15 * pi/180;

% current error
err = max_err;

% set true if at least one motor should be at max output at all 
% times if err >= max_err.
ALWAYS_MAX_OUTPUT = false;

%% Plot/Animation
N = 360;
radius = 1;
theta_span = linspace(-pi,pi,N+1);
f = figure();

if RECORD_VIDEO, F(N) = struct('cdata',[],'colormap',[]); end

% while ishandle(f)
    for i = 1:N
        % break if the figure is closed
        if ~ishandle(f), break, end
        
        curr_angle = theta_span(i);
        
        % shift motor locations to be within pi of current angle
        motor_loc_adj = adjust_angles(motor_loc,curr_angle);
        theta_span_adj = adjust_angles(theta_span,curr_angle);
        
        % calculate motor signals (0 <= motor_signals <= 1)
        if ALWAYS_MAX_OUTPUT, m = max(kernel(motor_loc_adj,curr_angle)); else m=1; end
        motor_signals = kernel(motor_loc_adj,curr_angle)/m * min(err,max_err)/max_err;
        
        % plot
        polar(theta_span,1 + radius*ones(size(theta_span)))
        hold on;
        polar(motor_loc,radius + motor_signals,'o')
        polar(theta_span,radius + kernel(theta_span_adj,curr_angle)/m)
        polar([0 curr_angle],[0 radius])
        polar(theta_span,radius*ones(size(theta_span)))
        hold off;
        
        % record
        if RECORD_VIDEO, F(i) = getframe(gcf); end
        
        % delay to refresh plot
        pause(0.01)
    end
% end

close all;

if RECORD_VIDEO
    vw = VideoWriter(video_filename);
    vw.open();
    vw.writeVideo(F);
    vw.close();
end

