#include <iostream>
#include <fstream>

int main () {
    std::string str;
    std::fstream f;
    f.open("/dev/ttyUSB0");
    while (1)
    {
        f >> str;
        std::cout << str;
    }
}