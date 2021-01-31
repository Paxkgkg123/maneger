#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "C:\Users\relaz\CLionProjects\maneger_password\cmake-build-debug\cription.h"

using namespace std;

const int MAX_NUM_PASS = 10000;
string base[MAX_NUM_PASS];

string check_master_pass(string);

void master(char*, char*);

void get(char*, char*);

void add(char*, char*, char*);

void del(char*);

void chk(char*);

void help();

void base_input(char*);
void base_output(char*);

int main(int argc, char **argv) {
    cout << "Input master-password\n";
    string master_pass, config;
    cin >> master_pass;
    master_pass = check_master_pass(master_pass);
    if (master_pass == "0") {
        cout << "Master-password is incorrect\n";
        return 0;
    }

    if (argc > 1) {
        if (strcmp(argv[1], "--config") == 0) {
            base_input(argv[2]); // заполняем временную базу паролями
            if (argc > 2) {
                if (strcmp(argv[3], "get") == 0) get(argv[4], &master_pass[0]);
                else if (strcmp(argv[3], "master") == 0) master(&master_pass[0], argv[4]);
                else if (strcmp(argv[3], "del") == 0) del(argv[4]);
                else if (strcmp(argv[3], "add") == 0) add(argv[4], argv[5], &master_pass[0]);
                else if (strcmp(argv[3], "chk") == 0) chk(argv[4]);
                else {
                    cout << "Incorrect command\n";
                    return 0;
                }
                base_output(argv[2]);
                return 0;
            } else {
                config.append(argv[2]);
            }
            base_output(&config[0]);
        } else {
            if (strcmp(argv[1], "--help") == 0)
                help();
            else
                cout << "No such command\n";
            return 0;
        }
    }

    if (config.empty()) {
        ifstream conf("config.txt");
        conf >> config;
        conf.close();
    }

    char* c = &config[0];
    base_input(c);
    string command, site_login;
    cout << "Input command\n";
    cin >> command;
    ofstream out(config);
    while (command != "q" && command != "quit" && command != "exit") {
        if (command == "master") {
            string n_master;
            cin >> n_master;
            master(&master_pass[0], &n_master[0]);
        }
        else if (command == "get") {
            cin >> site_login;
            get(&site_login[0], &master_pass[0]);
        }
        else if (command == "add") {
            string password;
            cin >> site_login >> password;
            add(&site_login[0], &password[0], &master_pass[0]);
        }
        else if (command == "del") {
            cin >> site_login;
            del(&site_login[0]);
        }
        else if (command == "chk") {
            cin >> site_login;
            chk(&site_login[0]);
        }
        else if (command == "help") help();
        else cout << "no such command\n";
        command.clear();
        cout << "Input command\n";
        cin >> command;
    }

    base_output(&config[0]);
    return 0;
}

string check_master_pass(string master_password) {
    ifstream in("master.txt");
    string encrypted, unencrypted;
    in >> unencrypted >> encrypted;

    in.close();
    if (encryption(unencrypted, master_password) == encrypted)
        return master_password;
    return "0";
}

void master(char* m, char* N) {
    string master_pass, new_master;
    master_pass.append(m);
    new_master.append(N);
    for (int i = 0; base[i] != "."; i++) {
        if (!base[i].empty()) {
            int n;
            for (int j = 0; base[i][j] != '/'; j++)
                n = j;
            string password;
            password.append(base[i], n + 2, base[i].size() - n - 2);
            base[i].erase(n + 1);
            password = decryption(password, master_pass);
            password = encryption(password, new_master);
            base[i].push_back('/');
            base[i].append(password);
        }
    }
    ifstream in("master.txt");
    string word;
    in >> word;
    ofstream out("master.txt");
    out << word << '\n' << encryption(word, new_master);
    cout << "Master-password is change\n";
}

void get(char* argv, char* m) {
    string site_login, password, master;
    master.append(m);
    site_login.append(argv);
    site_login.push_back('/');
    int n = site_login.size();
    for (int i = 0; base[i] != "."; i++) {
        if (base[i].substr(0, n) == site_login) {
            password.append(base[i], n, base[i].size() - n);
            break;
        }
    }
    if (password.empty()) {
        cout << "password is not copied\n";
        return;
    }
    password = decryption(password, master);
    char* str = &password[0];
    size_t len;
    HGLOBAL hMem;
    n = site_login.size();
    len = n + 1;
    hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), str, len);
    GlobalUnlock(hMem);
    OpenClipboard(nullptr);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    cout <<"password copied\n";
}

void add(char* s_l, char* p, char* m) {
    string site_login, master_pass, password;
    master_pass.append(m);
    site_login.append(s_l);
    password.append(p);
    int i = -1;
    do {
        i++;
        if (base[i].empty()) {
            base[i] = site_login;
            base[i].push_back('/');
            base[i].append(encryption(password, master_pass));
            break;
        }
        if (base[i] == ".") {
            base[i].erase();
            base[i] = site_login;
            base[i].push_back('/');
            base[i].append(encryption(password, master_pass));
            base[i + 1].erase();
            base[i + 1].push_back('.');
            break;
        }
    } while (base[i] != ".");
    cout << "password is add\n";
}

void del(char* argv) {
    string site_login;
    site_login.append(argv);
    int n = site_login.size();
    for (int i = 0; base[i] != "."; i++) {
        if (base[i].substr(0, n) == site_login) {
            base[i].erase();
            cout << "password deleted\n";
            return;
        }
    }
    cout << "password is not deleted\n";
}

void chk(char* argv) {
    string site_login;
    site_login.append(argv);
    site_login.push_back('/');
    int n = site_login.size();
    for (int i = 0; base[i] != "."; i++) {
        if (base[i].substr(0, n) == site_login) {
            cout << "yes\n";
            return;
        }
    }
    cout << "no\n";
}

void help() {
    cout << "master - Sets a new master password (the password is entered from the keyboard)\n";
    cout << "get SITE:LOGIN - Copies the password for the site and login to the clipboard\n";
    cout << "add SITE:LOGIN - Adds a password to the database for the site, login LOGIN (the password is entered from the keyboard)\n";
    cout << "del SITE:LOGIN - Deletes the password for the SITE, login from the database\n";
    cout << "chk SITE:LOGIN - Checks the presence of a password for the site SITE, login LOGIN\n";
    cout << "q / quit / exit - Terminates the program\n";
}

void base_input(char* argv) {
    string config;
    config.append(argv);
    if (argv[0] == 39)
        config.substr(1, config.size() - 2);
    ifstream in(config);
    string data;
    int i = -1;
    do {
        i++;
        in >> base[i];
    } while (base[i] != ".");
    in.close();
}
void base_output(char* config) {
    ofstream out(config);
    int i = -1;
    do {
        i++;
        if (!base[i].empty())
            out << base[i] << '\n';
    } while (base[i] != ".");
    out.close();
}
