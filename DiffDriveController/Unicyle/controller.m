function u = controller(stated_in, des)
    Kw = [0.1, 0, 0];
    Kv = [0.1, 0, 0];
    u = des;
    u(1) = Kv(1) * (des(1) - stated_in(1));
    u(2) = Kw(1) * (des(2) - stated_in(2));
end