#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

// 学生信息类
class Student {
private:
    string studentID;       // 学号（8位数字）
    string name;            // 姓名
    string gender;          // 性别（男/女）
    string birthDate;       // 出生日期（YYYY-MM-DD）
    string politicalStatus; // 政治面貌
    string phoneNumber;     // 联系方式（11位）
    string homeAddress;     // 家庭住址

public:
    Student();
    Student(string id, string n, string g, string birth, 
            string politic, string phone, string addr);
    
    // Getter方法
    string getStudentID() const { return studentID; }
    string getName() const { return name; }
    string getGender() const { return gender; }
    string getBirthDate() const { return birthDate; }
    string getPoliticalStatus() const { return politicalStatus; }
    string getPhoneNumber() const { return phoneNumber; }
    string getHomeAddress() const { return homeAddress; }
    
    // Setter方法
    void setName(const string& n) { name = n; }
    void setGender(const string& g) { gender = g; }
    void setBirthDate(const string& bd) { birthDate = bd; }
    void setPoliticalStatus(const string& ps) { politicalStatus = ps; }
    void setPhoneNumber(const string& pn) { phoneNumber = pn; }
    void setHomeAddress(const string& addr) { homeAddress = addr; }
    
    // 显示学生信息
    void display() const;
    void displayDetail() const;
    
    // 静态方法：显示表头
    static void displayHeader();
};

// AVL树节点（平衡二叉树）
struct AVLNode {
    Student student;
    AVLNode* left;
    AVLNode* right;
    int height;  // 节点高度
    
    AVLNode(const Student& stu) 
        : student(stu), left(nullptr), right(nullptr), height(1) {}
};

// 学生管理类
class StudentManager {
private:
    vector<Student> students;    // 学生顺序表
    AVLNode* avlRoot;           // AVL平衡树根节点
    const string STUDENT_FILE = "data/students.txt";
    
    // AVL树辅助函数
    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insertAVL(AVLNode* node, const Student& stu);
    Student* searchAVL(AVLNode* node, const string& id);
    AVLNode* deleteAVL(AVLNode* node, const string& id);
    AVLNode* findMin(AVLNode* node);
    void clearAVL(AVLNode* node);
    
public:
    StudentManager();
    ~StudentManager();
    
    // 添加学生
    bool addStudent(const Student& stu);
    
    // 删除学生
    bool deleteStudent(const string& studentID);
    
    // 修改学生信息
    bool modifyStudent(const string& studentID);
    
    // 查找学生（使用BST）
    Student* findStudent(const string& studentID);
    
    // 按条件查询学生
    vector<Student> searchByName(const string& name) const;
    vector<Student> searchByGender(const string& gender) const;
    
    // 显示所有学生
    void displayAllStudents() const;
    
    // 获取学生数量
    int getStudentCount() const { return students.size(); }
    
    // 保存和加载数据
    void saveData();
    void loadData();
};

#endif
