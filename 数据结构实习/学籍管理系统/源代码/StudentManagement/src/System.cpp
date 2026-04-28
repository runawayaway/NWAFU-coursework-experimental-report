#include "../include/System.h"
#include "../include/Utils.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

StudentManagementSystem::StudentManagementSystem() {
    // 构造函数：初始化各个管理器
}

StudentManagementSystem::~StudentManagementSystem() {
    // 析构函数：清理资源
}

void StudentManagementSystem::run() {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    
    cout << "\n╔═══════════════════════════════════════════╗" << endl;
    cout << "║       欢迎使用学籍管理系统 v2.0          ║" << endl;
    cout << "╚═══════════════════════════════════════════╝\n" << endl;
    
    // 登录
    if (!loginSystem()) {
        return;
    }
    
    User* currentUser = userManager.getCurrentUser();
    
    while (true) {
        if (currentUser->isAdmin()) {
            showAdminMenu();
        } else {
            showStudentMenu();
        }
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 0) {
            cout << "\n感谢使用学籍管理系统，再见！" << endl;
            userManager.logout();
            break;
        }
        
        if (currentUser->isAdmin()) {
            switch (choice) {
                case 1: adminStudentManagement(); break;
                case 2: adminCourseManagement(); break;
                case 3: adminSelectionManagement(); break;
                case 4: adminScoreManagement(); break;
                case 5: adminQueryStatistics(); break;
                case 6: adminUserManagement(); break;
                default: cout << "无效选择，请重新输入！" << endl;
            }
        } else {
            switch (choice) {
                case 1: studentViewInfo(); break;
                case 2: studentSelectCourse(); break;
                case 3: studentViewCourses(); break;
                case 4: studentViewScores(); break;
                case 5: studentChangePassword(); break;
                default: cout << "无效选择，请重新输入！" << endl;
            }
        }
        
        Utils::pause();
    }
}

bool StudentManagementSystem::loginSystem() {
    int attempts = 0;
    const int maxAttempts = 3;
    
    while (attempts < maxAttempts) {
        cout << "请登录系统" << endl;
        cout << "用户名：";
        string username;
        cin >> username;
        
        cout << "密码：";
        string password;
        cin >> password;
        cin.ignore();
        
        if (userManager.login(username, password)) {
            return true;
        }
        
        attempts++;
        if (attempts < maxAttempts) {
            cout << "您还有 " << (maxAttempts - attempts) << " 次尝试机会。\n" << endl;
        }
    }
    
    cout << "登录失败次数过多，系统退出。" << endl;
    return false;
}

// ==================== 菜单显示函数 ====================

void StudentManagementSystem::showAdminMenu() {
    cout << "\n╔═══════════════════════════════════════════╗" << endl;
    cout << "║       学籍管理系统 - 管理员菜单           ║" << endl;
    cout << "╠═══════════════════════════════════════════╣" << endl;
    cout << "║  1. 学生信息管理                          ║" << endl;
    cout << "║  2. 课程信息管理                          ║" << endl;
    cout << "║  3. 选课管理                              ║" << endl;
    cout << "║  4. 成绩管理                              ║" << endl;
    cout << "║  5. 查询统计                              ║" << endl;
    cout << "║  6. 用户管理                              ║" << endl;
    cout << "║  0. 退出系统                              ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;
    cout << "请选择功能（0-6）：";
}

void StudentManagementSystem::showStudentMenu() {
    cout << "\n╔═══════════════════════════════════════════╗" << endl;
    cout << "║       学籍管理系统 - 学生菜单             ║" << endl;
    cout << "╠═══════════════════════════════════════════╣" << endl;
    cout << "║  1. 查看个人信息                          ║" << endl;
    cout << "║  2. 选课/退课                             ║" << endl;
    cout << "║  3. 查看选课记录                          ║" << endl;
    cout << "║  4. 查看成绩                              ║" << endl;
    cout << "║  5. 修改密码                              ║" << endl;
    cout << "║  0. 退出系统                              ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;
    cout << "请选择功能（0-5）：";
}

void StudentManagementSystem::showStudentInfoMenu() {
    cout << "\n========== 学生信息管理 ==========" << endl;
    cout << "1. 添加学生" << endl;
    cout << "2. 删除学生" << endl;
    cout << "3. 修改学生信息" << endl;
    cout << "4. 查询学生" << endl;
    cout << "5. 显示所有学生" << endl;
    cout << "0. 返回上级菜单" << endl;
    cout << "请选择（0-5）：";
}

void StudentManagementSystem::showCourseInfoMenu() {
    cout << "\n========== 课程信息管理 ==========" << endl;
    cout << "1. 添加课程" << endl;
    cout << "2. 删除课程" << endl;
    cout << "3. 修改课程信息" << endl;
    cout << "4. 显示所有课程" << endl;
    cout << "0. 返回上级菜单" << endl;
    cout << "请选择（0-4）：";
}

void StudentManagementSystem::showSelectionMenu() {
    cout << "\n========== 选课管理 ==========" << endl;
    cout << "1. 添加选课记录" << endl;
    cout << "2. 删除选课记录" << endl;
    cout << "3. 查看学生选课" << endl;
    cout << "4. 查看课程选课情况" << endl;
    cout << "0. 返回上级菜单" << endl;
    cout << "请选择（0-4）：";
}

void StudentManagementSystem::showScoreMenu() {
    cout << "\n========== 成绩管理 ==========" << endl;
    cout << "1. 录入/更新成绩" << endl;
    cout << "2. 删除成绩" << endl;
    cout << "3. 查看学生成绩" << endl;
    cout << "0. 返回上级菜单" << endl;
    cout << "请选择（0-3）：";
}

void StudentManagementSystem::showQueryMenu() {
    cout << "\n========== 查询统计 ==========" << endl;
    cout << "1. 学生信息查询" << endl;
    cout << "2. 选课记录查询" << endl;
    cout << "3. 成绩查询" << endl;
    cout << "4. 系统统计信息" << endl;
    cout << "0. 返回上级菜单" << endl;
    cout << "请选择（0-4）：";
}

void StudentManagementSystem::showUserManageMenu() {
    cout << "\n========== 用户管理 ==========" << endl;
    cout << "1. 注册新用户" << endl;
    cout << "2. 删除用户" << endl;
    cout << "3. 查看所有用户" << endl;
    cout << "0. 返回上级菜单" << endl;
    cout << "请选择（0-3）：";
}

// ==================== 管理员功能 ====================

void StudentManagementSystem::adminStudentManagement() {
    while (true) {
        showStudentInfoMenu();
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: addStudentInfo(); break;
            case 2: deleteStudentInfo(); break;
            case 3: modifyStudentInfo(); break;
            case 4: searchStudentInfo(); break;
            case 5: displayAllStudentsInfo(); break;
            case 0: return;
            default: cout << "无效选择！" << endl;
        }
        
        if (choice != 0) Utils::pause();
    }
}

void StudentManagementSystem::adminCourseManagement() {
    while (true) {
        showCourseInfoMenu();
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: addCourseInfo(); break;
            case 2: deleteCourseInfo(); break;
            case 3: modifyCourseInfo(); break;
            case 4: displayAllCoursesInfo(); break;
            case 0: return;
            default: cout << "无效选择！" << endl;
        }
        
        if (choice != 0) Utils::pause();
    }
}

void StudentManagementSystem::adminSelectionManagement() {
    while (true) {
        showSelectionMenu();
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: addSelectionInfo(); break;
            case 2: deleteSelectionInfo(); break;
            case 3: viewStudentSelections(); break;
            case 4: viewCourseSelections(); break;
            case 0: return;
            default: cout << "无效选择！" << endl;
        }
        
        if (choice != 0) Utils::pause();
    }
}

void StudentManagementSystem::adminScoreManagement() {
    while (true) {
        showScoreMenu();
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: addScoreInfo(); break;
            case 2: deleteScoreInfo(); break;
            case 3: viewStudentScoresInfo(); break;
            case 0: return;
            default: cout << "无效选择！" << endl;
        }
        
        if (choice != 0) Utils::pause();
    }
}

void StudentManagementSystem::adminQueryStatistics() {
    while (true) {
        showQueryMenu();
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: queryStudent(); break;
            case 2: querySelection(); break;
            case 3: queryScore(); break;
            case 4: showStatistics(); break;
            case 0: return;
            default: cout << "无效选择！" << endl;
        }
        
        if (choice != 0) Utils::pause();
    }
}

void StudentManagementSystem::adminUserManagement() {
    while (true) {
        showUserManageMenu();
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: registerNewUser(); break;
            case 2: deleteUserInfo(); break;
            case 3: viewAllUsers(); break;
            case 0: return;
            default: cout << "无效选择！" << endl;
        }
        
        if (choice != 0) Utils::pause();
    }
}

// ==================== 学生功能 ====================

void StudentManagementSystem::studentViewInfo() {
    string studentID = userManager.getCurrentUser()->getRelatedID();
    Student* stu = studentManager.findStudent(studentID);
    
    if (stu != nullptr) {
        stu->displayDetail();
    } else {
        cout << "错误：未找到学生信息！" << endl;
    }
}

void StudentManagementSystem::studentSelectCourse() {
    cout << "\n========== 选课/退课 ==========" << endl;
    cout << "1. 选课" << endl;
    cout << "2. 退课" << endl;
    cout << "请选择：";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string studentID = userManager.getCurrentUser()->getRelatedID();
    
    if (choice == 1) {
        courseManager.displayAllCourses();
        cout << "\n请输入要选修的课程编号：";
        string courseID;
        cin >> courseID;
        
        if (courseManager.findCourse(courseID) == nullptr) {
            cout << "错误：课程不存在！" << endl;
            return;
        }
        
        selectionManager.addSelection(studentID, courseID);
    } else if (choice == 2) {
        cout << "请输入要退选的课程编号：";
        string courseID;
        cin >> courseID;
        
        selectionManager.deleteSelection(studentID, courseID);
    } else {
        cout << "无效选择！" << endl;
    }
}

void StudentManagementSystem::studentViewCourses() {
    string studentID = userManager.getCurrentUser()->getRelatedID();
    
    cout << "\n========== 我的选课记录 ==========" << endl;
    cout << "学号：" << studentID << endl;
    
    int count = 0;
    Course::displayHeader();
    
    selectionManager.getStudentCourses(studentID, [this, &count](const string& courseID) {
        Course* course = courseManager.findCourse(courseID);
        if (course != nullptr) {
            course->display();
            count++;
        }
    });
    
    if (count == 0) {
        cout << "暂无选课记录！" << endl;
    } else {
        cout << "共计选修 " << count << " 门课程。" << endl;
    }
}

void StudentManagementSystem::studentViewScores() {
    string studentID = userManager.getCurrentUser()->getRelatedID();
    Student* stu = studentManager.findStudent(studentID);
    
    if (stu == nullptr) {
        cout << "错误：学生信息不存在！" << endl;
        return;
    }
    
    cout << "\n========== 我的成绩单 ==========" << endl;
    cout << "学号：" << studentID << "  姓名：" << stu->getName() << endl;
    cout << left << setw(10) << "课程编号" << setw(22) << "课程名称"
         << setw(6) << "学分" << setw(8) << "成绩" << endl;
    cout << string(50, '-') << endl;
    
    int count = 0;
    double totalScore = 0;
    int totalCredits = 0;
    
    scoreManager.getStudentScores(studentID, [this, &count, &totalScore, &totalCredits]
                                  (const string& courseID, double score) {
        Course* course = courseManager.findCourse(courseID);
        if (course != nullptr) {
            cout << left << setw(10) << course->getCourseID()
                 << setw(22) << course->getCourseName()
                 << setw(6) << course->getCredits()
                 << setw(8) << fixed << setprecision(1) << score << endl;
            
            totalScore += score * course->getCredits();
            totalCredits += course->getCredits();
            count++;
        }
    });
    
    if (count == 0) {
        cout << "暂无成绩记录！" << endl;
    } else {
        cout << string(50, '-') << endl;
        cout << "已修课程数：" << count << "  总学分：" << totalCredits;
        if (totalCredits > 0) {
            cout << "  加权平均分：" << fixed << setprecision(2)
                 << totalScore / totalCredits << endl;
        } else {
            cout << endl;
        }
    }
}

void StudentManagementSystem::studentChangePassword() {
    cout << "\n========== 修改密码 ==========" << endl;
    cout << "请输入原密码：";
    string oldPassword;
    cin >> oldPassword;
    
    cout << "请输入新密码：";
    string newPassword;
    cin >> newPassword;
    
    cout << "请确认新密码：";
    string confirmPassword;
    cin >> confirmPassword;
    
    if (newPassword != confirmPassword) {
        cout << "错误：两次输入的密码不一致！" << endl;
        return;
    }
    
    userManager.changePassword(oldPassword, newPassword);
}

// ==================== 学生信息管理子功能 ====================

void StudentManagementSystem::addStudentInfo() {
    cout << "\n========== 添加学生信息 ==========" << endl;
    
    string id, name, gender, birth, politic, phone, addr;
    
    cout << "请输入学号（8位数字）：";
    cin >> id;
    id = Utils::trim(id);
    
    cout << "请输入姓名：";
    cin.ignore();
    getline(cin, name);
    name = Utils::trim(name);
    
    cout << "请输入性别（男/女）：";
    getline(cin, gender);
    gender = Utils::trim(gender);
    
    cout << "请输入出生日期（YYYY-MM-DD）：";
    getline(cin, birth);
    birth = Utils::trim(birth);
    
    cout << "请输入政治面貌：";
    getline(cin, politic);
    politic = Utils::trim(politic);
    
    cout << "请输入联系方式（11位手机号）：";
    getline(cin, phone);
    phone = Utils::trim(phone);
    
    cout << "请输入家庭住址：";
    getline(cin, addr);
    addr = Utils::trim(addr);
    
    Student stu(id, name, gender, birth, politic, phone, addr);
    studentManager.addStudent(stu);
}

void StudentManagementSystem::deleteStudentInfo() {
    cout << "\n请输入要删除的学号：";
    string id;
    cin >> id;
    
    if (studentManager.deleteStudent(id)) {
        // 同时删除相关选课和成绩记录
        selectionManager.deleteByStudent(id);
        scoreManager.deleteByStudent(id);
        cout << "已同时删除该学生的选课和成绩记录。" << endl;
    }
}

void StudentManagementSystem::modifyStudentInfo() {
    cout << "\n请输入要修改的学号：";
    string id;
    cin >> id;
    
    studentManager.modifyStudent(id);
}

void StudentManagementSystem::searchStudentInfo() {
    cout << "\n========== 学生查询 ==========" << endl;
    cout << "1. 按学号查询" << endl;
    cout << "2. 按姓名查询" << endl;
    cout << "3. 按性别查询" << endl;
    cout << "请选择查询方式：";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch (choice) {
        case 1: {
            cout << "请输入学号：";
            string id;
            cin >> id;
            Student* stu = studentManager.findStudent(id);
            if (stu != nullptr) {
                stu->displayDetail();
            } else {
                cout << "未找到该学生！" << endl;
            }
            break;
        }
        case 2: {
            cout << "请输入姓名：";
            string name;
            getline(cin, name);
            auto results = studentManager.searchByName(name);
            if (!results.empty()) {
                Student::displayHeader();
                for (const auto& stu : results) {
                    stu.display();
                }
            } else {
                cout << "未找到匹配的学生！" << endl;
            }
            break;
        }
        case 3: {
            cout << "请输入性别（男/女）：";
            string gender;
            getline(cin, gender);
            auto results = studentManager.searchByGender(gender);
            if (!results.empty()) {
                Student::displayHeader();
                for (const auto& stu : results) {
                    stu.display();
                }
            } else {
                cout << "未找到匹配的学生！" << endl;
            }
            break;
        }
        default:
            cout << "无效选择！" << endl;
    }
}

void StudentManagementSystem::displayAllStudentsInfo() {
    studentManager.displayAllStudents();
}

// ==================== 课程信息管理子功能 ====================

void StudentManagementSystem::addCourseInfo() {
    cout << "\n========== 添加课程信息 ==========" << endl;
    
    string id, name, sem, time, room;
    int cred;
    
    cout << "请输入课程编号（如CS101）：";
    cin >> id;
    cin.ignore();
    
    cout << "请输入课程名称：";
    getline(cin, name);
    
    cout << "请输入开课学期：";
    getline(cin, sem);
    
    cout << "请输入上课时间：";
    getline(cin, time);
    
    cout << "请输入上课地点：";
    getline(cin, room);
    
    cout << "请输入学分（1-8）：";
    cin >> cred;
    
    Course course(id, name, sem, time, room, cred);
    courseManager.addCourse(course);
}

void StudentManagementSystem::deleteCourseInfo() {
    cout << "\n请输入要删除的课程编号：";
    string id;
    cin >> id;
    
    if (courseManager.deleteCourse(id)) {
        // 同时删除相关选课和成绩记录
        selectionManager.deleteByCourse(id);
        scoreManager.deleteByCourse(id);
        cout << "已同时删除该课程的选课和成绩记录。" << endl;
    }
}

void StudentManagementSystem::modifyCourseInfo() {
    cout << "\n请输入要修改的课程编号：";
    string id;
    cin >> id;
    
    courseManager.modifyCourse(id);
}

void StudentManagementSystem::displayAllCoursesInfo() {
    courseManager.displayAllCourses();
}

// ==================== 选课管理子功能 ====================

void StudentManagementSystem::addSelectionInfo() {
    cout << "\n========== 添加选课记录 ==========" << endl;
    cout << "请输入学号：";
    string studentID;
    cin >> studentID;
    
    if (studentManager.findStudent(studentID) == nullptr) {
        cout << "错误：学生不存在！" << endl;
        return;
    }
    
    cout << "请输入课程编号：";
    string courseID;
    cin >> courseID;
    
    if (courseManager.findCourse(courseID) == nullptr) {
        cout << "错误：课程不存在！" << endl;
        return;
    }
    
    selectionManager.addSelection(studentID, courseID);
}

void StudentManagementSystem::deleteSelectionInfo() {
    cout << "\n========== 删除选课记录 ==========" << endl;
    cout << "请输入学号：";
    string studentID;
    cin >> studentID;
    
    cout << "请输入课程编号：";
    string courseID;
    cin >> courseID;
    
    selectionManager.deleteSelection(studentID, courseID);
}

void StudentManagementSystem::viewStudentSelections() {
    cout << "\n请输入学号：";
    string studentID;
    cin >> studentID;
    
    if (studentManager.findStudent(studentID) == nullptr) {
        cout << "错误：学生不存在！" << endl;
        return;
    }
    
    cout << "\n========== 学生选课记录 ==========" << endl;
    cout << "学号：" << studentID << endl;
    
    int count = 0;
    Course::displayHeader();
    
    selectionManager.getStudentCourses(studentID, [this, &count](const string& courseID) {
        Course* course = courseManager.findCourse(courseID);
        if (course != nullptr) {
            course->display();
            count++;
        }
    });
    
    if (count == 0) {
        cout << "该学生暂无选课记录！" << endl;
    } else {
        cout << "共计选修 " << count << " 门课程。" << endl;
    }
}

void StudentManagementSystem::viewCourseSelections() {
    cout << "\n请输入课程编号：";
    string courseID;
    cin >> courseID;
    
    if (courseManager.findCourse(courseID) == nullptr) {
        cout << "错误：课程不存在！" << endl;
        return;
    }
    
    cout << "\n========== 课程选课情况 ==========" << endl;
    cout << "课程编号：" << courseID << endl;
    
    int count = 0;
    Student::displayHeader();
    
    selectionManager.getCourseStudents(courseID, [this, &count](const string& studentID) {
        Student* stu = studentManager.findStudent(studentID);
        if (stu != nullptr) {
            stu->display();
            count++;
        }
    });
    
    if (count == 0) {
        cout << "该课程暂无学生选课！" << endl;
    } else {
        cout << "共计 " << count << " 名学生选修。" << endl;
    }
}

// ==================== 成绩管理子功能 ====================

void StudentManagementSystem::addScoreInfo() {
    cout << "\n========== 录入/更新成绩 ==========" << endl;
    cout << "请输入学号：";
    string studentID;
    cin >> studentID;
    
    if (studentManager.findStudent(studentID) == nullptr) {
        cout << "错误：学生不存在！" << endl;
        return;
    }
    
    cout << "请输入课程编号：";
    string courseID;
    cin >> courseID;
    
    if (courseManager.findCourse(courseID) == nullptr) {
        cout << "错误：课程不存在！" << endl;
        return;
    }
    
    if (!selectionManager.hasSelected(studentID, courseID)) {
        cout << "错误：学生未选择此课程！" << endl;
        return;
    }
    
    cout << "请输入成绩（0-100）：";
    double score;
    cin >> score;
    
    scoreManager.addOrUpdateScore(studentID, courseID, score);
}

void StudentManagementSystem::deleteScoreInfo() {
    cout << "\n========== 删除成绩 ==========" << endl;
    cout << "请输入学号：";
    string studentID;
    cin >> studentID;
    
    cout << "请输入课程编号：";
    string courseID;
    cin >> courseID;
    
    scoreManager.deleteScore(studentID, courseID);
}

void StudentManagementSystem::viewStudentScoresInfo() {
    cout << "\n请输入学号：";
    string studentID;
    cin >> studentID;
    
    Student* stu = studentManager.findStudent(studentID);
    if (stu == nullptr) {
        cout << "错误：学生不存在！" << endl;
        return;
    }
    
    cout << "\n========== 学生成绩单 ==========" << endl;
    cout << "学号：" << studentID << "  姓名：" << stu->getName() << endl;
    cout << left << setw(10) << "课程编号" << setw(22) << "课程名称"
         << setw(6) << "学分" << setw(8) << "成绩" << endl;
    cout << string(50, '-') << endl;
    
    int count = 0;
    double totalScore = 0;
    int totalCredits = 0;
    
    scoreManager.getStudentScores(studentID, [this, &count, &totalScore, &totalCredits]
                                  (const string& courseID, double score) {
        Course* course = courseManager.findCourse(courseID);
        if (course != nullptr) {
            cout << left << setw(10) << course->getCourseID()
                 << setw(22) << course->getCourseName()
                 << setw(6) << course->getCredits()
                 << setw(8) << fixed << setprecision(1) << score << endl;
            
            totalScore += score * course->getCredits();
            totalCredits += course->getCredits();
            count++;
        }
    });
    
    if (count == 0) {
        cout << "该学生暂无成绩记录！" << endl;
    } else {
        cout << string(50, '-') << endl;
        cout << "已修课程数：" << count << "  总学分：" << totalCredits;
        if (totalCredits > 0) {
            cout << "  加权平均分：" << fixed << setprecision(2)
                 << totalScore / totalCredits << endl;
        } else {
            cout << endl;
        }
    }
}

// ==================== 查询统计子功能 ====================

void StudentManagementSystem::queryStudent() {
    searchStudentInfo();
}

void StudentManagementSystem::querySelection() {
    viewStudentSelections();
}

void StudentManagementSystem::queryScore() {
    viewStudentScoresInfo();
}

void StudentManagementSystem::showStatistics() {
    cout << "\n========== 系统统计信息 ==========" << endl;
    cout << "学生总数：" << studentManager.getStudentCount() << endl;
    cout << "课程总数：" << courseManager.getCourseCount() << endl;
    cout << "选课记录数：" << selectionManager.getTotalSelections() << endl;
    cout << "成绩记录数：" << scoreManager.getTotalScores() << endl;
    cout << "用户总数：" << (studentManager.getStudentCount() + 1) << endl;  // +1为管理员
    cout << "================================" << endl;
}

// ==================== 用户管理子功能 ====================

void StudentManagementSystem::registerNewUser() {
    cout << "\n========== 注册新用户 ==========" << endl;
    
    string username, password, relatedID;
    int typeChoice;
    
    cout << "请输入用户名（3-20个字符）：";
    cin >> username;
    
    cout << "请输入密码（6-20个字符）：";
    cin >> password;
    
    cout << "请选择用户类型：\n1. 管理员\n2. 学生\n请选择：";
    cin >> typeChoice;
    
    UserType type = (typeChoice == 1) ? ADMIN : STUDENT;
    
    if (type == STUDENT) {
        cout << "请输入关联学号（8位数字）：";
        cin >> relatedID;
        
        if (studentManager.findStudent(relatedID) == nullptr) {
            cout << "警告：该学号不存在于学生信息中！" << endl;
            cout << "是否继续注册？(y/n)：";
            char confirm;
            cin >> confirm;
            if (confirm != 'y' && confirm != 'Y') {
                return;
            }
        }
    }
    
    userManager.registerUser(username, password, type, relatedID);
}

void StudentManagementSystem::deleteUserInfo() {
    cout << "\n请输入要删除的用户名：";
    string username;
    cin >> username;
    
    userManager.deleteUser(username);
}

void StudentManagementSystem::viewAllUsers() {
    userManager.displayAllUsers();
}
