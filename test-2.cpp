/* This code intentionally does not follow the LLVM code style */
#include <iostream>
#include <string>

using namespace std;

class Example {  // Class name not in CamelCase
private:
    string data;

public:
    Example(string input) : data(input) {}
    
    void printData() { // Method name not in LowerCamelCase
        cout << "Data: " << data << endl;
    }
};
