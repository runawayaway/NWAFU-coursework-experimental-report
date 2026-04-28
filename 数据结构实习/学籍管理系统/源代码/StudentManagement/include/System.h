#ifndef SYSTEM_H
#define SYSTEM_H

#include "User.h"
#include "Student.h"
#include "Course.h"
#include "Selection.h"
#include "Score.h"

// 学籍管理系统主类
class StudentManagementSystem {
private:
    UserManager userManager;
    StudentManager studentManager;
    CourseManager courseManager;
    SelectionManager selectionManager;
    ScoreManager scoreManager;
    
    // 菜单显示函数
    void showMainMenu();
    void showAdminMenu();
    void showStudentMenu();
    void showStudentInfoMenu();
    void showCourseInfoMenu();
    void showSelectionMenu();
    void showScoreMenu();
    void showQueryMenu();
    void showUserManageMenu();
    
    // 管理员功能
    void adminStudentManagement();
    void adminCourseManagement();
    void adminSelectionManagement();
    void adminScoreManagement();
    void adminQueryStatistics();
    void adminUserManagement();
    
    // 学生功能
    void studentViewInfo();
    void studentSelectCourse();
    void studentViewCourses();
    void studentViewScores();
    void studentChangePassword();
    
    // 学生信息管理子功能
    void addStudentInfo();
    void deleteStudentInfo();
    void modifyStudentInfo();
    void searchStudentInfo();
    void displayAllStudentsInfo();
    
    // 课程信息管理子功能
    void addCourseInfo();
    void deleteCourseInfo();
    void modifyCourseInfo();
    void displayAllCoursesInfo();
    
    // 选课管理子功能
    void addSelectionInfo();
    void deleteSelectionInfo();
    void viewStudentSelections();
    void viewCourseSelections();
    
    // 成绩管理子功能
    void addScoreInfo();
    void modifyScoreInfo();
    void deleteScoreInfo();
    void viewStudentScoresInfo();
    
    // 查询统计子功能
    void queryStudent();
    void querySelection();
    void queryScore();
    void showStatistics();
    
    // 用户管理子功能
    void registerNewUser();
    void deleteUserInfo();
    void viewAllUsers();
    
public:
    StudentManagementSystem();
    ~StudentManagementSystem();
    
    // 系统运行
    void run();
    
    // 用户登录
    bool loginSystem();
};

#endif
