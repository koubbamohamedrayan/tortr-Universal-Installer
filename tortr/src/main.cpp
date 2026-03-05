#include <iostream>
#include "manager.h"
#include "install.h"

void help() {
    std::cout << "tortr\n";
    std::cout << " install <pkg>\n";
    std::cout << " search <pkg>\n";
    std::cout << " update\n";
    std::cout << " fetch\n";
}

int main(int argc, char* argv[]) {

    if(argc < 2) {
        help();
        return 0;
    }

    std::string cmd = argv[1];

    Manager* m = detect_manager();

    if(!m) {
        std::cout << "no package manager detected\n";
        return 1;
    }

    if(cmd == "install" && argc > 2)
        install_pkg(m, argv[2]);

    else if(cmd == "search" && argc > 2)
        search_pkg(m, argv[2]);

    else if(cmd == "update")
        update_system(m);

    else
        help();
}
