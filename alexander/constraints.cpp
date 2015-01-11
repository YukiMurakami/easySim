//
//  constraints.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#include "constraints.h"
#include "question.h"
#include "utility.h"

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

string Constraint::getString() {
    
    char s[100];
    sprintf(s,"constraint:%d:%d:%s:%s:%s:%d",_beginTime,_endTime,_personName.c_str(),_placeName.c_str(),(getStringFromEnum(_constraint)).c_str(),_id);
    
    string ss = s;
    
    return ss;
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

vector<Constraint> makeConstraintsFromTestfileWithError(string filename,vector<string> &persons,vector<string> &places,double rate,bool isSameMode) {
    vector<Constraint> result = makeConstraintsFromTestfile(filename, persons, places, isSameMode);
    
    vector<Constraint> result2;
    vector<Constraint> remove;
    
    for(unsigned int i=0;i<result.size();i++) {
        if(xor128()%1000 >= 1000*rate) {
            result2.push_back(result[i]);
        } else {
            remove.push_back(result[i]);
        }
    }
    
    ofstream ofs("removeConstraints.txt");
    for(unsigned int i=0;i<remove.size();i++) {
        ofs << remove[i].getString() << endl;
    }
    ofs.close();
    
    return result2;
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




Coreference::Coreference(string title,Constraint constraint,vector<string> candidates) {
    _title = title;
    _constraint = constraint;
    _candidates = candidates;
}

void Coreference::show() {
    cout << _title << endl;
    _constraint.show();
    for(unsigned int i=0;i<_candidates.size();i++) {
        cout << _candidates[i];
        if(i != _candidates.size() - 1) {
            cout << ",";
        }
    }
    cout << endl;
}

vector<Coreference> makeCoreferencesFromTextfile(string filename) {
    vector<Coreference> coreferences;
    
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error: not found file '" << filename << "' @makeCoreferenceFromTextfile" << endl;
        exit(0);
    }
    
    string buf;
    while(getline(ifs, buf)) {
        if(buf == "") break;
        vector<string> out = SpritString(buf, ",");
        vector<string> candidates;
        for(unsigned int i=4;i<out.size();i++) {
            candidates.push_back(removeOrthographicalVariantString(out[i]));
        }
        int bc = atoi(out[1].c_str());
        int month = getMonthFromString(out[2]);
        int beginTime = getTimeFromBC(bc, 1);
        int endTime = getTimeFromBC(bc, 12);
        if(month != -1) {
            beginTime = getTimeFromBC(bc, month);
            endTime = getTimeFromBC(bc, month);
        }
        Constraint constraint(beginTime,endTime,"Alexander",out[3],there_is);
        Coreference coreference(out[0],constraint,candidates);
        coreferences.push_back(coreference);
    }
    
    return coreferences;
}

void solveCoreference(Coreference coreference,string episodeFileName) {
    int sumCount = 0;
    map<string,int> places;
    Constraint con = coreference._constraint;
    
    ifstream ifs(episodeFileName.c_str());
    if(!ifs) {
        cout << "error: not found file '" << episodeFileName << "' @solveCoreference" << endl;
        exit(0);
    }
    string buf;
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf, " ");
        if(out[0] == "newPlayout") {
            
        } else if(out[0] == "playoutEnd") {
            
        } else if(out[0] == "score") {
            
        } else if(out.size() >= 3) {
            int time = atoi(out[0].c_str());
            string personName = out[1];
            string placeName = "";
            for(unsigned int i=0;i<out.size() - 2;i++) {
                placeName += out[i+2];
                if(i != out.size()-3) {
                    placeName += " ";
                }
            }
            if(time >= con._beginTime && time <= con._endTime) {
                // cout << placeName << endl;
                if(con._constraint == there_is) {
                    // correct++;
                    if(places.find(placeName) == places.end()) {
                        places.insert(map<string,int>::value_type(placeName,1));
                    } else {
                        places[placeName]++;
                    }
                    sumCount++;
                }
                if(con._constraint == there_is_no) {
                    //  correct++;
                }
                // count++;
            }
        }
    }
    
    
    int maxCount = -1;
    string maxPlace = "";
    for(unsigned int i=0;i<coreference._candidates.size();i++) {
        string placeName = coreference._candidates[i];
        cout << placeName << " hits " << places[placeName] << endl;
        if(maxCount <= places[placeName]) {
            maxCount = places[placeName];
            maxPlace = placeName;
        }
    }
    
    cout << coreference._constraint._placeName << " is " << maxPlace << endl;
}