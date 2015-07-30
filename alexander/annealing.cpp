//
//  annealing.cpp
//  alexander
//
//  Created by Yuki Murakami on 2015/03/03.
//
//

#include "annealing.h"

#include "episode.h"
#include "utility.h"
#include <fstream>
#include <math.h>

#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct Loop {
    int id;
    int inTime;
    string inPlace;
    int outTime;
    string outPlace;
    string inSecondPlace;
} LOOP;

const int ENDSTEP3 = 71;

void makeAnnealingEpisodesWithFolder(string initializeFilename,int getTopCount,string inputFilePath,string outputFilePath,int *endIters,int nEndIter,int nRandomEpisode,int begin,int end) {
    cout << "make annealing episodes inputFilePath:" << inputFilePath << "  outputFilePath" << outputFilePath << " getTopCount:" << getTopCount << endl;
    cout << endl;
    
    //vector<string> filenames = getFilenamesByFoldername(inputFilePath);
    vector<string> filenames = getFilenamesByFilelistname("randomEpisodeList.txt");
    int iterKinds = nEndIter;
    
    for(int i=0;i<iterKinds;i++) {
        char iter[20];
        sprintf(iter, "%d",endIters[i]);
        string iterString = iter;
        string folderName = "annealingAll" + iterString;
        makeDirectory(outputFilePath, folderName);
    }
    
    cout << "found " << filenames.size() << " files" << endl;
    for(int i=0;i<filenames.size();i++) {
        cout << "id:" << (i+1) << " " << filenames[i] << endl;
    }
    
    cout << "use file:" << begin << "-" << end << endl;
    for(unsigned int i=begin-1;i<end/*filenames.size()*/;i++) {
        cout << "start SA in episodeId:" << (i+1) << endl;
        for(int j=0;j<iterKinds;j++) {//間違っていじらない
            //makeAnnealingEpisodeFileFromNormalEpisodeFile(filenames[i], initializeFilename, getTopCount,outputFilePath,endIters[j]);
            makeAnnealingEpisodeFileFromNormalEpisodeFileWithFolder(filenames[i], initializeFilename, getTopCount, inputFilePath, outputFilePath, endIters[j], nRandomEpisode);
        }
    }
}

void makeAnnealingEpisodeFileFromNormalEpisodeFileWithFolder(string episodeFilename, string initializeFilename,int getCount,string inputPath,string outputPath,int endIter,int nRandomEpisode) {
    vector< pair< vector<Episode>,vector<Constraint> > > episodeInfos = readEpisodesAndConstraintsFromEpisodeFile(inputPath + episodeFilename, initializeFilename);
    
    char iter[20];
    sprintf(iter, "%d",endIter);
    string iterString = iter;
    string folderName = "annealingAll" + iterString + "/";
    
    string outputFilename = outputPath + folderName + episodeFilename.substr(0,episodeFilename.length()-4) + "-Annealing.txt";
    
    //ei==0のみにするとpart,全て1000個の初期会についてやるならall
    cout << "this file '" << episodeFilename << "' contains " << episodeInfos.size() << " initial episodes" << endl;
    cout << "maxIter = " << endIter << " getCount:Top" << getCount << endl;
    cout << "outputFilename = " << outputFilename << endl;
    for(int ei = 0;ei < nRandomEpisode;ei++) {
        cout << "progress:" << (ei+1) << "/" << nRandomEpisode << endl;
        vector<Episode> episodes = episodeInfos[ei].first;
        vector<Constraint> constraints = episodeInfos[ei].second;
        
        string personName = (*episodes[0]._persons.begin()).second._name;
        double bestScore = 0.0;
        
        vector< vector<Episode> > annealignEpisodes = getAnnealingEpisodes(episodes, constraints, initializeFilename, getCount,bestScore,endIter);
        
        for(unsigned int i=0;i<annealignEpisodes.size();i++) {
            EpisodesOutput(annealignEpisodes[i], outputFilename, personName, bestScore, -1);
            
        }
    }
    cout << "outputEpisode: " << outputFilename << endl;
}

void makeAnnealingEpisodes(string folderPath, string initializeFilename,int getCount) {
    cout << "make annealing episodes folderPath:" << folderPath << " getCount:Top" << getCount << endl;
    cout << endl;
    vector<string> filenames = getFilenamesByFoldername(folderPath);
    
    //int endIters[7] = {5,10,20,50,100,200,500};
    int endIters[7] = {1000,2000,5000,10000,20000,50000,100000};
    for(int i=0;i<7;i++) {
        char iter[20];
        sprintf(iter, "%d",endIters[i]);
        string iterString = iter;
        string folderName = "annealingAll" + iterString;
        makeDirectory(folderPath+"top1/", folderName);
    }
    
    cout << "found " << filenames.size() << " files" << endl;
    
    cout << "1-32" << endl;
    for(unsigned int i=0;i<32/*filenames.size()*/;i++) {
        cout << "start SA in episodeId:" << (i+1) << endl;
        for(int j=0;j<7;j++) {//間違っていじらない
            makeAnnealingEpisodeFileFromNormalEpisodeFile(filenames[i], initializeFilename, getCount,folderPath,endIters[j]);
        }
    }
    
}


void makeAnnealingEpisodeFileFromNormalEpisodeFile(string episodeFilename, string initializeFilename,int getCount,string folderPath,int endIter) {
    vector< pair< vector<Episode>,vector<Constraint> > > episodeInfos = readEpisodesAndConstraintsFromEpisodeFile(folderPath + episodeFilename, initializeFilename);
    
    
    char iter[20];
    sprintf(iter, "%d",endIter);
    string iterString = iter;
    string folderName = "top1/annealingAll" + iterString + "/";
    
    string outputFilename = folderPath + folderName + episodeFilename.substr(0,episodeFilename.length()-4) + "-Annealing.txt";
    
    //ei==0のみにするとpart,全て1000個の初期会についてやるならall
    cout << "this file '" << episodeFilename << "' contains " << episodeInfos.size() << " initial episodes" << endl;
    cout << "maxIter = " << endIter << " getCount:Top" << getCount << endl;
    cout << "outputFilename = " << outputFilename << endl;
    for(int ei = 0;ei < 1000;ei++) {
    
        vector<Episode> episodes = episodeInfos[ei].first;
        vector<Constraint> constraints = episodeInfos[ei].second;
        
        string personName = (*episodes[0]._persons.begin()).second._name;
        double bestScore = 0.0;
        
        
        
        vector< vector<Episode> > annealignEpisodes = getAnnealingEpisodes(episodes, constraints, initializeFilename, getCount,bestScore,endIter);
        
        for(unsigned int i=0;i<annealignEpisodes.size();i++) {
            EpisodesOutput(annealignEpisodes[i], outputFilename, personName, bestScore, -1);
            
        }
    }
    cout << "outputEpisode: " << outputFilename << endl;
}


vector< vector<Episode> > getAnnealingEpisodes( vector<Episode> &episode ,vector<Constraint> &constraints,string initializeFilename,int getCount,double &bestScore,int endIter) {
    
    bool isDebug = false;
    
    int badTransitionCount = 0;
    int badTransitionCount10 = 0;
    
    map<string,Place> places;
    initPlace(places, initializeFilename);
    
    vector< vector<string> > loops = getLoopFromPlaces(places);
    if(isDebug) {
        for(unsigned int li = 0;li < loops.size();li++) {
            cout << "loop[" << li << "]:";
            for(unsigned int i=0;i<loops[li].size();i++) {
                cout << loops[li][i] << " ";
            }
            cout << endl;
        }
    }
    
    string personName = (*episode[0]._persons.begin()).second._name;
    string firstPlace = episode[0]._persons[personName]._nowPlace;
    
    vector<string> actions = getActionsFromEpisodes(episode);
    
    double bestVal = checkEpisodePerson(episode, constraints, personName, false);
    
    
    
    vector< vector<string> > bestActions;
    vector< vector<string> > secondBestActions;
    
    bestActions.push_back(actions);
    

    double nowVal = bestVal;
 
    for(int iter=0;iter<endIter;iter++) {
        vector<string> nextActions = getNeighbor(actions, false, firstPlace, places, loops);
        double nextVal = checkEpisodePersonWithActions(nextActions, constraints, personName, firstPlace, false);
        
        
        if(nextVal >= bestVal) {//記録更新
            if(nextVal > bestVal) {
                secondBestActions = bestActions;
                bestActions.clear();
            }
            bestActions.push_back(nextActions);
            bestVal = nextVal;
            bestScore = bestVal;
            //cout << "bestActions.size() = " << bestActions.size() << endl;
        }
        
        
        double random = getRandomZeroToOne();
        double prob = getAnnealingProbability(nowVal, nextVal, iter, endIter);
        if(random <= prob) {
            actions = nextActions;
            if(actions != nextActions) {
                //cout << "error" << endl;
            }
            if(isDebug) {
                cout << "iter=" << iter << " state transition: now:" << nowVal << " next:" << nextVal << "prob(" << random << "," << prob << ")" << endl;
            }
            if(prob < 1.0) {
                badTransitionCount++;
                if(iter < endIter*0.1) {
                    badTransitionCount10++;
                }
                if(isDebug) {
                    cout << "badTransitionCount = " << badTransitionCount << endl;
                }
            }
            nowVal = nextVal;
            //cout << "actions.size() = " << actions.size() << endl;
            //cout << "bestActions.size() = " << bestActions.size() << " bestScore = " << bestVal << endl;
            //cout << "secondBestActions.size() = " << secondBestActions.size() << endl;
        }
    }
    
    int selectCount = getCount;
   // cout << "bestActions.size() = " << bestActions.size() << endl;
    vector< vector<string> > selectedActions;
    for(int i=0;i<selectCount;i++) {
        if(bestActions.size()<=0) {
            if(secondBestActions.size() <= 0) {
                break;
            } else {
              //  cout << "in SecondBestActions" << endl;
                bestActions = secondBestActions;
                secondBestActions.clear();
            }
        }
        int index = xor128() % bestActions.size();
        vector<string> selectActions = bestActions[index];
        bestActions.erase(bestActions.begin()+index);
        selectedActions.push_back(selectActions);
    }
    
    vector< vector<Episode> > selectedEpisodes;
    for(unsigned int i=0;i<selectedActions.size();i++) {
        vector<Episode> episodes = getEpisodesFromActionsWithAlexander(selectedActions[i], initializeFilename);
        selectedEpisodes.push_back(episodes);
    }
    
    cout << "badTransitionCount = " << badTransitionCount << endl;
    cout << "badTransitionCount(10%%) = " << badTransitionCount10 << endl;
    
    return selectedEpisodes;
}

double getAnnealingProbability(double nowVal,double nextVal,int iter,int maxIter) {
    if(nowVal <= nextVal) {
        return 1;
    }
    
    double alpha = 0.001;
    double T = pow(alpha, (double)iter/(double)maxIter);
    return exp((nextVal-nowVal)*30/T);
}

vector<string> getNeighbor(vector<string> actions,bool isDebug,string firstPlace,map<string,Place> &places,vector< vector<string> > &loops) {
    
    if(xor128()%2==0) {
        actions = addNothingActionByTime(actions, xor128()%ENDSTEP3,isDebug);
        actions = removeNothingActionWithRandom(actions,isDebug);
    }
    if(xor128()%2==0) {
        actions = addDetourActionByTime(actions, xor128()%ENDSTEP3, isDebug, firstPlace, places);
    }
    if(xor128()%2==0) {
        actions = removeDetourActionWithRandom(actions, isDebug, firstPlace, places);
    }
    if(xor128()%2==0) {
        actions = changeLoopActionWithRandom(actions, isDebug, loops, firstPlace);
    }
    
    if(!checkActions(actions)) {
        cout << "error: same move: is continuous @annealing test in annealing.cpp" << endl;
        exit(0);
    }
    return actions;
}

vector<string> makeRandomActions(string firstPlace,map<string,Place> &places) {
    vector<string> actions;
    string nowPlace = firstPlace;
    for(int i=0;i<ENDSTEP3;i++) {
        vector<string> nextPlaces = places[nowPlace]._nextPlaces;
        nextPlaces.push_back(nowPlace);
        string nextPlace = nextPlaces[xor128()%nextPlaces.size()];
        
        if(nextPlace == nowPlace) {
            actions.push_back("nothing");
        } else {
            actions.push_back("move:"+nextPlace);
        }
        nowPlace = nextPlace;
    }
    return actions;
}

#pragma mark - GetNeighbor


vector<string> addNothingActionByTime(vector<string> actions,int time,bool isDebug) {
    
    if(isDebug) cout << "add nothing at " << time << endl;
    insertActionByTime(actions, time, "nothing");
    return actions;
}

vector<string> removeNothingActionWithRandom(vector<string> actions,bool isDebug) {
    
    vector<int> indexes;
    for(unsigned int i=0;i<actions.size();i++) {
        //cout << actions[i] << endl;
        if(actions[i] == "nothing") indexes.push_back(i);
    }
    
    if(indexes.size() <= 0) return actions;
    
    int index = indexes[xor128()%indexes.size()];
    
    eraseActionByTime(actions, index);
    
    
    if(isDebug) cout << "remove nothing at " << index << endl;
        
    return actions;
}

vector<string> removeNothingActionWithTimeWidth(vector<string> actions,bool isDebug,int start,int end) {
    vector<int> indexes;
    if(end >= actions.size()) end = (int)actions.size()-1;
    if(start < 0) start = 0;
    for(unsigned int i=start;i<=end;i++) {
        //cout << actions[i] << endl;
        if(actions[i] == "nothing") indexes.push_back(i);
    }
    
    if(indexes.size() <= 0) return actions;
    
    int index = indexes[xor128()%indexes.size()];
    
    eraseActionByTime(actions, index);
    
    if(isDebug) cout << "remove nothing at " << index << endl;
    
    return actions;
}

vector<string> addDetourActionByTime(vector<string> actions,int time,bool isDebug,string firstPlace,map<string,Place> &places) {
    string nowPlace = firstPlace;
    if(time > 0) {
        for(int i=time-1;i>=0;i--) {
            if(actions[i].substr(0,5) == "move:") {
                nowPlace = actions[i].substr(5);
                break;
            }
        }
    }
    Place place = places[nowPlace];
    string nextPlace = place._nextPlaces[xor128()%place._nextPlaces.size()];

    
    insertActionByTime(actions, time, "move:"+nowPlace);
    insertActionByTime(actions, time, "move:"+nextPlace);
    
    if(isDebug) {
        cout << "add detour to " << nextPlace << " at " << time << "-" << time+1 << endl;
        cout << "(add " << nextPlace << " at " << time << " and " << nowPlace << " at " << time+1 << ")" << endl;
    }
    
    for(int i=0;i<2;i++) {
        actions = removeNothingActionWithTimeWidth(actions, isDebug, time+2, (int)actions.size()-1);
    }
    
    return actions;
}

vector<string> removeDetourActionWithRandom(vector<string> actions,bool isDebug,string firstPlace,map<string,Place> &places) {
    string nextPlace = "";
    string nextnextPlace = "";
    string nowPlace = firstPlace;
    
    vector<int> candidates;
    
    for(unsigned int i=0;i<actions.size()-1;i++) {
        if(actions[i].substr(0,5) == "move:") {
            if(actions[i+1].substr(0,5) == "move:") {
                nextPlace = actions[i].substr(5);
                nextnextPlace = actions[i+1].substr(5);
                if(nowPlace == nextnextPlace) {
                    bool isFindPlace = false;
                    for(unsigned int it=0;it<places[nextPlace]._nextPlaces.size();it++) {
                        if(places[nextPlace]._nextPlaces[it] == nextnextPlace) {
                            isFindPlace = true;
                            break;
                        }
                    }
                    if(!isFindPlace){
                        cout << "error: move is not correct @removeDetourActionWithRandom in annealing.cpp" << endl;
                        exit(0);
                    }
                    /*
                    vector<string>::iterator it = find( places[nextPlace]._nextPlaces.begin(), places[nextPlace]._nextPlaces.end() , nextnextPlace );
                    if( it == places[nextPlace]._nextPlaces.end() ){
                        cout << "error: move is not correct @removeDetourActionWithRandom in annealing.cpp" << endl;
                        exit(0);
                    }
                     */
                    candidates.push_back(i);
                }
            }
            nowPlace = actions[i].substr(5);
        }
    }
    
    if(candidates.size() <= 0) return actions;
    
    int index = candidates[xor128()%candidates.size()];
    
    nextPlace = actions[index].substr(5);
    nextnextPlace = actions[index+1].substr(5);
    
    eraseActionByTime(actions, index);
    eraseActionByTime(actions, index);
    
    if(isDebug) {
        cout << "remove detour to " << nextPlace << " at " << index << "-" << index+1 << endl;
        cout << "(remove " << nextPlace << " at " << index << " and " << nextnextPlace << " at " << index+1 << ")" << endl;
    }
    
    /*
    for(int i=0;i<2;i++) {
        int count = (int)actions.size()-index;
        if(count >= 1) {
            actions = addNothingActionByTime(actions, index + xor128() % (actions.size()-index), isDebug);
        }
    }
    */
    
    return actions;
}


vector<string> changeLoopActionWithRandom(vector<string> actions,bool isDebug,vector< vector<string> > &loops,string firstPlace) {
    //ループを通っている箇所を探す ループへの入り口と出口の地名を探す
    //LOOP構造体で取得
    vector<LOOP> ioLoops;
    
    string inPlace,outPlace;
    int inTime = 0,outTime=0;
    
    
    for(unsigned li=0;li<loops.size();li++) {
        bool inLoop = false;
        bool inLoopSecondTrigger = false;
        vector<string> loop = loops[li];
        
        string nowPlace = firstPlace;
        string lastPlace = firstPlace;
        string secondPlace = "";
        for(unsigned ai = 0;ai < actions.size();ai++) {
            if(actions[ai].substr(0,5) == "move:") {
                nowPlace = actions[ai].substr(5);
                if(inLoopSecondTrigger) {
                    secondPlace = nowPlace;
                    inLoopSecondTrigger = false;
                }
                bool isFind = false;
                if(!inLoop) {
                    for(unsigned pi=0;pi<loop.size();pi++) {
                        if(loop[pi] == nowPlace) {
                            isFind = true;
                            break;
                        }
                    }
                    if(isFind) {
                        inPlace = nowPlace;
                        inTime = ai + 1;
                        inLoop = true;
                        inLoopSecondTrigger = true;
                    }
                } else {
                    for(unsigned pi=0;pi<loop.size();pi++) {
                        if(loop[pi] == nowPlace) {
                            isFind = true;
                            break;
                        }
                    }
                    if(!isFind) {
                        outPlace = lastPlace;
                        outTime = ai;
                        inLoop = false;
                        
                        if(inTime != outTime) {
                            LOOP ioLoop;
                            ioLoop.id = li;
                            ioLoop.inPlace = inPlace;
                            ioLoop.outPlace = outPlace;
                            ioLoop.inTime = inTime;
                            ioLoop.outTime = outTime;
                            ioLoop.inSecondPlace = secondPlace;
                            ioLoops.push_back(ioLoop);
                        }
                    }
                }
                
                lastPlace = nowPlace;
            }
        }
    }
    
    if(isDebug) {
        for(unsigned i=0;i<ioLoops.size();i++) {
            cout << ioLoops[i].id << "/" << ioLoops[i].inTime << ":" << ioLoops[i].inPlace << "/" << ioLoops[i].outTime << ":" << ioLoops[i].outPlace << "/" << ioLoops[i].inSecondPlace << endl;
        }
    }
    
    
    //通過しているループから一つ選び、経路を逆にする
    if(ioLoops.size() <= 0) return actions;
    
    vector<string> addActions;
    LOOP ioLoop = ioLoops[xor128() % ioLoops.size()];
    vector<string> loopStrings = loops[ioLoop.id];
    vector<string> loopStringsWide;
    for(int i=0;i<3;i++) {
        for(int j=0;j<loopStrings.size();j++) {
            loopStringsWide.push_back(loopStrings[j]);
        }
    }
    for(int pi = (int)loopStrings.size() ;pi<loopStrings.size()*2;pi++) {
        if(ioLoop.inPlace == loopStringsWide[pi]) {
            if(loopStringsWide[pi+1] != ioLoop.inSecondPlace) {
                //正の方向にoutPlaceまで回る
                for(int ni=pi+1;;ni++) {
                    addActions.push_back("move:"+loopStringsWide[ni]);
                    if(loopStringsWide[ni] == ioLoop.outPlace) break;
                }
            } else {
                //負の方向にoutPlaceまで回る
                for(int ni=pi-1;;ni--) {
                    addActions.push_back("move:"+loopStringsWide[ni]);
                    if(loopStringsWide[ni] == ioLoop.outPlace) break;
                }
            }
            break;
        }
    }
    
    for(int i=0;i<ioLoop.outTime-ioLoop.inTime;i++) {
        eraseActionByTime(actions, ioLoop.inTime);
    }
    
    for(int i = (int)addActions.size()-1;i>=0;i--) {
        insertActionByTime(actions, ioLoop.inTime, addActions[i]);
    }
    
    int addActionCount = (int)addActions.size() - (ioLoop.outTime-ioLoop.inTime);
    
    
    if(isDebug) {
        cout << "remove action :" << ioLoop.inTime << "-" << ioLoop.outTime-1 << endl;
        cout << "add action :";
        for(unsigned int i=0;i<addActions.size();i++) {
            cout << addActions[i] << ",";
        }
        cout << endl;
        
        cout << "addActionCount:" << addActionCount << endl;
    }
    
    if(addActionCount < 0) {
        for(int i=0;i<-addActionCount;i++) {
            actions = addNothingActionByTime(actions, ioLoop.inTime + xor128() % (ioLoop.outTime-ioLoop.inTime), isDebug);
        }
    }
    if(addActionCount > 0) {
        for(int i=0;i<addActionCount;i++) {
            actions = removeNothingActionWithTimeWidth(actions , isDebug, ioLoop.inTime, ENDSTEP3);
        }
    }
    
    return actions;
}


void eraseActionByTime(vector<string> &actions,int time) {
    if(time < 0 || time > actions.size()) {
        cout << "error: the value of time is not correct  @eraseActionByTime in annealing.cpp" << endl;
        exit(0);
    }
    actions.erase(actions.begin()+time);
    
    while(actions.size() < ENDSTEP3) {
        actions.push_back("nothing");
    }
}

void insertActionByTime(vector<string> &actions,int time,string action) {
    if(time < 0) time = 0;
    if(time >= actions.size()) {
        actions.push_back(action);
        return;
    }
    
    actions.insert(actions.begin()+time, action);
    
    while(actions.size() < ENDSTEP3) {
        actions.push_back("nothing");
    }
}

bool checkActions(vector<string> &actions) {
    for(unsigned int i=0;i<actions.size()-1;i++) {
        if(actions[i].substr(0,5) == "move:" && actions[i+1].substr(0,5) == "move:") {
            if(actions[i].substr(5) == actions[i+1].substr(5)) {
                cout << "same move:  at " << i << endl;
                cout << "debug--------" << endl;
                for(unsigned int j=0;j<actions.size();j++) {
                    cout << j << ":" << actions[j] << endl;
                }
                cout << "debugend-----" << endl;
                return false;
            }
        }
    }
    return true;
}

#pragma mark - graph theory

vector< vector<string> > getLoopFromPlaces(map<string,Place> &places) {
    vector< vector<string> > loops;
    
    //深さ優先探索でグラフを探索　既に通ったところはvisited = true として、直属の親以外でtrueのノードを見つけたら閉路とする
    map<string,bool> placeVisited;
    for(map<string,Place>::iterator it = places.begin();it != places.end();it++) {
        placeVisited.insert(make_pair((*it).second._name, false));
    }
    
    string currentPlace = (*places.begin()).second._name;
    string parentPlace = currentPlace;
    
    vector<string> episode;
    episode.push_back(currentPlace);
    placeVisited[currentPlace] = true;
    
    vector<string> nextPlaces = places[currentPlace]._nextPlaces;
    for(unsigned int pi = 0;pi < nextPlaces.size();pi++) {
        string nextPlace = nextPlaces[pi];
        if(nextPlace != parentPlace) {
            if(!placeVisited[nextPlace]) {
                subGetLoopFromPlaces(loops, places, placeVisited, currentPlace, nextPlace, episode);
            }
        }
    }
    
    //円順列的にかぶっているものを代表一つのこして削除
    int li=0;
    while(li < loops.size()) {
        vector<string> representLoop = loops[li];
        
        vector<int> removeIndexes;
        for(unsigned int i=li+1;i<loops.size();i++) {
            vector<string> tmp(representLoop);
            bool isSameLoop = true;
            for(unsigned int j=0;j<loops[i].size();j++) {
                string place = loops[i][j];
                
                bool isFind = false;
                for(unsigned int t=0;t<tmp.size();t++) {
                    if(tmp[t] == place) {
                        tmp.erase(tmp.begin()+t);
                        isFind = true;
                        break;
                    }
                }
                if(!isFind) {
                    isSameLoop = false;
                    break;
                }
            }
            
            if(isSameLoop && tmp.size() == 0) {
                removeIndexes.push_back(i);
            }
        }
        
        for(int i = (int)removeIndexes.size()-1;i>=0;i--) {
            loops.erase(loops.begin()+removeIndexes[i]);
        }
       
        li++;
    }
    
    return loops;
}

void subGetLoopFromPlaces(vector< vector<string> > &loops, map<string,Place> &places, map<string,bool> placeVisited,string parentPlace,string currentPlace,vector<string> episode) {
    
    placeVisited[currentPlace] = true;
    episode.push_back(currentPlace);
    
    vector<string> nextPlaces = places[currentPlace]._nextPlaces;
    for(unsigned int pi = 0;pi < nextPlaces.size();pi++) {
        string nextPlace = nextPlaces[pi];
        if(nextPlace != parentPlace) {
            if(!placeVisited[nextPlace]) {
                subGetLoopFromPlaces(loops, places, placeVisited, currentPlace, nextPlace, episode);
            } else {
                //ループ検出
                for(int ei = (int)episode.size() - 1;ei >= 0;ei--) {
                    if(episode[ei] == nextPlace) {
                        int loopSize = (int)episode.size()-1 - ei + 1;
                        if(loopSize <= 5) {//あまり大きいと近傍にならないため、ここでループのサイズを制限する
                            vector<string> loop;
                            for(int li = 0;li < loopSize;li++) {
                                loop.push_back(episode[ei + li]);
                            }
                            loops.push_back(loop);
                        }
                    }
                }
            }
        }
    }
}


#pragma mark - ConvertEpisodesAndActions

vector<Episode> getEpisodesFromActionsWithAlexander(vector<string> &actions,string initializeFilename) {
    
    string firstPlace = "";
    
    map<string,Place> places;
    ifstream ifs2(initializeFilename.c_str());
    string buf;
    if(!ifs2) {
        cout << "not found initialize file" << endl;
        exit(0);
    }
    vector<string> placeVector;
    while(getline(ifs2,buf)) {
        if(buf == "<area_start>") {
            while(getline(ifs2,buf)) {
                if(buf == "<area_end>") break;
                vector<string> out2 = SpritString(buf, ":");
                int placeIndex = atoi(out2[0].c_str());
                vector<string> out3 = SpritString(out2[1], ",");
                if(places.find(placeVector[placeIndex]) != places.end()) {
                    for(int i=0;i<(int)out3.size();i++) {
                        int nextIndex = atoi(out3[i].c_str());
                        if(places.find(placeVector[nextIndex]) != places.end()) {
                            places[placeVector[placeIndex]]._nextPlaces.push_back(placeVector[nextIndex]);
                        }
                    }
                }
            }
        }
        if(buf == "<area_end>") {
            continue;
        }
        vector<string> out = SpritString(buf, ":");
        if(out[0] == "place") {
            placeVector = SpritString(out[1],",");
            for(int i=0;i<(int)placeVector.size();i++) {
                Place place;
                place._name = placeVector[i];
                places.insert(make_pair(place._name, place));
            }
        }
        if(out[0] == "Alexander") {
            if(places.find(out[1]) != places.end()) {
                firstPlace = out[1];
            }
        }
    }
    ifs2.close();
  
    
    
    
    vector<Episode> episodes;
    
    string lastPlaceName = firstPlace;
    string nowPlaceName = firstPlace;
   
    for(unsigned int ai=0;ai<actions.size() + 1;ai++) {
        
        if(ai == 0) {
            string personName = "Alexander";
            Person person(personName,firstPlace,0);
            map<string,Person> persons;
            persons.insert(make_pair(personName, person));
            
            map<string,Place> copyPlaces(places);
            copyPlaces[firstPlace]._persons.push_back(person);
            Episode epi(0,persons,copyPlaces);
            episodes.push_back(epi);
        } else {
            int time = ai;
            if(time > ENDSTEP3) break;
            string personName = "Alexander";
            
            string action = actions[ai-1];
            if(action == "nothing") {
                lastPlaceName = nowPlaceName;
            } else {
                if(action.substr(0,5) == "move:") {
                    string toPlace = action.substr(5);
                    
                    lastPlaceName = nowPlaceName;
                    nowPlaceName = toPlace;
                }
            }
        
            Person person(personName,nowPlaceName,0);
            map<string,Person> persons;
            persons.insert(make_pair(personName, person));
                
            map<string,Place> copyPlaces(places);
            copyPlaces[nowPlaceName]._persons.push_back(person);
            Episode epi(time,persons,copyPlaces);
            episodes.push_back(epi);
        }
    }
    
    return episodes;
}

vector<string> getActionsFromEpisodes(vector<Episode> &episodes) {
    string lastPlace = episodes[0]._persons["Alexander"]._nowPlace;
    string nowPlace = "";
    
    string action = "";
    
    vector<string> actions;
    
    for(unsigned int i=1;i<episodes.size();i++) {
        Episode episode = episodes[i];
        Person person = episode._persons["Alexander"];
        nowPlace = person._nowPlace;
        
        if(nowPlace == lastPlace) {
            action = "nothing";
        } else {
            action = "move:" + nowPlace;
        }
        
        actions.push_back(action);
        
        lastPlace = nowPlace;
    }
    
    return actions;
}