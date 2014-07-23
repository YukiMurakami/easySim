//
//  constraints.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#include "constraints.h"

Constraint::Constraint(int time,string person,string place,CONSTRAINT constraint) {
    _time = time;
    _personName = person;
    _placeName = place;
    _constraint = constraint;
}

Constraint::Constraint() {}

CONSTRAINT getEnumFromString(string constraint) {
    if(constraint == "there_is")    return there_is;
    if(constraint == "there_is_no") return there_is_no;
    return none;
}

