#include "Sorts.h"
#include "Utils.h"
#include <iterator>
#include <iostream>

unsigned int GetPoint(std::list<unsigned int> myList, unsigned int value)
{

    // for (int i = 0; i < list.size(); i++)
    // {
    //     if (list.next() > value){
    //         return i;
    //     }
    // }

    //从list第一个iterator开始
    // std::list<unsigned int>::iterator itor = myList.begin();
    // while (itor != myList.end())
    // {
    //     std::cout << *itor++;
    // }

    int i = 0;
    auto beg = myList.begin();
    while(beg != myList.end()){
        if(*beg > value) break;
        ++beg;
        ++i;
    }
     std::cout << "point" <<i <<std::endl;
    return i;
}