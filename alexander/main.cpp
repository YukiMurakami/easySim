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
    
    
    
    map<string,Person> persons;
    map<string,Place> places;
    initAgent(persons,places,"initialize1127.txt");
    
    vector<string> personVector = makePersonVectorFromMap(persons);
    vector<string> placeVector = makePlaceVectorFromMap(places);
    
    vector<Constraint> constraints;
    constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc(-Tyre).txt", personVector, placeVector);
    showConstraints(constraints);
    
 //   for(int i=0;i<100;i++) {
    
        round++;
        vector<Episode> episodes;
        episodes.clear();
        
    //    map<string,Person> persons;
    //    map<string,Place> places;
     //   initAgent(persons,places,"initialize1127.txt");
        
  
        
    
    
        
 //  constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc.txt", personVector, placeVector);
      //  showConstraintsPlaceDistribution(constraints);
      //  checkConstraintGenerator("annotation.txt", constraints);
     //    initConstraints(constraints, "constraint4.txt");
 //   showConstraints(constraints);
    
   //     doActionMCTS(persons, places, constraints,140,"wikipediaEpisodes0102.txt");
      /*
        question.show();
        cout << "ok:" << question.getAnswerFromEpisodesFile("episodes.txt") << endl;
    */
        solve4selectionQuestion("question4-3.txt", personVector, placeVector, "wikipediaEpisodes0102.txt");
   
    Constraint constraint(getTimeFromBC(331, 1),getTimeFromBC(331, 12),"Alexander","the city",there_is);
    solvePlaceQuestion(constraint, "wikipediaEpisodes0102.txt");
 //   }
//   }
 
    return 0;
}
