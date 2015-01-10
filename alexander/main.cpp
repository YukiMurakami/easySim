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
    initAgent(persons,places,"initialize0110.txt");
    
    vector<string> personVector = makePersonVectorFromMap(persons);
    vector<string> placeVector = makePlaceVectorFromMap(places);
    
    vector<Constraint> constraints;
    //constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc(-Tyre).txt", personVector, placeVector,true);
    //constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc130.txt", personVector, placeVector,true);
    //constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc.txt", personVector, placeVector);
    initConstraintsFromAnnotation(constraints, "annotation.txt",personVector,placeVector,true);
    showConstraints(constraints);
    cout << constraints.size() << endl;
 //   showConstraintsPlaceDistribution(constraints);
 //   showAndOutputUnknownPlaceFromAnnotation("annotation.txt", personVector, placeVector,"unknownPlaces.txt");
   /*
    vector<Constraint> constraints2;
    initConstraintsFromAnnotation(constraints2, "annotation(-Tyre).txt",personVector,placeVector,true);
    showConstraints(constraints2);
    cout << constraints2.size() << endl;
    showDifferentConstraints(constraints, constraints2);
   */
 
    for(int i=0;i<100;i++) {
    
        round++;
        vector<Episode> episodes;
        episodes.clear();
        
    //    map<string,Person> persons;
    //    map<string,Place> places;
     //   initAgent(persons,places,"initialize1127.txt");
        
  
        
    
    
        
 //  constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc.txt", personVector, placeVector);
    //    showConstraintsPlaceDistribution(constraints);
     //   checkConstraintGenerator("annotation.txt", constraints);
     //    initConstraints(constraints, "constraint4.txt");
 //   showConstraints(constraints);
    
        doActionMCTS(persons, places, constraints,71,"wikipediaEpisodes0110-6.txt");
      /*
        question.show();
        cout << "ok:" << question.getAnswerFromEpisodesFile("episodes.txt") << endl;
    */
    
 //   }
   }
    
/*
    solve4selectionQuestion("question4-3.txt", personVector, placeVector, "wikipediaEpisodes0110.txt");
    
    Constraint constraint(getTimeFromBC(331, 1),getTimeFromBC(331, 12),"Alexander","the city",there_is);
    solvePlaceQuestion(constraint, "wikipediaEpisodes0108-2.txt");
    Constraint constraint2(getTimeFromBC(330, 1),getTimeFromBC(330, 12),"Alexander","the region",there_is);
    solvePlaceQuestion(constraint2, "wikipediaEpisodes0108-2.txt");
    Constraint constraint3(getTimeFromBC(333, 1),getTimeFromBC(333, 12),"Alexander","this coast",there_is);
    solvePlaceQuestion(constraint3, "wikipediaEpisodes0108-2.txt");
    Constraint constraint4(getTimeFromBC(329, 1),getTimeFromBC(329, 12),"Alexander","this coast",there_is);
    solvePlaceQuestion(constraint4, "wikipediaEpisodes0108-2.txt");
*/
    //outputCoreferenceTestFileFromUnknownPlaces("coreference_output", "unknownPlaces.txt");
    
    //outputCoreferenceTestFileFromWikipedia("coreference_output2.txt",constraints , "./old_episodes/alexander_wikipedia.txt");
    return 0;
}
