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
#include "question.h"


int main(int argc, const char * argv[])
{
    for(int n = 0; n < 100; n++)
    {
        cout << xor128() << endl;
    }
    
    /*
    srand((unsigned)time(NULL));
    int round = 0;
    
    Question question;
    initQuestion(question, "question.txt");
    
 //   for(int i=0;i<50;i++) {
    
        round++;
        vector<Episode> episodes;
        episodes.clear();
        
        map<string,Person> persons;
        map<string,Place> places;
        initAgent(persons,places,"initialize.txt");
        
  
        vector<string> personVector = makePersonVectorFromMap(persons);
        vector<string> placeVector = makePlaceVectorFromMap(places);
     */
    /*
        vector<Constraint> constraints;
        constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc.txt", personVector, placeVector);
 //   showConstraints(constraints);
    cout << "made " << constraints.size() << " constraints" << endl;
    */
        // initConstraints(constraints, "constraint.txt");
    
    
        //    doActionMCTS(persons, places, constraints,140,"wikipediaEpisodes.txt");
      /*
        question.show();
        cout << "ok:" << question.getAnswerFromEpisodesFile("episodes.txt") << endl;
    */
      //  solve4selectionQuestion("question4.txt", personVector, placeVector, "wikipediaEpisodes.txt");
//    }
//    }
 
    return 0;
}
