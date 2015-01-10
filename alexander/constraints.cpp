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

int ENDSTEP = 71;

Constraint::Constraint(int beginTime,int endTime,string person,string place,CONSTRAINT constraint) {
    _beginTime = beginTime;
    _endTime = endTime;
    _personName = person;
    _placeName = place;
    _constraint = constraint;
}

Constraint::Constraint(int beginTime,int endTime,string person,string place,CONSTRAINT constraint,int id) {
    _beginTime = beginTime;
    _endTime = endTime;
    _personName = person;
    _placeName = place;
    _constraint = constraint;
    _id = id;
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
    cout << "constraint:" << _beginTime << ":" << _endTime << ":" << _personName << ":" << _placeName << ":" << getStringFromEnum(_constraint) <<  ":" << _id << endl;
}

bool isSameConstraint(Constraint a,Constraint b) {
    return (a._beginTime == b._beginTime) && (a._endTime == b._endTime) && (a._personName == b._personName) && (a._placeName == b._placeName) && (a._constraint == b._constraint);
}

vector<Constraint> makeAllConstraintsFromTestfile(string filename,vector<string> &persons,vector<string> &places) {
    cout << "now making constraints from file '" << filename << "'" << endl;
    vector<Constraint> result;
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error:not found file '" << filename << "' @makeConstraintsFromTestfile" << endl;
        exit(0);
    }
    string buf;
    
    int count = 0;
    while(getline(ifs,buf)) {
        cout << count++ << endl;
        Constraint constraint = makeQuestionFromString(buf, persons, places);
        result.push_back(constraint);
    }
    return result;
}

vector<Constraint> makeConstraintsFromAnnotationFile(string filename,vector<string> &persons,vector<string> &places) {
    cout << "now making constraints from file '" << filename << "'" << endl;
    vector<Constraint> result;
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error:not found file '" << filename << "' @makeConstraintsFromTestfile" << endl;
        exit(0);
    }
    string buf;
    
    int personCount = 0;
    int placeCount = 0;
    int timeCount = 0;
    
    int allCount = 0;
    
    while(getline(ifs,buf)) {
        allCount++;
        cout << allCount << endl;
        Constraint constraint = makeQuestionFromString(buf, persons, places);
        if(constraint._personName != "") personCount++;
        if(constraint._placeName != "") placeCount++;
        if(constraint._beginTime >= 0 && constraint._beginTime <= ENDSTEP && constraint._endTime >= 0 && constraint._endTime <= ENDSTEP) timeCount++;
        /*
         if(constraint._personName != "" && constraint._placeName != "" && (constraint._beginTime < 0 || constraint._endTime > ENDSTEP)) {
         constraint.show();
         }
         */
        //result.push_back(constraint);
        
        if(constraint._personName != "" && constraint._placeName != "" && constraint._beginTime >= 0 && constraint._beginTime <= ENDSTEP && constraint._endTime >= 0 && constraint._endTime <= ENDSTEP) {
            result.push_back(constraint);
            if(constraint._beginTime < 0 || constraint._beginTime > ENDSTEP || constraint._endTime < 0 || constraint._endTime > ENDSTEP) {
                cout << buf << endl;
                constraint.show();
                cout << endl;
            }
        }
        
        //  cout << buf << endl;
        //  constraint.show();
        // cout << endl;
    }
    cout << "made " << result.size() << " constraints" << endl;
    cout << "personOk:" << personCount << " placeOk:" << placeCount << " timeOk:" << timeCount << " allOk:" << result.size() << " all:" << allCount << endl;
    return result;
}

vector<Constraint> makeConstraintsFromTestfile(string filename,vector<string> &persons,vector<string> &places,bool isSameMode) {
    cout << "now making constraints from file '" << filename << "'" << endl;
    vector<Constraint> result;
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error:not found file '" << filename << "' @makeConstraintsFromTestfile" << endl;
        exit(0);
    }
    string buf;
    
    int personCount = 0;
    int placeCount = 0;
    int timeCount = 0;
    
    int allCount = 0;
    
    while(getline(ifs,buf)) {
        allCount++;
        cout << allCount << endl;
        Constraint constraint = makeQuestionFromString(buf, persons, places);
        if(constraint._personName != "") personCount++;
        if(constraint._placeName != "") placeCount++;
        if(constraint._beginTime >= 0 && constraint._beginTime <= ENDSTEP && constraint._endTime >= 0 && constraint._endTime <= ENDSTEP) timeCount++;
        /*
        if(constraint._personName != "" && constraint._placeName != "" && (constraint._beginTime < 0 || constraint._endTime > ENDSTEP)) {
            constraint.show();
        }
         */
        //result.push_back(constraint);
    //アノテーションチェックのときは、情報がそろってなくても制約条件を生成するようにする
        if(constraint._personName != "" && constraint._placeName != "" && constraint._beginTime >= 0 && constraint._beginTime <= ENDSTEP && constraint._endTime >= 0 && constraint._endTime <= ENDSTEP) {
            constraint._id = allCount;
            
            if(isSameMode) {
                bool isFind = false;
                for(unsigned int i=0;i<result.size();i++) {
                    if(isSameConstraint(result[i],constraint)) {
                        isFind = true;
                        break;
                    }
                }
                if(!isFind) {
                    result.push_back(constraint);
                }
            } else {
                result.push_back(constraint);
            }
            /*
            if(constraint._beginTime < 0 || constraint._beginTime > ENDSTEP || constraint._endTime < 0 || constraint._endTime > ENDSTEP) {
                cout << buf << endl;
                constraint.show();
                cout << endl;
            }
             */
        }
        
      //  cout << buf << endl;
      //  constraint.show();
       // cout << endl;
    }
    cout << "made " << result.size() << " constraints" << endl;
    cout << "personOk:" << personCount << " placeOk:" << placeCount << " timeOk:" << timeCount << " allOk:" << result.size() << " all:" << allCount << endl;
    return result;
}

void showConstraints(vector<Constraint> &constraints) {
    cout << "constraints show ------" << endl;
    for(unsigned i=0;i<constraints.size();i++) {
        constraints[i].show();
    }
    cout << "constraints show end ---" << endl;
}