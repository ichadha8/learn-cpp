#include "pulsekv.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    pulsekv::Store store;
    std::string line;

    std::cerr << "pulsekv starter: stdin REPL mode. Implement CLI and TCP server as capstone work.\n";
    while (std::getline(std::cin, line)) {
        auto command = pulsekv::parse_command(line);
        std::cout << pulsekv::execute_command(store, command);
        if (command.type == pulsekv::CommandType::Quit) {
            break;
        }
    }
    return 0;
}
