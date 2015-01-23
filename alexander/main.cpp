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
    /*
    vector<string> files;
    files.push_back("wikipediaEpisodes0111-3.txt");
    files.push_back("wikipediaEpisodes0113.txt");
    files.push_back("wikipediaEpisodes0113-2.txt");
    files.push_back("wikipediaEpisodes0113-3.txt");
    files.push_back("wikipediaEpisodes0113-4.txt");
    
    showErrorConstraintsFromEpisodeFiles(files);
    
    return 0;
    */
    
    srand((unsigned)time(NULL));
    int round = 0;
    /*
    Question question;
    initQuestion(question, "question.txt");
    */

    
    
    map<string,Person> persons;
    map<string,Place> places;
    initAgent(persons,places,"initialize0110.txt");
    
    map<string, map<string,int> > shortestInfo = getShortestLengthToNextConstraint(places);
    string a = "Samarkand";
    string b = "Granicus";
    cout << a << ":" << b << ":" << shortestInfo[a][b] << endl;
    return 0;
    vector<string> personVector = makePersonVectorFromMap(persons);
    vector<string> placeVector = makePlaceVectorFromMap(places);
    
    vector<Constraint> constraints;
  //  constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc(-Tyre).txt", personVector, placeVector,true);
  //  constraints = makeConstraintsFromTestfileWithError("alexander_wikipedia_bc(-Tyre).txt", personVector, placeVector, 0.8, true);
    //constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc130.txt", personVector, placeVector,true);
    constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc.txt", personVector, placeVector,true);
 //   showConstraints(constraints);
    //constraints = makeConstraintsFromConstraintFile("constraint0114.txt", 1, 32);
    cout << constraints.size() << endl;
    /*
    vector<Constraint> annotationConstraints;
    initConstraintsFromAnnotation(annotationConstraints, "annotation.txt",personVector,placeVector,true);
    showConstraints(annotationConstraints);
    cout << annotationConstraints.size() << endl;
     */
    vector<Constraint> annotationConstraints = makeConstraintsFromConstraintFile("constraint0114.txt",16,16);
    showConstraints(annotationConstraints);
    cout << annotationConstraints.size() << endl;
    /*
    showDifferentConstraints(constraints, annotationConstraints);
    return 0;
    */
    /*
    for(int i=0;i<32;i++) {
        annotationConstraints = makeConstraintsFromConstraintFile("constraint0114.txt",i+1,i+1);
    */
//        vector<FourChoiceQuestion> questions = makeFourChoiceQuestionsWithCount(100, annotationConstraints, placeVector);
     
     //   vector<FourChoiceQuestion> questions = readFourChoiceQuestions("fourChoiceQuestions58-52.txt");
        /*
        for(unsigned int i=0;i<questions.size();i++) {
            questions[i].show();
        }
         
        outputFourChoiceQuestions(questions, "fourChoiceQuestions.txt");
         */
        //vector<FourChoiceQuestion> questions = readFourChoiceQuestions("fourChoiceQuestions.txt");
        
      //  solveFourChoiceQuestions("",questions, 0.6, annotationConstraints);
     //   solveFourChoiceQuestions("",questions, 0.8, annotationConstraints);
     //   solveFourChoiceQuestions("",questions, 1.0, annotationConstraints);
     //   solveFourChoiceQuestions("annotation", questions, 1.0, annotationConstraints,-1);
    /*
        solveFourChoiceQuestions("question16", questions, 1.0, annotationConstraints,0.5);
    solveFourChoiceQuestions("question16", questions, 1.0, annotationConstraints,1.0);
    solveFourChoiceQuestions("question16", questions, 1.0, annotationConstraints,1.4);
    solveFourChoiceQuestions("question16", questions, 1.0, annotationConstraints,2.0);
    return 0;
     */
  //  }
 //   return 0;
    
  //  return 0;
    /*
    vector<Coreference> coreferences = makeCoreferencesFromTextfile("co-reference.txt");
    for(unsigned int i=0;i<coreferences.size();i++) {
        coreferences[i].show();
        solveCoreference(coreferences[i], "./place1.0Episodes/wikipediaEpisodesBabylon-52.txt");
    }
     
    
    return 0;
   */
    
  //  return 0;
    
 //   showConstraintsPlaceDistribution(constraints);
 //   showAndOutputUnknownPlaceFromAnnotation("annotation.txt", personVector, placeVector,"unknownPlaces.txt");
    
    /*
    vector<string> placeNames;
    placeNames.push_back("Tyre");
    placeNames.push_back("Granicus");
    placeNames.push_back("Gaugamela");
    placeNames.push_back("Issus");
    vector<Constraint> constraints2 = removeConstraintsWithPlaceName(constraints, placeNames);
    showConstraints(constraints2);
    cout << constraints2.size() << endl;
     
     */
//    showDifferentConstraints(constraints, constraints2);
   
    
  //  return 0;
 //   double valSum = 0.0;
 //   int valCount = 0;
    
    for(int n=0;n<annotationConstraints.size();n++) {
        
        vector<string> placeNames;
        placeNames.push_back(annotationConstraints[n]._placeName);
        vector<Constraint> constraints2 = removeConstraintsWithPlaceName(constraints, placeNames);
        //showConstraints(constraints2);
        cout << constraints2.size() << endl;
        
        double cp = 0.2;
        char fileChara[40];
        sprintf(fileChara,"%s-%d-%3.2f",(annotationConstraints[n]._placeName).c_str(),annotationConstraints[n]._id,cp);
        string filename = fileChara;
        
    //    for(int i=0;i<20;i++) {
        //    cout << filename << ":" << (i+1) << "/20 round" << endl;
            vector<Constraint> tmpConstraints = selectRandomConstraintsWithRate(constraints2, 1.0);
            cout << tmpConstraints.size() << endl;
            round++;
            vector<Episode> episodes;
            episodes.clear();
            
        
     //       doActionMCTS(persons, places, tmpConstraints,71,"wikipediaEpisodes" + filename + ".txt",cp);
        doActionMCTSWithRank(persons, places, tmpConstraints, 71, "wikipediaEpisodes" + filename + ".txt", cp);

  //      }
        
        /*
        double val = getCorrectRateWithConstraintAndEpisodeFile(annotationConstraints[n], "./place1.0Episodes/wikipediaEpisodes" + filename + ".txt");
        cout << (n+1) << ":" << filename << ":" << val << endl;
        if(val != -1) {
           valSum += val;
            valCount++;
        }
         */
        
    }
    /*
    cout << "average : " << valSum/valCount << endl;
  
    
    

    solve4selectionQuestion("question4-4.txt", personVector, placeVector, "old_episodes/wikipediaEpisodes0111-3.txt");
    solve4selectionQuestion("question4-4.txt", personVector, placeVector, "old_episodes/wikipediaEpisodes0113.txt");
    solve4selectionQuestion("question4-4.txt", personVector, placeVector, "old_episodes/wikipediaEpisodes0113-2.txt");
    solve4selectionQuestion("question4-4.txt", personVector, placeVector, "old_episodes/wikipediaEpisodes0113-3.txt");
    solve4selectionQuestion("question4-4.txt", personVector, placeVector, "old_episodes/wikipediaEpisodes0113-4.txt");
     */
/*
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
