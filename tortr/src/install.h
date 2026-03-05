#pragma once
#include <string>
#include "manager.h"

void install_pkg(Manager* m,std::string pkg);
void search_pkg(Manager* m,std::string pkg);
void update_system(Manager* m);
