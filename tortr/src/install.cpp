#include "install.h"
#include "sources.h"
#include <iostream>

void install_pkg(Manager* m,std::string pkg) {

    std::vector<std::string> cmd = m->install;
    cmd.push_back(pkg);

    if(run_cmd(cmd)==0)
        return;

    if(try_flatpak(pkg))
        return;

    if(try_snap(pkg))
        return;

    std::cout << "package not found\n";
}

void search_pkg(Manager* m,std::string pkg) {

    std::vector<std::string> cmd = m->search;
    cmd.push_back(pkg);

    run_cmd(cmd);
}

void update_system(Manager* m) {

    run_cmd(m->update);
}
