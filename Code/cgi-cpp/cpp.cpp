#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream file("jajo.jpg", std::ios::binary);
    if (!file) {
        std::cout << "Content-Type: text/plain\r\n\r\n";
        std::cout << "Error: Cannot open image file";
        return 1;
    }

    // file.seekg(0, std::ios::end);
    // size_t size = file.tellg();
    // file.seekg(0, std::ios::beg);

    // std::cout << "Content-Type: image/jpeg\r\n";
    // std::cout << "Content-Length: " << size << "\r\n\r\n";

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        std::cout.write(buffer, file.gcount());
    }
    std::cout.write(buffer, file.gcount());

    return 0;
}