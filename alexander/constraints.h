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
#include <vector>

using namespace std;

enum CONSTRAINT {
    none,
    there_is,
    there_is_no,
};

class Constraint {
public:
    int _beginTime;
    int _endTime;
    string _personName;
    string _placeName;
    CONSTRAINT _constraint;
    
    int _id;
    
    Constraint(int beginTime,int endTime,string person,string place,CONSTRAINT constraint);
    Constraint(int beginTime,int endTime,string person,string place,CONSTRAINT constraint,int id);
    Constraint();
    void show();
};

CONSTRAINT getEnumFromString(string constraint);
string getStringFromEnum(CONSTRAINT constraint);

vector<Constraint> makeConstraintsFromTestfile(string filename,vector<string> &persons,vector<string> &places);

//条件にあわないもの、情報がそろってないものも制約条件として生成する
vector<Constraint> makeAllConstraintsFromTestfile(string filename,vector<string> &persons,vector<string> &places);
void showConstraints(vector<Constraint> &constraints);

#endif /* defined(__easySimu__constraints__) */
