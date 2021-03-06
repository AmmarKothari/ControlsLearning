function A = A_linearized(Q1,Q2,b1,b2,g,l1,l2,m1,m2,th1,th2,th1_d,th2_d)
%A_LINEARIZED
%    A = A_LINEARIZED(Q1,Q2,B1,B2,G,L1,L2,M1,M2,TH1,TH2,TH1_D,TH2_D)

%    This function was generated by the Symbolic Math Toolbox version 7.2.
%    15-Aug-2018 11:28:57

t2 = l1.^2;
t3 = cos(th2);
t4 = cos(th1);
t5 = l2.*m1.*t2.*4.0;
t6 = l2.*m2.*t2.*2.0;
t7 = t3.^2;
t12 = l2.*m2.*t2.*t7;
t8 = t5+t6-t12;
t9 = 1.0./t8;
t10 = l2.^2;
t11 = sin(th2);
t13 = th1_d.^2;
t14 = th1+th2;
t15 = cos(t14);
t16 = th2_d.^2;
t17 = sin(th1);
t18 = sin(t14);
t19 = l1.*m2.*t10.*t11.*th1_d.*4.0;
t20 = l1.*m2.*t10.*t11.*th2_d.*4.0;
t21 = m2.^2;
t22 = t2.*t10.*t21.*2.0;
t23 = m1.*m2.*t2.*t10.*4.0;
t26 = t2.*t7.*t10.*t21;
t24 = t22+t23-t26;
t25 = 1.0./t24;
t27 = t11.^2;
t28 = l1.*l2.*t10.*t11.*t21.*th1_d.*4.0;
t29 = l1.*l2.*t10.*t11.*t21.*th2_d.*4.0;
t30 = t2.*t3.*t10.*t11.*t21.*th2_d.*2.0;
A = reshape([0.0,-t9.*(g.*l1.*l2.*m1.*t4.*2.0+g.*l1.*l2.*m2.*t4.*2.0-g.*l1.*l2.*m2.*t3.*t15),0.0,t25.*(g.*l1.*t4.*t10.*t21.*2.0-g.*l2.*t2.*t15.*t21+g.*l2.*t2.*t3.*t4.*t21-g.*l1.*t3.*t10.*t15.*t21+g.*l1.*m1.*m2.*t4.*t10.*2.0-g.*l2.*m1.*m2.*t2.*t15.*2.0+g.*l2.*m1.*m2.*t2.*t3.*t4),1.0,t9.*(t19+t20-b1.*l2.*2.0+l2.*m2.*t2.*t3.*t11.*th1_d.*2.0),0.0,-t25.*(t28+t29+t30-b1.*m2.*t10.*2.0-b1.*l1.*l2.*m2.*t3+l1.*l2.*t2.*t11.*t21.*th1_d.*2.0+t2.*t3.*t10.*t11.*t21.*th1_d.*4.0+l1.*l2.*m1.*m2.*t2.*t11.*th1_d.*4.0),0.0,t9.*(Q2.*l1.*t11-b2.*l1.*t11.*th2_d+l2.*m2.*t2.*t7.*t13+l1.*m2.*t3.*t10.*t13.*2.0+l1.*m2.*t3.*t10.*t16.*2.0-l2.*m2.*t2.*t13.*t27+l1.*m2.*t3.*t10.*th1_d.*th2_d.*4.0+g.*l1.*l2.*m2.*t3.*t15-g.*l1.*l2.*m2.*t11.*t18)-l2.*m2.*t2.*t3.*1.0./t8.^2.*t11.*(Q1.*l2.*2.0-Q2.*l2.*2.0-Q2.*l1.*t3-b1.*l2.*th1_d.*2.0+b2.*l2.*th2_d.*2.0+b2.*l1.*t3.*th2_d-g.*l1.*l2.*m1.*t17.*2.0-g.*l1.*l2.*m2.*t17.*2.0+l1.*m2.*t10.*t11.*t13.*2.0+l1.*m2.*t10.*t11.*t16.*2.0+l2.*m2.*t2.*t3.*t11.*t13+l1.*m2.*t10.*t11.*th1_d.*th2_d.*4.0+g.*l1.*l2.*m2.*t3.*t18).*2.0,0.0,-t25.*(-Q1.*l1.*l2.*m2.*t11+Q2.*l1.*l2.*m2.*t11.*2.0+g.*l2.*t2.*t15.*t21+t2.*t7.*t10.*t13.*t21.*2.0+t2.*t7.*t10.*t16.*t21-t2.*t10.*t13.*t21.*t27.*2.0-t2.*t10.*t16.*t21.*t27+g.*l1.*t3.*t10.*t15.*t21+g.*l2.*t2.*t11.*t17.*t21-g.*l1.*t10.*t11.*t18.*t21+l1.*l2.*t2.*t3.*t13.*t21+l1.*l2.*t3.*t10.*t13.*t21.*2.0+l1.*l2.*t3.*t10.*t16.*t21.*2.0+t2.*t7.*t10.*t21.*th1_d.*th2_d.*2.0-t2.*t10.*t21.*t27.*th1_d.*th2_d.*2.0+b1.*l1.*l2.*m2.*t11.*th1_d-b2.*l1.*l2.*m2.*t11.*th2_d.*2.0+g.*l2.*m1.*m2.*t2.*t15.*2.0+g.*l2.*m1.*m2.*t2.*t11.*t17+l1.*l2.*m1.*m2.*t2.*t3.*t13.*2.0+l1.*l2.*t3.*t10.*t21.*th1_d.*th2_d.*4.0)+t2.*t3.*t10.*t11.*t21.*1.0./t24.^2.*(Q2.*m1.*t2.*-2.0-Q2.*m2.*t2+Q1.*m2.*t10.*2.0-Q2.*m2.*t10.*2.0+b2.*m1.*t2.*th2_d.*2.0+b2.*m2.*t2.*th2_d-b1.*m2.*t10.*th1_d.*2.0+b2.*m2.*t10.*th2_d.*2.0+Q1.*l1.*l2.*m2.*t3-Q2.*l1.*l2.*m2.*t3.*2.0+g.*l2.*t2.*t18.*t21-g.*l1.*t10.*t17.*t21.*2.0-g.*l2.*t2.*t3.*t17.*t21+g.*l1.*t3.*t10.*t18.*t21+l1.*l2.*t2.*t11.*t13.*t21+l1.*l2.*t10.*t11.*t13.*t21.*2.0+l1.*l2.*t10.*t11.*t16.*t21.*2.0+t2.*t3.*t10.*t11.*t13.*t21.*2.0+t2.*t3.*t10.*t11.*t16.*t21-b1.*l1.*l2.*m2.*t3.*th1_d+b2.*l1.*l2.*m2.*t3.*th2_d.*2.0+g.*l2.*m1.*m2.*t2.*t18.*2.0-g.*l1.*m1.*m2.*t10.*t17.*2.0-g.*l2.*m1.*m2.*t2.*t3.*t17+l1.*l2.*m1.*m2.*t2.*t11.*t13.*2.0+l1.*l2.*t10.*t11.*t21.*th1_d.*th2_d.*4.0+t2.*t3.*t10.*t11.*t21.*th1_d.*th2_d.*2.0).*2.0,0.0,t9.*(t19+t20+b2.*l2.*2.0+b2.*l1.*t3),1.0,-t25.*(t28+t29+t30+b2.*m1.*t2.*2.0+b2.*m2.*t2+b2.*m2.*t10.*2.0+b2.*l1.*l2.*m2.*t3.*2.0+t2.*t3.*t10.*t11.*t21.*th1_d.*2.0)],[4,4]);
