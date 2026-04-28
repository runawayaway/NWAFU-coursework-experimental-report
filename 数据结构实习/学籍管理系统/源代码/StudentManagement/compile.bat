# 创建必要的目录
if not exist build mkdir build
if not exist bin mkdir bin

# 编译所有源文件
g++ -std=c++11 -Wall -Wextra -I./include -c src/Utils.cpp -o build/Utils.o
g++ -std=c++11 -Wall -Wextra -I./include -c src/User.cpp -o build/User.o
g++ -std=c++11 -Wall -Wextra -I./include -c src/Student.cpp -o build/Student.o
g++ -std=c++11 -Wall -Wextra -I./include -c src/Course.cpp -o build/Course.o
g++ -std=c++11 -Wall -Wextra -I./include -c src/Selection.cpp -o build/Selection.o
g++ -std=c++11 -Wall -Wextra -I./include -c src/Score.cpp -o build/Score.o
g++ -std=c++11 -Wall -Wextra -I./include -c src/System.cpp -o build/System.o
g++ -std=c++11 -Wall -Wextra -I./include -c main.cpp -o build/main.o

# 链接生成可执行文件
g++ build/Utils.o build/User.o build/Student.o build/Course.o build/Selection.o build/Score.o build/System.o build/main.o -o bin/StudentManagement.exe

@REM echo "编译完成！可执行文件位于 bin/StudentManagement.exe"
pause
