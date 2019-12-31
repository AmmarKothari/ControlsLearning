#include <string>
#include <iostream>


void void_func(std::string input){
    std::cout << "Void return: " << input << std::endl;
}

std::string string_func(std::string input){
    std::string output = "String return: " + input;
    std::cout << output << std::endl;
    return output;
}

std::string string_func_2(std::string input){
    std::string output = "String return 2: " + input;
    std::cout << output << std::endl;
    return output;
}

std::string string_func_partial(std::string input, std::string suffix){
    std::string output = "String return " + suffix + ": " + input;
    std::cout << output << std::endl;
    return output;
}

// void test_pass_func(std::string string_func(std::string), std::string input){
//     string_func(input);
// }

void test_pass_func(std::function<std::string (std::string)> string_func, std::string input){
    string_func(input);
}

int main(){
    void (*pointer_void_func) (std::string);
    pointer_void_func = &void_func;
    pointer_void_func("Test 1");

    std::string (*pointer_string_func) (std::string);
    pointer_string_func = &string_func;
    std::cout << pointer_string_func("Test 2") << std::endl;

    test_pass_func(string_func, "Test 3");
    test_pass_func(string_func_2, "Test 3");

    auto string_func_no_capture_partial = [](std::string input) {return string_func_partial(input, "no capture partial");};
    test_pass_func(string_func_no_capture_partial, "Test 4");

    std::string partial_str = "capture partial";
    auto string_func_capture_partial = [partial_str](std::string input) {return string_func_partial(input, partial_str);};
    // std::string (*pointer_string_func_capture_partial) (std::string);
    // pointer_string_func_capture_partial = &string_func_capture_partial;
    test_pass_func(string_func_capture_partial, "Test 4");

    return 0;
}