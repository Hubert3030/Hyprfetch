#include <iostream>
#include <string>
#include <array>
#include <cstdio>
#include <regex>
#include <cstdlib>
#include <unistd.h>

std::string getCommandOutput(const std::string& cmd) {
    std::array<char, 256> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "N/A";
    while (fgets(buffer.data(), buffer.size(), pipe)) result += buffer.data();
    pclose(pipe);
    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result.empty() ? "N/A" : result;
}

std::string getResolution() {
    std::string json = getCommandOutput("hyprctl monitors -j");
    std::regex width_regex("\"width\":\\s*(\\d+)");
    std::regex height_regex("\"height\":\\s*(\\d+)");
    std::smatch w_match, h_match;
    if (std::regex_search(json, w_match, width_regex) && std::regex_search(json, h_match, height_regex))
        return w_match[1].str() + "x" + h_match[1].str();
    return "N/A";
}

std::string getIcons() {
    std::string icons = getCommandOutput("gsettings get org.gnome.desktop.interface icon-theme");
    if (icons != "N/A") {
        icons.erase(remove(icons.begin(), icons.end(), '\''), icons.end());
        return icons + " [GTK2/3/4]";
    }
    return "N/A";
}

std::string simplifyShell(const std::string& shellPath) {
    size_t pos = shellPath.find_last_of('/');
    return pos != std::string::npos ? shellPath.substr(pos + 1) : shellPath;
}

std::string simplifyTerminal() {
    const char* term_prog = std::getenv("TERM_PROGRAM");
    if(term_prog) return std::string(term_prog);
    const char* term_env = std::getenv("TERM");
    if(term_env) {
        std::string t = term_env;
        if(t.find("256color") != std::string::npos) t.erase(t.find("-256color"));
        return t;
    }
    return "N/A";
}

int main() {
    std::string wmVersion    = getCommandOutput("hyprctl version | head -n 1");
    std::string activeWindow = getCommandOutput("hyprctl activewindow | grep 'class:' | awk '{print $2}'");
    std::string monitor      = getCommandOutput("hyprctl monitors | grep 'Monitor' | awk '{print $2}'");
    std::string resolution   = getResolution();
    std::string uptime       = getCommandOutput("uptime -p | sed 's/up //'");

    char hostname[128];
    gethostname(hostname, sizeof(hostname));
    std::string host = hostname;

    const char* user_env = std::getenv("USER");
    std::string user = user_env ? user_env : "N/A";

    const char* shell_env = std::getenv("SHELL");
    std::string shell = shell_env ? simplifyShell(shell_env) : "N/A";

    std::string terminal = simplifyTerminal();
    std::string icons = getIcons();

    std::cout << "\033[1;31m╔╗╔╗╔╗╔╗╔═══╗╔═══╗╔╗──╔══╗╔╗─╔╗╔══╗─\033[0m\n";
    std::cout << "\033[1;33m║║║║║║║║║╔═╗║║╔═╗║║║──║╔╗║║╚═╝║║╔╗╚╗\033[0m\n";
    std::cout << "\033[1;32m║╚╝║║╚╝║║╚═╝║║╚═╝║║║──║╚╝║║╔╗─║║║╚╗║\033[0m\n";
    std::cout << "\033[1;36m║╔╗║╚═╗║║╔══╝║╔╗╔╝║║──║╔╗║║║╚╗║║║─║║\033[0m\n";
    std::cout << "\033[1;34m║║║║─╔╝║║║───║║║║─║╚═╗║║║║║║─║║║╚═╝║\033[0m\n";
    std::cout << "\033[1;35m╚╝╚╝─╚═╝╚╝───╚╝╚╝─╚══╝╚╝╚╝╚╝─╚╝╚═══╝\033[0m\n";
    std::cout << "------------------------------------------\n";

    std::cout << "User:           " << user << "\n";
    std::cout << "Host:           " << host << "\n";
    std::cout << "WM Version:     " << wmVersion << "\n";
    std::cout << "Active Window:  " << activeWindow << "\n";
    std::cout << "Monitor:        " << monitor << "\n";
    std::cout << "Resolution:     " << resolution << "\n";
    std::cout << "Shell:          " << shell << "\n";
    std::cout << "Terminal:       " << terminal << "\n";
    std::cout << "Icons:          " << icons << "\n";
    std::cout << "Uptime:         " << uptime << "\n";
    std::cout << "------------------------------------------\n";

    return 0;
}
