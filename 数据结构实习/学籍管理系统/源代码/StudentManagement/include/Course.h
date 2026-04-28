#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

// 课程信息类
class Course {
private:
    string courseID;        // 课程编号
    string courseName;      // 课程名称
    string semester;        // 开课学期
    string classTime;       // 上课时间
    string classroom;       // 上课地点
    int credits;            // 学分（1-8）

public:
    Course();
    Course(string id, string name, string sem, string time, 
           string room, int cred);
    
    // Getter方法
    string getCourseID() const { return courseID; }
    string getCourseName() const { return courseName; }
    string getSemester() const { return semester; }
    string getClassTime() const { return classTime; }
    string getClassroom() const { return classroom; }
    int getCredits() const { return credits; }
    
    // Setter方法
    void setCourseName(const string& name) { courseName = name; }
    void setSemester(const string& sem) { semester = sem; }
    void setClassTime(const string& time) { classTime = time; }
    void setClassroom(const string& room) { classroom = room; }
    void setCredits(int cred) { credits = cred; }
    
    // 显示课程信息
    void display() const;
    
    // 静态方法：显示表头
    static void displayHeader();
};

// 课程管理类
class CourseManager {
private:
    vector<Course> courses;
    const string COURSE_FILE = "data/courses.txt";
    
public:
    CourseManager();
    ~CourseManager();
    
    // 添加课程
    bool addCourse(const Course& course);
    
    // 删除课程
    bool deleteCourse(const string& courseID);
    
    // 修改课程信息
    bool modifyCourse(const string& courseID);
    
    // 查找课程
    Course* findCourse(const string& courseID);
    
    // 显示所有课程
    void displayAllCourses() const;
    
    // 获取课程数量
    int getCourseCount() const { return courses.size(); }
    
    // 保存和加载数据
    void saveData();
    void loadData();
};

#endif
