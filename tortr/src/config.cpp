#include "config.h"
#include <fstream>

std::string config_preference() {

    std::ifstream f(std::string(getenv("HOME")) + "/.config/tortr/config.conf");

    std::string k,v;

    while(f >> k >> v)
        if(k=="prefer")
            return v;

    return "repo";
}
