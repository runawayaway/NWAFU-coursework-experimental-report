#include "../include/Course.h"
#include "../include/Utils.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <algorithm>

using namespace std;

// ==================== Course类实现 ====================

Course::Course() : courseID(""), courseName(""), semester(""),
                   classTime(""), classroom(""), credits(0) {}

Course::Course(string id, string name, string sem, string time,
              string room, int cred)
    : courseID(id), courseName(name), semester(sem),
      classTime(time), classroom(room), credits(cred) {}

void Course::display() const {
    cout << left << setw(10) << courseID << setw(22) << courseName
         << setw(12) << semester << setw(16) << classTime
         << setw(12) << classroom << setw(6) << credits << endl;
}

void Course::displayHeader() {
    
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    cout << left << setw(10) << "课程编号" << setw(22) << "课程名称"
         << setw(12) << "学期" << setw(16) << "上课时间"
         << setw(12) << "上课地点" << setw(6) << "学分" << endl;
    cout << string(80, '-') << endl;
}

// ==================== CourseManager类实现 ====================

CourseManager::CourseManager() {
    loadData();
}

CourseManager::~CourseManager() {
    saveData();
}

bool CourseManager::addCourse(const Course& course) {
    
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    // 验证课程编号
    if (!Utils::validateCourseID(course.getCourseID())) {
        cout << "错误：课程编号格式不正确（2-3个大写字母+3位数字，如CS101）！" << endl;
        return false;
    }
    
    // 检查课程编号是否已存在
    if (findCourse(course.getCourseID()) != nullptr) {
        cout << "错误：课程编号已存在！" << endl;
        return false;
    }
    
    // 验证学分
    if (!Utils::validateCredits(course.getCredits())) {
        cout << "错误：学分必须在1-8之间！" << endl;
        return false;
    }
    
    courses.push_back(course);
    saveData();
    cout << "课程信息添加成功！" << endl;
    return true;
}

bool CourseManager::deleteCourse(const string& courseID) {
        
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    
    auto it = find_if(courses.begin(), courses.end(),
        [&courseID](const Course& c) { return c.getCourseID() == courseID; });
    
    if (it != courses.end()) {
        courses.erase(it);
        saveData();
        cout << "课程信息删除成功！" << endl;
        return true;
    }
    
    cout << "错误：未找到该课程！" << endl;
    return false;
}

bool CourseManager::modifyCourse(const string& courseID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    Course* course = findCourse(courseID);
    if (course == nullptr) {
        cout << "错误：未找到该课程！" << endl;
        return false;
    }
    
    cout << "\n当前课程信息：" << endl;
    cout << "课程编号：" << course->getCourseID() << endl;
    cout << "课程名称：" << course->getCourseName() << endl;
    cout << "开课学期：" << course->getSemester() << endl;
    cout << "上课时间：" << course->getClassTime() << endl;
    cout << "上课地点：" << course->getClassroom() << endl;
    cout << "学分：" << course->getCredits() << endl;
    
    cout << "\n请选择要修改的项目：" << endl;
    cout << "1. 课程名称" << endl;
    cout << "2. 开课学期" << endl;
    cout << "3. 上课时间" << endl;
    cout << "4. 上课地点" << endl;
    cout << "5. 学分" << endl;
    cout << "请选择（1-5）：";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string newValue;
    
    switch (choice) {
        case 1:
            cout << "请输入新课程名称：";
            getline(cin, newValue);
            course->setCourseName(newValue);
            break;
        case 2:
            cout << "请输入新开课学期：";
            getline(cin, newValue);
            course->setSemester(newValue);
            break;
        case 3:
            cout << "请输入新上课时间：";
            getline(cin, newValue);
            course->setClassTime(newValue);
            break;
        case 4:
            cout << "请输入新上课地点：";
            getline(cin, newValue);
            course->setClassroom(newValue);
            break;
        case 5: {
            int newCredits;
            cout << "请输入新学分（1-8）：";
            cin >> newCredits;
            if (!Utils::validateCredits(newCredits)) {
                cout << "错误：学分必须在1-8之间！" << endl;
                return false;
            }
            course->setCredits(newCredits);
            break;
        }
        default:
            cout << "无效选择！" << endl;
            return false;
    }
    
    saveData();
    cout << "修改成功！" << endl;
    return true;
}

Course* CourseManager::findCourse(const string& courseID) {
    for (auto& course : courses) {
        if (course.getCourseID() == courseID) {
            return &course;
        }
    }
    return nullptr;
}

void CourseManager::displayAllCourses() const {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    if (courses.empty()) {
        cout << "\n暂无课程信息！" << endl;
        return;
    }
    
    cout << "\n========== 所有课程信息 ==========" << endl;
    Course::displayHeader();
    
    for (const auto& course : courses) {
        course.display();
    }
    cout << "共计 " << courses.size() << " 门课程。" << endl;
}

void CourseManager::saveData() {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    ofstream file(COURSE_FILE);
    if (!file.is_open()) {
        cerr << "警告：无法保存课程数据！" << endl;
        return;
    }
    
    for (const auto& course : courses) {
        file << course.getCourseID() << "|" << course.getCourseName() << "|"
             << course.getSemester() << "|" << course.getClassTime() << "|"
             << course.getClassroom() << "|" << course.getCredits() << endl;
    }
    
    file.close();
}

void CourseManager::loadData() {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    ifstream file(COURSE_FILE);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, sem, time, room;
        int cred;
        
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, sem, '|');
        getline(ss, time, '|');
        getline(ss, room, '|');
        ss >> cred;
        
        Course course(id, name, sem, time, room, cred);
        courses.push_back(course);
    }
    
    file.close();
}
