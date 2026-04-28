#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

// 用户类型枚举
enum UserType {
    ADMIN = 1,      // 管理员
    STUDENT = 2     // 普通用户（学生）
};

// 用户类
class User {
private:
    string username;        // 用户名
    string password;        // 密码
    UserType type;          // 用户类型
    string relatedID;       // 关联ID（学生用户关联学号）

public:
    User();
    User(string uname, string pwd, UserType t, string rid = "");
    
    // Getter方法
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    UserType getType() const { return type; }
    string getRelatedID() const { return relatedID; }
    
    // Setter方法
    void setPassword(const string& pwd) { password = pwd; }
    
    // 验证密码
    bool checkPassword(const string& pwd) const;
    
    // 判断是否为管理员
    bool isAdmin() const { return type == ADMIN; }
    
    // 显示用户信息
    void display() const;
};

// 用户管理类
class UserManager {
private:
    vector<User> users;
    User* currentUser;
    const string USER_FILE = "data/users.txt";
    
public:
    UserManager();
    ~UserManager();
    
    // 用户登录
    bool login(const string& username, const string& password);
    
    // 用户注销
    void logout();
    
    // 获取当前用户
    User* getCurrentUser() { return currentUser; }
    
    // 注册新用户（管理员权限）
    bool registerUser(const string& username, const string& password, 
                     UserType type, const string& relatedID = "");
    
    // 修改密码
    bool changePassword(const string& oldPassword, const string& newPassword);
    
    // 删除用户（管理员权限）
    bool deleteUser(const string& username);
    
    // 显示所有用户（管理员权限）
    void displayAllUsers() const;
    
    // 检查用户名是否存在
    bool userExists(const string& username) const;
    
    // 保存用户数据
    void saveUsers();
    
    // 加载用户数据
    void loadUsers();
    
    // 初始化默认管理员
    void initDefaultAdmin();
};

#endif
