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
#include <math.h>
#include <algorithm>

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

bool isCoverConstraint(Constraint a,Constraint b) {
    return (a._beginTime <= b._endTime) && (a._endTime >= b._endTime) && (a._personName == b._personName) && (a._placeName == b._placeName) && (a._constraint == b._constraint);
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

double getCorrectRateWithConstraintAndEpisodeFile(Constraint con,string episodeFileName) {
    int count = 0;
    int correct = 0;
    
    ifstream ifs(episodeFileName.c_str());
    if(!ifs) {
        cout << "not found file '" << episodeFileName << "' @getCorrectRateWithConstraintAndEpisodeFile" << endl;
        return -1.0;
    }
    string buf;
    
    bool okFlag = false;
    
    while (getline(ifs,buf)) {
        vector<string> out = SpritString(buf, " ");
        if(out[0] == "newPlayout") {
            okFlag = false;
            count++;
        } else if(out[0] == "playoutEnd") {
            if(okFlag && con._constraint == there_is) correct++;
            if(!okFlag && con._constraint == there_is_no) correct++;
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
                if(con._constraint == there_is && con._placeName == placeName) {
                    okFlag = true;
                    // correct++;
                }
                if(con._constraint == there_is_no && con._placeName == placeName) {
                    //  correct++;
                    okFlag = true;
                }
                // count++;
            }
        }
    }
    
    if(count <= 0) {
        cout << "error: count <= 0 @getCorrectRateWithConstraintAndEpisodeFile" << endl;
        return -1.0;
    }
   // cout << "episode count:" << count << endl;
    return (double)correct/(double)count;
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

//対象の制約条件と時間条件、場所条件が重なるものを削除する。
vector<Constraint> removeConstraintsWithPlaceAndTimeCondition(vector<Constraint> constraints,Constraint c) {
    vector<Constraint> result;
    
    for(unsigned int i=0;i<constraints.size();i++) {
        if(!isCoverConstraint(constraints[i], c)) {
            result.push_back(constraints[i]);
        }
    }
    return result;
}


vector<Constraint> removeConstraintsWithPlaceName(vector<Constraint> constraints,vector<string> removePlaceName) {
    vector<Constraint> result;
    
    for(unsigned int i=0;i<constraints.size();i++) {
        string placeName = constraints[i]._placeName;
        bool isEqualFlag = false;
        for(unsigned int j=0;j<removePlaceName.size();j++) {
            if(isEqualStringWithoutOrthographicalVariant(placeName, removePlaceName[j])) {
                isEqualFlag = true;
                break;
            }
        }
        if(!isEqualFlag) {
            result.push_back(constraints[i]);
        }
    }
    return result;
}

vector<Constraint> selectRandomConstraintsWithRate(vector<Constraint> constraints,double rate) {
    vector<Constraint> result;
    
    int count = (int)(constraints.size() * rate);
    
    for(int i=0;i<count;i++) {
        unsigned int size = constraints.size();
        int index = xor128() % size;
        result.push_back(constraints[index]);
        constraints.erase(constraints.begin()+index);
    }
    
    return result;
}

void answerPlaceFromTimeConstraintWithBaseline(vector<Constraint> annotationConstraints,vector<Constraint> constraints) {
    int count = 0;
    int correct[10] = {0};
    
    for(int i=0;i<annotationConstraints.size();i++) {
        Constraint aCon = annotationConstraints[i];
        string answer = aCon._placeName;
        
        vector< pair<int,string> > scores;
        for(int ci=0;ci<constraints.size();ci++) {
            Constraint con = constraints[ci];
            int score = 0;
            if(con._id == aCon._id) continue;//same information should be removed.
            if(con._beginTime >= aCon._beginTime && con._endTime <= aCon._endTime) {
                if(con._placeName == aCon._placeName) continue;//same information should be removed.
                score = 0;
            } else {
                score = abs(con._beginTime - aCon._beginTime);
            }
            scores.push_back(make_pair(score, con._placeName));
        }
        
        sort(scores.begin(),scores.end());
        
        for(int rank=1;rank<=10;rank++) {
            bool isCorrect = false;
            for(int j=0;j<rank;j++) {
                if(scores[j].second == answer){
                    isCorrect = true;
                    break;
                }
            }
            if(isCorrect) {
                correct[rank-1]++;
                if(rank==1) aCon.show();
            }
        }
        count++;
    }
    
    cout << "Baseline" << endl;
    for(int rank=1;rank<=10;rank++) {
        double rate = (double)correct[rank-1] / (double)count;
        cout << "result: rank=" << rank << " :" << correct[rank-1] << "/" << count << " rate:" << rate << endl;
    }
}

void answerPlaceFromTimeConstraintByEpisode(string episodeFilePath,int maxIter,vector<Constraint> annotationConstraints,string initFilename,vector<string> &placeVector) {
    int count = 0;
    int correct[10] = {0};
    for(int i=0;i<annotationConstraints.size();i++) {
        annotationConstraints[i].show();
        
        char iter[50];
        sprintf(iter, "annealingAll%d/", maxIter);
        string iterPath = iter;
        char filename[100];
        sprintf(filename, "wikipediaEpisodes%s-%d-0.20-Annealing.txt",annotationConstraints[i]._placeName.c_str(),annotationConstraints[i]._id);
        string filenameString = filename;
        string episodeFilename = episodeFilePath + iterPath + filenameString;
        
        vector< pair<string,int> > answers = getMostAppearPlaceByEpisode(episodeFilename, annotationConstraints[i], initFilename, placeVector);
        for(int rank=1;rank<=10;rank++) {
            bool isCorrect = false;
            for(int j=0;j<(int)answers.size();j++) {
                if(answers[j].second > rank) break;
                if(answers[j].first == annotationConstraints[i]._placeName){
                    isCorrect = true;
                    break;
                }
            }
            if(isCorrect) {
                correct[rank-1]++;
            }
        }
        count++;
    }
    
    cout << "maxIter=" << maxIter << endl;
    for(int rank=1;rank<=10;rank++) {
        double rate = (double)correct[rank-1] / (double)count;
        cout << "result: rank=" << rank << " :" << correct[rank-1] << "/" << count << " rate:" << rate << endl;
    }
}

void answerPlaceFromTimeConstraint(string episodeFilePath,int maxIter,vector<Constraint> annotationConstraints,string initFilename,vector<string> &placeVector) {
    int count = 0;
    int correct[10] = {0};
    for(int i=0;i<annotationConstraints.size();i++) {
        annotationConstraints[i].show();
        
        char iter[50];
        sprintf(iter, "annealingAll%d/", maxIter);
        string iterPath = iter;
        char filename[100];
        sprintf(filename, "wikipediaEpisodes%s-%d-0.20-Annealing.txt",annotationConstraints[i]._placeName.c_str(),annotationConstraints[i]._id);
        string filenameString = filename;
        string episodeFilename = episodeFilePath + iterPath + filenameString;
        
        vector<string> answers = getMostAppearPlace(episodeFilename, annotationConstraints[i], initFilename, placeVector,annotationConstraints);
        for(int rank=1;rank<=10;rank++) {
            bool isCorrect = false;
            for(int j=0;j<rank;j++) {
                if(answers[j] == annotationConstraints[i]._placeName){
                    isCorrect = true;
                    break;
                }
            }
            if(isCorrect) {
                correct[rank-1]++;
            }
        }
        count++;
    }
    
    
    cout << "maxIter=" << maxIter << endl;
    for(int rank=1;rank<=10;rank++) {
        double rate = (double)correct[rank-1] / (double)count;
        cout << "result: rank=" << rank << " :" << correct[rank-1] << "/" << count << " rate:" << rate << endl;
    }
}

vector< pair<string,int> > getMostAppearPlaceByEpisode(string episodeFilename,Constraint constraint,string initFilename, vector<string> &placeVector) {
    bool isDebug = true;
    
    map<string,int> counter;
    for(unsigned int i=0;i<placeVector.size();i++) {
        counter.insert(map<string,int>::value_type(placeVector[i],0));
    }
    
    //cout << "ccc" << endl;
    
    vector< pair< vector<Episode>,vector<Constraint> > > episodesInfo = readEpisodesAndConstraintsFromEpisodeFile(episodeFilename, initFilename);
    for(int ei = 0;ei < episodesInfo.size();ei++) {
        vector<string> alreadyCountPlacenames;
        vector<Episode> episode = episodesInfo[ei].first;
        for(int t=0;t<episode.size();t++) {
            if(episode[t]._time >= constraint._beginTime && episode[t]._time <= constraint._endTime) {
                bool isFind = false;
                for(unsigned int i=0;i<alreadyCountPlacenames.size();i++) {
                    if(alreadyCountPlacenames[i] == episode[t]._persons[constraint._personName]._nowPlace) {
                        isFind = true;
                        break;
                    }
                }
                if(!isFind) {
                    counter[episode[t]._persons[constraint._personName]._nowPlace]++;
                    alreadyCountPlacenames.push_back(episode[t]._persons[constraint._personName]._nowPlace);
                }
            }
        }
    }
    
    vector< pair<int,string> > ranker;
    for(map<string,int>::iterator it = counter.begin();it != counter.end();it++) {
        ranker.push_back(make_pair((*it).second, (*it).first));
    }
    sort(ranker.begin(),ranker.end());
    
    //一時的にトップ１０のみ表示
    for(unsigned int i=(int)ranker.size()-1;i>(int)ranker.size()-11/*ranker.size()*/;i--) {
        if(isDebug) {
            cout << ranker[i].second << ":" << ranker[i].first << endl;
        }
    }
    
    vector< pair<string,int> > result;
    int nowRank = 0;
    int nowFrequency = 10000000;
    for(int i=(int)ranker.size()-1;i>=0;i--) {
        int frequency = ranker[i].first;
        string placename = ranker[i].second;
        if(nowFrequency > frequency) {
            nowRank = (int)result.size()+1;
            nowFrequency = frequency;
        }
        result.push_back(make_pair(placename, nowRank));
    }
    
    //一時的にトップ１０のみ表示
    /*
    for(unsigned int i=0;i<(int)result.size();i++) {
        if(isDebug) {
            cout << result[i].first << ":" << result[i].second << endl;
        }
    }
     */
    
    return result;
}

vector<string> getMostAppearPlace(string episodeFilename,Constraint constraint,string initFilename,vector<string> &placeVector,vector<Constraint> &annotationConstraints) {
    
    bool isDebug = true;
    
    map<string,int> counter;
    for(unsigned int i=0;i<placeVector.size();i++) {
        counter.insert(map<string,int>::value_type(placeVector[i],0));
    }
    
    //cout << "ccc" << endl;
    
    vector< pair< vector<Episode>,vector<Constraint> > > episodesInfo = readEpisodesAndConstraintsFromEpisodeFile(episodeFilename, initFilename);
    for(int ei = 0;ei < episodesInfo.size();ei++) {
        vector<Episode> episode = episodesInfo[ei].first;
        for(int t=0;t<episode.size();t++) {
            if(episode[t]._time >= constraint._beginTime && episode[t]._time <= constraint._endTime) {
                counter[episode[t]._persons[constraint._personName]._nowPlace]++;
            }
        }
    }
    
    vector< pair<int,string> > ranker;
    for(map<string,int>::iterator it = counter.begin();it != counter.end();it++) {
        ranker.push_back(make_pair((*it).second, (*it).first));
    }
    sort(ranker.begin(),ranker.end());
    
    //一時的にトップ１０のみ表示
    for(unsigned int i=(int)ranker.size()-1;i>(int)ranker.size()-11/*ranker.size()*/;i--) {
        if(isDebug) {
            bool isFind = false;
            for(unsigned int j=0;j<annotationConstraints.size();j++) {
                if(annotationConstraints[j]._beginTime >= constraint._beginTime && annotationConstraints[j]._endTime <= constraint._endTime) {
                    string annotationPlacename = annotationConstraints[j]._placeName;
                    if(ranker[i].second == annotationPlacename) {
                        isFind = true;
                        break;
                    }
                }
            }
            if(isFind) {
                cout << ranker[i].second << ":" << ranker[i].first << "is included in annotation data" << endl;
            } else {
                cout << ranker[i].second << ":" << ranker[i].first << endl;
            }
        }
    }
    
    vector<string> result;
    for(int i=(int)ranker.size()-1;i>=0;i--) {
        result.push_back(ranker[i].second);
    }
    //cout << "aaa" << endl;
    return result;
}

vector<string> getMostAppearPlace(string episodeFilename,Constraint constraint,string initFilename,vector<string> &placeVector) {
    
    bool isDebug = true;
    
    map<string,int> counter;
    for(unsigned int i=0;i<placeVector.size();i++) {
        counter.insert(map<string,int>::value_type(placeVector[i],0));
    }
    
    //cout << "ccc" << endl;
    
    vector< pair< vector<Episode>,vector<Constraint> > > episodesInfo = readEpisodesAndConstraintsFromEpisodeFile(episodeFilename, initFilename);
    for(int ei = 0;ei < episodesInfo.size();ei++) {
        vector<Episode> episode = episodesInfo[ei].first;
        for(int t=0;t<episode.size();t++) {
            if(episode[t]._time >= constraint._beginTime && episode[t]._time <= constraint._endTime) {
                counter[episode[t]._persons[constraint._personName]._nowPlace]++;
            }
        }
    }
    
    vector< pair<int,string> > ranker;
    for(map<string,int>::iterator it = counter.begin();it != counter.end();it++) {
        ranker.push_back(make_pair((*it).second, (*it).first));
    }
    sort(ranker.begin(),ranker.end());
    
    //一時的にトップ１０のみ表示
    for(unsigned int i=(int)ranker.size()-1;i>(int)ranker.size()-11/*ranker.size()*/;i--) {
        if(isDebug) {
            cout << ranker[i].second << ":" << ranker[i].first << endl;
        }
    }
    
    vector<string> result;
    for(int i=(int)ranker.size()-1;i>=0;i--) {
        result.push_back(ranker[i].second);
    }
    //cout << "aaa" << endl;
    return result;
}

void solveCoreference(Coreference coreference,string episodeFileName) {
    int sumCount = 0;

    Constraint con = coreference._constraint;
    
    map<string,bool> isFindMap;
    for(unsigned int i=0;i<coreference._candidates.size();i++) {
        isFindMap.insert(map<string,bool>::value_type(coreference._candidates[i],false));
        cout << coreference._candidates[i] << endl;
    }
    
    map<string,int> countMap;
    for(unsigned int i=0;i<coreference._candidates.size();i++) {
        countMap.insert(map<string,int>::value_type(coreference._candidates[i],0));
    }
    
    ifstream ifs(episodeFileName.c_str());
    if(!ifs) {
        cout << "error: not found file '" << episodeFileName << "' @solveCoreference" << endl;
        exit(0);
    }
    string buf;
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf, " ");
        if(out[0] == "newPlayout") {
            isFindMap.clear();
            for(unsigned int i=0;i<coreference._candidates.size();i++) {
                isFindMap.insert(map<string,bool>::value_type(coreference._candidates[i],false));
            }
        } else if(out[0] == "playoutEnd") {
            for(unsigned int i=0;i<coreference._candidates.size();i++) {
                if(isFindMap[coreference._candidates[i]]) {
                    countMap[coreference._candidates[i]]++;
                }
            }
            
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
                    if(isFindMap.find(placeName) != isFindMap.end()) {
                        isFindMap[placeName] = true;
                    }
                }
                if(con._constraint == there_is_no) {
                    //  correct++;
                }
                // count++;
            }
        }
    }
    cout << endl;
    int maxCount = -1;
    string maxPlace = "";
    for(map<string,int>::iterator it = countMap.begin();it != countMap.end();it++) {
        int count = (*it).second;
        cout << (*it).first << ":" << (*it).second << endl;
        if(maxCount <= count) {
            maxCount = count;
            maxPlace = (*it).first;
        }
    }
    cout << endl;
    
    cout << coreference._constraint._placeName << " is " << maxPlace << endl;
}

vector<Constraint> makeConstraintsFromConstraintFile(string filename) {
    cout << "makeConstraintsFromConstraintFile '" << filename << endl;
    vector<Constraint> result;
    
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error: not found file '" << filename << "' @makeConstraintsFromConstraintFile" << endl;
        exit(0);
    }
    
    string buf;
    int count = 0;
    while(getline(ifs,buf)) {
        count++;
        vector<string> out = SpritString(buf, ":");
        int beginTime = atoi(out[1].c_str());
        int endTime = atoi(out[2].c_str());
        string personName = out[3];
        string placeName = out[4];
        CONSTRAINT constraint = getEnumFromString(out[5]);
        int id = atoi(out[6].c_str());
        Constraint con(beginTime,endTime,personName,placeName,constraint,id);
        result.push_back(con);
    }
    return result;
}

vector<Constraint> makeConstraintsFromConstraintFile(string filename,int beginLine,int endLine) {
    cout << "makeConstraintsFromConstraintFile '" << filename << "' begin:" << beginLine << " end:" << endLine << endl;
    vector<Constraint> result;
    
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error: not found file '" << filename << "' @makeConstraintsFromConstraintFile" << endl;
        exit(0);
    }
    
    string buf;
    int count = 0;
    while(getline(ifs,buf)) {
        count++;
        if(count < beginLine || count > endLine) continue;
        vector<string> out = SpritString(buf, ":");
        int beginTime = atoi(out[1].c_str());
        int endTime = atoi(out[2].c_str());
        string personName = out[3];
        string placeName = out[4];
        CONSTRAINT constraint = getEnumFromString(out[5]);
        int id = atoi(out[6].c_str());
        Constraint con(beginTime,endTime,personName,placeName,constraint,id);
        result.push_back(con);
    }
    return result;
}

void FourChoiceQuestion::show() {
    for(unsigned int i=0;i<_constraints.size();i++) {
        cout << i+1 << " / " << _constraints[i].getString() << endl;
    }
    cout << "answer: " << _answerIndex+1 << endl;
}

FourChoiceQuestion makeFourChoiceQuestionByCorrectConstraintAndAnnotationConstraints(Constraint correctConstriant,vector<Constraint> annotationConstraints,vector<string> placeVector) {
    FourChoiceQuestion fourChoiceQuestion;
    
    //不正解選択肢を３つ作る だたしannotationConstraintsには正しい条件が入っているので、それを満たしていないか確認する
    for(int i=0;i<3;i++) {
        int randomBC=-1,beginTime=-1,endTime=-1;
        string randomPlace;
        
        bool okFlag = false;
        while(!okFlag) {
            randomBC = xor128()%12 + 323;
            //randomBC = getBCfromTime(correctConstriant._beginTime)[0];
            
            beginTime = getTimeFromBC(randomBC, 1);
            endTime = getTimeFromBC(randomBC, 12);
        
            randomPlace = placeVector[ xor128() % placeVector.size() ];
            
            okFlag = true;
            for(unsigned int j=0;j<annotationConstraints.size();j++) {
                if(isEqualStringWithoutOrthographicalVariant(randomPlace, annotationConstraints[j]._placeName)) {
                    if(beginTime <= annotationConstraints[j]._endTime && endTime >= annotationConstraints[j]._beginTime) {
                        okFlag = false;
                        break;
                    }
                }
            }
        }
        
        Constraint constraint(beginTime,endTime,"Alexander",randomPlace,there_is);
        
        fourChoiceQuestion._constraints.push_back(constraint);
    }
    
    int answerIndex = xor128() % 4;
    fourChoiceQuestion._constraints.insert(fourChoiceQuestion._constraints.begin()+answerIndex, correctConstriant);
    fourChoiceQuestion._answerIndex = answerIndex;
    
    return fourChoiceQuestion;
}


vector<FourChoiceQuestion> makeFourChoiceQuestionsWithCount(int count,vector<Constraint> annotationConstraints,vector<string> placeVector) {
    vector<FourChoiceQuestion> questions;
    
    for(unsigned int i=0;i<count;i++) {
        int annotationIndex = xor128() % annotationConstraints.size();
        FourChoiceQuestion fourChoiceQuestion = makeFourChoiceQuestionByCorrectConstraintAndAnnotationConstraints(annotationConstraints[annotationIndex], annotationConstraints, placeVector);
        questions.push_back(fourChoiceQuestion);
    }
    
    return questions;
}

void outputFourChoiceQuestions(vector<FourChoiceQuestion> questions,string filename) {
    ofstream ofs(filename.c_str());
    
    for(unsigned int i=0;i<questions.size();i++) {
        ofs << "fourChoiceQuestionStart ----------------------" << endl;
        for(unsigned int j=0;j < questions[i]._constraints.size();j++) {
            ofs << questions[i]._constraints[j].getString() << endl;
        }
        ofs << "answer " << questions[i]._answerIndex << endl;
        ofs << "fourChoiceQuestionEnd -----------------------" << endl;
    }
    
    ofs.close();
}

vector<FourChoiceQuestion> readFourChoiceQuestions(string filename) {
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error: not found file '" << filename << "' @readFourChoiceQuestions" << endl;
        exit(0);
    }
    
    vector<FourChoiceQuestion> questions;
    
    string buf;
    int answerIndex = -1;
    vector<Constraint> tmpConstraints;
    
    bool isStart = false;
    while(getline(ifs, buf)) {
        vector<string> out = SpritString(buf, " ");
        if(out[0] == "fourChoiceQuestionStart") {
            isStart = true;
            continue;
        }
        if(out[0] == "fourChoiceQuestionEnd") {
            FourChoiceQuestion question;
            question._answerIndex = answerIndex;
            copy(tmpConstraints.begin(),tmpConstraints.end(),back_inserter(question._constraints));
            questions.push_back(question);
            
            isStart = false;
            answerIndex = -1;
            tmpConstraints.clear();
            continue;
        }
        if(out[0] == "answer") {
            answerIndex = atoi(out[1].c_str());
            continue;
        }
        vector<string> out2 = SpritString(buf, ":");
        int beginTime = atoi(out2[1].c_str());
        int endTime = atoi(out2[2].c_str());
        string personName = out2[3];
        string placeName = out2[4];
        CONSTRAINT constraint = getEnumFromString(out2[5]);
        int id = atoi(out2[6].c_str());
        Constraint con(beginTime,endTime,personName,placeName,constraint,id);
        tmpConstraints.push_back(con);
    }
    
    return questions;
}

void solveFourChoiceQuestionsWithFilename(vector<FourChoiceQuestion> questions,vector<Constraint> annotationConstraints,string filename) {
    int count = 0;
    int correct = 0;
    
    int missCount = 0;
    int missZeroCorrect = 0;
    
    for(unsigned int qi=0;qi<questions.size();qi++) {
        int answerIndex = questions[qi]._answerIndex;
        Constraint answerConstraint = questions[qi]._constraints[answerIndex];
        
        vector<Constraint> hitConstraints;
        for(int n=0;n<annotationConstraints.size();n++) {
            if(isSameConstraint(answerConstraint, annotationConstraints[n])) {
                hitConstraints.push_back(annotationConstraints[n]);
            }
        }
        if(hitConstraints.size() <= 0) {
            // cout << "error: not found annotationConstraints @solveFourChoiceQuestions" << endl;
            continue;
        }
        
        double maxVal = -0.1;
        vector<int> maxIndexs;
        vector<double> values;
        for(unsigned int ci=0;ci<questions[qi]._constraints.size();ci++) {
            //cout << hitConstraints.size() << endl;
            for(unsigned int hi=0;hi<hitConstraints.size();hi++) {
                
                double val = getCorrectRateWithConstraintAndEpisodeFile(questions[qi]._constraints[ci], filename);
                values.push_back(val);
                //    cout << ci << ":" << val << endl;
                if(ci != answerIndex) {
                    missCount++;
                    if(val == 0) missZeroCorrect++;
                }
                if(val >= maxVal) {
                    if(val != maxVal) {
                        maxIndexs.clear();
                    }
                    maxVal = val;
                    maxIndexs.push_back(ci);
                }
            }
        }
        int maxIndex = maxIndexs[xor128() % maxIndexs.size()];
        //   cout << answerIndex << endl;
        //        cout << values[answerIndex] << endl;
        count++;
        if(maxIndex == answerIndex) {
            //正解
            correct++;
            /*
             cout << count << endl;
             int isShowCount = 0;
             for(unsigned int ci=0;ci<questions[qi]._constraints.size();ci++) {
             if(values[ci] > 0) isShowCount++;
             }
             if(isShowCount >= 3) {
             for(unsigned int ci=0;ci<questions[qi]._constraints.size();ci++) {
             questions[qi]._constraints[ci].show();
             cout << values[ci] << endl;
             }
             
             }
             */
        } else {
            //不正解
            
        }
    }
    double correctRate = (double)correct / (double)count;
    cout << "correct:" << correct << " count:" << count << endl;
    cout << "correctRate:" << correctRate << endl;
    
    cout << "missZeroCorrect:" << missZeroCorrect << " missCount:" << missCount << endl;
    cout << "missCorrectRate:" << (double)missZeroCorrect/(double)missCount << endl;
}

void solveFourChoiceQuestions(string prefix, vector<FourChoiceQuestion> questions,double rate,vector<Constraint> annotationConstraints,double Cp) {
    cout << "rate:" << rate << endl;
    int count = 0;
    int correct = 0;
    
    int missCount = 0;
    int missZeroCorrect = 0;
    
    for(unsigned int qi=0;qi<questions.size();qi++) {
        int answerIndex = questions[qi]._answerIndex;
        Constraint answerConstraint = questions[qi]._constraints[answerIndex];
        
        vector<Constraint> hitConstraints;
        for(int n=0;n<annotationConstraints.size();n++) {
            if(isSameConstraint(answerConstraint, annotationConstraints[n])) {
                hitConstraints.push_back(annotationConstraints[n]);
            }
        }
        if(hitConstraints.size() <= 0) {
           // cout << "error: not found annotationConstraints @solveFourChoiceQuestions" << endl;
            continue;
        }
        
        double maxVal = -0.1;
        vector<int> maxIndexs;
        vector<double> values;
        for(unsigned int ci=0;ci<questions[qi]._constraints.size();ci++) {
            //cout << hitConstraints.size() << endl;
            for(unsigned int hi=0;hi<hitConstraints.size();hi++) {
                char fileChara[40];
                if(Cp >= 0) {
                    sprintf(fileChara,"%s-%d-%3.2f",(hitConstraints[hi]._placeName).c_str(),hitConstraints[hi]._id,Cp);
                } else {
                    sprintf(fileChara,"%s-%d",(hitConstraints[hi]._placeName).c_str(),hitConstraints[hi]._id);
                }
                string filename = fileChara;
                char rateChara[10];
                sprintf(rateChara, "%2.1f",rate);
                string ratename = rateChara;
                
                string placeString = "place";
                if(prefix != "") placeString = "Place";
                
                double val = getCorrectRateWithConstraintAndEpisodeFile(questions[qi]._constraints[ci], "./" + prefix + placeString + ratename + "Episodes/wikipediaEpisodes" + filename + ".txt");
                values.push_back(val);
            //    cout << ci << ":" << val << endl;
                if(ci != answerIndex) {
                    missCount++;
                    if(val == 0) missZeroCorrect++;
                }
                if(val >= maxVal) {
                    if(val != maxVal) {
                        maxIndexs.clear();
                    }
                    maxVal = val;
                    maxIndexs.push_back(ci);
                }
            }
        }
        int maxIndex = maxIndexs[xor128() % maxIndexs.size()];
     //   cout << answerIndex << endl;
//        cout << values[answerIndex] << endl;
        count++;
        if(maxIndex == answerIndex) {
            //正解
            correct++;
            /*
            cout << count << endl;
            int isShowCount = 0;
            for(unsigned int ci=0;ci<questions[qi]._constraints.size();ci++) {
                if(values[ci] > 0) isShowCount++;
            }
            if(isShowCount >= 3) {
            for(unsigned int ci=0;ci<questions[qi]._constraints.size();ci++) {
                questions[qi]._constraints[ci].show();
                cout << values[ci] << endl;
            }
             
            }
             */
        } else {
            //不正解
            
        }
    }
    double correctRate = (double)correct / (double)count;
    cout << "correct:" << correct << " count:" << count << endl;
    cout << "correctRate:" << correctRate << endl;
    
    cout << "missZeroCorrect:" << missZeroCorrect << " missCount:" << missCount << endl;
    cout << "missCorrectRate:" << (double)missZeroCorrect/(double)missCount << endl;
}

void getArccuracyWithAnnotationData(vector<Constraint> annotationConstraints,int maxIter,string episodeFilePath,string initializeFilename) {
    int sumCount = 0;
    int sumCorrect = 0;
    
    for(int i=0;i<annotationConstraints.size();i++) {
        annotationConstraints[i].show();
        int count = 0;
        int correct = 0;
        
        char iter[50];
        sprintf(iter, "annealingAll%d/", maxIter);
        string iterPath = iter;
        char filename[100];
        sprintf(filename, "wikipediaEpisodes%s-%d-0.20-Annealing.txt",annotationConstraints[i]._placeName.c_str(),annotationConstraints[i]._id);
        string filenameString = filename;
        string episodeFilename = episodeFilePath + iterPath + filenameString;
        //cout << episodeFilename << endl;
        
        vector< pair< vector<Episode>,vector<Constraint> > > episodesInfo = readEpisodesAndConstraintsFromEpisodeFile(episodeFilename, initializeFilename);
        for(int ei = 0;ei < episodesInfo.size();ei++) {
            vector<Episode> episode = episodesInfo[ei].first;
            if(isEpisodeSatisfyWithConstraint(episode, annotationConstraints[i])) {
                correct++;
            }
            count++;
        }
        double rate = (double)correct / (double)count;
        cout << "result: " << correct << "/" << count << " rate:" << rate << endl;
        sumCount += count;
        sumCorrect += correct;
    }
    
    double allrate = (double)sumCorrect / (double)sumCount;
    cout << "All result: " << sumCorrect << "/" << sumCount << " rate:" << allrate << endl;
}

void solveFourChoiceQuestionsBySAWithIter(vector<FourChoiceQuestion> questions,vector<Constraint> annotationConstraints,int maxIter,string episodeFilePath,string resultPath) {
    int count = 0;
    int correct = 0;
    
    int missCount = 0;
    int missZeroCorrect = 0;
    
    cout << "RULE BASE" << endl;
    
    
    cout << "ANNEALED" << endl;
    char iter[30];
    sprintf(iter, "%d",maxIter);
    string iterString = iter;
    cout << "maxIter:" << iterString << endl;
    
    bool isFirstQuestion = true;
    bool isShow = false;
    int finishRate = 0;
    
    bool isDebug = true;
    
    for(unsigned int qi=0;qi<questions.size();qi++) {
        if(qi > 23) exit(0);
        if(isDebug) cout << "questionId:" << qi << endl;
        
        if(!isShow) {
            finishRate = (int)((double)qi / (double)questions.size() * 100);
            cout << finishRate/1.0 << "%" << endl;
            isShow = true;
        } else {
            if((int)((double)qi / (double)questions.size() * 100) != finishRate) {
                isShow = false;
            }
        }
        bool canUseQuestionFlag = true;
        int answerIndex = questions[qi]._answerIndex;
        Constraint answerConstraint = questions[qi]._constraints[answerIndex];
        
        vector<Constraint> hitConstraints;
        for(int n=0;n<annotationConstraints.size();n++) {
            if(isSameConstraint(answerConstraint, annotationConstraints[n])) {
                hitConstraints.push_back(annotationConstraints[n]);
            }
        }
        if(hitConstraints.size() <= 0) {
            // cout << "error: not found annotationConstraints @solveFourChoiceQuestions" << endl;
            continue;
        }
        
        double maxVal = -0.1;
        vector<int> maxIndexs;
        vector<double> values;
        for(unsigned int ci=0;ci<questions[qi]._constraints.size();ci++) {
            //cout << hitConstraints.size() << endl;
            for(unsigned int hi=0;hi<hitConstraints.size();hi++) {
                char fileChara[40];
                sprintf(fileChara,"wikipediaEpisodes%s-%d-0.20-Annealing.txt",(hitConstraints[hi]._placeName).c_str(),hitConstraints[hi]._id);
                string filename = fileChara;
                
                char iter[30];
                sprintf(iter, "%d",maxIter);
                string iterString = iter;
                string episodeFilename = episodeFilePath + "annealingAll" + iterString + "/" + filename;
                
                double val = -1;
                
                val = getCorrectRateWithConstraintAndEpisodeFile(questions[qi]._constraints[ci], episodeFilename);
                if(isFirstQuestion) {
                    isFirstQuestion = false;
                    cout << "using episode file :" << episodeFilename << endl;
                }
                
                if(val < 0) {
                    canUseQuestionFlag = false;
                    break;
                }
                values.push_back(val);
                
                if(isDebug) {
                    cout << ci << ":" << val << endl;
                    questions[qi]._constraints[ci].show();
                }
                if(ci != answerIndex) {
                    missCount++;
                    if(val == 0) missZeroCorrect++;
                }
                if(val >= maxVal) {
                    if(val != maxVal) {
                        maxIndexs.clear();
                    }
                    maxVal = val;
                    maxIndexs.push_back(ci);
                }
            }
            if(!canUseQuestionFlag) break;
        }
        if(!canUseQuestionFlag) continue;
        int maxIndex = maxIndexs[xor128() % maxIndexs.size()];
        
        if(isDebug) {
            cout << answerIndex << endl;
            cout << values[answerIndex] << endl;
        }
        
        count++;
        if(maxIndex == answerIndex) {
            //正解
            correct++;
            
        } else {
            //不正解
            
        }
    }
    double correctRate = (double)correct / (double)count;
    cout << "correct:" << correct << " count:" << count << endl;
    cout << "correctRate:" << correctRate << endl;
    
    cout << "missZeroCorrect:" << missZeroCorrect << " missCount:" << missCount << endl;
    cout << "missCorrectRate:" << (double)missZeroCorrect/(double)missCount << endl;
    
    double sigma = sqrt( count * correctRate * (1 - correctRate) ) / count;
    cout << "95%confidence: " << correctRate - 2*sigma << " - " << correctRate + 2*sigma << endl;
    cout << "sigma:" << sigma << endl;
    
    
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    char dateChara[100];
    sprintf(dateChara,"%04d%02d%02d-%02d%02d%02d-annealedAll-rule-iter%d.txt",pnow->tm_year+1900
            ,pnow->tm_mon+1
            ,pnow->tm_mday
            ,pnow->tm_hour
            ,pnow->tm_min
            ,pnow->tm_sec
            ,maxIter);
    string dateString = resultPath + dateChara;
    string outputFilename = dateString;
    
    ofstream ofs(outputFilename.c_str());
    
    ofs << "RULE BASE" << endl;
    
   
    ofs << "ANNEALED" << endl;
    char iter2[30];
    sprintf(iter2, "%d",maxIter);
    string iterString2 = iter2;
    ofs << "maxIter:" << iterString2 << endl;
    
    ofs << "correct:" << correct << " count:" << count << endl;
    ofs << "correctRate:" << correctRate << endl;
    
    ofs << "missZeroCorrect:" << missZeroCorrect << " missCount:" << missCount << endl;
    ofs << "missCorrectRate:" << (double)missZeroCorrect/(double)missCount << endl;
    
    ofs << "95%confidence: " << correctRate - 2*sigma << " - " << correctRate + 2*sigma << endl;
    ofs << "sigma:" << sigma << endl;
    
    ofs.close();
}

void solveFourChoiceQuestionsByPlayout(vector<FourChoiceQuestion> questions,vector<Constraint> annotationConstraints,double Cp,int playout,bool isAnnotation,bool isAnnealed,int annealingIter) {
    int count = 0;
    int correct = 0;
    
    int missCount = 0;
    int missZeroCorrect = 0;
    
    cout << "playouts:" << playout << endl;
    if(isAnnotation) {
        cout << "ANNOTATION" << endl;
    } else {
        cout << "RULE BASE" << endl;
    }
    if(isAnnealed) {
        cout << "ANNEALED" << endl;
        char iter[30];
        sprintf(iter, "%d",annealingIter);
        string iterString = iter;
        cout << "maxIter:" << iterString << endl;
    }
    bool isFirstQuestion = true;
    bool isShow = false;
    int finishRate = 0;
    for(unsigned int qi=0;qi<questions.size();qi++) {
        
        cout << "questionId:" << qi << " (" << correct << "/" << count << ")" << endl;
        //cout << "questionId:" << qi << endl;
        if(!isShow) {
            finishRate = (int)((double)qi / (double)questions.size() * 100);
            cout << finishRate/1.0 << "%" << endl;
            isShow = true;
        } else {
            if((int)((double)qi / (double)questions.size() * 100) != finishRate) {
                isShow = false;
            }
        }
        bool canUseQuestionFlag = true;
        int answerIndex = questions[qi]._answerIndex;
        Constraint answerConstraint = questions[qi]._constraints[answerIndex];
        
        vector<Constraint> hitConstraints;
        for(int n=0;n<annotationConstraints.size();n++) {
            if(isSameConstraint(answerConstraint, annotationConstraints[n])) {
                hitConstraints.push_back(annotationConstraints[n]);
            }
        }
        if(hitConstraints.size() <= 0) {
            // cout << "error: not found annotationConstraints @solveFourChoiceQuestions" << endl;
            continue;
        }
        
        double maxVal = -0.1;
        vector<int> maxIndexs;
        vector<double> values;
        for(unsigned int ci=0;ci<questions[qi]._constraints.size();ci++) {
            //cout << hitConstraints.size() << endl;
            for(unsigned int hi=0;hi<hitConstraints.size();hi++) {
                char fileChara[40];
                if(Cp >= 0) {
                    sprintf(fileChara,"%s-%d-%3.2f",(hitConstraints[hi]._placeName).c_str(),hitConstraints[hi]._id,Cp);
                } else {
                    sprintf(fileChara,"%s-%d",(hitConstraints[hi]._placeName).c_str(),hitConstraints[hi]._id);
                }
                string filename = fileChara;
                
                char playoutChara[10];
                sprintf(playoutChara, "%d",playout);
                string playoutString = playoutChara;
                string annealingString = "",annealingString2 = "";
                if(isAnnealed) {
                    char iter[30];
                    sprintf(iter, "%d",annealingIter);
                    string iterString = iter;
                    annealingString = "/top1/annealingAll" + iterString;
                    annealingString2 = "-Annealing";
                }
                double val = -1;
                if(isAnnotation) {
                    val = getCorrectRateWithConstraintAndEpisodeFile(questions[qi]._constraints[ci], "./playoutsAnnotation/" + playoutString + annealingString + "/wikipediaEpisodes" + filename + annealingString2 + ".txt");
                } else {
                    val = getCorrectRateWithConstraintAndEpisodeFile(questions[qi]._constraints[ci], "./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/playouts/" + playoutString + annealingString + "/wikipediaEpisodes" + filename + annealingString2 + ".txt");
                    if(isFirstQuestion) {
                        isFirstQuestion = false;
                        cout << "using episode file :" << "./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/playouts/" << playoutString << annealingString << "/wikipediaEpisodes" << filename << annealingString2 << ".txt" << endl;
                    }
                }
                if(val < 0) {
                    canUseQuestionFlag = false;
                    break;
                }
                values.push_back(val);
                cout << ci << ":" << val << endl;
                questions[qi]._constraints[ci].show();
                if(ci != answerIndex) {
                    missCount++;
                    if(val == 0) missZeroCorrect++;
                }
                if(val >= maxVal) {
                    if(val != maxVal) {
                        maxIndexs.clear();
                    }
                    maxVal = val;
                    maxIndexs.push_back(ci);
                }
            }
            if(!canUseQuestionFlag) break;
        }
        if(!canUseQuestionFlag) continue;
        int maxIndex = maxIndexs[xor128() % maxIndexs.size()];
        
        cout << answerIndex << endl;
        cout << values[answerIndex] << endl;
        count++;
        if(maxIndex == answerIndex) {
            //正解
            correct++;
            
        } else {
            //不正解
            
        }
    }
    double correctRate = (double)correct / (double)count;
    cout << "correct:" << correct << " count:" << count << endl;
    cout << "correctRate:" << correctRate << endl;
    
    cout << "missZeroCorrect:" << missZeroCorrect << " missCount:" << missCount << endl;
    cout << "missCorrectRate:" << (double)missZeroCorrect/(double)missCount << endl;
    
    double sigma = sqrt( count * correctRate * (1 - correctRate) ) / count;
    cout << "95%confidence: " << correctRate - 2*sigma << " - " << correctRate + 2*sigma << endl;
    cout << "sigma:" << sigma << endl;
    
    
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    char dateChara[100];
    sprintf(dateChara, "./../../../../data/local/murakami/alexanderTmp/result/result%04d%02d%02d-%02d%02d%02d-annealedAll-rule-iter%d.txt",pnow->tm_year+1900
            ,pnow->tm_mon+1
            ,pnow->tm_mday
            ,pnow->tm_hour
            ,pnow->tm_min
            ,pnow->tm_sec
            ,annealingIter);
    string dateString = dateChara;
    string outputFilename = dateString;
    
    ofstream ofs(outputFilename.c_str());
    
    ofs << "playouts:" << playout << endl;
    if(isAnnotation) {
        ofs << "ANNOTATION" << endl;
    } else {
        ofs << "RULE BASE" << endl;
    }
    if(isAnnealed) {
        ofs << "ANNEALED" << endl;
        char iter[30];
        sprintf(iter, "%d",annealingIter);
        string iterString = iter;
        ofs << "maxIter:" << iterString << endl;
    }
    ofs << "correct:" << correct << " count:" << count << endl;
    ofs << "correctRate:" << correctRate << endl;
    
    ofs << "missZeroCorrect:" << missZeroCorrect << " missCount:" << missCount << endl;
    ofs << "missCorrectRate:" << (double)missZeroCorrect/(double)missCount << endl;
    
    ofs << "95%confidence: " << correctRate - 2*sigma << " - " << correctRate + 2*sigma << endl;
    ofs << "sigma:" << sigma << endl;
    
    ofs.close();
}



