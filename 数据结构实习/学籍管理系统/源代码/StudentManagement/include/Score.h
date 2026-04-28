#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <iostream>
#include <functional>

using namespace std;

// 成绩记录节点（链表）
struct ScoreNode {
    string studentID;
    string courseID;
    double score;
    ScoreNode* next;
    
    ScoreNode(string sid, string cid, double s)
        : studentID(sid), courseID(cid), score(s), next(nullptr) {}
};

// 成绩管理类
class ScoreManager {
private:
    ScoreNode* head;
    const string SCORE_FILE = "data/scores.txt";
    
public:
    ScoreManager();
    ~ScoreManager();
    
    // 添加或更新成绩
    bool addOrUpdateScore(const string& studentID, const string& courseID, double score);
    
    // 删除成绩记录
    bool deleteScore(const string& studentID, const string& courseID);
    
    // 删除学生的所有成绩
    void deleteByStudent(const string& studentID);
    
    // 删除某课程的所有成绩
    void deleteByCourse(const string& courseID);
    
    // 获取成绩
    double getScore(const string& studentID, const string& courseID) const;
    
    // 检查是否有成绩记录
    bool hasScore(const string& studentID, const string& courseID) const;
    
    // 获取学生的成绩信息（使用回调函数）
    void getStudentScores(const string& studentID,
                         function<void(const string&, double)> callback) const;
    
    // 计算学生加权平均分
    double calculateGPA(const string& studentID, 
                       int (*getCreditFunc)(const string&)) const;
    
    // 统计学生成绩数量
    int getStudentScoreCount(const string& studentID) const;
    
    // 获取总成绩记录数
    int getTotalScores() const;
    
    // 保存和加载数据
    void saveData();
    void loadData();
    
    // 清空链表
    void clear();
};

#endif
