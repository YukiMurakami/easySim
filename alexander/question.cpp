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
                cout << placeName << endl;
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