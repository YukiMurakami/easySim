//
//  person.h
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/17.
//
//

#ifndef __easySimu__person__
#define __easySimu__person__

#include <iostream>

using namespace std;

class Person {
public:
    int _hasApples;
    string _name;
    string _nowPlace;
    
    Person(string name,string firstPlace,int apples);
    Person();
 
};

#endif /* defined(__easySimu__person__) */
