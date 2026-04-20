#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

//规定单次传输文件容量
#define BUFFER_SIZE 8192


//规定配置文件的名称
#define CONFIG_FILE "webserver.conf"
// 规定存储配置文件的结构体
struct Config {
    char home_dir[64];
    char target_dir[64];
    char index_file[64];
    int port;
};

// 读取配置文件
void read_config(struct Config *config){
    FILE *fp = fopen(CONFIG_FILE, "r");
    if(!fp){
        perror("无法打开配置文件");
        exit(1);
    }
    char line[1024];
    while(fgets(line, sizeof(line), fp)){
        
        //拆分名称与数值
        char *key = strtok(line, "=\n");
        char *value = strtok(NULL, "=\n");
        
        //根据名称将数组填入对应的存储变量
        if (strcmp(line, "HOME_DIR") == 0){
            strcpy(config->home_dir, value);
        }else if (strcmp(line, "TARGET_DIR") == 0){	
            strcpy(config->target_dir, value);
        }else if (strcmp(line, "INDEX_FILE") == 0){
            strcpy(config->index_file, value);
        }else if (strcmp(line, "PORT") == 0){
            config->port = atoi(value);
        }
    }
    fclose(fp);
}

// 发送HTTP响应头
void send_headers(int fd, const char *content_type, int content_length){
    char headers[BUFFER_SIZE];
    sprintf(headers, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n",
        content_type,
        content_length);
    send(fd, headers, strlen(headers), 0);
}

// 发送文件列表页面
void send_file_list(int fd, const char *dir_path){
    DIR *dp = opendir(dir_path);
    //无法正确打开文件夹，发送500错误
    if (!dp){
        char error_response[] = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        send(fd, error_response, strlen(error_response), 0);
        return;
    }

    // 构建HTML页面
    char *html_start = "<!DOCTYPE html><html><head>"
                      "<meta charset=\"UTF-8\">"
                      "<title>服务器文件列表</title>"
                      "<style>"
                      "body { font-family: Arial, sans-serif; max-width: 800px; margin: 0 auto; padding: 20px; }"
                      "a { color: #0066cc; text-decoration: none; display: block; padding: 10px; margin: 5px 0; }"
                      "a:hover { background-color: #f0f0f0; }"
                      "</style></head><body>"
                      "<h1>服务器文件列表</h1><div>";
    
    // 计算响应大小
    long content_length = strlen(html_start) + strlen("</div></body></html>");
    struct dirent *entry;
    char temp[BUFFER_SIZE];
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] != '.') {
            sprintf(temp, "<a href=\"/download/%s\">%s</a>", entry->d_name, entry->d_name);
            content_length += strlen(temp);
        }
    }

    // 发送响应头
    send_headers(fd, "text/html", content_length);

    // 发送页面内容
    send(fd, html_start, strlen(html_start), 0);
    
    // 重新遍历目录发送文件列表
    rewinddir(dp);
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] != '.') {
            sprintf(temp, "<a href=\"/download/%s\">%s</a>", entry->d_name, entry->d_name);
            send(fd, temp, strlen(temp), 0);
        }
    }
    
    char *html_end = "</div></body></html>";
    send(fd, html_end, strlen(html_end), 0);
    closedir(dp);
}

// 发送文件
void send_file(int fd, const char *file_path){
    int nfd = open(file_path, O_RDONLY);
    //找不到文件，发送404错误
    if (nfd < 0) {
        char error_response[] = "HTTP/1.1 404 Not Found\r\n\r\n";
        send(fd, error_response, strlen(error_response), 0);
        return;
    }

    // 获取文件大小
    struct stat file_stat;
    fstat(nfd, &file_stat);
    
    // 获取文件类型
    const char *content_type = "application/octet-stream";
    char *ext = strrchr(file_path, '.');
    if(ext){
        if (strcmp(ext, ".html") == 0) content_type = "text/html";
        else if (strcmp(ext, ".txt") == 0) content_type = "text/plain";
        else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) content_type = "image/jpeg";
        else if (strcmp(ext, ".png") == 0) content_type = "image/png";
    }

    // 发送响应头
    send_headers(fd, content_type, file_stat.st_size);

    // 分块发送文件内容
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(nfd, buffer, BUFFER_SIZE)) > 0){
        send(fd, buffer, bytes_read, 0);
    }
    close(nfd);
}
void handle_quest(int sock_id, int fd, struct Config *config)
{
    //接收请求内容
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(fd, buffer, BUFFER_SIZE - 1, 0);
    if(bytes_received > 0){
        // 解析路径
        buffer[bytes_received] = '\0';
        char *path = strchr(buffer, ' ');
        if(path){
            path++;
            char *end = strchr(path, ' ');
            if (end) *end = '\0';
            // 发送首页
            if (strcmp(path, "/") == 0){
                char index_path[1024];
                //构建路径
                snprintf(index_path, sizeof(index_path), "%s/%s", config->home_dir, config->index_file);
                send_file(fd, index_path);
            }
            // 发送文件列表
            else if(strcmp(path, "/files") == 0){
                send_file_list(fd, config->target_dir);
            }
            // 发送请求的文件
            else if (strncmp(path, "/download/", 9) == 0){
                char file_path[1024];
                snprintf(file_path, sizeof(file_path), "%s/%s", config->target_dir, path + 9);
                send_file(fd, file_path);
            }
            else{
                // 未找到文件，返回404错误
                char error_response[] = "HTTP/1.1 404 Not Found\r\n\r\n";
                send(fd, error_response, strlen(error_response), 0);
            }
        }
    }
}
int main(){
    
    //读取配置信息
    struct Config config;
    read_config(&config);
    
    //为存储配置的字符串加入结束符，让相关字符串函数可以正常使用
    config.target_dir[strlen(config.target_dir) - 1] = '\0';
    config.home_dir[strlen(config.home_dir) - 1 ] = '\0';
    config.index_file[strlen(config.index_file) - 1] = '\0';
    
    // 创建服务器socket
    int sock_id = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_id < 0){
        perror("创建socket失败");
        exit(1);
    }

    // 配置服务器地址
    struct sockaddr_in saddr;
    bzero((void *)&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(config.port);

    // 绑定socket
    if(bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) < 0){
        perror("绑定失败");
        exit(1);
    }

    // 令监听连接，并设置最大连接数
    if(listen(sock_id, 10) < 0){
        perror("监听失败");
        exit(1);
    }
    
    //显示从配置读入的信息
    printf("服务器启动在端口 %d\n", config.port);
    printf("根目录为 %s\n", config.home_dir);
    printf("资源目录为 %s\n", config.target_dir);
    printf("首页文件为 %s\n", config.index_file);

    //持续接收请求
    while(1){
        struct sockaddr_in caddr;
        socklen_t clen = sizeof(caddr);
        int fd = accept(sock_id, (struct sockaddr*)&caddr, &clen);
        if (fd < 0) {
            perror("接受连接失败");
            continue;
        }
        
        // 读取并处理HTTP请求
        handle_quest(sock_id, fd, &config);
        close(fd);
    }
    close(sock_id);
    return 0;
}

