//
//  question.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/30.
//
//

#include "question.h"

#include <stdlib.h>

#include <fstream>

#include "utility.h"

double Question::getAnswerFromEpisodess(const vector<vector<Episode> > &episodess) {

    int count = 0;
    int correct = 0;
    
    for(unsigned int i=0;i<episodess.size();i++) {
        vector<Episode> personEpisode = getOnlyPersonEpisode(_personName, episodess[i]);
        for(unsigned int j=0;j<personEpisode.size();j++) {
            if(personEpisode[j]._time >= _beginTime && personEpisode[j]._time <= _endTime) {
                if(_constraint == there_is && _placeName == personEpisode[j]._persons[_personName]._nowPlace) {
                    correct++;
                }
                if(_constraint == there_is_no && _placeName != personEpisode[j]._persons[_personName]._nowPlace) {
                    correct++;
                }
                count++;
            }
        }
    }
    if(count <= 0) {
        cout << "error: count <= 0 @Question/getAnswerFromEpisodess" << endl;
        exit(0);
    }
    return (double)correct/(double)count;
}

double Question::getAnswerFromEpisodesFile(string filename) {
    int count = 0;
    int correct = 0;
    
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error: not found file '" << filename << "' @getAnswerFromEpisodesFile" << endl;
        exit(0);
    }
    string buf;
    
    bool okFlag = false;
    
    while (getline(ifs,buf)) {
        vector<string> out = SpritString(buf, " ");
        if(out[0] == "newPlayout") {
            okFlag = false;
            count++;
        } else if(out[0] == "playoutEnd") {
            if(okFlag) correct++;
        } else if(out[0] == "score") {
            
        } else if(out.size() == 3) {
            int time = atoi(out[0].c_str());
            string personName = out[1];
            string placeName = out[2];
            if(time >= _beginTime && time <= _endTime) {
               // cout << placeName << endl;
                if(_constraint == there_is && _placeName == placeName) {
                    okFlag = true;
                   // correct++;
                }
                if(_constraint == there_is_no && _placeName != placeName) {
                  //  correct++;
                    okFlag = true;
                }
               // count++;
            }
        }
    }
    
    if(count <= 0) {
        cout << "error: count <= 0 @Question/getAnswerFromEpisodess" << endl;
        exit(0);
    }
    cout << "episode count:" << count << endl;
    return (double)correct/(double)count;
}

Question makeQuestionFromString(string querry,vector<string> &persons ,vector<string> &places) {
    int beginTime = -1;
    int endTime = -1;
    int month = -1;
    string personName = "";
    string placeName = "";
    CONSTRAINT constraint = there_is;
    
    
    vector<string> out = SpritString(querry, " ");
    for(unsigned int i=0;i<out.size();i++) {
     //   cout << out[i] << endl;
        if(out[i] == "not" || out[i] == "no") constraint = there_is_no;
        
        int tmpmonth = getMonthFromString(out[i]);
        if(tmpmonth != -1) {
            month = tmpmonth;
        }
        
        if(out[i] == "BC") {
            if(i > 0) {
                int bc = atoi(out[i-1].c_str());
                if(month != -1) {
                    beginTime = getTimeFromBC(bc, month);
                    endTime = getTimeFromBC(bc,month);
                } else {
                    beginTime = getTimeFromBC(bc, 1);
                    endTime = getTimeFromBC(bc,12);
                }
            }
        }
        
        for(unsigned int j=0;j<persons.size();j++) {
            if(isEqualStringWithoutOrthographicalVariant(out[i], persons[j])) personName = out[i];
        }
        
        
        string anameString = "";
        if(isEqualStringWithoutCapital(out[i], "A") || isEqualStringWithoutCapital(out[i], "Alexandria")) {
            int size = (int)out.size() - i;
            for(int n=4;n>=1;n--) {
                if(size > n) size = n;
                string aString = "";
                for(unsigned int k=0;k<(unsigned int)size;k++) {
                    aString += out[i+k];
                    if((int)k != size-1) aString += " ";
                }
                string alexString = processingOfAlexandria(aString);
                if(alexString != aString) {
                    for(unsigned int j=0;j<places.size();j++) {
               //         cout << j << ":" << places[j] << endl;
                    }
                    anameString = alexString;
                    break;
                }
            }
        }
        
        for(unsigned int j=0;j<places.size();j++) {
            if(anameString == "") {
                if(isEqualStringWithoutOrthographicalVariant(out[i], places[j])) placeName = out[i];
            } else {
                if(isEqualStringWithoutOrthographicalVariant(anameString, places[j])) {
                    placeName = anameString;
                }
            }
        }
        if(anameString != "" && placeName == "") {
            for(unsigned int j=0;j<places.size();j++) {
                if(isEqualStringWithoutOrthographicalVariant(out[i], places[j])) placeName = out[i];
            }
        }
    }

    
    Question question;
    question._beginTime = beginTime;
    question._endTime = endTime;
    question._personName = personName;
    question._placeName = placeName;
    question._constraint = constraint;
   // question.show();
    return question;
}

void solvePlaceQuestion(Constraint constraint,string episodeFile) {
    map<string,int> places;
    
    string _personName = constraint._personName;
    string _placeName = constraint._placeName;
    int _beginTime = constraint._beginTime;
    int _endTime = constraint._endTime;
    CONSTRAINT _constraint = constraint._constraint;
    
    
    ifstream ifs(episodeFile.c_str());
    if(!ifs) {
        cout << "error: not found file '" << episodeFile << "' @solvePlaceQuestion" << endl;
        exit(0);
    }
    int sumCount = 0;
    string buf;
    while (getline(ifs,buf)) {
        vector<string> out = SpritString(buf, " ");
        if(out[0] == "newPlayout") {
         
        } else if(out[0] == "playoutEnd") {
            
        } else if(out[0] == "score") {
            
        } else if(out.size() == 3) {
            int time = atoi(out[0].c_str());
            string personName = out[1];
            string placeName = out[2];
            if(time >= _beginTime && time <= _endTime) {
                // cout << placeName << endl;
                if(_constraint == there_is) {
                    // correct++;
                    if(places.find(placeName) == places.end()) {
                        places.insert(map<string,int>::value_type(placeName,1));
                    } else {
                        places[placeName]++;
                    }
                    sumCount++;
                }
                if(_constraint == there_is_no) {
                    //  correct++;
                }
                // count++;
            }
        }
    }
    
    vector< pair<string,int> > rank;
    
    while(places.size() > 0) {
        int maxCount = -1;
        string maxPlace = "";
        for(map<string,int>::iterator it = places.begin();it != places.end();it++) {
            int count = (*it).second;
            if(count >= maxCount) {
                maxCount = count;
                maxPlace = (*it).first;
            }
        }
        rank.push_back(make_pair(maxPlace, maxCount));
        places.erase(maxPlace);
    }
    
    cout << "show place count /all:" << sumCount << endl;
    cout << "[" << _placeName << "]" << endl;
    for(unsigned int i=0;i<rank.size();i++) {
        cout << rank[i].first << ":" << (double)rank[i].second / sumCount << endl;
    }
    cout << endl;
}

void solve4selectionQuestion(string filename,vector<string> &persons,vector<string> &places,string episodeFile) {
    ifstream ifs(filename.c_str());
    string buf;
    vector<Question> questions;
    vector<string> questionStrings;
    vector<double> rates;
    double maxRate = -100;
    int answerIndex = -1;
    int count = 0;
    if(!ifs) {
        cout << "error: not found file @solve4selectionQuestion" << endl;
        exit(0);
    }
    while (getline(ifs,buf)) {
      //  cout << buf << endl;
        Question question = makeQuestionFromString(buf, persons, places);
        questionStrings.push_back(buf);
        questions.push_back(question);
     //   question.show();
        double answerRate = question.getAnswerFromEpisodesFile(episodeFile);
       // cout <<  answerRate << endl;
        rates.push_back(answerRate);
        if(maxRate <= answerRate) {
            answerIndex = count;
            maxRate = answerRate;
        }
        count++;
    }
    
    for(unsigned int i=0;i<questions.size();i++) {
        cout << "<" << i+1 << "> " << questionStrings[i] << endl;
        questions[i].show();
        cout << "point:" << rates[i] << endl;
    }
    cout << endl;
    cout << "answer : " << answerIndex+1 << endl;
}