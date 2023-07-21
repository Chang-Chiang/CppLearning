f(vec.begin(), vec.end(), bind2nd(less<int>(), 48));
    // // lamda 表达式
    // auto it2 = find_if(ve