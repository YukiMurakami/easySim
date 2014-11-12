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
 //   int sum = 0;
 //   clock_t start,end;
    
    vector<double> result;
    vector<double> resultTime;
    
    srand((unsigned)time(NULL));
    int round = 0;
    
    Question question;
    initQuestion(question, "question.txt");
    
    //for(int C=1;C<=7;C++) {
     //   start = clock();
  //  for(int R=1;R<=100;R++) {
        round++;
        vector<Episode> episodes;
        episodes.clear();
        
        map<string,Person> persons;
        map<string,Place> places;
        initAgent(persons,places,"initialize.txt");
        
        vector<Constraint> constraints;
        initConstraints(constraints, "constraint.txt");
        
        
  //      int sumPlayout = doActionMCTS(persons, places, constraints,140);
  //      int sumPlayout = randomAction(persons, places, constraints);
      //  allSearchAction(persons, places, constraints);
        //sum += sumPlayout;
       // cout << sum/(double)R << "/" << R << endl;
        /*
        showEpisode(resultEpisodes);
        if(checkQuestion(resultEpisodes, question)) {
            cout << "true" << endl;
        } else {
            cout << "false" << endl;
        }
         */
  //  }
      //  end = clock();
      //  result.push_back(sum/100.0);
      //  sum = 0;
      //  double time = (double)(end-start)/CLOCKS_PER_SEC;
      //  cout << "time:" << time << endl;
      //  resultTime.push_back(time/100.0);
  //  }
/*
    for(int i=0;i<result.size();i++) {
        cout << i << ":" << result[i] << "playout" << ":" << resultTime[i] << "sec" << endl;
    }
    */
    return 0;
}
