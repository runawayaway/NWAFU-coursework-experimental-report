#ifndef SELECTION_H
#define SELECTION_H

#include <string>
#include <iostream>
#include <functional>

using namespace std;

// 选课记录节点（链表）
struct SelectionNode {
    string studentID;
    string courseID;
    SelectionNode* next;
    
    SelectionNode(string sid, string cid) 
        : studentID(sid), courseID(cid), next(nullptr) {}
};

// 选课管理类
class SelectionManager {
private:
    SelectionNode* head;
    const string SELECTION_FILE = "data/selections.txt";
    
public:
    SelectionManager();
    ~SelectionManager();
    
    // 添加选课记录
    bool addSelection(const string& studentID, const string& courseID);
    
    // 删除选课记录
    bool deleteSelection(const string& studentID, const string& courseID);
    
    // 删除学生的所有选课记录
    void deleteByStudent(const string& studentID);
    
    // 删除某课程的所有选课记录
    void deleteByCourse(const string& courseID);
    
    // 检查是否已选课
    bool hasSelected(const string& studentID, const string& courseID) const;
    
    // 获取学生的选课列表
    void getStudentCourses(const string& studentID, 
                          function<void(const string&)> callback) const;
    
    // 获取课程的选课学生列表
    void getCourseStudents(const string& courseID,
                          function<void(const string&)> callback) const;
    
    // 统计学生选课数量
    int getStudentCourseCount(const string& studentID) const;
    
    // 统计课程选课人数
    int getCourseStudentCount(const string& courseID) const;
    
    // 获取总选课数
    int getTotalSelections() const;
    
    // 保存和加载数据
    void saveData();
    void loadData();
    
    // 清空链表
    void clear();
};

#endif
