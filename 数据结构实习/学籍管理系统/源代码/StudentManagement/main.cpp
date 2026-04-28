#include "include/System.h"
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001); 
    StudentManagementSystem sms;
    sms.run();
    return 0;
}
