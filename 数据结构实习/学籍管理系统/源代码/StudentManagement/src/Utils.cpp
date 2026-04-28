#include "../include/Utils.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <windows.h>
using namespace std;

namespace Utils {
    
    // 验证学号格式（8位数字）
    bool validateStudentID(const string& id) {
        if (id.length() != 8) return false;
        return regex_match(id, regex("^[0-9]{8}$"));
    }
    
    // 验证课程编号格式（2-3个字母+3位数字）
    bool validateCourseID(const string& id) {
        return regex_match(id, regex("^[A-Z]{2,3}[0-9]{3}$"));
    }
    
    // 验证电话号码（11位数字）
    bool validatePhoneNumber(const string& phone) {
        if (phone.length() != 11) return false;
        return regex_match(phone, regex("^1[3-9][0-9]{9}$"));
    }
    
    // 验证日期格式（YYYY-MM-DD）
    bool validateDate(const string& date) {
        if (!regex_match(date, regex("^[0-9]{4}-[0-9]{2}-[0-9]{2}$"))) {
            return false;
        }
        
        // 提取年月日
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));
        
        // 验证月份
        if (month < 1 || month > 12) return false;
        
        // 验证日期
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // 闰年判断
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            daysInMonth[1] = 29;
        }
        
        if (day < 1 || day > daysInMonth[month - 1]) return false;
        
        // 验证年份合理性（1900-2020）
        if (year < 1900 || year > 2020) return false;
        
        return true;
    }
    
    // 验证成绩范围（0-100）
    bool validateScore(double score) {
        return score >= 0 && score <= 100;
    }
    
    // 验证学分范围（1-8）
    bool validateCredits(int credits) {
        return credits >= 1 && credits <= 8;
    }
    
    // 验证性别
    bool validateGender(const string& gender) {
        return gender == "男" || gender == "女";
    }
    
    // 验证用户名（3-20个字符，字母数字下划线）
    bool validateUsername(const string& username) {
        if (username.length() < 3 || username.length() > 20) return false;
        return regex_match(username, regex("^[a-zA-Z0-9_]+$"));
    }
    
    // 验证密码（6-20个字符）
    bool validatePassword(const string& password) {
        return password.length() >= 6 && password.length() <= 20;
    }
    
    // 字符串去除前后空格
    string trim(const string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }
    
    // 清屏
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    // 暂停（按任意键继续）
    void pause() {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
        cout << "\n按回车键继续...";
        cin.ignore();
        cin.get();
    }
}
