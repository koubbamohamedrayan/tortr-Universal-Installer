#include "sources.h"
#include "manager.h"

bool try_flatpak(std::string pkg) {

    if(!command_exists("flatpak"))
        return false;

    return run_cmd({"flatpak","install","flathub",pkg}) == 0;
}

bool try_snap(std::string pkg) {

    if(!command_exists("snap"))
        return false;

    return run_cmd({"sudo","snap","install",pkg}) == 0;
}
