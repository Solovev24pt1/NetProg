#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(7777); // используем порт 7777

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Echo server listening on port 7777...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }

        std::cout << "Client connected.\n";

        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytes_read] = '\0';
            send(client_fd, buffer, bytes_read, 0);
        }

        std::cout << "Client disconnected.\n";
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
