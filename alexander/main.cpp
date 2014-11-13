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
    
    Question question;
    initQuestion(question, "question.txt");
    
    for(int i=0;i<100;i++) {
    
        round++;
        vector<Episode> episodes;
        episodes.clear();
        
        map<string,Person> persons;
        map<string,Place> places;
        initAgent(persons,places,"initialize.txt");
        
        vector<Constraint> constraints;
        initConstraints(constraints, "constraint.txt");
        
        
        doActionMCTS(persons, places, constraints,140);
    }
 
    return 0;
}
