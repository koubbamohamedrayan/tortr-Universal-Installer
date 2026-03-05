#pragma once
#include <string>
#include <vector>

struct Manager {
    std::string name;
    std::string detect;
    std::vector<std::string> install;
    std::vector<std::string> search;
    std::vector<std::string> update;
};

Manager* detect_manager();
bool command_exists(const std::string& cmd);
int run_cmd(const std::vector<std::string>& args);
