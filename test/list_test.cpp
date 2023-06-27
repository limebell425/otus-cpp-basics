#include "MyList.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(List, DefaultConstructorTest)
{
    MyList<int> list;
    EXPECT_EQ(list.size(), 0);
}

class ListTest : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            for (int i = 0; i < 5; ++i)
            {
                list2.push_back(i);
            }   
        }

        MyList<int> list1;
        MyList<int> list2;
};

TEST_F(ListTest, PushBackInEmptyList)
{
    int val = 5;
    list1.push_back(val);
    EXPECT_EQ(list1[0], val);
    EXPECT_EQ(list1.size(), 1);
}

TEST_F(ListTest, PushBackNotEmpty)
{
    list2.push_back(5); 
    EXPECT_THAT(list2, testing::ElementsAre(0, 1, 2, 3, 4, 5));
    EXPECT_EQ(list2.size(), 6);
}

TEST_F(ListTest, InsertInEndEmptyList)
{
    int val = 5;
    list1.insert(list1.end(), val);
    EXPECT_EQ(list1[0], val);
    EXPECT_EQ(list1.size(), 1);
}

TEST_F(ListTest, InsertInBeginEmptyList)
{
    int val = 5;
    list1.insert(list1.begin(), val);
    EXPECT_EQ(list1[0], val);
    EXPECT_EQ(list1.size(), 1);
}

TEST_F(ListTest, InsertBegin)
{
    list2.insert(list2.begin(), -1);
    EXPECT_THAT(list2, testing::ElementsAre(-1, 0, 1, 2, 3, 4)); 
    EXPECT_EQ(list2.size(), 6);
}

TEST_F(ListTest, InsertMiddle)
{
    list2.insert(std::next(list2.begin(), list2.size()/2), -1);
    EXPECT_THAT(list2, testing::ElementsAre(0, 1, -1, 2, 3, 4));
    EXPECT_EQ(list2.size(), 6);
}

TEST_F(ListTest, InsertEnd)
{
    list2.insert(list2.end(), 3); 
    EXPECT_THAT(list2, testing::ElementsAre(0, 1, 2, 3, 4, 3));
    EXPECT_EQ(list2.size(), 6);
}

TEST_F(ListTest, EraseEnd)
{
    list2.erase(list2.end());
    EXPECT_THAT(list2, testing::ElementsAre(0, 1, 2, 3));   
    EXPECT_EQ(list2.size(), 4);
}

TEST_F(ListTest, EraseBegin)
{
    list2.erase(list2.begin());
    EXPECT_THAT(list2, testing::ElementsAre(1, 2, 3, 4)); 
    EXPECT_EQ(list2.size(), 4);
}

TEST_F(ListTest, EraseMiddle)
{
    list2.erase(std::next(list2.begin(), list2.size()/2));
    EXPECT_THAT(list2, testing::ElementsAre(0, 1, 3, 4)); 
    EXPECT_EQ(list2.size(), 4);
}

TEST_F(ListTest, EraseFromEmptyList)
{
    EXPECT_THROW(
    {
        list1.erase(list1.begin());
    },  std::out_of_range);

    EXPECT_THROW(
    {
        list1.erase(list1.end());
    },  std::out_of_range);
}


TEST_F(ListTest, GetElement)
{
    int first = list2[0];
    EXPECT_EQ(first, 0);
    
    int last = list2[list2.size()-1];
    EXPECT_EQ(last, 4);

    int middle = list2[list2.size()/2];
    EXPECT_EQ(middle, 2);

    EXPECT_THROW(
    {
        int error = list2[list2.size()+2];
    },  std::out_of_range);
    
    EXPECT_THROW(
    {
        int error3 = list1[0];
    },  std::out_of_range);
}

TEST_F(ListTest, GetSize)
{
    EXPECT_EQ(list1.size(), 0);
    EXPECT_EQ(list2.size(), 5);
}

TEST_F(ListTest, CopyList)
{
    list1 = list2;
    EXPECT_THAT(list1, testing::ContainerEq(list2));
}

TEST_F(ListTest, CopyEmptyList)
{
    list2 = list1;
    EXPECT_THAT(list1, testing::ContainerEq(list2));
}

TEST_F(ListTest, DestructElements)
{
    auto sp0 = std::make_shared<int>(2);
    {
        MyList<std::shared_ptr<int>> list;
        list.push_back(sp0);
        EXPECT_EQ(sp0.use_count(), 2);
    }
    EXPECT_EQ(sp0.use_count(), 1);
}

TEST_F(ListTest, MoveList)
{
    int *p = &list2[0];
    list1 = std::move(list2);
    EXPECT_EQ(list2.size(), 0);
    EXPECT_EQ(&list1[0], p);
}
