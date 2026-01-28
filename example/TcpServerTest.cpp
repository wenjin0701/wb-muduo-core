#include <iostream>
#include <thread>
#include <chrono>
#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"

void testTcpServerEcho()
{
    std::cout << "=== Testing TcpServer Echo Server ===" << std::endl;
    
    EventLoop loop;
    InetAddress listenAddr(8888);
    TcpServer server(&loop, listenAddr, "EchoServer");
    
    // 设置连接回调
    server.setConnectionCallback([](const TcpConnectionPtr &conn) {
        if (conn->connected()) {
            std::cout << "New connection: " << conn->peerAddress().toIpPort() << std::endl;
        } else {
            std::cout << "Connection closed: " << conn->peerAddress().toIpPort() << std::endl;
        }
    });
    
    // 设置消息回调（回显）
    server.setMessageCallback([](const TcpConnectionPtr &conn, Buffer *buf, Timestamp time) {
        std::string message = buf->retrieveAllAsString();
        std::cout << "Received message: " << message << " from " << conn->peerAddress().toIpPort() << std::endl;
        conn->send(message);
    });
    
    // 设置线程数
    server.setThreadNum(4);
    
    // 启动服务器
    server.start();
    std::cout << "Echo server started on port 8888" << std::endl;
    std::cout << "Server will run for 30 seconds..." << std::endl;
    
    // 运行30秒后退出
    std::thread stopThread([&loop]() {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        loop.quit();
        std::cout << "Server stopped" << std::endl;
    });
    
    // 运行事件循环
    loop.loop();
    
    stopThread.join();
    std::cout << "TcpServer echo test completed!" << std::endl;
}

int main()
{
    testTcpServerEcho();
    return 0;
}
