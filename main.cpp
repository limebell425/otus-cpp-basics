#include "MyVector.h"
#include "MyForwardList.h"
#include "MyList.h"
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>



template<typename T>
void container_out(std::string_view message, const T &cont){
    std::cout << message << ": ";
    for (int i: cont){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

template<typename T>
void check_container(std::string_view name, T &cont)
{
    for (int i = 0; i < 10; ++i)
    {
        cont.push_back(i);
    }
    container_out(name, cont);
    std::cout << "Size: " << cont.size() << std::endl;
    cont.erase(std::next(cont.begin(), 2));
    cont.erase(std::next(cont.begin(), 3));
    cont.erase(std::next(cont.begin(), 4));
    container_out("Erase", cont);
    cont.insert(cont.begin(), 10);
    container_out("Insert_begin: ", cont);
    cont.insert(std::next(cont.begin(), cont.size()/2), 20);
    container_out("Insert_middle: ", cont);
    cont.insert(cont.end(), 30);
    container_out("Insert_end: ", cont);
    std::cout << name << "[7] = " << cont[7] << std::endl;
    std::cout << std::endl;
}


int main(){
    MyVector<int> vector;
    check_container("Vector", vector);
    MyForwardList<int> forward_list;
    check_container("Forward_List", forward_list);
    MyList<int> list;
    check_container("List", list);
    return 1;
}