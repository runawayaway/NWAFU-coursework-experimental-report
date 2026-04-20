#include <stdio.h>      
#include <stdlib.h> 
#include <string.h>     
#include <unistd.h>   
#include <fcntl.h>     
#include <errno.h>     

// 定义设备读取路径与缓冲区最大长度
#define DEVICE_PATH   "/tmp/device"  // 设备文件路径
#define BUFFER_SIZE   50             // 缓冲区最长长度
static char device_buffer[BUFFER_SIZE] = {0}; // 设备内部缓冲区

// 设备初始化函数
int init() {
    printf("open success!\n");
    sprintf(device_buffer, "open success!\n"); 
    return 0;
}

// 设备读数据
ssize_t readDevice(char *user_buffer, size_t bytes_to_read) {
    if (bytes_to_read > BUFFER_SIZE) 
        bytes_to_read = BUFFER_SIZE;
    
    memcpy(user_buffer, device_buffer, bytes_to_read);
    printf("read %zu bytes from device\n", bytes_to_read);
    return bytes_to_read;
}

// 设备写数据
ssize_t writeDevice(const char *user_data, size_t bytes_to_write) {
    if (bytes_to_write >= BUFFER_SIZE) 
        bytes_to_write = BUFFER_SIZE - 1;
    
    memset(device_buffer, 0, BUFFER_SIZE);
    memcpy(device_buffer, user_data, bytes_to_write);
    device_buffer[bytes_to_write] = '\0';
    
    printf("write %zu bytes to device: %s\n", bytes_to_write, device_buffer);
    return bytes_to_write;
}

// 设备清理
int cleanup() {
    printf("device release\n");
    return 0;
}

// 主程序
int main(int argc, char *argv[]) {
    
    int file_descriptor;
    char user_space_buffer[100];

    // 处理创建设备文件的参数
    if (argc > 1 && strcmp(argv[1], "create") == 0) {
        file_descriptor = open(DEVICE_PATH, O_CREAT | O_RDWR, 0666);
        if (file_descriptor < 0) {
            perror("Failed to create device file");
            return EXIT_FAILURE;
        }
        close(file_descriptor);
        printf("Device file created at %s\n", DEVICE_PATH);
        return 0;
    }

    // 模拟设备初始化流程
    init();

    // 模拟数据写入操作
    const char *test_message = "Test Message";
    writeDevice(test_message, strlen(test_message));

    // 模拟数据读取操作
    memset(user_space_buffer, 0, sizeof(user_space_buffer));
    readDevice(user_space_buffer, sizeof(user_space_buffer));
    printf("Read data: %s\n", user_space_buffer);

    cleanup();
    return 0;
}