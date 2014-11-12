//
//  person.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/17.
//
//

#include "person.h"

Person::Person(string name,string firstPlace,int apples) {
    _hasApples = apples;
    _name = name;
    _nowPlace = firstPlace;
}

Person::Person() {
    _hasApples = 0;
    _name = "";
    _nowPlace = "";
}
