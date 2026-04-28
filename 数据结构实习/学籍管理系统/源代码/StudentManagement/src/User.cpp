#include "../include/User.h"
#include "../include/Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <windows.h>
using namespace std;

// ==================== User类实现 ====================

User::User() : username(""), password(""), type(STUDENT), relatedID("") {}

User::User(string uname, string pwd, UserType t, string rid)
    : username(uname), password(pwd), type(t), relatedID(rid) {}

bool User::checkPassword(const string& pwd) const {
    return password == pwd;
}

void User::display() const {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    cout << left << setw(15) << username 
         << setw(10) << (type == ADMIN ? "管理员" : "学生")
         << setw(12) << relatedID << endl;
}

// ==================== UserManager类实现 ====================

UserManager::UserManager() : currentUser(nullptr) {
    loadUsers();
    if (users.empty()) {
        initDefaultAdmin();
    }
}

UserManager::~UserManager() {
    saveUsers();
    if (currentUser != nullptr) {
        delete currentUser;
    }
}

bool UserManager::login(const string& username, const string& password) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    for (const auto& user : users) {
        if (user.getUsername() == username && user.checkPassword(password)) {
            currentUser = new User(user);
            cout << "\n登录成功！欢迎，" << username << "！" << endl;
            return true;
        }
    }
    cout << "\n登录失败：用户名或密码错误！" << endl;
    return false;
}

void UserManager::logout() {
    if (currentUser != nullptr) {
        delete currentUser;
        currentUser = nullptr;
    }
}

bool UserManager::registerUser(const string& username, const string& password,
                               UserType type, const string& relatedID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    // 验证用户名
    if (!Utils::validateUsername(username)) {
        cout << "错误：用户名格式不正确（3-20个字符，仅限字母数字下划线）！" << endl;
        return false;
    }
    
    // 验证密码
    if (!Utils::validatePassword(password)) {
        cout << "错误：密码长度必须在6-20个字符之间！" << endl;
        return false;
    }
    
    // 检查用户名是否已存在
    if (userExists(username)) {
        cout << "错误：用户名已存在！" << endl;
        return false;
    }
    
    // 如果是学生用户，验证学号格式
    if (type == STUDENT && !Utils::validateStudentID(relatedID)) {
        cout << "错误：学号格式不正确（必须是8位数字）！" << endl;
        return false;
    }
    
    users.push_back(User(username, password, type, relatedID));
    saveUsers();
    cout << "用户注册成功！" << endl;
    return true;
}

bool UserManager::changePassword(const string& oldPassword, const string& newPassword) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    if (currentUser == nullptr) {
        cout << "错误：未登录！" << endl;
        return false;
    }
    
    if (!currentUser->checkPassword(oldPassword)) {
        cout << "错误：原密码不正确！" << endl;
        return false;
    }
    
    if (!Utils::validatePassword(newPassword)) {
        cout << "错误：新密码长度必须在6-20个字符之间！" << endl;
        return false;
    }
    
    // 在用户列表中找到并修改
    for (auto& user : users) {
        if (user.getUsername() == currentUser->getUsername()) {
            user.setPassword(newPassword);
            currentUser->setPassword(newPassword);
            saveUsers();
            cout << "密码修改成功！" << endl;
            return true;
        }
    }
    
    return false;
}

bool UserManager::deleteUser(const string& username) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    if (username == "admin") {
        cout << "错误：不能删除默认管理员账户！" << endl;
        return false;
    }
    
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->getUsername() == username) {
            users.erase(it);
            saveUsers();
            cout << "用户删除成功！" << endl;
            return true;
        }
    }
    
    cout << "错误：用户不存在！" << endl;
    return false;
}

void UserManager::displayAllUsers() const {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    if (users.empty()) {
        cout << "\n暂无用户信息！" << endl;
        return;
    }
    
    cout << "\n========== 用户列表 ==========" << endl;
    cout << left << setw(15) << "用户名" << setw(10) << "类型" 
         << setw(12) << "关联学号" << endl;
    cout << string(40, '-') << endl;
    
    for (const auto& user : users) {
        user.display();
    }
    
    cout << "共计 " << users.size() << " 个用户。" << endl;
}

bool UserManager::userExists(const string& username) const {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            return true;
        }
    }
    return false;
}

void UserManager::saveUsers() {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    ofstream file(USER_FILE);
    if (!file.is_open()) {
        cerr << "警告：无法保存用户数据！" << endl;
        return;
    }
    
    for (const auto& user : users) {
        file << user.getUsername() << "|" << user.getPassword() << "|"
             << user.getType() << "|" << user.getRelatedID() << endl;
    }
    
    file.close();
}

void UserManager::loadUsers() {
    ifstream file(USER_FILE);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string username, password, relatedID;
        int type;
        
        getline(ss, username, '|');
        getline(ss, password, '|');
        ss >> type;
        ss.ignore();
        getline(ss, relatedID);
        
        users.push_back(User(username, password, (UserType)type, relatedID));
    }
    
    file.close();
}

void UserManager::initDefaultAdmin() {
    users.push_back(User("admin", "admin123", ADMIN, ""));
    saveUsers();
    cout << "administrator: admin / admin123" << endl;
}
