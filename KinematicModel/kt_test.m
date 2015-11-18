ktree = ... 
    KinematicTree( ... % Chest
        Quaternion([1,0,0,0]), ... [.966,0,-0.2588,0]
        Quaternion([0,0,0,0]), ...
        Quaternion([0,0,0,0]), ...
        [ ...
            KinematicTree( ... % Left shoulder
                Quaternion([1,0,0,0]), ...
                Quaternion([0,-2,0,0]), ...
                Quaternion([0,0,0,1]), ...
                [ ...
                    KinematicTree( ... % Left upper arm
                        Quaternion([1/sqrt(2),1/sqrt(2),0,0]), ...
                        Quaternion([0,0,3,0]), ...
                        Quaternion([0,0,0,0]), ...
                        [ ...
                            KinematicTree( ... % Left forearm
                                Quaternion([1,0,0,0]), ...
                                Quaternion([0,0,3,0]), ...
                                Quaternion([0,0,0,0]), ...
                                [ ...
                                ] ...
                            ) ...
                        ] ...
                    ) ...
                ] ...
            ), ...
            KinematicTree(... % Right shoulder
                Quaternion([1,0,0,0]), ... 
                Quaternion([0,2,0,0]), ...
                Quaternion([0,0,0,1]), ...
                [ ...
                    KinematicTree( ... % Right upper arm
                        Quaternion([1/sqrt(2),1/sqrt(2),0,0]), ...
                        Quaternion([0,0,3,0]), ...
                        Quaternion([0,0,0,0]), ...
                        [ ...
                            KinematicTree( ... % Right forearm
                                Quaternion([1,0,0,0]), ...
                                Quaternion([0,0,3,0]), ...
                                Quaternion([0,0,0,0]), ...
                                [ ...
                                ] ...
                            ) ...
                        ] ...
                    ) ...
                ] ...
            ) 
        ] ...
    );

% 1 degree rotation about z
deg1z = Quaternion([0.9999619230641713,0,0,0.008726535498373935]);

degsPerStep = 6;

for i=1:(360/degsPerStep)
    for i = 1:degsPerStep
        % 1 degree rotation about z
        ktree.rotateAll(deg1z);
    end
    
    qs = ktree.getPoints();
    N = numel(qs);
    pts = zeros(3,N);
    for i=1:N
        pts(:,i) = qs(i).getVec();
    end

    scatter3(pts(1,:),pts(2,:),pts(3,:))
    xlabel('x')
    ylabel('y')
    zlabel('z')
    axis([-8,8,-8,8,-8,8])
    pause(0.01)
end