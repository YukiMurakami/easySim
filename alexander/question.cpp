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
    
    for(int i=0;i<episodess.size();i++) {
        vector<Episode> personEpisode = getOnlyPersonEpisode(_personName, episodess[i]);
        for(int j=0;j<personEpisode.size();j++) {
            if(personEpisode[j]._time == _time) {
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
    string buf;
    
    while (getline(ifs,buf)) {
        vector<string> out = SpritString(buf, " ");
        if(out[0] == "newPlayout") {
            
        } else if(out[0] == "playoutEnd") {
            
        } else if(out.size() == 3) {
            int time = atoi(out[0].c_str());
            string personName = out[1];
            string placeName = out[2];
            if(time == _time) {
               // cout << placeName << endl;
                if(_constraint == there_is && _placeName == placeName) {
                    correct++;
                }
                if(_constraint == there_is_no && _placeName != placeName) {
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

Question makeQuestionFromString(string querry,vector<string> &persons ,vector<string> &places) {
    int time = -1;
    int month = -1;
    string personName = "";
    string placeName = "";
    CONSTRAINT constraint = there_is;
    
    
    vector<string> out = SpritString(querry, " ");
    for(int i=0;i<out.size();i++) {
        cout << out[i] << endl;
        if(out[i] == "not" || out[i] == "no") constraint = there_is_no;
        
        int tmpmonth = getMonthFromString(out[i]);
        if(tmpmonth != -1) {
            month = tmpmonth;
        }
        
        if(out[i] == "BC") {
            if(i != (int)out.size()-1) {
                int bc = atoi(out[i+1].c_str());
                if(month != -1) {
                    time = getTimeFromBC(bc, month);
                } else {
                    time = getTimeFromBC(bc, rand()%12);
                }
            }
        }
        
        for(unsigned int j=0;j<persons.size();j++) {
            if(isEqualStringWithoutCapital(out[i], persons[j])) personName = out[i];
        }
        for(unsigned int j=0;j<places.size();j++) {
            if(isEqualStringWithoutCapital(out[i], places[j])) placeName = out[i];
        }
    }

    
    Question question;
    question._time = time;
    question._personName = personName;
    question._placeName = placeName;
    question._constraint = constraint;
    question.show();
    return question;
}