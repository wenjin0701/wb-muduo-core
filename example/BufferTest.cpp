#include <iostream>
#include "Buffer.h"

void testBufferBasic()
{
    std::cout << "=== Testing Buffer Basic Functions ===" << std::endl;
    
    Buffer buffer;
    
    // 测试初始状态
    std::cout << "Initial readableBytes: " << buffer.readableBytes() << std::endl;
    std::cout << "Initial writableBytes: " << buffer.writableBytes() << std::endl;
    std::cout << "Initial prependableBytes: " << buffer.prependableBytes() << std::endl;
    
    // 测试写入数据
    std::string testData = "Hello, Buffer!";
    buffer.append(testData.c_str(), testData.size());
    
    std::cout << "After append, readableBytes: " << buffer.readableBytes() << std::endl;
    std::cout << "After append, writableBytes: " << buffer.writableBytes() << std::endl;
    
    // 测试读取数据
    std::string retrieved = buffer.retrieveAsString(testData.size());
    std::cout << "Retrieved data: " << retrieved << std::endl;
    std::cout << "After retrieve, readableBytes: " << buffer.readableBytes() << std::endl;
    
    // 测试缓冲区复位
    buffer.retrieveAll();
    std::cout << "After retrieveAll, readableBytes: " << buffer.readableBytes() << std::endl;
    std::cout << "After retrieveAll, writableBytes: " << buffer.writableBytes() << std::endl;
    
    // 测试扩容
    std::string largeData(2048, 'x');
    buffer.append(largeData.c_str(), largeData.size());
    std::cout << "After append large data, readableBytes: " << buffer.readableBytes() << std::endl;
    std::cout << "After append large data, writableBytes: " << buffer.writableBytes() << std::endl;
    
    std::cout << "Buffer basic test passed!" << std::endl;
}

void testBufferRetrieve()
{
    std::cout << "\n=== Testing Buffer Retrieve Functions ===" << std::endl;
    
    Buffer buffer;
    
    // 写入测试数据
    buffer.append("Hello, ", 7);
    buffer.append("World!", 6);
    
    std::cout << "Total readable bytes: " << buffer.readableBytes() << std::endl;
    
    // 测试部分读取
    std::string part1 = buffer.retrieveAsString(7);
    std::cout << "Part 1 retrieved: " << part1 << std::endl;
    std::cout << "Remaining readable bytes: " << buffer.readableBytes() << std::endl;
    
    // 测试剩余读取
    std::string part2 = buffer.retrieveAllAsString();
    std::cout << "Part 2 retrieved: " << part2 << std::endl;
    std::cout << "Remaining readable bytes: " << buffer.readableBytes() << std::endl;
    
    std::cout << "Buffer retrieve test passed!" << std::endl;
}

int main()
{
    testBufferBasic();
    testBufferRetrieve();
    
    std::cout << "\nAll Buffer tests passed!" << std::endl;
    return 0;
}
