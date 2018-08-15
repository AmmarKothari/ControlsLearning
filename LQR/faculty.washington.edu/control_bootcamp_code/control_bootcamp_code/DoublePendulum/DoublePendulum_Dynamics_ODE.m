function dy = DoublePendulum_Dynamics_ODE(Q1,Q2,b1,b2,g,l1,l2,m1,m2,y)
    
    th1 = y(1);
    th1_d = y(2);
    th2 = y(3);
    th2_d = y(4);
    out1 = DoublePendulum_Dynamics(Q1,Q2,b1,b2,g,l1,l2,m1,m2,th1,th2,th1_d,th2_d);

    dy = zeros(4,1);
    dy(1) = y(2);
    dy(2) = out1(1);
    dy(3) = y(4);
    dy(4) = out1(2);
end