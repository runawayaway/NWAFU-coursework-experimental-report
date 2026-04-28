#include "../include/Student.h"
#include "../include/Utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windows.h>
using namespace std;

// ==================== Student类实现 ====================

Student::Student() : studentID(""), name(""), gender(""), birthDate(""),
                     politicalStatus(""), phoneNumber(""), homeAddress("") {}

Student::Student(string id, string n, string g, string birth,
                string politic, string phone, string addr)
    : studentID(id), name(n), gender(g), birthDate(birth),
      politicalStatus(politic), phoneNumber(phone), homeAddress(addr) {}

void Student::display() const {
    cout << left << setw(12) << studentID << setw(10) << name << setw(6) << gender
         << setw(12) << birthDate << setw(12) << politicalStatus
         << setw(14) << phoneNumber << setw(25) << homeAddress << endl;
}

void Student::displayDetail() const {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
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

void Student::displayHeader() {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    cout << left << setw(12) << "学号" << setw(10) << "姓名" << setw(6) << "性别"
         << setw(12) << "出生日期" << setw(12) << "政治面貌"
         << setw(14) << "联系方式" << setw(25) << "家庭住址" << endl;
    cout << string(95, '-') << endl;
}

// ==================== StudentManager类实现 ====================

StudentManager::StudentManager() : avlRoot(nullptr) {
    loadData();
}

StudentManager::~StudentManager() {
    saveData();
    clearAVL(avlRoot);
}

bool StudentManager::addStudent(const Student& stu) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    // 验证学号
    if (!Utils::validateStudentID(stu.getStudentID())) {
        cout << "错误：学号格式不正确（必须是8位数字）！" << endl;
        return false;
    }
    
    // 检查学号是否已存在
    if (findStudent(stu.getStudentID()) != nullptr) {
        cout << "错误：学号已存在！" << endl;
        return false;
    }
    
    // 验证性别
    if (!Utils::validateGender(stu.getGender())) {
        cout << "错误：性别必须是'男'或'女'！" << endl;
        return false;
    }
    
    // 验证出生日期
    if (!Utils::validateDate(stu.getBirthDate())) {
        cout << "错误：出生日期格式不正确（YYYY-MM-DD）或日期不合理！" << endl;
        return false;
    }
    
    // 验证电话号码
    if (!Utils::validatePhoneNumber(stu.getPhoneNumber())) {
        cout << "错误：电话号码格式不正确（11位手机号）！" << endl;
        return false;
    }
    
    students.push_back(stu);
    avlRoot = insertAVL(avlRoot, stu);
    saveData();
    cout << "学生信息添加成功！" << endl;
    return true;
}

bool StudentManager::deleteStudent(const string& studentID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    auto it = find_if(students.begin(), students.end(),
        [&studentID](const Student& s) { return s.getStudentID() == studentID; });
    
    if (it != students.end()) {
        students.erase(it);
        avlRoot = deleteAVL(avlRoot, studentID);
        saveData();
        cout << "学生信息删除成功！" << endl;
        return true;
    }
    
    cout << "错误：未找到该学生！" << endl;
    return false;
}

bool StudentManager::modifyStudent(const string& studentID) {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    Student* stu = findStudent(studentID);
    if (stu == nullptr) {
        cout << "错误：未找到该学生！" << endl;
        return false;
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
    
    string newValue;
    
    switch (choice) {
        case 1:
            cout << "请输入新姓名：";
            getline(cin, newValue);
            stu->setName(newValue);
            break;
        case 2:
            cout << "请输入新性别（男/女）：";
            getline(cin, newValue);
            if (!Utils::validateGender(newValue)) {
                cout << "错误：性别必须是'男'或'女'！" << endl;
                return false;
            }
            stu->setGender(newValue);
            break;
        case 3:
            cout << "请输入新出生日期（YYYY-MM-DD）：";
            getline(cin, newValue);
            if (!Utils::validateDate(newValue)) {
                cout << "错误：日期格式不正确或不合理！" << endl;
                return false;
            }
            stu->setBirthDate(newValue);
            break;
        case 4:
            cout << "请输入新政治面貌：";
            getline(cin, newValue);
            stu->setPoliticalStatus(newValue);
            break;
        case 5:
            cout << "请输入新联系方式：";
            getline(cin, newValue);
            if (!Utils::validatePhoneNumber(newValue)) {
                cout << "错误：电话号码格式不正确！" << endl;
                return false;
            }
            stu->setPhoneNumber(newValue);
            break;
        case 6:
            cout << "请输入新家庭住址：";
            getline(cin, newValue);
            stu->setHomeAddress(newValue);
            break;
        default:
            cout << "无效选择！" << endl;
            return false;
    }
    
    saveData();
    cout << "修改成功！" << endl;
    return true;
}

Student* StudentManager::findStudent(const string& studentID) {
    return searchAVL(avlRoot, studentID);
}

vector<Student> StudentManager::searchByName(const string& name) const {
    vector<Student> results;
    for (const auto& stu : students) {
        if (stu.getName().find(name) != string::npos) {
            results.push_back(stu);
        }
    }
    return results;
}

vector<Student> StudentManager::searchByGender(const string& gender) const {
    vector<Student> results;
    for (const auto& stu : students) {
        if (stu.getGender() == gender) {
            results.push_back(stu);
        }
    }
    return results;
}

void StudentManager::displayAllStudents() const {
    SetConsoleOutputCP(65001);  // 设置UTF-8编码
    if (students.empty()) {
        cout << "\n暂无学生信息！" << endl;
        return;
    }
    
    cout << "\n========== 所有学生信息 ==========" << endl;
    Student::displayHeader();
    for (const auto& stu : students) {
        stu.display();
    }
    cout << "共计 " << students.size() << " 名学生。" << endl;
}

void StudentManager::saveData() {
    ofstream file(STUDENT_FILE);
    if (!file.is_open()) {
        cerr << "警告：无法保存学生数据！" << endl;
        return;
    }
    
    for (const auto& stu : students) {
        file << stu.getStudentID() << "|" << stu.getName() << "|"
             << stu.getGender() << "|" << stu.getBirthDate() << "|"
             << stu.getPoliticalStatus() << "|" << stu.getPhoneNumber() << "|"
             << stu.getHomeAddress() << endl;
    }
    
    file.close();
}

void StudentManager::loadData() {
    ifstream file(STUDENT_FILE);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, gender, birth, politic, phone, addr;
        
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, gender, '|');
        getline(ss, birth, '|');
        getline(ss, politic, '|');
        getline(ss, phone, '|');
        getline(ss, addr);
        
        Student stu(id, name, gender, birth, politic, phone, addr);
        students.push_back(stu);
        avlRoot = insertAVL(avlRoot, stu);
    }
    
    file.close();
}

// ==================== AVL树辅助函数 ====================

// 获取节点高度
int StudentManager::getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

// 获取平衡因子
int StudentManager::getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// 右旋转（LL型）
AVLNode* StudentManager::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    // 执行旋转
    x->right = y;
    y->left = T2;
    
    // 更新高度
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    
    return x;
}

// 左旋转（RR型）
AVLNode* StudentManager::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    // 执行旋转
    y->left = x;
    x->right = T2;
    
    // 更新高度
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    
    return y;
}

// AVL树插入
AVLNode* StudentManager::insertAVL(AVLNode* node, const Student& stu) {
    // 1. 执行标准BST插入
    if (node == nullptr) {
        return new AVLNode(stu);
    }
    
    if (stu.getStudentID() < node->student.getStudentID()) {
        node->left = insertAVL(node->left, stu);
    } else if (stu.getStudentID() > node->student.getStudentID()) {
        node->right = insertAVL(node->right, stu);
    } else {
        return node;  // 学号相同，不插入
    }
    
    // 2. 更新节点高度
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    
    // 3. 获取平衡因子
    int balance = getBalance(node);
    
    // 4. 如果不平衡，进行旋转
    // LL型：左子树的左子树
    if (balance > 1 && stu.getStudentID() < node->left->student.getStudentID()) {
        return rightRotate(node);
    }
    
    // RR型：右子树的右子树
    if (balance < -1 && stu.getStudentID() > node->right->student.getStudentID()) {
        return leftRotate(node);
    }
    
    // LR型：左子树的右子树
    if (balance > 1 && stu.getStudentID() > node->left->student.getStudentID()) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // RL型：右子树的左子树
    if (balance < -1 && stu.getStudentID() < node->right->student.getStudentID()) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

// AVL树查找
Student* StudentManager::searchAVL(AVLNode* node, const string& id) {
    if (node == nullptr) {
        return nullptr;
    }
    if (id == node->student.getStudentID()) {
        return &(node->student);
    } else if (id < node->student.getStudentID()) {
        return searchAVL(node->left, id);
    } else {
        return searchAVL(node->right, id);
    }
}

// AVL树删除
AVLNode* StudentManager::deleteAVL(AVLNode* node, const string& id) {
    // 1. 执行标准BST删除
    if (node == nullptr) return nullptr;
    
    if (id < node->student.getStudentID()) {
        node->left = deleteAVL(node->left, id);
    } else if (id > node->student.getStudentID()) {
        node->right = deleteAVL(node->right, id);
    } else {
        // 找到要删除的节点
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode* temp = node->left ? node->left : node->right;
            
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            AVLNode* temp = findMin(node->right);
            node->student = temp->student;
            node->right = deleteAVL(node->right, temp->student.getStudentID());
        }
    }
    
    if (node == nullptr) return node;
    
    // 2. 更新节点高度
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    
    // 3. 获取平衡因子
    int balance = getBalance(node);
    
    // 4. 如果不平衡，进行旋转
    // LL型
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rightRotate(node);
    }
    
    // LR型
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // RR型
    if (balance < -1 && getBalance(node->right) <= 0) {
        return leftRotate(node);
    }
    
    // RL型
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

// 找最小节点
AVLNode* StudentManager::findMin(AVLNode* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// 清空AVL树
void StudentManager::clearAVL(AVLNode* node) {
    if (node != nullptr) {
        clearAVL(node->left);
        clearAVL(node->right);
        delete node;
    }
}
