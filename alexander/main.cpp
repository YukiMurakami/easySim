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

#include "annealing.h"


void showErrorConstraintsFromEpisodeFiles() {
     vector<string> files;
     files.push_back("wikipediaEpisodes0111-3.txt");
     files.push_back("wikipediaEpisodes0113.txt");
     files.push_back("wikipediaEpisodes0113-2.txt");
     files.push_back("wikipediaEpisodes0113-3.txt");
     files.push_back("wikipediaEpisodes0113-4.txt");
     
     showErrorConstraintsFromEpisodeFiles(files);
}


int main(int argc, const char * argv[])
{
    
    srand((unsigned)time(NULL));
    int round = 0;
    
    string initializeFilename = "initialize0110.txt";
    
    int startId,endId;
    if(argc <= 2) {
        startId = 1;
        endId = 32;
    } else {
        startId = atoi(argv[1]);
        endId = atoi(argv[2]);
    }
    
    cout << startId << " " << endId << endl;
    
    
    
    /*  ４択問題 一問
    Question question;
    initQuestion(question, "question.txt");
    */
    
     //Coreference Resolurion
    /*
    vector<Coreference> coreferences = makeCoreferencesFromTextfile("co-reference.txt");
    for(unsigned int i=0;i<coreferences.size();i++) {
        coreferences[i].show();
        
        solveCoreference(coreferences[i], "./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/coreference/top1/annealingAll100000/wikipediaEpisodesRandomWithoutBabylonAndBactraAndCarmania1000-Annealing.txt");
       // solveCoreference(coreferences[i], "./playouts/1000000/annealing/wikipediaEpisodesBabylon-52-0.20-Annealing.txt");
    }
    return 0;
    */
    

    
    
    int endIter[1] = {100000};
    
    //makeAnnealingEpisodes("./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/random5000/", initializeFilename, 1);//TOP1
    
    makeAnnealingEpisodesWithFolder(initializeFilename, 1,
                                    "./../../../../data/local/murakami/alexanderTmp/episodes/20150730/random1000/",
                                    "./../../../../data/local/murakami/alexanderTmp/episodes/20150730/SA/",
                                    endIter,1, 1000,startId,endId); //crane2
    
    
    return 0;
    
    
    
    /*
    makeAnnealingEpisodeFileFromNormalEpisodeFileWithFolder("wikipediaEpisodesRandomWithoutBabylonAndBactraAndCarmania1000.txt"
                                                            , initializeFilename
                                                            , 1
                                                            , "./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/coreference/"
                                                            , "./../../../../data/local/murakami/alexanderTmp/tmp/"
                                                            , 100000, 1000);
    return 0;
    
    */
     
    
    map<string,Person> persons;
    map<string,Place> places;
    initAgent(persons,places,initializeFilename);
    
 //   map<string, map<string,int> > shortestInfo = getShortestLengthToNextConstraint(places);
    
    vector<string> personVector = makePersonVectorFromMap(persons);
    vector<string> placeVector = makePlaceVectorFromMap(places);
    
    
    
    vector<Constraint> constraints;
    //constraints = makeConstraintsFromTestfile("alexander_wikipedia_bc.txt", personVector, placeVector,true);
    constraints = makeConstraintsFromConstraintFile("constraint20150730.txt");
    //constraints = makeConstraintsFromConstraintFile("constraint0114.txt", 1, 32); //annotationConstraints
//    initConstraintsFromAnnotation(constraints, "annotation.txt",personVector,placeVector,true);
    showConstraints(constraints);
    cout << constraints.size() << endl;
    /*
    vector<Constraint> annotationConstraints;
    initConstraintsFromAnnotation(annotationConstraints, "annotation.txt",personVector,placeVector,true);
    showConstraints(annotationConstraints);
    cout << annotationConstraints.size() << endl;
     */
    
    vector<Constraint> annotationConstraints = makeConstraintsFromConstraintFile("annotationConstraint20150730.txt",startId,endId);
    showConstraints(annotationConstraints);
    cout << annotationConstraints.size() << endl;
    
    //showDifferentConstraintsWithoutId(annotationConstraints, constraints);
    //return 0;
    
    
    // vector<FourChoiceQuestion> questions = makeFourChoiceQuestionsWithCount(10000, annotationConstraints, placeVector);
    
    //vector<FourChoiceQuestion> questions = readFourChoiceQuestions("fourChoiceQuestions-20150210-214542.txt");
    
    /*
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    char dateChara[30];
    sprintf(dateChara, "fourChoiceQuestions-%02d%02d%02d-%02d%02d%02d.txt",pnow->tm_year+1900
                                                                    ,pnow->tm_mon+1
                                                                    ,pnow->tm_mday
                                                                    ,pnow->tm_hour
                                                                    ,pnow->tm_min
                                                                    ,pnow->tm_sec);
    string dateString = dateChara;
     */
    //    outputFourChoiceQuestions(questions, dateString);
    
     //   vector<FourChoiceQuestion> questions = readFourChoiceQuestions("fourChoiceQuestions.txt");
   
      //  solveFourChoiceQuestions("",questions, 0.6, annotationConstraints,-1);
      //  solveFourChoiceQuestions("",questions, 0.8, annotationConstraints,-1);
    //    solveFourChoiceQuestions("",questions, 1.0, annotationConstraints,-1);
   // solveFourChoiceQuestionsWithFilename(questions, annotationConstraints, "./Heuristics/wikipediaEpisodesEcbatana-174-1.40.txt");
    /*
    for(unsigned int i=0;i<2;i++) {
        bool isAnnotation = false;
        if(i==1) isAnnotation = true;
        solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 10000,isAnnotation);
        solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 20000,isAnnotation);
        solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 50000,isAnnotation);
        solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 100000,isAnnotation);
        solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 200000,isAnnotation);
        solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 500000,isAnnotation);
        solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 1000000,isAnnotation);
    }
    */
    /*
    solve4selectionQuestion("question0423.txt", personVector, placeVector, "./../../../data/local/murakami/alexanderTmp/episodesByPlayout/playouts/1/annealingAll100000/wikipediaEpisodesA Eschate-478-0.20-Annealing.txt");
    return 0;
     */
 
     //４択問題定量評価
    /*
    int iters[7] = {1000,2000,5000,10000,20000,50000,100000};
    //int iters[6] = {10,20,50,100,200,500};
    //for(int i=0;i<7;i++) {
        int index = 6;
        cout << "solving four choice question iter=" << iters[index] << endl;
        //solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 1, false,true,iters[index]);
    
        //crane0 10-500
        //solveFourChoiceQuestionsBySAWithIter(questions, annotationConstraints, iters[index], "./../../../../data/local/murakami/alexanderTmp/annealingEpisodes/random1000/top1/", "./../../../../data/local/murakami/alexanderTmp/result/random1000/");
    
        //crane2 1000-100000
        solveFourChoiceQuestionsBySAWithIter(questions, annotationConstraints, iters[index], "./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/random5000/top1/", "./../../../../data/local/murakami/alexanderTmp/result/random1000/");
    //}
    //solveFourChoiceQuestionsByPlayout(questions, annotationConstraints, 0.2, 1000000, true,true);
    return 0;
     */
    
    /*　ベースラインによる各アノテーションデータの地名を答えるテスト
    answerPlaceFromTimeConstraintWithBaseline(annotationConstraints, constraints);
    return 0;
    */
    /*
    int iters2[7] = {1000,2000,5000,10000,20000,50000,100000};
    int iters0[6] = {10,20,50,100,200,500};
    
    for(int i=6;i<7;i++) {
        answerPlaceFromTimeConstraint("./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/random5000/top1/", iters2[i], annotationConstraints, initializeFilename, placeVector);
        //answerPlaceFromTimeConstraintByEpisode("./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/random5000/top1/", iters2[i], annotationConstraints, initializeFilename, placeVector);
    }
     */
     
    /*
    for(int i=0;i<6;i++) {
        answerPlaceFromTimeConstraint("./../../../../data/local/murakami/alexanderTmp/annealingEpisodes/random1000/top1/", iters0[i], annotationConstraints, initializeFilename, placeVector);
    }
    */
   // return 0;
    
    
    //出現頻度問題評価
    /*
    int iters[7] = {200000,2000,5000,10000,20000,50000,100000};
    //int iters[6] = {10,20,50,100,200,500};
    for(int i=0;i<1;i++) {
        cout << endl;
        cout << "maxIter = " << iters[i] << endl;
        
        getArccuracyWithAnnotationData(annotationConstraints, iters[i], "./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/random5000/top1/",initializeFilename);
        //getArccuracyWithAnnotationData(annotationConstraints, iters[i], "./../../../../data/local/murakami/alexanderTmp/annealingEpisodes/random1000/top1/",initializeFilename);
    }
    return 0;
    */

     //   solveFourChoiceQuestions("annotation", questions, 1.0, annotationConstraints,-1);
    /*
        solveFourChoiceQuestions("question16", questions, 1.0, annotationConstraints,0.5);
    solveFourChoiceQuestions("question16", questions, 1.0, annotationConstraints,1.0);
    solveFourChoiceQuestions("question16", questions, 1.0, annotationConstraints,1.4);
    solveFourChoiceQuestions("question16", questions, 1.0, annotationConstraints,2.0);
    return 0;
     */
 //   }
  //  return 0;
    
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
    
    
    /* //manually make randomEpisodes
    vector<string> placeNames;
    placeNames.push_back("Babylon");
    placeNames.push_back("Bactra");
    placeNames.push_back("A Carmania");
    vector<Constraint> constraints2 = removeConstraintsWithPlaceName(constraints, placeNames);
    showConstraints(constraints2);
    cout << constraints2.size() << endl;
    
    string filename = "RandomWithoutBabylonAndBactra1000";
    
    for(int i=0;i<1000;i++) {
        //    cout << filename << ":" << (i+1) << "/20 round" << endl;
        vector<Constraint> tmpConstraints = selectRandomConstraintsWithRate(constraints2, 1.0);
        cout << tmpConstraints.size() << endl;
        round++;
        vector<Episode> episodes;
        episodes.clear();
        
        doActionMCTS(persons, places, tmpConstraints,71,"./../../../../data/local/murakami/alexanderTmp/episodesByPlayout/coreference/wikipediaEpisodes" + filename + ".txt",0.2);
    }
    return 0;
    */
    
    
    for(int n=0;n<annotationConstraints.size();n++) {
        
        vector<string> placeNames;
        placeNames.push_back(annotationConstraints[n]._placeName);
        //vector<Constraint> constraints2 = removeConstraintsWithPlaceName(constraints, placeNames);
        vector<Constraint> constraints2 = removeConstraintsWithPlaceAndTimeCondition(constraints, annotationConstraints[n]);
        showConstraints(constraints2);
        cout << constraints2.size() << endl;
        
        double cp = 0.2;
        char fileChara[40];
        sprintf(fileChara,"%s-%d-%3.2f",(annotationConstraints[n]._placeName).c_str(),annotationConstraints[n]._id,cp);
        string filename = fileChara;
        
        for(int i=0;i<1000;i++) {
        //    cout << filename << ":" << (i+1) << "/20 round" << endl;
            //vector<Constraint> tmpConstraints = selectRandomConstraintsWithRate(constraints2, 1.0);
            vector<Constraint> tmpConstraints = constraints2;
            //cout << tmpConstraints.size() << endl;
            round++;
            vector<Episode> episodes;
            episodes.clear();
            
        
            doActionMCTS(persons, places, tmpConstraints,71,"./../../../../data/local/murakami/alexanderTmp/episodes/20150730/random1000/wikipediaEpisodes" + filename + ".txt",cp);
      //  doActionMCTSWithRank(persons, places, tmpConstraints, 71, "wikipediaEpisodes" + filename + ".txt", cp,shortestInfo);

        }
        
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
