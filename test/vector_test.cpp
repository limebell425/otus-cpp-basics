#include "MyVector.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

TEST(Vector, EmptyConstructorTest)
{
    MyVector<int> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

TEST(Vector, ConstructorTest)
{
    MyVector<int> vec(5);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 7);
}

class VectorTest : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            for (int i = 0; i < 3; ++i)
            {
                vec2.push_back(i);
                vec3.push_back(i);
            }   
        }

        MyVector<int> vec1;
        MyVector<int> vec2{8};
        MyVector<int> vec3{2};
};

TEST_F(VectorTest, PushBackInEmptyVector)
{
    int val = 5;
    vec1.push_back(val);
    EXPECT_EQ(vec1[0], val);
    EXPECT_EQ(vec1.size(), 1);
    EXPECT_EQ(vec1.capacity(), 1);
}

TEST_F(VectorTest, PushBackNotRealloc)
{
    vec2.push_back(3); 
    EXPECT_THAT(vec2, testing::ElementsAre(0, 1, 2, 3));
    EXPECT_EQ(vec2.size(), 4);
    EXPECT_EQ(vec2.capacity(), 12);
}

TEST_F(VectorTest, PushBackRealloc)
{
    vec3.push_back(3);
    for (int i = 0; i < 4; ++i)
    EXPECT_THAT(vec3, testing::ElementsAre(0, 1, 2, 3));        
    EXPECT_EQ(vec3.size(), 4);
    EXPECT_EQ(vec3.capacity(), 6);
}

TEST_F(VectorTest, InsertInBeginEmptyVector)
{
    int val = 5;
    vec1.insert(vec1.begin(), val);
    EXPECT_EQ(vec1[0], val);
    EXPECT_EQ(vec1.size(), 1);
    EXPECT_EQ(vec1.capacity(), 1);
}

TEST_F(VectorTest, InsertInEndEmptyVector)
{
    int val = 5;
    vec1.insert(vec1.end(), val);
    EXPECT_EQ(vec1[0], val);
    EXPECT_EQ(vec1.size(), 1);
    EXPECT_EQ(vec1.capacity(), 1);
}

TEST_F(VectorTest, InsertBeginNotRealloc)
{
    vec2.insert(vec2.begin(), -1);
    EXPECT_THAT(vec2, testing::ElementsAre(-1, 0, 1, 2)); 
    EXPECT_EQ(vec2.size(), 4);
    EXPECT_EQ(vec2.capacity(), 12);
}

TEST_F(VectorTest, InsertBeginRealloc)
{
    vec3.insert(vec3.begin(), -1);
    EXPECT_THAT(vec3, testing::ElementsAre(-1, 0, 1, 2));
    EXPECT_EQ(vec3.size(), 4);
    EXPECT_EQ(vec3.capacity(), 6);
}

TEST_F(VectorTest, InsertMiddleNotRealloc)
{
    vec2.insert(std::next(vec2.begin(), vec2.size()/2), -1);
    EXPECT_THAT(vec2, testing::ElementsAre(0, -1, 1, 2));
    EXPECT_EQ(vec2.size(), 4);
    EXPECT_EQ(vec2.capacity(), 12);
}

TEST_F(VectorTest, InsertMiddleRealloc)
{
    vec3.insert(std::next(vec3.begin(), vec3.size()/2), -1);
    EXPECT_THAT(vec3, testing::ElementsAre(0, -1, 1, 2));
    EXPECT_EQ(vec3.size(), 4);
    EXPECT_EQ(vec3.capacity(), 6);
}

TEST_F(VectorTest, InsertEndNotRealloc)
{
    vec2.insert(vec2.end(), 3); 
    EXPECT_THAT(vec2, testing::ElementsAre(0, 1, 2, 3));
    EXPECT_EQ(vec2.size(), 4);
    EXPECT_EQ(vec2.capacity(), 12);
}

TEST_F(VectorTest, InsertEndRealloc)
{
    vec3.insert(vec3.end(), 3);
    EXPECT_THAT(vec3, testing::ElementsAre(0, 1, 2, 3));
    EXPECT_EQ(vec3.size(), 4);
    EXPECT_EQ(vec3.capacity(), 6);
}

TEST_F(VectorTest, InsertOutOfRange)
{
    EXPECT_THROW(
    {
        vec1.insert(std::next(vec1.end(), 3), 20);
    },  std::out_of_range);
    
    EXPECT_THROW(
    {
        vec2.insert(std::next(vec2.end(), 3), 20);
    },  std::out_of_range);

    EXPECT_THROW(
    {
        vec3.insert(std::next(vec3.end(), 3), 20);
    },  std::out_of_range);
}

TEST_F(VectorTest, EraseEnd)
{
    vec2.erase(vec2.end());
    EXPECT_THAT(vec2, testing::ElementsAre(0, 1));   
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2.capacity(), 12);

    vec3.erase(vec3.end());
    EXPECT_THAT(vec3, testing::ElementsAre(0, 1));  
    EXPECT_EQ(vec3.size(), 2);
    EXPECT_EQ(vec3.capacity(), 3);
}

TEST_F(VectorTest, EraseBegin)
{
    vec2.erase(vec2.begin());
    EXPECT_THAT(vec2, testing::ElementsAre(1, 2)); 
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2.capacity(), 12);

    vec3.erase(vec3.begin());
    EXPECT_THAT(vec3, testing::ElementsAre(1, 2));
    EXPECT_EQ(vec3.size(), 2);
    EXPECT_EQ(vec3.capacity(), 3);
}

TEST_F(VectorTest, EraseMiddle)
{
    vec2.erase(std::next(vec2.begin(), vec2.size()/2));
    EXPECT_THAT(vec2, testing::ElementsAre(0, 2)); 
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2.capacity(), 12);

    vec3.erase(std::next(vec3.begin(), vec3.size()/2));
    EXPECT_THAT(vec3, testing::ElementsAre(0, 2)); 
    EXPECT_EQ(vec3.size(), 2);
    EXPECT_EQ(vec3.capacity(), 3);
}

TEST_F(VectorTest, EraseOutOfRange)
{
    EXPECT_THROW(
    {
        vec1.erase(vec1.begin());
    },  std::out_of_range);

    EXPECT_THROW(
    {
        vec1.erase(vec1.end());
    },  std::out_of_range);
    
    EXPECT_THROW(
    {
        vec2.erase(std::next(vec2.end(), 2));
    },  std::out_of_range);

    EXPECT_THROW(
    {
        vec3.erase(std::next(vec3.end(), 2));
    },  std::out_of_range);
}

TEST_F(VectorTest, GetElement)
{
    int first = vec2[0];
    EXPECT_EQ(first, 0);
    
    int last = vec2[vec2.size()-1];
    EXPECT_EQ(last, 2);

    int middle = vec2[vec2.size()/2];
    EXPECT_EQ(middle, 1);

    EXPECT_THROW(
    {
        int error = vec2[vec2.size()+2];
    },  std::out_of_range);
    
    EXPECT_THROW(
    {
        int error2 = vec3[vec3.size()+2];
    },  std::out_of_range);

    EXPECT_THROW(
    {
        int error3 = vec1[0];
    },  std::out_of_range);
}

TEST_F(VectorTest, GetSize)
{
    EXPECT_EQ(vec1.size(), 0);
    EXPECT_EQ(vec2.size(), 3);
    EXPECT_EQ(vec3.size(), 3);
}

TEST_F(VectorTest, CopyVector)
{
    vec1 = vec2;
    EXPECT_THAT(vec1, testing::ContainerEq(vec2));
}

TEST_F(VectorTest, CopyEmptyVector)
{
    vec2 = vec1;
    EXPECT_THAT(vec1, testing::ContainerEq(vec2));
}

TEST_F(VectorTest, DestructElements)
{
    auto sp0 = std::make_shared<int>(2);
    {
        MyVector<std::shared_ptr<int>> vec;
        vec.push_back(sp0);
        EXPECT_EQ(sp0.use_count(), 2);
    }
    EXPECT_EQ(sp0.use_count(), 1);
}

TEST_F(VectorTest, MoveVector)
{
    int *p = &vec2[0];
    vec1 = std::move(vec2);
    EXPECT_EQ(vec2.size(), 0);
    EXPECT_EQ(vec2.capacity(), 0);
    EXPECT_EQ(&vec1[0], p);
}
