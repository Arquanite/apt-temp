#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <cstdlib>
#include <sys/stat.h>
using namespace std;

typedef unordered_set<string> string_list;

const string GREEN = "\033[32;1m";
const string RED = "\033[31;1m";
const string YELLOW = "\033[33;1m";
const string RESET = "\033[0m";

string_list installed_packages;

bool is_installed(string name){
    string command = "dpkg --get-selections | awk '/" + name + "\t/' | grep -w install > /dev/null";
    if(system(command.c_str()) == 0) return true;
    else return false;
}

bool is_removed(string name){
    string command = "dpkg --get-selections | awk '/" + name + "\t/' | grep -w deinstall > /dev/null";
    if(system(command.c_str()) == 0) return true;
    else return false;
}

void test(string_list packages){
    for(string p : packages){
        cout<<"- ";
        if(is_installed(p)) cout<<p<<GREEN<<" is installed"<<RESET<<endl;
        else if(is_removed(p)) cout<<p<<YELLOW<<" was removed but it's files still exists on disk"<<RESET<<endl;
        else cout<<p<<RED<<" is not installed"<<RESET<<endl;
    }
}

void error(string name){
    cout<<"At least 1 package name needed."<<endl;
    cout<<"Use: apt-temp " + name + " <package> [package2] [package3]..."<<endl;
    exit(-1);
}

void install(string_list packages){
    string arg;
    string_list to_remove;
    for(string p : packages){
        if(is_installed(p)){
            cout<<YELLOW<<p<<" is already installed, skipping..."<<RESET<<endl;
            to_remove.insert(p);
            continue;
        }
        arg += p + " ";
    }
    for(string t : to_remove) packages.erase(t);
    if(system(string("apt-get install " + arg).c_str()) == 0){
        for(string p : packages) installed_packages.insert(p);
    }
    else {
        cout<<RED<<"Error occured when installing packages, exiting..."<<endl<<endl;
        exit(-1);
    }
}

void remove(string_list packages){
    if(packages.empty()){
        cout<<"Nothing to do, exiting...";
        exit(0);
    }

    string_list invalid;
    for(string p : packages){
        if(installed_packages.count(p) == 0) invalid.insert(p + ' ');
    }
    if(!invalid.empty()){
        cout<<"Error: you can not remove packages that were not installed by apt-temp!"<<endl;
        cout<<"Invalid package names: ";
        for(string p : invalid) cout<<p;
        cout<<endl;
        exit(-1);
    }

    string arg;
    for(string p : packages) arg += p + " ";

    if(system(string("apt-get remove " + arg).c_str()) == 0){
        for(string p : packages) installed_packages.erase(p);
    }
    else {
        cout<<"Error occured when removing packages, exiting..."<<endl;
        exit(-1);
    }
}

void purge(string_list packages){
    if(packages.empty()){
        cout<<"Nothing to do, exiting...";
        exit(0);
    }

    string_list invalid;
    for(string p : packages){
        if(installed_packages.count(p) == 0) invalid.insert(p + ' ');
    }
    if(!invalid.empty()){
        cout<<"Error: you can not purge packages that were not installed by apt-temp!"<<endl;
        cout<<"Invalid package names: ";
        for(string p : invalid) cout<<p;
        cout<<endl;
        exit(-1);
    }

    string arg;
    for(string p : packages) arg += p + " ";

    if(system(string("apt-get purge " + arg).c_str()) == 0){
        for(string p : packages) installed_packages.erase(p);
    }
    else {
        cout<<"Error occured when purging packages, exiting..."<<endl;
        exit(-1);
    }
}

void approve(string_list packages){
    if(packages.empty()){
        cout<<"Nothing to do, exiting...";
        exit(0);
    }

    string_list invalid;
    for(string p : packages){
        if(installed_packages.count(p) == 0) invalid.insert(p);
    }
    if(!invalid.empty()){
        cout<<"Error: you can not approve packages that were not installed by apt-temp!"<<endl;
        cout<<"Invalid package names: ";
        for(string p : invalid) cout<<p;
        cout<<endl;
        exit(-1);
    }
    else for(string p : packages) installed_packages.erase(p);
}

void clean(){
    system("apt-get autoremove");
    system("apt-get autoclean");
    system("apt-get purge `dpkg --get-selections | grep deinstall | awk '{print $1}'`");
}

bool help(){
    cout<<"Usage: apt-temp <action> [package1] [package2] ..."<<endl;
    cout<<"Actions:"<<endl;
    cout<<"    help - show this page"<<endl;
    cout<<"    list - show list of installed temporary packages"<<endl;
    cout<<"    remove <package> - remove package(s)"<<endl;
    cout<<"    purge <package> - purge package(s)"<<endl;
    cout<<"    remove-all - remove all temporary packages"<<endl;
    cout<<"    purge-all - purge all temporary packages"<<endl;
    cout<<"    install <package> - install package(s)"<<endl;
    cout<<"    approve <package> - remove package(s) from the list"<<endl;
    cout<<"    test <package> - check state of package(s)"<<endl;
    cout<<"    clean - autoremove, autoclean and purge removed packages"<<endl;
}

int main(int argc, char *argv[]){
    // check arg count
    if(argc == 1){
        help();
        return -1;
    }
    string action(argv[1]);

    if(action != "help" && action != "test" && action != "list" && system("test \"`whoami`\" = \"root\"")){
        cout<<YELLOW<<"You need to be root!"<<RESET<<endl;
        return -1;
    }

    // load from file
    if(system("if [ ! -d /etc/apt-temp ]; then exit 0; else exit 1; fi") == 0){
        cout<<RED<<"/etc/apt-temp directory does not exist, creating new.."<<RESET<<endl;
        system("mkdir /etc/apt-temp");
    }
    ifstream in("/etc/apt-temp/temporary_packages");
    if(!in.good()){
        struct stat buffer;
        if(stat ("/etc/apt-temp/temporary_packages", &buffer)) cout<<RED<<"List file does not exist, creating new.."<<RESET<<endl;
        system("touch /etc/apt-temp/temporary_packages");
    }

    while(!in.eof() && in.good()){
        string package;
        in>>package;
        installed_packages.insert(package);
    }
    in.close();

    installed_packages.erase("");

    // one argument actions
    if(action == "list"){
        if(installed_packages.empty()) cout<<YELLOW<<"No temporary packages installed."<<RESET<<endl;
        else {
            cout<<GREEN<<"There are "<<installed_packages.size()<<" temporary packages installed:"<<RESET<<endl;
            for(string p : installed_packages) cout<<"- "<<p<<endl;
        }
    }
    else if(action == "remove-all") remove(installed_packages);
    else if(action == "purge-all") purge(installed_packages);
    else if(action == "help") help();
    else if(action  == "clean") clean();
    else {
        // parse package names
        string_list packages;
        for(int i=2; i<argc; ++i){
            packages.insert(string(argv[i]));
        }

        // multi argument actions
        if(argc <3) error(action);
        else if(action == "install") install(packages);
        else if(action == "remove") remove(packages);
        else if(action == "purge") purge(packages);
        else if(action == "approve") approve(packages);
        else if(action == "test") test(packages);
    }

    ofstream out("/etc/apt-temp/temporary_packages");
    for(string p : installed_packages) out<<p<<endl;
    out.flush();
    out.close();
}

