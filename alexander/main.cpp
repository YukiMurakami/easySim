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
    //constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc(-Tyre).txt", personVector, placeVector);
    //constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc130.txt", personVector, placeVector);
    //constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc.txt", personVector, placeVector);
    initConstraintsFromAnnotation(constraints, "annotation.txt",personVector,placeVector);
    showConstraints(constraints);
    cout << constraints.size() << endl;
    
    /*
    vector<Constraint> constraints2;
    constraints2 = makeConstraintsFromTestfile("alexander_wikipedia_bc.txt", personVector, placeVector);
    showConstraints(constraints2);
    cout << constraints2.size() << endl;
    showDifferentConstraints(constraints2, constraints);
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
    
        doActionMCTS(persons, places, constraints,140,"wikipediaEpisodes0108.txt");
      /*
        question.show();
        cout << "ok:" << question.getAnswerFromEpisodesFile("episodes.txt") << endl;
    */
    
 //   }
   }
    /*

    solve4selectionQuestion("question4-3.txt", personVector, placeVector, "wikipediaEpisodes0107-3.txt");
    
    Constraint constraint(getTimeFromBC(331, 1),getTimeFromBC(331, 12),"Alexander","the city",there_is);
    solvePlaceQuestion(constraint, "wikipediaEpisodes0107.txt");
    Constraint constraint2(getTimeFromBC(330, 1),getTimeFromBC(330, 12),"Alexander","the region",there_is);
    solvePlaceQuestion(constraint2, "wikipediaEpisodes0107.txt");
    Constraint constraint3(getTimeFromBC(333, 1),getTimeFromBC(333, 12),"Alexander","this coast",there_is);
    solvePlaceQuestion(constraint3, "wikipediaEpisodes0107.txt");
    Constraint constraint4(getTimeFromBC(329, 1),getTimeFromBC(329, 12),"Alexander","this coast",there_is);
    solvePlaceQuestion(constraint4, "wikipediaEpisodes0107.txt");
*/
     
    return 0;
}
