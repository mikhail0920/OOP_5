#include <gtest/gtest.h>
#include "Memory.h"
#include "Queue.h"


struct TestStruct {
    int id;
    double val;
};

TEST(QueueTest, SimpleNumbers) {
    MemoryRes res(1000); // 1000 байт памяти
    Queue<int> q(&res);

    q.push(10);
    q.push(20);
    q.push(30);

    EXPECT_EQ(q.front(), 10);
    q.pop();
    EXPECT_EQ(q.front(), 20);
    q.pop();
    EXPECT_EQ(q.front(), 30);
    q.pop();
    
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, Structs) {
    MemoryRes res(2000);
    Queue<TestStruct> q(&res);

    q.push({1, 1.1});
    q.push({2, 2.2});

    EXPECT_EQ(q.front().id, 1);
    EXPECT_EQ(q.front().val, 1.1);
    
    q.pop();
    EXPECT_EQ(q.front().id, 2);
}

TEST(QueueTest, IteratorWorks) {
    MemoryRes res(1000);
    Queue<int> q(&res);

    q.push(1);
    q.push(2);
    q.push(3);

    int sum = 0;
    for(auto it = q.begin(); it != q.end(); ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 6);
}

TEST(QueueTest, ReuseMemory) {
    MemoryRes res(40);
    Queue<int> q(&res);

    q.push(100);
    q.push(200);

    EXPECT_THROW(q.push(300), std::bad_alloc);

    q.pop(); 

    EXPECT_NO_THROW(q.push(300));
    
    EXPECT_EQ(q.front(), 200);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}