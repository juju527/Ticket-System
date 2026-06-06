#include "User.hpp"

using std::cout;
using std::endl;

// User 实现

User::User(Username username, Password password, Name name,
           MailAddress mailaddress, int privilege) {
    this->username = username;
    this->password = password;
    this->name = name;
    this->mailaddress = mailaddress;
    this->privilege = privilege;
}

User &User::operator=(const User &other) {
    if (this == (&other))
        return *this;
    username = other.username;
    password = other.password;
    name = other.name;
    mailaddress = other.mailaddress;
    privilege = other.privilege;
    return *this;
}

bool User::operator<(const User &other) const {
    return username < other.username;
}
bool User::operator==(const User &other) const {
    return username == other.username;
}

// UserManager 实现

UserManager::UserManager(BPT<Username, User, 100> *bpt1,
                         BPT<Username, bool, 100> *bpt2) {
    this->bpt1 = bpt1;
    this->bpt2 = bpt2;
}
UserManager::~UserManager() { this->bpt2->clear(); }
void UserManager::clear() {
    bpt1->clear();
    bpt2->clear();
    return;
}

void UserManager::add_user(Username cur, Username username, Password password,
                           Name name, MailAddress mailaddress, int privilege) {
    User user(username, password, name, mailaddress, privilege);
    if (bpt1->empty()) {
        user.privilege = 10;
        bpt1->ins(username, user);
        cout << 0 << endl;
        return;
    }
    if (bpt1->find(username).size() || !bpt2->find(cur).size()) {
        cout << -1 << endl;
        return;
    }
    User aux = bpt1->find(cur)[0];
    if (aux.privilege > privilege)
        bpt1->ins(username, user), cout << 0 << endl;
    else
        cout << -1 << endl;
    return;
}

void UserManager::login(Username username, Password password) {
    vector<User> tmp = bpt1->find(username);
    if (!tmp.size() || bpt2->find(username).size()) {
        cout << -1 << endl;
        return;
    }
    User user = tmp[0];
    if (user.password == password)
        bpt2->ins(username, true), cout << 0 << endl;
    else
        cout << -1 << endl;
    return;
}

void UserManager::logout(Username username) {
    if (bpt2->find(username).size())
        bpt2->del(username, true), cout << 0 << endl;
    else
        cout << -1 << endl;
    return;
}

void UserManager::query_profile(Username cur, Username username) {
    if (!bpt2->find(cur).size()) {
        cout << -1 << endl;
        return;
    }
    vector<User> tmp = bpt1->find(username);
    if (!tmp.size()) {
        cout << -1 << endl;
        return;
    }
    User c = bpt1->find(cur)[0], u = tmp[0];
    if (c.privilege < u.privilege ||
        c.privilege == u.privilege && !(cur == username)) {
        cout << -1 << endl;
        return;
    }
    cout << u.username << " " << u.name << " " << u.mailaddress << " "
         << u.privilege << endl;
    return;
}

void UserManager::modify_profile(Username cur, Username username, string st) {
    if (!bpt2->find(cur).size()) {
        cout << -1 << endl;
        return;
    }
    vector<User> tmp = bpt1->find(username);
    if (!tmp.size()) {
        cout << -1 << endl;
        return;
    }
    User c = bpt1->find(cur)[0], u = tmp[0];
    if (c.privilege < u.privilege ||
        (c.privilege == u.privilege && !(cur == username))) {
        cout << -1 << endl;
        return;
    }
    vector<string> str = Parse(st);
    User buf = u;
    bool tag = 0;
    for (int i = 2; i < str.size(); i += 2) {
        if (str[i] == "-p")
            u.password = str[i + 1];
        else if (str[i] == "-n")
            u.name = str[i + 1];
        else if (str[i] == "-m")
            u.mailaddress = str[i + 1];
        else if (str[i] == "-g")
            u.privilege = str_to_int(str[i + 1]), tag = 1;
    }
    if (tag && c.privilege <= u.privilege) {
        cout << -1 << endl;
        return;
    }
    bpt1->del(username, buf);
    bpt1->ins(username, u);
    cout << u.username << " " << u.name << " " << u.mailaddress << " "
         << u.privilege << endl;
    return;
}