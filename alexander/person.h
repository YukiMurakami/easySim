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
#include <vector>
#include <map>

using namespace std;

class Person {
public:
    int _hasApples;
    string _name;
    string _nowPlace;
    
    Person(string name,string firstPlace,int apples);
    Person();
 
};

vector<string> makePersonVectorFromMap(map<string,Person> &person);

#endif /* defined(__easySimu__person__) */
