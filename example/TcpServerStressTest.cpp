#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class StressTestClient
{
public:
    StressTestClient(const std::string &ip, int port, int id)
        : ip_(ip), port_(port), id_(id), sockfd_(-1) {}
    
    bool connect()
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ < 0) {
            std::cerr << "Client " << id_ << " create socket failed" << std::endl;
            return false;
        }
        
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_);
        inet_pton(AF_INET, ip_.c_str(), &addr.sin_addr);
        
        if (::connect(sockfd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Client " << id_ << " connect failed" << std::endl;
            close(sockfd_);
            sockfd_ = -1;
            return false;
        }
        
        std::cout << "Client " << id_ << " connected" << std::endl;
        return true;
    }
    
    void sendData(int count, int dataSize)
    {
        if (sockfd_ < 0) return;
        
        char *buffer = new char[dataSize];
        memset(buffer, 'a', dataSize);
        buffer[dataSize - 1] = '\0';
        
        for (int i = 0; i < count; ++i) {
            int ret = send(sockfd_, buffer, dataSize, 0);
            if (ret < 0) {
                std::cerr << "Client " << id_ << " send failed" << std::endl;
                break;
            }
            
            // 接收响应
            char recvBuffer[4096];
            ret = recv(sockfd_, recvBuffer, sizeof(recvBuffer), 0);
            if (ret < 0) {
                std::cerr << "Client " << id_ << " recv failed" << std::endl;
                break;
            }
        }
        
        delete[] buffer;
    }
    
    void close()
    {
        if (sockfd_ >= 0) {
            ::close(sockfd_);
            sockfd_ = -1;
        }
    }
    
private:
    std::string ip_;
    int port_;
    int id_;
    int sockfd_;
};

void testStress(int clientCount, int messagesPerClient, int messageSize)
{
    std::cout << "=== Stress Test Parameters ===" << std::endl;
    std::cout << "Client count: " << clientCount << std::endl;
    std::cout << "Messages per client: " << messagesPerClient << std::endl;
    std::cout << "Message size: " << messageSize << " bytes" << std::endl;
    
    std::vector<std::thread> clientThreads;
    std::atomic<int> connectedClients(0);
    std::atomic<int> completedClients(0);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 启动所有客户端
    for (int i = 0; i < clientCount; ++i) {
        clientThreads.emplace_back([i, &connectedClients, &completedClients, clientCount, messagesPerClient, messageSize]() {
            StressTestClient client("127.0.0.1", 8888, i);
            
            if (client.connect()) {
                connectedClients++;
                
                // 等待所有客户端连接
                while (connectedClients < clientCount) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                
                // 发送数据
                client.sendData(messagesPerClient, messageSize);
                
                completedClients++;
                std::cout << "Client " << i << " completed" << std::endl;
            }
            
            client.close();
        });
    }
    
    // 等待所有客户端完成
    for (auto &t : clientThreads) {
        t.join();
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    int totalMessages = clientCount * messagesPerClient;
    std::cout << "=== Stress Test Results ===" << std::endl;
    std::cout << "Total clients: " << clientCount << std::endl;
    std::cout << "Total messages: " << totalMessages << std::endl;
    std::cout << "Total time: " << duration << " ms" << std::endl;
    std::cout << "Messages per second: " << (totalMessages * 1000.0) / duration << std::endl;
    std::cout << "Stress test completed!" << std::endl;
}

int main(int argc, char *argv[])
{
    int clientCount = 100;
    int messagesPerClient = 100;
    int messageSize = 1024;
    
    if (argc >= 2) {
        clientCount = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        messagesPerClient = std::stoi(argv[2]);
    }
    if (argc >= 4) {
        messageSize = std::stoi(argv[3]);
    }
    
    testStress(clientCount, messagesPerClient, messageSize);
    return 0;
}
