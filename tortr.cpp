#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

// check if command exists
bool exists(string cmd) {
    string check = "which " + cmd + " > /dev/null 2>&1";
    return system(check.c_str()) == 0;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "usage:\n";
        cout << "tortr -ins <package>\n";
        cout << "tortr -lf <package>\n";
        cout << "tortr -pu\n";
        return 1;
    }

    string command = argv[1];
    string package = "";
    if (argc >= 3) package = argv[2];

    // detect package manager
    string manager;

    if (exists("pacman")) manager = "pacman";
    else if (exists("apt")) manager = "apt";
    else if (exists("dnf")) manager = "dnf";
    else if (exists("zypper")) manager = "zypper";
    else manager = "unknown";

    cout << "Detected package manager: " << manager << "\n";

    // INSTALL
    if (command == "-ins") {

        if (package == "") {
            cout << "package required\n";
            return 1;
        }

        int result = 1;

        if (manager == "pacman")
            result = system(("sudo pacman -S " + package).c_str());

        else if (manager == "apt")
            result = system(("sudo apt install " + package).c_str());

        else if (manager == "dnf")
            result = system(("sudo dnf install " + package).c_str());

        else if (manager == "zypper")
            result = system(("sudo zypper install " + package).c_str());

        if (result != 0) {
            cout << "Native install failed, trying flatpak...\n";
            system(("flatpak install flathub " + package).c_str());
        }
    }

    // SEARCH
    else if (command == "-lf") {

        if (package == "") {
            cout << "package required\n";
            return 1;
        }

        if (manager == "pacman")
            system(("pacman -Ss " + package).c_str());

        else if (manager == "apt")
            system(("apt search " + package).c_str());

        else if (manager == "dnf")
            system(("dnf search " + package).c_str());

        else if (manager == "zypper")
            system(("zypper search " + package).c_str());

        cout << "Searching flatpak...\n";
        system(("flatpak search " + package).c_str());
    }

    // UPDATE
    else if (command == "-pu") {

        if (manager == "pacman")
            system("sudo pacman -Syu");

        else if (manager == "apt")
            system("sudo apt update && sudo apt upgrade -y");

        else if (manager == "dnf")
            system("sudo dnf upgrade -y");

        else if (manager == "zypper")
            system("sudo zypper update");

        cout << "Updating flatpaks...\n";
        system("flatpak update");
    }

    else {
        cout << "unknown command\n";
    }

    return 0;
}
