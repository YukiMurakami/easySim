//
//  person.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/17.
//
//

#include "person.h"
#include <vector>
#include <map>

Person::Person(string name,string firstPlace,int apples) {
    _hasApples = apples;
    _name = name;
    _nowPlace = firstPlace;
    _lastPlace = "";
}

Person::Person() {
    _hasApples = 0;
    _name = "";
    _nowPlace = "";
    _lastPlace = "";
}

vector<string> makePersonVectorFromMap(map<string,Person> &person) {
    vector<string> persons;
    for(map<string,Person>::iterator it = person.begin();it != person.end();it++) {
        string personName = (*it).second._name;
        persons.push_back(personName);
    }
    
    return persons;
}
