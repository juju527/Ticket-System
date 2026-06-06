#ifndef USER_HPP
#define USER_HPP
#include "BPT.hpp"
#include "utility.hpp"

using Username = String<20>;
using Password = String<30>;
using Name = String<15>;
using MailAddress = String<30>;

class User {
  public:
    Username username;
    Password password;
    Name name;
    MailAddress mailaddress;
    int privilege;
    User() = default;
    User(Username, Password, Name, MailAddress, int);
    User &operator=(const User &);
    bool operator<(const User &) const;
    bool operator==(const User &) const;
};

class UserManager {
  private:
    BPT<Username, User, 100> *bpt1; // 所有用户信息存储
    BPT<Username, bool, 100> *bpt2; // 用户是否登录
  public:
    UserManager(BPT<Username, User, 100> *, BPT<Username, bool, 100> *);
    ~UserManager();
    void clear();
    void add_user(Username, Username, Password, Name, MailAddress, int);
    void login(Username, Password);
    void logout(Username);
    void query_profile(Username, Username);
    void modify_profile(Username, Username, string);
};

#endif