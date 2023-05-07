#include "MyVector.h"
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>

template<typename T>
void vector_out(std::string_view message, const MyVector<T> &vec){
    std::cout << message << ": ";
    for (int i: vec){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main(){
    MyVector<int> vector;
    for (int i = 0; i < 10; ++i){
        vector.push_back(i);
    }
    vector_out("Vector", vector);
    std::cout << "Size: " << vector.size() << std::endl;
    vector.erase(std::remove_if(vector.begin(),
                                vector.end(),
                                [&vector](const int &el)
                                {
                                    int idx = &el - vector.begin().operator->();
                                    return (idx == 2) || (idx == 4) || (idx == 6);

                                }
                                )
                ,vector.end());
    vector_out("Erase", vector); 
    vector.insert(vector.begin(), 10);
    vector_out("Insert_begin", vector);
    vector.insert(vector.begin() + vector.size()/2, 20);
    vector_out("Insert_middle", vector);
    vector.insert(vector.end(), 30);
    vector_out("Insert_end", vector);
    
    return 1;
}