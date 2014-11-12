//
//  constraints.h
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#ifndef __easySimu__constraints__
#define __easySimu__constraints__

#include <iostream>
#include <string>

using namespace std;

enum CONSTRAINT {
    none,
    there_is,
    there_is_no,
};

class Constraint {
public:
    int _time;
    string _personName;
    string _placeName;
    CONSTRAINT _constraint;
    
    Constraint(int time,string person,string place,CONSTRAINT constraint);
    Constraint();
    void show();
};

CONSTRAINT getEnumFromString(string constraint);

#endif /* defined(__easySimu__constraints__) */
