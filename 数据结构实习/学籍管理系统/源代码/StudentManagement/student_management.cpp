#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <windows.h>

using namespace std;

// ==================== 学生信息类（顺序表） ====================
class Student {
public:
    string studentID;      // 学号
    string name;           // 姓名
    string gender;         // 性别
    string birthDate;      // 出生日期
    string politicalStatus;// 政治面貌
    string phoneNumber;    // 联系方式
    string homeAddress;    // 家庭住址

    Student() {}
    
    Student(string id, string n, string g, string birth, string politic, string phone, string addr)
        : studentID(id), name(n), gender(g), birthDate(birth), 
          politicalStatus(politic), phoneNumber(phone), homeAddress(addr) {}

    void display() const {
        cout << left << setw(12) << studentID << setw(10) << name << setw(6) << gender
             << setw(12) << birthDate << setw(12) << politicalStatus
             << setw(14) << phoneNumber << setw(20) << homeAddress << endl;
    }

    void displayDetail() const {
        cout << "\n========== 学生详细信息 ==========" << endl;
        cout << "学号：" << studentID << endl;
        cout << "姓名：" << name << endl;
        cout << "性别：" << gender << endl;
        cout << "出生日期：" << birthDate << endl;
        cout << "政治面貌：" << politicalStatus << endl;
        cout << "联系方式：" << phoneNumber << endl;
        cout << "家庭住址：" << homeAddress << endl;
        cout << "================================\n" << endl;
    }
};

// ==================== 课程信息类 ====================
class Course {
public:
    string courseID;       // 课程编号
    string courseName;     // 课程名称
    string semester;       // 开课学期
    string classTime;      // 上课时间
    string classroom;      // 上课地点
    int credits;           // 学分

    Course() : credits(0) {}
    
    Course(string id, string name, string sem, string time, string room, int cred)
        : courseID(id), courseName(name), semester(sem), 
          classTime(time), classroom(room), credits(cred) {}

    void display() const {
        cout << left << setw(10) << courseID << setw(20) << courseName 
             << setw(10) << semester << setw(15) << classTime
             << setw(12) << classroom << setw(6) << credits << endl;
    }
};

// ==================== 选课记录节点（链表） ====================
struct CourseNode {
    string studentID;
    string courseID;
    CourseNode* next;

    CourseNode(string sid, string cid) : studentID(sid), courseID(cid), next(nullptr) {}
};

// ==================== 成绩记录节点（链表） ====================
struct ScoreNode {
    string studentID;
    string courseID;
    double score;
    ScoreNode* next;

    ScoreNode(string sid, string cid, double s) 
        : studentID(sid), courseID(cid), score(s), next(nullptr) {}
};

// ==================== 二叉排序树节点 ====================
struct BSTNode {
    Student student;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Student& stu) : student(stu), left(nullptr), right(nullptr) {}
};

// ==================== 学籍管理系统类 ====================
class StudentManagementSystem {
private:
    vector<Student> students;        // 学生顺序表
    vector<Course> courses;          // 课程顺序表
    CourseNode* courseListHead;      // 选课链表头
    ScoreNode* scoreListHead;        // 成绩链表头
    BSTNode* bstRoot;                // 二叉排序树根节点

    // 文件路径
    const string STUDENT_FILE = "students.txt";
    const string COURSE_FILE = "courses.txt";
    const string SELECTION_FILE = "selections.txt";
    const string SCORE_FILE = "scores.txt";

public:
    StudentManagementSystem() : courseListHead(nullptr), scoreListHead(nullptr), bstRoot(nullptr) {
        loadAllData();
    }

    ~StudentManagementSystem() {
        saveAllData();
        clearCourseList();
        clearScoreList();
        clearBST(bstRoot);
    }

    // ==================== 学生信息管理 ====================
    
    // 插入学生
    void addStudent() {
        Student stu;
        cout << "\n========== 添加学生信息 ==========" << endl;
        cout << "请输入学号：";
        cin >> stu.studentID;
        
        // 检查学号是否已存在
        if (findStudentByID(stu.studentID) != nullptr) {
            cout << "错误：学号已存在！" << endl;
            return;
        }

        cout << "请输入姓名：";
        cin >> stu.name;
        cout << "请输入性别：";
        cin >> stu.gender;
        cout << "请输入出生日期（如 2000-01-01）：";
        cin >> stu.birthDate;
        cout << "请输入政治面貌：";
        cin >> stu.politicalStatus;
        cout << "请输入联系方式：";
        cin >> stu.phoneNumber;
        cout << "请输入家庭住址：";
        cin.ignore();
        getline(cin, stu.homeAddress);

        students.push_back(stu);
        insertBST(bstRoot, stu);  // 同时插入二叉排序树
        cout << "学生信息添加成功！" << endl;
    }

    // 删除学生
    void deleteStudent() {
        string id;
        cout << "\n请输入要删除的学号：";
        cin >> id;

        auto it = find_if(students.begin(), students.end(),
            [&id](const Student& s) { return s.studentID == id; });

        if (it != students.end()) {
            students.erase(it);
            bstRoot = deleteBST(bstRoot, id);  // 从二叉排序树删除
            
            // 同时删除相关选课和成绩记录
            deleteCoursesByStudent(id);
            deleteScoresByStudent(id);
            
            cout << "学生信息删除成功！" << endl;
        } else {
            cout << "未找到该学生！" << endl;
        }
    }

    // 修改学生信息
    void modifyStudent() {
        string id;
        cout << "\n请输入要修改的学号：";
        cin >> id;

        Student* stu = findStudentByID(id);
        if (stu == nullptr) {
            cout << "未找到该学生！" << endl;
            return;
        }

        cout << "\n当前学生信息：" << endl;
        stu->displayDetail();

        cout << "请选择要修改的项目：" << endl;
        cout << "1. 姓名" << endl;
        cout << "2. 性别" << endl;
        cout << "3. 出生日期" << endl;
        cout << "4. 政治面貌" << endl;
        cout << "5. 联系方式" << endl;
        cout << "6. 家庭住址" << endl;
        cout << "请选择（1-6）：";
        
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "请输入新姓名：";
                getline(cin, stu->name);
                break;
            case 2:
                cout << "请输入新性别：";
                getline(cin, stu->gender);
                break;
            case 3:
                cout << "请输入新出生日期：";
                getline(cin, stu->birthDate);
                break;
            case 4:
                cout << "请输入新政治面貌：";
                getline(cin, stu->politicalStatus);
                break;
            case 5:
                cout << "请输入新联系方式：";
                getline(cin, stu->phoneNumber);
                break;
            case 6:
                cout << "请输入新家庭住址：";
                getline(cin, stu->homeAddress);
                break;
            default:
                cout << "无效选择！" << endl;
                return;
        }
        
        cout << "修改成功！" << endl;
    }

    // 查询学生
    void searchStudent() {
        cout << "\n========== 学生查询 ==========" << endl;
        cout << "1. 按学号查询" << endl;
        cout << "2. 按姓名查询" << endl;
        cout << "3. 按性别查询" << endl;
        cout << "请选择查询方式（1-3）：";
        
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string id;
                cout << "请输入学号：";
                cin >> id;
                Student* stu = searchBST(bstRoot, id);  // 使用二叉排序树查找
                if (stu != nullptr) {
                    stu->displayDetail();
                } else {
                    cout << "未找到该学生！" << endl;
                }
                break;
            }
            case 2: {
                string name;
                cout << "请输入姓名：";
                getline(cin, name);
                bool found = false;
                for (const auto& stu : students) {
                    if (stu.name == name) {
                        stu.displayDetail();
                        found = true;
                    }
                }
                if (!found) {
                    cout << "未找到该学生！" << endl;
                }
                break;
            }
            case 3: {
                string gender;
                cout << "请输入性别：";
                getline(cin, gender);
                bool found = false;
                displayStudentHeader();
                for (const auto& stu : students) {
                    if (stu.gender == gender) {
                        stu.display();
                        found = true;
                    }
                }
                if (!found) {
                    cout << "未找到符合条件的学生！" << endl;
                }
                break;
            }
            default:
                cout << "无效选择！" << endl;
        }
    }

    // 输出所有学生
    void displayAllStudents() {
        if (students.empty()) {
            cout << "\n暂无学生信息！" << endl;
            return;
        }

        cout << "\n========== 所有学生信息 ==========" << endl;
        displayStudentHeader();
        for (const auto& stu : students) {
            stu.display();
        }
        cout << "共计 " << students.size() << " 名学生。" << endl;
    }

    // ==================== 课程信息管理 ====================
    
    void addCourse() {
        Course course;
        cout << "\n========== 添加课程信息 ==========" << endl;
        cout << "请输入课程编号：";
        cin >> course.courseID;
        
        if (findCourseByID(course.courseID) != nullptr) {
            cout << "错误：课程编号已存在！" << endl;
            return;
        }

        cout << "请输入课程名称：";
        cin.ignore();
        getline(cin, course.courseName);
        cout << "请输入开课学期（如 2024春季）：";
        getline(cin, course.semester);
        cout << "请输入上课时间（如 周一1-2节）：";
        getline(cin, course.classTime);
        cout << "请输入上课地点：";
        getline(cin, course.classroom);
        cout << "请输入学分：";
        cin >> course.credits;

        courses.push_back(course);
        cout << "课程信息添加成功！" << endl;
    }

    void displayAllCourses() {
        if (courses.empty()) {
            cout << "\n暂无课程信息！" << endl;
            return;
        }

        cout << "\n========== 所有课程信息 ==========" << endl;
        cout << left << setw(10) << "课程编号" << setw(20) << "课程名称"
             << setw(10) << "学期" << setw(15) << "上课时间"
             << setw(12) << "上课地点" << setw(6) << "学分" << endl;
        cout << string(80, '-') << endl;
        
        for (const auto& course : courses) {
            course.display();
        }
        cout << "共计 " << courses.size() << " 门课程。" << endl;
    }

    // ==================== 选课管理（链表） ====================
    
    void selectCourse() {
        string studentID, courseID;
        cout << "\n========== 学生选课 ==========" << endl;
        cout << "请输入学号：";
        cin >> studentID;

        if (findStudentByID(studentID) == nullptr) {
            cout << "错误：学生不存在！" << endl;
            return;
        }

        cout << "请输入课程编号：";
        cin >> courseID;

        if (findCourseByID(courseID) == nullptr) {
            cout << "错误：课程不存在！" << endl;
            return;
        }

        // 检查是否已选课
        if (hasSelectedCourse(studentID, courseID)) {
            cout << "该学生已选择此课程！" << endl;
            return;
        }

        // 插入到链表头部
        CourseNode* newNode = new CourseNode(studentID, courseID);
        newNode->next = courseListHead;
        courseListHead = newNode;

        cout << "选课成功！" << endl;
    }

    void displayStudentCourses() {
        string studentID;
        cout << "\n请输入学号：";
        cin >> studentID;

        if (findStudentByID(studentID) == nullptr) {
            cout << "错误：学生不存在！" << endl;
            return;
        }

        cout << "\n========== 学生选课记录 ==========" << endl;
        cout << "学号：" << studentID << endl;
        
        CourseNode* current = courseListHead;
        int count = 0;
        
        cout << left << setw(10) << "课程编号" << setw(20) << "课程名称"
             << setw(10) << "学期" << setw(15) << "上课时间"
             << setw(12) << "上课地点" << setw(6) << "学分" << endl;
        cout << string(80, '-') << endl;

        while (current != nullptr) {
            if (current->studentID == studentID) {
                Course* course = findCourseByID(current->courseID);
                if (course != nullptr) {
                    course->display();
                    count++;
                }
            }
            current = current->next;
        }

        if (count == 0) {
            cout << "该学生暂无选课记录！" << endl;
        } else {
            cout << "共计选修 " << count << " 门课程。" << endl;
        }
    }

    // ==================== 成绩管理（链表） ====================
    
    void addScore() {
        string studentID, courseID;
        double score;
        
        cout << "\n========== 录入成绩 ==========" << endl;
        cout << "请输入学号：";
        cin >> studentID;

        if (findStudentByID(studentID) == nullptr) {
            cout << "错误：学生不存在！" << endl;
            return;
        }

        cout << "请输入课程编号：";
        cin >> courseID;

        if (findCourseByID(courseID) == nullptr) {
            cout << "错误：课程不存在！" << endl;
            return;
        }

        if (!hasSelectedCourse(studentID, courseID)) {
            cout << "错误：学生未选择此课程！" << endl;
            return;
        }

        cout << "请输入成绩（0-100）：";
        cin >> score;

        if (score < 0 || score > 100) {
            cout << "错误：成绩必须在0-100之间！" << endl;
            return;
        }

        // 检查是否已有成绩
        ScoreNode* current = scoreListHead;
        while (current != nullptr) {
            if (current->studentID == studentID && current->courseID == courseID) {
                current->score = score;
                cout << "成绩更新成功！" << endl;
                return;
            }
            current = current->next;
        }

        // 插入新成绩到链表头部
        ScoreNode* newNode = new ScoreNode(studentID, courseID, score);
        newNode->next = scoreListHead;
        scoreListHead = newNode;

        cout << "成绩录入成功！" << endl;
    }

    void displayStudentScores() {
        string studentID;
        cout << "\n请输入学号：";
        cin >> studentID;

        Student* stu = findStudentByID(studentID);
        if (stu == nullptr) {
            cout << "错误：学生不存在！" << endl;
            return;
        }

        cout << "\n========== 学生成绩单 ==========" << endl;
        cout << "学号：" << studentID << "  姓名：" << stu->name << endl;
        cout << left << setw(10) << "课程编号" << setw(20) << "课程名称"
             << setw(6) << "学分" << setw(8) << "成绩" << endl;
        cout << string(50, '-') << endl;

        ScoreNode* current = scoreListHead;
        int count = 0;
        double totalScore = 0;
        int totalCredits = 0;

        while (current != nullptr) {
            if (current->studentID == studentID) {
                Course* course = findCourseByID(current->courseID);
                if (course != nullptr) {
                    cout << left << setw(10) << course->courseID
                         << setw(20) << course->courseName
                         << setw(6) << course->credits
                         << setw(8) << fixed << setprecision(1) << current->score << endl;
                    
                    totalScore += current->score * course->credits;
                    totalCredits += course->credits;
                    count++;
                }
            }
            current = current->next;
        }

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

    void modifyScore() {
        string studentID, courseID;
        double newScore;
        
        cout << "\n========== 修改成绩 ==========" << endl;
        cout << "请输入学号：";
        cin >> studentID;
        cout << "请输入课程编号：";
        cin >> courseID;

        ScoreNode* current = scoreListHead;
        while (current != nullptr) {
            if (current->studentID == studentID && current->courseID == courseID) {
                cout << "当前成绩：" << current->score << endl;
                cout << "请输入新成绩（0-100）：";
                cin >> newScore;
                
                if (newScore < 0 || newScore > 100) {
                    cout << "错误：成绩必须在0-100之间！" << endl;
                    return;
                }
                
                current->score = newScore;
                cout << "成绩修改成功！" << endl;
                return;
            }
            current = current->next;
        }

        cout << "未找到该成绩记录！" << endl;
    }

    void deleteScore() {
        string studentID, courseID;
        
        cout << "\n========== 删除成绩 ==========" << endl;
        cout << "请输入学号：";
        cin >> studentID;
        cout << "请输入课程编号：";
        cin >> courseID;

        ScoreNode* current = scoreListHead;
        ScoreNode* prev = nullptr;

        while (current != nullptr) {
            if (current->studentID == studentID && current->courseID == courseID) {
                if (prev == nullptr) {
                    scoreListHead = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                cout << "成绩删除成功！" << endl;
                return;
            }
            prev = current;
            current = current->next;
        }

        cout << "未找到该成绩记录！" << endl;
    }

    // ==================== 统计分析 ====================
    
    void statistics() {
        cout << "\n========== 系统统计信息 ==========" << endl;
        cout << "学生总数：" << students.size() << endl;
        cout << "课程总数：" << courses.size() << endl;
        
        int selectionCount = 0;
        CourseNode* cn = courseListHead;
        while (cn != nullptr) {
            selectionCount++;
            cn = cn->next;
        }
        cout << "选课记录数：" << selectionCount << endl;
        
        int scoreCount = 0;
        ScoreNode* sn = scoreListHead;
        while (sn != nullptr) {
            scoreCount++;
            sn = sn->next;
        }
        cout << "成绩记录数：" << scoreCount << endl;
        cout << "================================" << endl;
    }

    // ==================== 文件操作 ====================
    
    void saveAllData() {
        saveStudents();
        saveCourses();
        saveSelections();
        saveScores();
    }

    void loadAllData() {
        loadStudents();
        loadCourses();
        loadSelections();
        loadScores();
    }

private:
    // ==================== 辅助函数 ====================
    
    Student* findStudentByID(const string& id) {
        for (auto& stu : students) {
            if (stu.studentID == id) {
                return &stu;
            }
        }
        return nullptr;
    }

    Course* findCourseByID(const string& id) {
        for (auto& course : courses) {
            if (course.courseID == id) {
                return &course;
            }
        }
        return nullptr;
    }

    bool hasSelectedCourse(const string& studentID, const string& courseID) {
        CourseNode* current = courseListHead;
        while (current != nullptr) {
            if (current->studentID == studentID && current->courseID == courseID) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void displayStudentHeader() {
        cout << left << setw(12) << "学号" << setw(10) << "姓名" << setw(6) << "性别"
             << setw(12) << "出生日期" << setw(12) << "政治面貌"
             << setw(14) << "联系方式" << setw(20) << "家庭住址" << endl;
        cout << string(90, '-') << endl;
    }

    // ==================== 二叉排序树操作 ====================
    
    void insertBST(BSTNode*& node, const Student& stu) {
        if (node == nullptr) {
            node = new BSTNode(stu);
        } else if (stu.studentID < node->student.studentID) {
            insertBST(node->left, stu);
        } else if (stu.studentID > node->student.studentID) {
            insertBST(node->right, stu);
        }
    }

    Student* searchBST(BSTNode* node, const string& id) {
        if (node == nullptr) {
            return nullptr;
        }
        if (id == node->student.studentID) {
            return &(node->student);
        } else if (id < node->student.studentID) {
            return searchBST(node->left, id);
        } else {
            return searchBST(node->right, id);
        }
    }

    BSTNode* deleteBST(BSTNode* node, const string& id) {
        if (node == nullptr) return nullptr;
        
        if (id < node->student.studentID) {
            node->left = deleteBST(node->left, id);
        } else if (id > node->student.studentID) {
            node->right = deleteBST(node->right, id);
        } else {
            if (node->left == nullptr) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            
            BSTNode* temp = findMin(node->right);
            node->student = temp->student;
            node->right = deleteBST(node->right, temp->student.studentID);
        }
        return node;
    }

    BSTNode* findMin(BSTNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void clearBST(BSTNode* node) {
        if (node != nullptr) {
            clearBST(node->left);
            clearBST(node->right);
            delete node;
        }
    }

    void deleteCoursesByStudent(const string& studentID) {
        CourseNode* current = courseListHead;
        CourseNode* prev = nullptr;

        while (current != nullptr) {
            if (current->studentID == studentID) {
                CourseNode* temp = current;
                if (prev == nullptr) {
                    courseListHead = current->next;
                    current = courseListHead;
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

    void deleteScoresByStudent(const string& studentID) {
        ScoreNode* current = scoreListHead;
        ScoreNode* prev = nullptr;

        while (current != nullptr) {
            if (current->studentID == studentID) {
                ScoreNode* temp = current;
                if (prev == nullptr) {
                    scoreListHead = current->next;
                    current = scoreListHead;
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

    void clearCourseList() {
        while (courseListHead != nullptr) {
            CourseNode* temp = courseListHead;
            courseListHead = courseListHead->next;
            delete temp;
        }
    }

    void clearScoreList() {
        while (scoreListHead != nullptr) {
            ScoreNode* temp = scoreListHead;
            scoreListHead = scoreListHead->next;
            delete temp;
        }
    }

    // ==================== 文件读写 ====================
    
    void saveStudents() {
        ofstream file(STUDENT_FILE);
        if (!file.is_open()) return;

        for (const auto& stu : students) {
            file << stu.studentID << "|" << stu.name << "|" << stu.gender << "|"
                 << stu.birthDate << "|" << stu.politicalStatus << "|"
                 << stu.phoneNumber << "|" << stu.homeAddress << endl;
        }
        file.close();
    }

    void loadStudents() {
        ifstream file(STUDENT_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            Student stu;
            size_t pos = 0;
            size_t nextPos;
            
            nextPos = line.find('|', pos);
            stu.studentID = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            stu.name = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            stu.gender = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            stu.birthDate = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            stu.politicalStatus = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            stu.phoneNumber = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            stu.homeAddress = line.substr(pos);

            students.push_back(stu);
            insertBST(bstRoot, stu);
        }
        file.close();
    }

    void saveCourses() {
        ofstream file(COURSE_FILE);
        if (!file.is_open()) return;

        for (const auto& course : courses) {
            file << course.courseID << "|" << course.courseName << "|"
                 << course.semester << "|" << course.classTime << "|"
                 << course.classroom << "|" << course.credits << endl;
        }
        file.close();
    }

    void loadCourses() {
        ifstream file(COURSE_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            Course course;
            size_t pos = 0;
            size_t nextPos;
            
            nextPos = line.find('|', pos);
            course.courseID = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            course.courseName = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            course.semester = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            course.classTime = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            nextPos = line.find('|', pos);
            course.classroom = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            course.credits = stoi(line.substr(pos));

            courses.push_back(course);
        }
        file.close();
    }

    void saveSelections() {
        ofstream file(SELECTION_FILE);
        if (!file.is_open()) return;

        CourseNode* current = courseListHead;
        while (current != nullptr) {
            file << current->studentID << "|" << current->courseID << endl;
            current = current->next;
        }
        file.close();
    }

    void loadSelections() {
        ifstream file(SELECTION_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            size_t pos = line.find('|');
            if (pos != string::npos) {
                string studentID = line.substr(0, pos);
                string courseID = line.substr(pos + 1);
                
                CourseNode* newNode = new CourseNode(studentID, courseID);
                newNode->next = courseListHead;
                courseListHead = newNode;
            }
        }
        file.close();
    }

    void saveScores() {
        ofstream file(SCORE_FILE);
        if (!file.is_open()) return;

        ScoreNode* current = scoreListHead;
        while (current != nullptr) {
            file << current->studentID << "|" << current->courseID << "|"
                 << current->score << endl;
            current = current->next;
        }
        file.close();
    }

    void loadScores() {
        ifstream file(SCORE_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            
            if (pos1 != string::npos && pos2 != string::npos) {
                string studentID = line.substr(0, pos1);
                string courseID = line.substr(pos1 + 1, pos2 - pos1 - 1);
                double score = stod(line.substr(pos2 + 1));
                
                ScoreNode* newNode = new ScoreNode(studentID, courseID, score);
                newNode->next = scoreListHead;
                scoreListHead = newNode;
            }
        }
        file.close();
    }
};

// ==================== 主菜单系统 ====================

void displayMainMenu() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════╗" << endl;
    cout << "║       学籍管理系统 - 主菜单               ║" << endl;
    cout << "╠═══════════════════════════════════════════╣" << endl;
    cout << "║  1. 学生信息管理                          ║" << endl;
    cout << "║  2. 课程信息管理                          ║" << endl;
    cout << "║  3. 选课管理                              ║" << endl;
    cout << "║  4. 成绩管理                              ║" << endl;
    cout << "║  5. 查询统计                              ║" << endl;
    cout << "║  0. 退出系统                              ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;
    cout << "请选择功能（0-5）：";
}

void displayStudentMenu() {
    cout << "\n========== 学生信息管理 ==========" << endl;
    cout << "1. 添加学生" << endl;
    cout << "2. 删除学生" << endl;
    cout << "3. 修改学生信息" << endl;
    cout << "4. 查询学生" << endl;
    cout << "5. 显示所有学生" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "请选择（0-5）：";
}

void displayCourseMenu() {
    cout << "\n========== 课程信息管理 ==========" << endl;
    cout << "1. 添加课程" << endl;
    cout << "2. 显示所有课程" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "请选择（0-2）：";
}

void displaySelectionMenu() {
    cout << "\n========== 选课管理 ==========" << endl;
    cout << "1. 学生选课" << endl;
    cout << "2. 查看学生选课记录" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "请选择（0-2）：";
}

void displayScoreMenu() {
    cout << "\n========== 成绩管理 ==========" << endl;
    cout << "1. 录入成绩" << endl;
    cout << "2. 修改成绩" << endl;
    cout << "3. 删除成绩" << endl;
    cout << "4. 查看学生成绩" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "请选择（0-4）：";
}

void displayQueryMenu() {
    cout << "\n========== 查询统计 ==========" << endl;
    cout << "1. 学生信息查询" << endl;
    cout << "2. 学生选课查询" << endl;
    cout << "3. 学生成绩查询" << endl;
    cout << "4. 系统统计信息" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "请选择（0-4）：";
}

int main() {
    // 设置控制台为UTF-8编码
    SetConsoleOutputCP(65001);
    
    StudentManagementSystem sms;
    int choice;

    cout << "\n欢迎使用学籍管理系统！" << endl;

    while (true) {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1: {  // 学生信息管理
                int stuChoice;
                while (true) {
                    displayStudentMenu();
                    cin >> stuChoice;
                    
                    switch (stuChoice) {
                        case 1:
                            sms.addStudent();
                            break;
                        case 2:
                            sms.deleteStudent();
                            break;
                        case 3:
                            sms.modifyStudent();
                            break;
                        case 4:
                            sms.searchStudent();
                            break;
                        case 5:
                            sms.displayAllStudents();
                            break;
                        case 0:
                            break;
                        default:
                            cout << "无效选择，请重新输入！" << endl;
                    }
                    if (stuChoice == 0) break;
                }
                break;
            }
            case 2: {  // 课程信息管理
                int courseChoice;
                while (true) {
                    displayCourseMenu();
                    cin >> courseChoice;
                    
                    switch (courseChoice) {
                        case 1:
                            sms.addCourse();
                            break;
                        case 2:
                            sms.displayAllCourses();
                            break;
                        case 0:
                            break;
                        default:
                            cout << "无效选择，请重新输入！" << endl;
                    }
                    if (courseChoice == 0) break;
                }
                break;
            }
            case 3: {  // 选课管理
                int selChoice;
                while (true) {
                    displaySelectionMenu();
                    cin >> selChoice;
                    
                    switch (selChoice) {
                        case 1:
                            sms.selectCourse();
                            break;
                        case 2:
                            sms.displayStudentCourses();
                            break;
                        case 0:
                            break;
                        default:
                            cout << "无效选择，请重新输入！" << endl;
                    }
                    if (selChoice == 0) break;
                }
                break;
            }
            case 4: {  // 成绩管理
                int scoreChoice;
                while (true) {
                    displayScoreMenu();
                    cin >> scoreChoice;
                    
                    switch (scoreChoice) {
                        case 1:
                            sms.addScore();
                            break;
                        case 2:
                            sms.modifyScore();
                            break;
                        case 3:
                            sms.deleteScore();
                            break;
                        case 4:
                            sms.displayStudentScores();
                            break;
                        case 0:
                            break;
                        default:
                            cout << "无效选择，请重新输入！" << endl;
                    }
                    if (scoreChoice == 0) break;
                }
                break;
            }
            case 5: {  // 查询统计
                int queryChoice;
                while (true) {
                    displayQueryMenu();
                    cin >> queryChoice;
                    
                    switch (queryChoice) {
                        case 1:
                            sms.searchStudent();
                            break;
                        case 2:
                            sms.displayStudentCourses();
                            break;
                        case 3:
                            sms.displayStudentScores();
                            break;
                        case 4:
                            sms.statistics();
                            break;
                        case 0:
                            break;
                        default:
                            cout << "无效选择，请重新输入！" << endl;
                    }
                    if (queryChoice == 0) break;
                }
                break;
            }
            case 0:
                cout << "\n感谢使用学籍管理系统，再见！" << endl;
                return 0;
            default:
                cout << "无效选择，请重新输入！" << endl;
        }
    }

    return 0;
}
