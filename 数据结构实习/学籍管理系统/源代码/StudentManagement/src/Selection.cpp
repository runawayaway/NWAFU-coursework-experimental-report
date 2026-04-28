#include "../include/Selection.h"
#include <fstream>
#include <sstream>
#include <windows.h>
using namespace std;

SelectionManager::SelectionManager() : head(nullptr) {
    loadData();
}

SelectionManager::~SelectionManager() {
    saveData();
    clear();
}

bool SelectionManager::addSelection(const string& studentID, const string& courseID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    // 检查是否已选课
    if (hasSelected(studentID, courseID)) {
        cout << "错误：该学生已选择此课程！" << endl;
        return false;
    }
    
    // 插入到链表头部
    SelectionNode* newNode = new SelectionNode(studentID, courseID);
    newNode->next = head;
    head = newNode;
    
    saveData();
    cout << "选课成功！" << endl;
    return true;
}

bool SelectionManager::deleteSelection(const string& studentID, const string& courseID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    SelectionNode* current = head;
    SelectionNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->studentID == studentID && current->courseID == courseID) {
            if (prev == nullptr) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            saveData();
            cout << "退课成功！" << endl;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    cout << "错误：未找到该选课记录！" << endl;
    return false;
}

void SelectionManager::deleteByStudent(const string& studentID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    SelectionNode* current = head;
    SelectionNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->studentID == studentID) {
            SelectionNode* temp = current;
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

void SelectionManager::deleteByCourse(const string& courseID) {
    SelectionNode* current = head;
    SelectionNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->courseID == courseID) {
            SelectionNode* temp = current;
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

bool SelectionManager::hasSelected(const string& studentID, const string& courseID) const {
    SelectionNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID && current->courseID == courseID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void SelectionManager::getStudentCourses(const string& studentID,
                                        function<void(const string&)> callback) const {
    SelectionNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID) {
            callback(current->courseID);
        }
        current = current->next;
    }
}

void SelectionManager::getCourseStudents(const string& courseID,
                                        function<void(const string&)> callback) const {
    SelectionNode* current = head;
    while (current != nullptr) {
        if (current->courseID == courseID) {
            callback(current->studentID);
        }
        current = current->next;
    }
}

int SelectionManager::getStudentCourseCount(const string& studentID) const {
    int count = 0;
    SelectionNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID) {
            count++;
        }
        current = current->next;
    }
    return count;
}

int SelectionManager::getCourseStudentCount(const string& courseID) const {
    int count = 0;
    SelectionNode* current = head;
    while (current != nullptr) {
        if (current->courseID == courseID) {
            count++;
        }
        current = current->next;
    }
    return count;
}

int SelectionManager::getTotalSelections() const {
    int count = 0;
    SelectionNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void SelectionManager::saveData() {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    ofstream file(SELECTION_FILE);
    if (!file.is_open()) {
        cerr << "警告：无法保存选课数据！" << endl;
        return;
    }
    
    SelectionNode* current = head;
    while (current != nullptr) {
        file << current->studentID << "|" << current->courseID << endl;
        current = current->next;
    }
    
    file.close();
}

void SelectionManager::loadData() {
    ifstream file(SELECTION_FILE);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string studentID, courseID;
        
        getline(ss, studentID, '|');
        getline(ss, courseID);
        
        SelectionNode* newNode = new SelectionNode(studentID, courseID);
        newNode->next = head;
        head = newNode;
    }
    
    file.close();
}

void SelectionManager::clear() {
    while (head != nullptr) {
        SelectionNode* temp = head;
        head = head->next;
        delete temp;
    }
}
