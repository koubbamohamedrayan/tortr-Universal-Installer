#include "manager.h"
#include <unistd.h>
#include <sys/wait.h>

int run_cmd(const std::vector<std::string>& args) {

    pid_t pid = fork();

    if(pid == 0) {

        std::vector<char*> cargs;

        for(auto& a : args)
            cargs.push_back(const_cast<char*>(a.c_str()));

        cargs.push_back(nullptr);

        execvp(cargs[0], cargs.data());
        exit(1);
    }

    int status;
    waitpid(pid,&status,0);
    return WEXITSTATUS(status);
}

bool command_exists(const std::string& cmd) {

    pid_t pid = fork();

    if(pid == 0) {
        execlp("which","which",cmd.c_str(),NULL);
        exit(1);
    }

    int status;
    waitpid(pid,&status,0);
    return WEXITSTATUS(status)==0;
}

Manager managers[] = {

{"pacman","pacman",
 {"sudo","pacman","-S"},
 {"pacman","-Ss"},
 {"sudo","pacman","-Syu"}},

{"apt","apt",
 {"sudo","apt","install","-y"},
 {"apt","search"},
 {"sudo","apt","upgrade","-y"}},

{"dnf","dnf",
 {"sudo","dnf","install","-y"},
 {"dnf","search"},
 {"sudo","dnf","upgrade","-y"}},

{"zypper","zypper",
 {"sudo","zypper","install"},
 {"zypper","search"},
 {"sudo","zypper","update"}},

{"xbps","xbps-install",
 {"sudo","xbps-install","-S"},
 {"xbps-query","-Rs"},
 {"sudo","xbps-install","-Su"}},

{"nix","nix-env",
 {"nix-env","-iA"},
 {"nix-env","-qa"},
 {"nix-env","-u"}}

};

Manager* detect_manager() {

    for(auto& m : managers)
        if(command_exists(m.detect))
            return &m;

    return nullptr;
}
