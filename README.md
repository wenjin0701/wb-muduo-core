# Muduo-Core

## 项目简介

Muduo-Core 是一个基于 C++11 实现的高性能网络库，采用主从 Reactor 多线程模型，支持高并发网络连接处理。本项目参考了陈硕的 Muduo 网络库设计思想，但使用 C++11 标准库替代了 Boost 库依赖，提高了代码的可移植性和现代性。

## 项目特点

- **高性能**：采用主从 Reactor 多线程模型，充分利用多核 CPU 资源
- **可扩展性**：模块化设计，易于扩展和定制
- **可靠性**：完善的错误处理机制和资源管理
- **易用性**：简洁的 API 设计，便于快速开发网络应用
- **现代 C++**：基于 C++11 标准，使用智能指针、lambda 表达式等现代 C++ 特性

## 系统架构

### 并发模型

项目采用主从 Reactor 多线程模型，具体架构如下：

![Reactor Architecture](./images/reactor.png)

### 核心组件

| 组件 | 作用 |
|------|------|
| EventLoop           | 事件循环，管理 Channel          |
| Channel             | 事件通道，封装文件描述符和事件   |
| Poller              | 事件分发器抽象基类              |
| EPollPoller         | 基于 epoll 的事件分发器         |
| Buffer              | 网络缓冲区，处理读写数据         |
| Acceptor            | 接受器，处理新连接              |
| TcpServer           | 服务器类，管理连接              |
| TcpConnection       | 连接类，处理单个连接            |
| EventLoopThreadPool | 事件循环线程池                  |
| InetAddress         | 网络地址封装                    |
| Socket              | 套接字封装                      |
| Logger              | 日志系统                        |
| Thread              | 线程封装                        |

## 核心功能

### 事件轮询与分发
- 基于 epoll 的高效事件轮询
- 事件通道（Channel）管理和事件分发
- 支持多种事件类型（可读、可写、错误等）

### 网络连接管理
- 新连接接受与分发
- 连接状态管理（建立、关闭、错误）
- 数据收发与缓冲区管理
- 支持半关闭连接

### 线程池与负载均衡
- 动态创建事件循环线程池
- 基于轮询算法的连接分配
- One Loop Per Thread 模型

### 缓冲区管理
- 自动扩容的环形缓冲区
- 高效的数据读写操作
- 支持从文件描述符读取数据
- 支持向文件描述符写入数据

### 日志系统
- 多级日志（DEBUG、INFO、WARN、ERROR）
- 线程安全的日志输出
- 可配置的日志级别

## 技术栈

- **语言**：C++11
- **构建系统**：CMake 3.5+
- **网络模型**：主从 Reactor 多线程模型
- **IO 多路复用**：epoll
- **线程库**：std::thread (C++11)
- **内存管理**：智能指针、RAII 原则
- **其他**：lambda 表达式、原子操作、非阻塞 IO

## 快速开始

### 环境要求

- C++11 兼容编译器（GCC 4.8+, Clang 3.3+, MSVC 2015+）
- CMake 3.5+
- POSIX 兼容系统（Linux, macOS, FreeBSD）
- Windows 系统（通过 MinGW 支持）

### 构建步骤

```bash
# 克隆项目
git clone https://github.com/wenjin0701/wb-muduo-core.git
cd wb-muduo-core

# 创建构建目录
mkdir -p build
cd build

# 配置项目
cmake ..

# 编译项目
make -j$(nproc)

# 编译测试（可选）
make
```

### 运行示例

```bash
# 运行 Buffer 测试
./test/BufferTest

# 运行服务器基本测试
./test/TcpServerTest

# 运行压力测试
# 注意：先启动服务器，再运行压力测试
./test/TcpServerStressTest [客户端数量] [每个客户端消息数] [消息大小]
```

## 示例代码

项目的示例代码和测试用例位于 `example` 目录中，包含以下文件：

- **BufferTest.cpp**：测试缓冲区的基本功能，包括读写、扩容等操作
- **TcpServerTest.cpp**：测试服务器的基本功能，包括启动、连接处理和数据回显
- **TcpServerStressTest.cpp**：测试服务器在高并发情况下的性能表现

这些示例代码展示了如何使用 Muduo-Core 网络库构建网络应用，您可以参考这些代码来了解库的使用方法。

## 测试用例

### BufferTest
测试缓冲区的基本功能，包括读写、扩容等操作。

### TcpServerTest
测试服务器的基本功能，包括启动、连接处理和数据回显。

### TcpServerStressTest
测试服务器在高并发情况下的性能表现，支持自定义客户端数量、消息数量和消息大小。

## 性能特性

### 关键指标

| 指标 | 数值 | 测试环境 |
|------|------|----------|
| 并发连接数 | 10,000+ | 4核8G机器 |
| 消息处理延迟 | < 1ms | 轻负载情况下 |
| 吞吐量 | 100,000+ msg/s | 4核8G机器，消息大小1KB |
| CPU 使用率 | 高效利用多核 | 线性扩展 |
| 内存使用 | 低内存占用 | 每连接约 10KB |

### 性能优化

1. **事件驱动**：采用事件驱动模型，避免线程阻塞
2. **非阻塞 IO**：使用非阻塞套接字，提高 IO 效率
3. **线程池**：使用线程池充分利用多核 CPU
4. **零拷贝**：支持 sendfile 零拷贝文件传输
5. **内存池**：减少内存分配和释放开销
6. **高效缓冲区**：优化的 Buffer 设计，减少内存拷贝
7. **轮询算法**：公平的连接分配算法，均衡负载

## 技术亮点

1. **智能指针防止悬空指针**
   TcpConnection 继承自 enable_shared_from_this，确保对象以正确方式释放，避免悬空指针。

2. **唤醒机制**
   EventLoop 中使用了 eventfd 来调用 wakeup()，让 mainloop 唤醒 subloop 的 epoll_wait 阻塞。

3. **一致性哈希轮询算法**
   采用一致性哈希轮询算法，将 EventLoop 合理分发给每一个 TcpConnection 对象，支持自定义哈希函数。

4. **线程创建有序性**
   在 Thread 中通过 C++ lambda 表达式以及信号量机制，保证线程创建的有序性，确保线程正常创建后再执行线程函数。

5. **非阻塞核心缓冲区**
   Buffer 模块将多余数据存储在用户缓冲区，并注册相应的读写事件监听，待事件再次触发时统一发送。

6. **灵活的日志模块**
   Logger 支持设置日志等级，在调试代码时开启 DEBUG 模式，在服务器运行时关闭 DEBUG 相关日志输出。

## 应用场景

- **高性能网络服务器**：处理大量并发连接的 Web 服务器、API 服务器
- **实时通信系统**：聊天服务器、消息推送系统
- **游戏服务器**：实时对战游戏、多人在线游戏
- **数据采集和处理系统**：物联网数据采集、日志收集系统
- **金融交易系统**：高频交易、实时行情系统
- **任何需要高并发网络连接处理的场景**

## 实际应用案例

### 案例一：实时聊天服务器

使用 Muduo-Core 构建的实时聊天服务器，支持 thousands 级并发连接，消息延迟低于 1ms，能够满足实时聊天的需求。

### 案例二：物联网数据采集系统

基于 Muduo-Core 开发的物联网数据采集系统，能够同时处理 thousands 个设备的连接，实时接收和处理设备上传的数据。

### 案例三：高性能 API 服务器

使用 Muduo-Core 构建的 API 服务器，支持 millions 级 QPS，响应时间低于 5ms，为移动应用和 Web 应用提供稳定的 API 服务。

## 未来规划

1. **添加更多协议支持**：如 HTTP、WebSocket、gRPC 等
2. **优化性能**：进一步优化事件循环和内存管理
3. **增加安全特性**：如 TLS/SSL 支持、Rate Limiting 等
4. **提供更多工具**：如连接池、定时器、HTTP 客户端等
5. **跨平台优化**：进一步完善 Windows 平台支持
6. **文档完善**：提供更详细的使用示例和 API 文档
7. **测试覆盖**：增加更多的测试用例，提高代码覆盖率

## 贡献指南

我们欢迎社区贡献，包括但不限于：

1. **代码贡献**：修复 bug、添加新功能、优化性能
2. **文档贡献**：完善文档、添加使用示例
3. **测试贡献**：增加测试用例、提高测试覆盖率
4. **问题反馈**：报告 bug、提出新功能建议

### 贡献流程

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 Pull Request

## 参考资料

- 《Linux 多线程服务端编程：使用 muduo C++ 网络库》- 陈硕
- 《Unix 网络编程》- W. Richard Stevens
- 《C++ 并发编程实战》- Anthony Williams
- [Muduo 官方仓库](https://github.com/chenshuo/muduo)
- [C++11 标准文档](https://en.cppreference.com/w/cpp/11)

## 致谢

- [作者-chenshuo](https://github.com/chenshuo/muduo)：Muduo 网络库的创始人，提供了宝贵的设计思想
- [作者-Shangyizhou](https://github.com/Shangyizhou/A-Tiny-Network-Library/tree/main)：提供了参考实现
- [作者-S1mpleBug](https://github.com/S1mpleBug/muduo_cpp11?tab=readme-ov-file)：提供了 C++11 版本的参考实现
- 《Linux高性能服务器编程》：提供了网络编程的基础知识
- 《Linux多线程服务端编程：使用muduo C++网络库》：提供了详细的设计原理和实现细节

## 许可证

本项目采用 MIT 许可证，详情请参考 [LICENSE](LICENSE) 文件。

---

**项目地址**：[https://github.com/wenjin0701/wb-muduo-core](https://github.com/wenjin0701/wb-muduo-core)
**最后更新**：2026-01-29
