#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <regex>

using namespace std;

namespace Utils {
    // 验证学号格式（8位数字）
    bool validateStudentID(const string& id);
    
    // 验证课程编号格式（2-3个字母+3位数字）
    bool validateCourseID(const string& id);
    
    // 验证电话号码（11位数字）
    bool validatePhoneNumber(const string& phone);
    
    // 验证日期格式（YYYY-MM-DD）
    bool validateDate(const string& date);
    
    // 验证成绩范围（0-100）
    bool validateScore(double score);
    
    // 验证学分范围（1-8）
    bool validateCredits(int credits);
    
    // 验证性别
    bool validateGender(const string& gender);
    
    // 验证用户名（3-20个字符，字母数字下划线）
    bool validateUsername(const string& username);
    
    // 验证密码（6-20个字符）
    bool validatePassword(const string& password);
    
    // 字符串去除前后空格
    string trim(const string& str);
    
    // 清屏
    void clearScreen();
    
    // 暂停（按任意键继续）
    void pause();
}

#endif
