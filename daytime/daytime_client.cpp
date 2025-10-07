#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>  // для struct timeval

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>\n";
        return 1;
    }

    const char* server_ip = argv[1];
    int port = std::stoi(argv[2]);

    // Создаем UDP-сокет
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    // Устанавливаем таймаут на получение данных (5 секунд)
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("setsockopt SO_RCVTIMEO");
        close(sock);
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported\n";
        close(sock);
        return 1;
    }

    // Отправляем пустое сообщение (по спецификации daytime)
    if (sendto(sock, "", 0, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto");
        close(sock);
        return 1;
    }

    // Получаем ответ
    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);
    ssize_t bytes_received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                                      (struct sockaddr*)&server_addr, &addr_len);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Server time: " << buffer << std::endl;
    } else if (bytes_received == 0) {
        std::cout << "Server sent an empty response.\n";
    } else {
        // Ошибка: обычно "Operation timed out"
        perror("recvfrom");
        std::cerr << "Error: No response from server within 5 seconds.\n";
    }

    close(sock);
    return 0;
}
