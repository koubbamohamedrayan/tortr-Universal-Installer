#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>

bool exists(const std::string& cmd)
{
    std::string path = "/usr/bin/" + cmd;
    return access(path.c_str(), X_OK) == 0;
}

int run(std::vector<std::string> args)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        std::vector<char*> cargs;
        for (auto& a : args)
            cargs.push_back(const_cast<char*>(a.c_str()));

        cargs.push_back(nullptr);

        execvp(cargs[0], cargs.data());
        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);
    return status;
}

std::string detect_manager()
{
    if (exists("pacman")) return "pacman";
    if (exists("apt")) return "apt";
    if (exists("dnf")) return "dnf";
    if (exists("zypper")) return "zypper";
    return "unknown";
}

int try_native(const std::string& manager, const std::string& pkg)
{
    std::cout << "[1] trying native repo\n";

    if (manager == "pacman")
        return run({"sudo","pacman","-S","--needed",pkg});

    if (manager == "apt")
        return run({"sudo","apt","install","-y",pkg});

    if (manager == "dnf")
        return run({"sudo","dnf","install","-y",pkg});

    if (manager == "zypper")
        return run({"sudo","zypper","install",pkg});

    return 1;
}

int try_flatpak(const std::string& pkg)
{
    if (!exists("flatpak")) return 1;

    std::cout << "[2] trying flatpak\n";
    return run({"flatpak","install","flathub",pkg});
}

int try_snap(const std::string& pkg)
{
    if (!exists("snap")) return 1;

    std::cout << "[3] trying snap\n";
    return run({"sudo","snap","install",pkg});
}

int try_aur(const std::string& pkg)
{
    if (!exists("git") || !exists("makepkg")) return 1;

    std::cout << "[4] trying AUR\n";

    run({"git","clone","https://aur.archlinux.org/" + pkg + ".git"});
    run({"bash","-c","cd " + pkg + " && makepkg -si"});

    return 0;
}

void install(const std::string& manager, const std::string& pkg)
{
    if (try_native(manager, pkg) == 0)
    {
        std::cout << "installed from native repo\n";
        return;
    }

    if (try_flatpak(pkg) == 0)
    {
        std::cout << "installed via flatpak\n";
        return;
    }

    if (try_snap(pkg) == 0)
    {
        std::cout << "installed via snap\n";
        return;
    }

    if (manager == "pacman")
    {
        try_aur(pkg);
        return;
    }

    std::cout << "package not found anywhere\n";
}

void search(const std::string& manager, const std::string& pkg)
{
    std::cout << "native repo results\n";

    if (manager == "pacman")
        run({"pacman","-Ss",pkg});

    else if (manager == "apt")
        run({"apt","search",pkg});

    else if (manager == "dnf")
        run({"dnf","search",pkg});

    else if (manager == "zypper")
        run({"zypper","search",pkg});

    if (exists("flatpak"))
    {
        std::cout << "\nflatpak\n";
        run({"flatpak","search",pkg});
    }

    if (exists("snap"))
    {
        std::cout << "\nsnap\n";
        run({"snap","find",pkg});
    }
}

void update(const std::string& manager)
{
    std::cout << "updating system\n";

    if (manager == "pacman")
        run({"sudo","pacman","-Syu"});

    else if (manager == "apt")
    {
        run({"sudo","apt","update"});
        run({"sudo","apt","upgrade","-y"});
    }

    else if (manager == "dnf")
        run({"sudo","dnf","upgrade","-y"});

    else if (manager == "zypper")
        run({"sudo","zypper","update"});

    if (exists("flatpak"))
        run({"flatpak","update"});

    if (exists("snap"))
        run({"sudo","snap","refresh"});
}

void help()
{
    std::cout << "tortr universal package installer\n\n";
    std::cout << "commands\n";
    std::cout << " install <package>\n";
    std::cout << " search <package>\n";
    std::cout << " update\n";
    std::cout << " help\n";
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        help();
        return 1;
    }

    std::string command = argv[1];
    std::string manager = detect_manager();

    std::cout << "detected package manager: " << manager << "\n";

    if (command == "install" && argc > 2)
        install(manager, argv[2]);

    else if (command == "search" && argc > 2)
        search(manager, argv[2]);

    else if (command == "update")
        update(manager);

    else
        help();

    return 0;
}
