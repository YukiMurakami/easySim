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
    
    srand((unsigned)time(NULL));
    int round = 0;
    
    Question question;
    initQuestion(question, "question.txt");
    
 //   for(int i=0;i<100;i++) {
    
        round++;
        vector<Episode> episodes;
        episodes.clear();
        
        map<string,Person> persons;
        map<string,Place> places;
        initAgent(persons,places,"initialize.txt");
        
        vector<Constraint> constraints;
        initConstraints(constraints, "constraint.txt");
    
        string querry = "Alexander the Great was starting the pella in February BC 334 .";
        vector<string> personVector = makePersonVectorFromMap(persons);
        vector<string> placeVector = makePlaceVectorFromMap(places);
        makeQuestionFromString(querry, personVector,placeVector);
    
       // doActionMCTS(persons, places, constraints,140);
        
        question.show();
        cout << "ok:" << question.getAnswerFromEpisodesFile("episodes.txt") << endl;
//    }
 
    return 0;
}
