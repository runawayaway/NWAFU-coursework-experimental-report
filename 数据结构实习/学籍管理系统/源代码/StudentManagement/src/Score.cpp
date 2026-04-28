#include "../include/Score.h"
#include "../include/Utils.h"
#include <fstream>
#include <sstream>
#include <windows.h>
using namespace std;

ScoreManager::ScoreManager() : head(nullptr) {
    loadData();
}

ScoreManager::~ScoreManager() {
    saveData();
    clear();
}

bool ScoreManager::addOrUpdateScore(const string& studentID, const string& courseID, double score) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    // 验证成绩范围
    if (!Utils::validateScore(score)) {
        cout << "错误：成绩必须在0-100之间！" << endl;
        return false;
    }
    
    // 检查是否已有成绩记录
    ScoreNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID && current->courseID == courseID) {
            current->score = score;
            saveData();
            cout << "成绩更新成功！" << endl;
            return true;
        }
        current = current->next;
    }
    
    // 插入新成绩到链表头部
    ScoreNode* newNode = new ScoreNode(studentID, courseID, score);
    newNode->next = head;
    head = newNode;
    
    saveData();
    cout << "成绩录入成功！" << endl;
    return true;
}

bool ScoreManager::deleteScore(const string& studentID, const string& courseID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    ScoreNode* current = head;
    ScoreNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->studentID == studentID && current->courseID == courseID) {
            if (prev == nullptr) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            saveData();
            cout << "成绩删除成功！" << endl;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    cout << "错误：未找到该成绩记录！" << endl;
    return false;
}

void ScoreManager::deleteByStudent(const string& studentID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    ScoreNode* current = head;
    ScoreNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->studentID == studentID) {
            ScoreNode* temp = current;
            if (prev == nullptr) {
                head = current->next;
                current = head;
            } else {
                prev->next = current->next;
                current = prev->next;
            }
            delete temp;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void ScoreManager::deleteByCourse(const string& courseID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    ScoreNode* current = head;
    ScoreNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->courseID == courseID) {
            ScoreNode* temp = current;
            if (prev == nullptr) {
                head = current->next;
                current = head;
            } else {
                prev->next = current->next;
                current = prev->next;
            }
            delete temp;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

double ScoreManager::getScore(const string& studentID, const string& courseID) const {
    ScoreNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID && current->courseID == courseID) {
            return current->score;
        }
        current = current->next;
    }
    return -1;
}

bool ScoreManager::hasScore(const string& studentID, const string& courseID) const {
    return getScore(studentID, courseID) >= 0;
}

void ScoreManager::getStudentScores(const string& studentID,
                                   function<void(const string&, double)> callback) const {
    ScoreNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID) {
            callback(current->courseID, current->score);
        }
        current = current->next;
    }
}

double ScoreManager::calculateGPA(const string& studentID,
                                 int (*getCreditFunc)(const string&)) const {
    double totalWeighted = 0;
    int totalCredits = 0;
    
    ScoreNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID) {
            int credits = getCreditFunc(current->courseID);
            totalWeighted += current->score * credits;
            totalCredits += credits;
        }
        current = current->next;
    }
    
    if (totalCredits == 0) {
        return 0;
    }
    
    return totalWeighted / totalCredits;
}

int ScoreManager::getStudentScoreCount(const string& studentID) const {
    int count = 0;
    ScoreNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID) {
            count++;
        }
        current = current->next;
    }
    return count;
}

int ScoreManager::getTotalScores() const {
    int count = 0;
    ScoreNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void ScoreManager::saveData() {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    ofstream file(SCORE_FILE);
    if (!file.is_open()) {
        cerr << "警告：无法保存成绩数据！" << endl;
        return;
    }
    
    ScoreNode* current = head;
    while (current != nullptr) {
        file << current->studentID << "|" << current->courseID << "|"
             << current->score << endl;
        current = current->next;
    }
    
    file.close();
}

void ScoreManager::loadData() {
    ifstream file(SCORE_FILE);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string studentID, courseID;
        double score;
        
        getline(ss, studentID, '|');
        getline(ss, courseID, '|');
        ss >> score;
        
        ScoreNode* newNode = new ScoreNode(studentID, courseID, score);
        newNode->next = head;
        head = newNode;
    }
    
    file.close();
}

void ScoreManager::clear() {
    while (head != nullptr) {
        ScoreNode* temp = head;
        head = head->next;
        delete temp;
    }
}
