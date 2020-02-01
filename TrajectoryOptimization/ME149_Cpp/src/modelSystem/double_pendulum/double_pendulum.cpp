#include "double_pendulum.h"

DoublePendulum::DoublePendulum(Link l1_in, Link l2_in, float g_in){
    l1 = l1_in;
    l2 = l2_in;
    g = g_in;
}

DoublePendulum::DoublePendulum(){
    Link l1_in, l2_in;
    DoublePendulum(l1_in, l2_in, -9.8);
}

state_type DoublePendulum::forward_dynamics(state_type &state, u_type &u_input, float t) {
    // This function is copied from https://github.com/MatthewPeterKelly/ME149_Spring2018/blob/master/codeLibrary/modelSystems/doublePendulum/autoGen_doublePendulumDynamics.m
    float q1 = state[0];
    float q2 = state[1];
    float dq1 = state[2];
    float dq2 = state[3];

    float u1 = u_input[0];
    float u2 = u_input[1];

    float t2 = q1*2.0;
    float t7 = q2*2.0;
    float t3 = t2-t7;
    float t4 = q1-q2;
    float t5 = sin(q1);
    float t6 = l1.mass*2.0;
    float t8 = cos(t3);
    float t9 = l2.mass+t6-l2.mass*t8;
    float t10 = 1.0/t9;
    float t11 = cos(t4);
    float t12 = pow(l1.length, 2);
    float t13 = pow(dq1, 2);
    float t14 = sin(t4);
    float t15 = pow(l2.mass, 2);
    float t16 = pow(l2.length, 2);
    float t17 = pow(dq2, 2);
    float t18 = sin(t3);
    float ddq1 = (1.0/pow(l1.length, 2)*t10*(l2.length*u1*2.0-l2.length*u2*2.0-l1.length*t11*u2*2.0+l1.length*l2.length*g*l2.mass*sin(q1-q2*2.0)+l1.length*l2.length*g*l1.mass*t5*2.0+l1.length*l2.length*g*l2.mass*t5-l2.length*l2.mass*t12*t13*t18-l1.length*l2.mass*t14*t16*t17*2.0))/l2.length;
    float t19 = sin(q2);
    float t20 = -q2+t2;
    float t21 = sin(t20);
    float ddq2 = (1.0/pow(l2.length, 2)*t10*(l1.length*l1.mass*u2*2.0+l1.length*l2.mass*u2*2.0-l2.length*l2.mass*t11*u1*2.0+l2.length*l2.mass*t11*u2*2.0+l1.length*l2.length*g*t15*t19-l1.length*l2.length*g*t15*t21+l2.length*t12*t13*t14*t15*2.0+l1.length*t15*t16*t17*t18+l1.length*l2.length*g*l1.mass*l2.mass*t19-l1.length*l2.length*g*l1.mass*l2.mass*t21+l2.length*l1.mass*l2.mass*t12*t13*t14*2.0))/(l1.length*l2.mass);
    state_type state_d = {dq1, dq2, ddq1, ddq2};
    return state_d;
}

state_vector_type DoublePendulum::assemble_full_state(state_vector_type &) {
    
}
