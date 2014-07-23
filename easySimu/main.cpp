//
//  main.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/17.
//
//

#include <iostream>
#include <map>
#include <stdlib.h>
#include <time.h>

#include "place.h"
#include "person.h"
#include "utility.h"
#include "episode.h"


int main(int argc, const char * argv[])
{
    srand((unsigned)time(NULL));
    int round = 0;
    double checkval = 1.0;
    
    Question question;
    initQuestion(question, "question.txt");
    
    int sumRound = 0;
//    for(int y=1;y<=100;y++) {
//        round = 0;
//        checkval = 1.0;
    while(true) {
        round++;
        //checkval *= 0.99999;
        //cout << "round:" << round << "checkval:" << checkval << endl;
        vector<Episode> episodes;
        episodes.clear();
        vector<Constraint> constraints;
        initConstraints(constraints,"constraint.txt");
        map<string,Person> persons;
        map<string,Place> places;
        initAgent(persons,places,"initialize.txt");
        
        vector< vector<string> > actionLists;
        for(int i=0;i<persons.size();i++) {
            vector<string> actionList;
            actionLists.push_back(actionList);
        }
        doActionMCTS(persons, places, constraints,actionLists);
        
        /*
    
        int time = 0;
    
        //show(time,places);
    
        while(time < 10) {
            time++;
            //doAction(persons, places);
            doActionUCB(persons, places, time, 10, constraints, episodes);
            Episode episode(time,persons,places);
            episodes.push_back(episode);
            //show(time,places);
        }
    
        double val = checkEpisode(episodes, constraints);
       // if(val >= checkval) {
            showEpisode(episodes);
            if(checkQuestion(episodes, question)) {
               // cout << "true" << endl;
            } else {
               // cout << "false" << endl;
            }
            cout << "val:" << val << endl;
            //break;
        //}
         
         */
        break;
    }
        
        sumRound += round;
       // cout << ((double)sumRound / y) << ":" << y << endl;
        cout << round << endl;
 //   }
    
    
    return 0;
}
