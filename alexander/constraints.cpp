//
//  constraints.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#include "constraints.h"
#include "question.h"

#include <fstream>

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

string getStringFromEnum(CONSTRAINT constraint) {
    if(constraint == there_is) return "there_is";
    if(constraint == there_is_no) return "there_is_no";
    return "";
}

void Constraint::show() {
    cout << "constraint:" << _time << ":" << _personName << ":" << _placeName << ":" << getStringFromEnum(_constraint) << endl;
}

vector<Constraint> makeConstraintsFromTestfile(string filename,vector<string> &persons,vector<string> &places) {
    cout << "now making constraints from file[" << filename << "]" << endl;
    vector<Constraint> result;
    ifstream ifs(filename.c_str());
    string buf;
    while(getline(ifs,buf)) {
        Constraint constraint = makeQuestionFromString(buf, persons, places);
        if(constraint._personName != "" && constraint._placeName != "" && constraint._time >= 0 && constraint._time <= 140) {
            result.push_back(constraint);
        }
    }
    cout << "made " << result.size() << " constraints" << endl;
    return result;
}

void showConstraints(vector<Constraint> &constraints) {
    cout << "constraints show ------" << endl;
    for(unsigned i=0;i<constraints.size();i++) {
        constraints[i].show();
    }
    cout << "constraints show end ---" << endl;
}