//
//  utility.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#include "utility.h"

#include <fstream>
#include <math.h>

#include <stdlib.h>

#include <string>

#include <stdio.h>

using namespace std;

#pragma mark -
#pragma mark initalize

void initAgent(map<string,Person> &persons,map<string,Place> &places,string filename) {
    persons.clear();
    places.clear();
    
    vector<string> placeVector;
    
    ifstream ifs(filename.c_str());
    string buf;
    if(!ifs) {
        cout << "not found initialize file" << endl;
        exit(0);
    }
    while(getline(ifs,buf)) {
        
        if(buf == "<area_start>") {
            while(getline(ifs,buf)) {
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
        if(out[0] == "human") {
            vector<string> names = SpritString(out[1],",");
            for(int i=0;i<(int)names.size();i++) {
                Person person;
                person._name = names[i];
                persons.insert(make_pair(person._name,person));
            }
        }
        if(out[0] == "place") {
            placeVector = SpritString(out[1],",");
            for(int i=0;i<(int)placeVector.size();i++) {
                Place place;
                place._name = placeVector[i];
                places.insert(make_pair(place._name, place));
            }
        }
        if(persons.find(out[0]) != persons.end()) {
            if(places.find(out[1]) != places.end()) {
                persons[out[0]]._nowPlace = out[1];
                places[out[1]]._persons.push_back(persons[out[0]]);
            }
        }
        
       
    }
    /*
    for(map<string,Place>::iterator it = places.begin();it != places.end();it++) {
        Place place = (*it).second;
        cout << place._name << endl;
        cout << "next:";
        for(int j=0;j<place._nextPlaces.size();j++) {
            cout << place._nextPlaces[j] << ",";
        }
        cout << endl;
    }
    
    for(map<string,Person>::iterator it = persons.begin();it != persons.end();it++) {
        Person person = (*it).second;
        cout << person._name << ",";
    }
    cout << endl;
     */
}

void initConstraints(vector<Constraint> &constraints,string filename) {
    constraints.clear();
    
    ifstream ifs(filename.c_str());
    string buf;
    if(!ifs) {
        cout << "not found constraint file" << endl;
        exit(0);
    }
    while(getline(ifs,buf)) {
        if(buf.c_str()[0] == '*') continue;
        vector<string> out = SpritString(buf, ",");
        int time = atoi(out[0].c_str());
        Constraint con(time,time,out[1],out[2],getEnumFromString(out[3]));
        constraints.push_back(con);
    }
}

void showAndOutputUnknownPlaceFromAnnotation(string filename,vector<string> &persons,vector<string> &places,string outputFileName) {
    ofstream ofs(outputFileName.c_str());
    
    vector< pair<string, int> > unknownPlaces;
    
    cout << "show unknownPlaceName From Annotation" << endl;
    ifstream ifs(filename.c_str());
    string buf;
    if(!ifs) {
        cout << "not found constraint file" << endl;
        exit(0);
    }
    int count = 0;
    while(getline(ifs,buf)) {
        count++;
        if(buf.c_str()[0] == '*') continue;
        vector<string> out = SpritString(buf, ",");
        int bc = -1;
        int month = -1;
        if(out[0] != "-") {
            bc = atoi(out[0].c_str());
        }
        if(out[1] != "-") {
            month = getMonthFromString(out[1]);
        }
        
        int beginTime=0;
        int endTime = 0;
        
        if(month == -1) {
            beginTime = getTimeFromBC(bc, 1);
            endTime = getTimeFromBC(bc, 12);
        } else {
            beginTime = getTimeFromBC(bc, month);
            endTime = getTimeFromBC(bc, month);
        }
        
        string personName = "";
        string placeName = "";
        string constraint = out[4];
        
        for(unsigned int j=0;j<persons.size();j++) {
            if(isEqualStringWithoutOrthographicalVariant(out[2], persons[j])) personName = out[2];
        }
        
        for(unsigned int j=0;j<places.size();j++) {
            if(isEqualStringWithoutOrthographicalVariant(out[3], places[j])) placeName = out[3];
        }
        
        if(personName != "" && beginTime >= 0 && beginTime <= 143 && endTime >= 0 && endTime <= 143 && constraint != "") {
            if(placeName == "" && out[3] != "-") {
                cout << "unknownPlace:" << out[3] << endl;
                ofs << out[3] << ":" << count << endl;
                
                bool isFind = false;
                for(unsigned int k=0;k<unknownPlaces.size();k++) {
                    if(unknownPlaces[k].first == out[3]) {
                        unknownPlaces[k].second++;
                        isFind = true;
                        break;
                    }
                }
                if(!isFind) {
                    unknownPlaces.push_back(make_pair(out[3], 1));
                }
            }
        }
    }
    
    ifs.close();
    ofs.close();
    
    for(unsigned int i=0;i<unknownPlaces.size();i++) {
        cout << unknownPlaces[i].first << "/" << unknownPlaces[i].second << endl;
    }
}

void initConstraintsFromAnnotation(vector<Constraint> &constraints,string filename,vector<string> &persons,vector<string> &places,bool isSameMode) {
    constraints.clear();
    
    ifstream ifs(filename.c_str());
    string buf;
    if(!ifs) {
        cout << "not found constraint file" << endl;
        exit(0);
    }
    int count = 0;
    while(getline(ifs,buf)) {
        count++;
        if(buf.c_str()[0] == '*') continue;
        vector<string> out = SpritString(buf, ",");
        int bc = -1;
        int month = -1;
        if(out[0] != "-") {
            bc = atoi(out[0].c_str());
        }
        if(out[1] != "-") {
            month = getMonthFromString(out[1]);
        }
        
        int beginTime=0;
        int endTime = 0;
        
        if(month == -1) {
            beginTime = getTimeFromBC(bc, 1);
            endTime = getTimeFromBC(bc, 12);
        } else {
            beginTime = getTimeFromBC(bc, month);
            endTime = getTimeFromBC(bc, month);
        }
        
        string personName = "";
        string placeName = "";
        string constraint = out[4];
        
        for(unsigned int j=0;j<persons.size();j++) {
            if(isEqualStringWithoutOrthographicalVariant(out[2], persons[j])) personName = out[2];
        }
        
        for(unsigned int j=0;j<places.size();j++) {
            if(isEqualStringWithoutOrthographicalVariant(out[3], places[j])) placeName = out[3];
        }
        
        if(personName != "" && placeName != "" && beginTime >= 0 && beginTime <= 143 && endTime >= 0 && endTime <= 143 && constraint != "") {
            Constraint con(beginTime,endTime,personName,placeName,getEnumFromString(constraint),count);
            
            if(isSameMode) {
                bool isFind = false;
                for(unsigned int i=0;i<constraints.size();i++) {
                    if(isSameConstraint(constraints[i], con)) {
                        isFind = true;
                        break;
                    }
                }
                if(!isFind) {
                    constraints.push_back(con);
                }
            } else {
                constraints.push_back(con);
            }
        }
        
    }
}

void initConstraintsWithCount(vector<Constraint> &constraints,string filename,int count) {
    constraints.clear();
    
    vector<Constraint> tmp;
    ifstream ifs(filename.c_str());
    string buf;
    if(!ifs) {
        cout << "not found constraint file" << endl;
        exit(0);
    }
    while(getline(ifs,buf)) {
        if(buf.c_str()[0] == '*') continue;
        vector<string> out = SpritString(buf, ",");
        int time = atoi(out[0].c_str());
        Constraint con(time,time,out[1],out[2],getEnumFromString(out[3]));
        tmp.push_back(con);
    }
    
    if((int)tmp.size() < count) {
        cout << "error: number of count is bigger than constraints" << endl;
        exit(0);
    }
    
    for(int i=0;i<count;i++) {
       // int index = xor128()%(tmp.size());
        int index = 0;
        tmp[index].show();
        constraints.push_back(tmp[index]);
        tmp.erase(tmp.begin()+index);
    }
}

void initQuestion(Question &question,string filename) {
    ifstream ifs(filename.c_str());
    string buf;
    if(!ifs) {
        cout << "not found question file" << endl;
        exit(0);
    }
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf, ",");
        int time = atoi(out[0].c_str());
        question._beginTime = time;
        question._endTime = time;
        question._personName = out[1];
        question._placeName = out[2];
        question._constraint = getEnumFromString(out[3]);
    }
}

#pragma mark -
#pragma mark string

vector<string> SpritString(const string &src,const string &delim) {
    string::size_type start = 0;
    vector<string> dest;
    while(true){
        string::size_type end = src.find(delim, start);
        if(end != string::npos){
            dest.push_back(src.substr(start, end - start));
        }
        else{
            dest.push_back(src.substr(start, src.length() - start));
            break;
        }
        start = end + delim.length();
    }
    return dest;
}


#pragma mark -
#pragma mark Action

void doAction(map<string,Person> &persons,map<string,Place> &places) {
    
    for(map<string,Person>::iterator it = persons.begin();it != persons.end();it++) {
        Person person = (*it).second;
        Place place = places[person._nowPlace];
        int nextSize = (int)place._nextPlaces.size();
        int id = xor128() % (nextSize+1);
        
        if(id == 0) {
            //do nothing
        } else {
            move(person,persons,places,places[place._nextPlaces[id-1]]);
        }
    }
}

void doActionWithHeuristics(map<string,Person> &persons,map<string,Place> &places) {
    
    int nothingPoint = 2;
    int moveNextPlacePoint = 2;
    int moveLastPlacePoint = 1;
    
    for(map<string,Person>::iterator it = persons.begin();it != persons.end();it++) {
        Person person = (*it).second;
        Place place = places[person._nowPlace];
        int nextSize = (int)place._nextPlaces.size();
        
        if(person._lastPlace == "") moveLastPlacePoint = moveNextPlacePoint;
  
        vector<string> actions((nextSize - 1)*moveNextPlacePoint + nothingPoint + moveLastPlacePoint);

        int index = 0;
        for(int i=0;i<nothingPoint;i++) {
            actions[index] = "nothing";
            index++;
        }
        for(unsigned int i=0;i<(unsigned int)nextSize;i++) {
            string nextPlaceName = place._nextPlaces[i];
            if(person._lastPlace != "") {
                if(nextPlaceName == person._lastPlace) {
                    actions[index] = "move:" + nextPlaceName;
                    index++;
                } else {
                    for(int j=0;j<2;j++) {
                        actions[index] = "move:" + nextPlaceName;
                        index++;
                    }
                }
            } else {
                for(int j=0;j<2;j++) {
                    actions[index] = "move:" + nextPlaceName;
                    index++;
                }
            }
        }
        
        int id = 0;
        int actionArrayCount = (int)actions.size();
        if(actionArrayCount != 0) {
            id = xor128() % actionArrayCount;
        }
 
        
        string action = actions[id];
        if(action == "nothing") {
            //do nothing
        }
        if(action.substr(0,5) == "move:") {
            string nextPlaceString = action.substr(5);
            move(person,persons,places,places[nextPlaceString]);
        }
        
        
    }
}

void doActionUCB(map<string,Person> &persons,map<string,Place> &places,int nowTime,int endTime,vector<Constraint> &constraints,vector<Episode> &episodes) {
    //１ステップ分の行動を原始モンテカルロで決定し、処理する。
    double cp = 1.0;
    vector< vector<MCTNode> > personsUCB;
    
    //人数分ノードを初期化
    for(map<string,Person>::iterator it = persons.begin();it != persons.end();it++) {
        Person person = (*it).second;
        vector<MCTNode> nodes;
        
        //取れるアクションの数だけノードを追加
        nodes.push_back(makeNode("nothing", 0, 0, 99999999));
        for(int i=0;i<(int)places[person._nowPlace]._nextPlaces.size();i++) {
            string next = places[person._nowPlace]._nextPlaces[i];
            nodes.push_back(makeNode("move:"+next, 0, 0, 99999999));
        }
        
        personsUCB.push_back(nodes);
    }
    
    
    for(int round = 0;round < 1000;round++) {
        //プレイアウト開始
        
        //各種情報のコピー(リセット）
        map<string,Person> nowPersons(persons);
        map<string,Place> nowPlaces(places);
        vector<Episode> nowEpisodes(episodes);
        
        
        vector<string> actions;
        actions.clear();
        
        int time = nowTime;//現在時刻の設定
        
        //show(time, nowPlaces);
        
        //各エージェントはUCBが最大のアクションを選択
        int p=0;
        for(map<string,Person>::iterator it = nowPersons.begin();it != nowPersons.end();it++) {
            Person person = (*it).second;
            double maxUcb = -99999999;
            string action = "nothing";
            for(int i=0;i<(int)personsUCB[p].size();i++) {
                if(maxUcb < personsUCB[p][i].ucb) {
                    maxUcb = personsUCB[p][i].ucb;
                    action = personsUCB[p][i].action;
                }
            }
            if(action.substr(0,5) == "move:") {
                string next = action.substr(5);
                move(person,nowPersons,nowPlaces,nowPlaces[next]);
            }
            actions.push_back(action);
            p++;
        }
        Episode episode1(time,nowPersons,nowPlaces);
        nowEpisodes.push_back(episode1);
        
        //あとはランダムに実行
        while(time < endTime) {
            time++;
            doAction(nowPersons, nowPlaces);
            Episode episode(time,nowPersons,nowPlaces);
            nowEpisodes.push_back(episode);
        }
        
        
        p=0;
        for(map<string,Person>::iterator it = nowPersons.begin();it != nowPersons.end();it++) {
            Person person = (*it).second;
            string selectedAction = actions[p];
            int index = -1;
            for(int i=0;i<(int)personsUCB[p].size();i++) {
                if(personsUCB[p][i].action == selectedAction) {
                    index = i;
                    break;
                }
            }
            
            string personName = person._name;
            double val = checkEpisodePerson(nowEpisodes, constraints,personName,false);
            
            personsUCB[p][index].count++;
            personsUCB[p][index].sumValue += val;
            
            int totalCount = 0;
            for(int i=0;i<(int)personsUCB[p].size();i++) {
                totalCount += personsUCB[p][i].count;
            }
            for(int i=0;i<(int)personsUCB[p].size();i++) {
                MCTNode node = personsUCB[p][i];
                personsUCB[p][i].ucb = calcUcb1(node.sumValue, node.count, totalCount,cp);
            }
            p++;
        }
        //プレイアウト終了
        showEpisode(nowEpisodes);
    }

    
    //最もUCB値が高いものを選択し、処理する
    int p=0;
    for(map<string,Person>::iterator it = persons.begin();it != persons.end();it++) {
        Person person = (*it).second;
        int maxUcb = -99999999;
        string action = "nothing";
        for(int i=0;i<(int)personsUCB[p].size();i++) {
            if(maxUcb < personsUCB[p][i].ucb) {
                maxUcb = personsUCB[p][i].ucb;
                action = personsUCB[p][i].action;
            }
        }
        if(action.substr(0,5) == "move:") {
            string next = action.substr(5);
            move(person,persons,places,places[next]);
        }
        p++;
    }
}

int randomAction(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints) {
    map<string,Person> nowPersons(persons);
    map<string,Place> nowPlaces(places);
    vector<Episode> nowEpisodes;
    Episode episode(0,nowPersons,nowPlaces);
    nowEpisodes.push_back(episode);
    
    int sumPlayout = 0;
    

    
    
    vector< vector<Episode> >completeEpisodess;
    
    for(map<string,Person>::iterator it = nowPersons.begin();it != nowPersons.end();it++) {
        Person person = (*it).second;
        
        vector<Episode> completeEpisodes;
        bool isFinish = false;
        
        
        int playout = 100000;
        
        for(int round=0;round<playout;round++) {
            sumPlayout++;
            if(isFinish) break;
             
            map<string,Person> randomPersons(nowPersons);
            map<string,Place> randomPlaces(nowPlaces);
            vector<Episode> randomEpisodes(nowEpisodes);
            int time = 0;
            while(time < 10) {
                time++;
                doAction(randomPersons, randomPlaces);
                Episode episode(time,randomPersons,randomPlaces);
                randomEpisodes.push_back(episode);
            }
            /*
             if(round%50 == 0) {
             cout << person._name << ":" << round << endl;
             showEpisode(randomEpisodes);
             }
             */
            
            // showEpisode(randomEpisodes);
            double getVal = checkEpisodePerson(randomEpisodes, constraints, person._name,false);
           
            if(getVal == 1.0) {
                completeEpisodes = randomEpisodes;
                isFinish = true;
            }
            
        }
        
        
        //showEpisode(getOnlyPersonEpisode(person._name, completeEpisodes));
        completeEpisodess.push_back(getOnlyPersonEpisode(person._name, completeEpisodes));
    }
    
    //vector<Episode> resultEpisode = getFusionEpisode(completeEpisodess);
    //showEpisode(resultEpisode);
    
    return sumPlayout;
}

void allSearchAction(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints) {
    map<string,Person> nowPersons(persons);
    map<string,Place> nowPlaces(places);
    vector<Episode> nowEpisodes;
    Episode episode(0,nowPersons,nowPlaces);
    nowEpisodes.push_back(episode);
    
    int step = 0;
    
    vector< vector<Episode> >completeEpisodess;
    
    for(map<string,Person>::iterator it = persons.begin();it != persons.end();it++) {
        Person person = (*it).second;
        
        vector<Episode> completeEpisodes;
        
        bool isFinish = false;
        
        vector<string> actions;
        actions.push_back("nothing");
        for(int i=0;i<(int)places[persons[person._name]._nowPlace]._nextPlaces.size();i++) {
            string next = places[persons[person._name]._nowPlace]._nextPlaces[i];
            actions.push_back("move:"+next);
        }
        for(int i=0;i<(int)actions.size();i++) {
            //アクションを実行
            map<string,Person> nowPersons(persons);
            map<string,Place> nowPlaces(places);
            vector<Episode> nowEpisodes2(nowEpisodes);
            if(actions[i].substr(0,5) == "move:") {
                string next = actions[i].substr(5);
                move(person,nowPersons,nowPlaces,nowPlaces[next]);
            }
            Episode episode(step+1,nowPersons,nowPlaces);
            nowEpisodes2.push_back(episode);
            subAllSearch(nowPersons, nowPlaces, constraints, nowEpisodes2, isFinish, actions[i], person, step+1,completeEpisodes);
        }
        
        
        
        //showEpisode(getOnlyPersonEpisode(person._name, completeEpisodes));
        completeEpisodess.push_back(getOnlyPersonEpisode(person._name, completeEpisodes));
    }
    
    vector<Episode> resultEpisode = getFusionEpisode(completeEpisodess);
    showEpisode(resultEpisode);
}

void subAllSearch(map<string,Person> persons,map<string,Place> places,vector<Constraint> &constraints,vector<Episode>episodes,bool &isFinish,string action,Person person,int step,vector<Episode> &answerEpisodes) {
    if(isFinish) return;
    
    if(step == 10) {
        //探索の底なので評価する
        double getVal = checkEpisodePerson(episodes, constraints, person._name,false);
        
        if(getVal == 1.0) {
            isFinish = true;
            answerEpisodes = episodes;
        }
        return;
    } else {
        vector<string> actions;
        actions.push_back("nothing");
        for(int i=0;i<(int)places[persons[person._name]._nowPlace]._nextPlaces.size();i++) {
            string next = places[persons[person._name]._nowPlace]._nextPlaces[i];
            actions.push_back("move:"+next);
        }
        for(int i=0;i<(int)actions.size();i++) {
            //アクションを実行
            map<string,Person> nowPersons(persons);
            map<string,Place> nowPlaces(places);
            
            if(actions[i].substr(0,5) == "move:") {
                string next = actions[i].substr(5);
                move(person,nowPersons,nowPlaces,nowPlaces[next]);
            }
            vector<Episode> nowEpisodes(episodes);
            Episode episode(step+1,nowPersons,nowPlaces);
            nowEpisodes.push_back(episode);
            subAllSearch(nowPersons, nowPlaces, constraints, nowEpisodes, isFinish, actions[i], person, step+1,answerEpisodes);
        }
    }
    return;
}

int doActionMCTS(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints,int endTime,string outputFilename) {
    cout << "start mcts" << endl;
    clock_t start,end;
    start = clock();
    
    map<string,Person> nowPersons(persons);
    map<string,Place> nowPlaces(places);
 //   vector<Episode> nowEpisodes;
    Episode episode(0,nowPersons,nowPlaces);
 //   nowEpisodes.push_back(episode);
 
    int maxDepth = 0;
    
    int sumPlayout = 0;
    
    double cp = 0.2;
    
    vector< vector<Episode> >completeEpisodess;
    
    for(map<string,Person>::iterator it = nowPersons.begin();it != nowPersons.end();it++) {
        Person person = (*it).second;
        
        vector<Episode> completeEpisodes;
        bool isFinish = false;
        
  //      double maxValue = -100000;
        double score = 0;
        
        //rootノードを作成
        int sumCount = 0;
        MCTREE root;
        root.count = 0;
        root.ucb = 0;
        root.sumVal = 0;
        root.parent = NULL;
        root.episode = episode;
        root.sumCount = &sumCount;
        root.action = "nothing";
        root.childs.clear();
        
        MCTREE *current = &root;
        
        int playout = 1000000;
        double finishRate = 0.98;
        
        
        
        for(int round=0;round<playout;round++) {
            sumPlayout++;
            if(isFinish) break;
            bool downFlag = true;//trueなら木を潜り続ける
            int nowDepth = 0;
            //showTree(&root);
            
            double getVal = 0;//取得した報酬
            
            while(downFlag && !isFinish) {
                //取れるアクションを取得
                
                if(current->episode._time >= endTime) {
                    downFlag = false;
                    
                    vector<Episode> episodes;
                    makeEpisodesFromTree(&root, current, episodes);
                    getVal = checkEpisodePerson(episodes, constraints, person._name,false);
                    
                    break;
                }
                vector<string> actions;
                actions.push_back("nothing");
                for(int i=0;i<(int)current->episode._places[current->episode._persons[person._name]._nowPlace]._nextPlaces.size();i++) {
                    string next = places[current->episode._persons[person._name]._nowPlace]._nextPlaces[i];
                    actions.push_back("move:"+next);
                }
                //各アクションの対応する子ノードが全部存在すれば、UCB値を比較することにする
                //なかったら、なかった者のなかからランダムで選ぶ
                double maxUcb = -99999999;
             
                vector<int> maxIndexs;
                vector<string>notFoundActions;
                for(int i=0;i<(int)actions.size();i++) {
                    string action = actions[i];
                    //actionと一致する子ノードがあるかどうか調べる
                    int index = -1;
                    for(int j=0;j<(int)current->childs.size();j++) {
                        if(current->childs[j]->action == action) {
                            index = j;
                            break;
                        }
                    }
                    if(index == -1) {
                        notFoundActions.push_back(action);
                        continue;
                    } else {
                        current->childs[index]->ucb = calcUcb1(current->childs[index]->sumVal, current->childs[index]->count, *current->childs[index]->sumCount,cp);
                        if(maxUcb <= current->childs[index]->ucb) {
                            maxIndexs.push_back(index);
                            if(maxUcb != current->childs[index]->ucb) {
                                maxIndexs.clear();
                                maxIndexs.push_back(index);
                            }
                            maxUcb = current->childs[index]->ucb;
                        }
                    }
                }
                if(notFoundActions.size() <= 0) {
                    int randIndex = xor128() % maxIndexs.size();
                    int maxindex = maxIndexs[randIndex];
                    current = current->childs[maxindex];
                    nowDepth++;
                    continue;
                } else {
                    int randIndex = xor128() % notFoundActions.size();
                    string notFoundAction = notFoundActions[randIndex];
                    //対応するアクションの子ノードがない場合は、子ノードを拡張し、プレイアウトを行いucb値を更新しながらrootに戻る
                    MCTREE* child = makeTree(current->episode ,current, current->episode._time + 1, current->sumCount, notFoundAction);
                    current->childs.push_back(child);
                    current = child;
                //    assertEpisode(current->episodes);
                    if(notFoundAction.substr(0,5) == "move:") {
                        string next = notFoundAction.substr(5);
                        
                        move(person,current->episode._persons,current->episode._places,current->episode._places[next]);
                    }
                    if(notFoundAction == "nothing") {
                       
                    }
                 //   assertEpisode(current->episodes);
                    //あとはランダムに実行
                    map<string,Person> randomPersons(current->episode._persons);
                    map<string,Place> randomPlaces(current->episode._places);
                //    vector<Episode> randomEpisodes(current->episodes);
                    int time = current->episode._time;
                    
                    Episode *episodes = new Episode[endTime - time];
                    
                    int I = 0;
                    while(time < endTime) {
                        time++;
                     
                        doActionWithHeuristics(randomPersons, randomPlaces);
                     
                    //    Episode episode(time,randomPersons,randomPlaces);
                     
                        episodes[I]._persons = randomPersons;
                        episodes[I]._places = randomPlaces;
                        episodes[I]._time = time;
                        I++;
                      //  randomEpisodes.push_back(episode);
                        
                   //     cout << randomPersons["Alexander"]._nowPlace << "," << randomPersons["Alexander"]._lastPlace << endl;
                    
                    }
                    
                    
                   // showEpisode(randomEpisodes);
                 //   getVal = checkEpisodePerson(randomEpisodes, constraints, person._name);
                  //  showEpisodeWithPerson(current->episodes);
                    /*
                    vector<Episode> TreeEpisodes;
                    makeEpisodesFromTree(&root, current, TreeEpisodes);
                    getVal = checkEpisodePersonWithArray(&TreeEpisodes, constraints, person._name, episodes);
                     */
                    getVal = checkEpisodePersonWithArrayWithTree(&root, current, constraints, person._name, episodes,false);
                    
                    
                    
                    if(round%1000 == 0) {
                        /*
                         cout << person._name << ":" << round << endl;
                         showEpisodeWithPerson(randomEpisodes);
                         */
                        end = clock();
                        double time = (double)(end-start)/CLOCKS_PER_SEC / round;
                        cout << round << ":" << getVal << "/" << time << "/" << 1.0/time << "/depth:" << nowDepth << endl;
                       // outputAverageReward("averageReward1202", round, getVal);
                    }
                    /*
                    if(maxValue < getVal) {
                        maxValue = getVal;
                        completeEpisodes = randomEpisodes;
                       // cout << "name:" << person._name << " round:" << round << "val:" << getVal <<endl;
                    }
                     */
                    if(round >= playout-1 || getVal >= finishRate) {
                        score = getVal;
                        vector<Episode> TreeEpisodes;
                        makeEpisodesFromTree(&root, current, TreeEpisodes);
                        completeEpisodes = TreeEpisodes;
                        for(int s=0;s<I;s++) {
                            Episode addEpisode;
                            addEpisode._persons = episodes[s]._persons;
                            addEpisode._places = episodes[s]._places;
                            addEpisode._time = episodes[s]._time;
                            completeEpisodes.push_back(addEpisode);
                        }
                        isFinish = true;
                    }
                    
                    delete [] episodes;
                    
                    downFlag = false;
                    sumCount++;
                    break;
                }
            }
            
            if(maxDepth < nowDepth) {
                maxDepth = nowDepth;
            }
            
            while(!downFlag && !isFinish) {
                //rootに戻りながらucb値を更新していく
                
                current->sumVal += getVal;
                current->count++;
                current->ucb = calcUcb1(current->sumVal, current->count, *current->sumCount,cp);
                if(current != &root) {
                    current = current->parent;
                    continue;
                } else {
                    downFlag = true;
                    break;
                }
            }
        }
        checkEpisodePerson(getOnlyPersonEpisode(person._name, completeEpisodes), constraints, person._name, true);
        
        showEpisodeWithPerson(getOnlyPersonEpisode(person._name, completeEpisodes));
        completeEpisodess.push_back(getOnlyPersonEpisode(person._name, completeEpisodes));
        
        EpisodesOutput(completeEpisodes,outputFilename,person._name,score,maxDepth);
        
        //showTree(&root);
        
        deleteTree(&root);
    }
    
    //vector<Episode> resultEpisode = getFusionEpisode(completeEpisodess);
    //showEpisode(resultEpisode);
    return sumPlayout;
}


void move(Person &person,map<string,Person> &persons,map<string,Place> &places,Place &nextplace) {
    string movePersonName = person._name;
    string nowPlaceName = persons[movePersonName]._nowPlace;
    string nextPlaceName = nextplace._name;
    if(nowPlaceName != nextPlaceName) {
        persons[movePersonName]._lastPlace = nowPlaceName;
    }
    
    Place *place = &places[persons[movePersonName]._nowPlace];

  //  cout << "move:" << person._name << "(" << place->_name << "▶︎" << nextplace._name << ")" << endl;
    
    for(int i=0;i<(int)place->_persons.size();i++) {
        if(place->_persons[i]._name == person._name) {
            place->_persons.erase(place->_persons.begin()+i);
            //show(-1,places);
            break;
        }
    }
    //cout << "move:" << person._name << "(" << place->_name << "▶︎" << nextplace._name << ")" << endl;
    persons[person._name]._nowPlace = nextplace._name;
    nextplace._persons.push_back(person);
}

#pragma mark -
#pragma mark check

bool checkQuestion(vector<Episode> episodes,Question question) {
    bool flag = false;
    
    vector<Episode> episodeT = findEpisodeFromTime(question._beginTime,question._endTime, episodes);
    for(int i=0;i<(int)episodeT.size();i++) {
        if(question._constraint == there_is) {
            if(episodeT[i]._persons[question._personName]._nowPlace == question._placeName) {
                flag = true;
                break;
            }
        }
        if(question._constraint == there_is_no) {
            if(episodeT[i]._persons[question._personName]._nowPlace != question._placeName) {
                flag = true;
                break;
            }
        }
    }
    
    return flag;
}

double checkEpisode(vector<Episode> episodes,vector<Constraint> constraints) {
    double val = 0;
    int count = (int)constraints.size();
    int correct = 0;
    
    
    for(int i=0;i<count;i++) {
        int beginTime = constraints[i]._beginTime;
        int endTime = constraints[i]._endTime;
        string personName = constraints[i]._personName;
        string placeName = constraints[i]._placeName;
        CONSTRAINT constraint = constraints[i]._constraint;
        vector<Episode> episodeT = findEpisodeFromTime(beginTime,endTime, episodes);
        
        bool okFlag = false;
        
        for(int j=0;j<(int)episodeT.size();j++) {
            Episode episode = episodeT[j];
         
            if(constraint == there_is) {
                if(episode._persons[personName]._nowPlace == placeName) {
                    okFlag = true;
                    break;
                }
            }
            if(constraint == there_is_no) {
                if(episode._persons[personName]._nowPlace != placeName) {
                    okFlag = true;
                    break;
                }
            }
        }
        
        if(constraint == there_is && okFlag) {
            correct++;
        }
        if(constraint == there_is_no && !okFlag) {
            correct++;
        }
    }
    
    //cout << "this episode's result are " << correct << "/" << count << endl;
    val = (double)correct / (double)count ;
    
    return val;
}

double checkEpisodePerson(vector<Episode> episodes,vector<Constraint> constraints,string _personName,bool isShowConstraints) {
    double val = 0;
    int count = (int)constraints.size();
    int correct = 0;
    int hitCount = 0;
    
    
    for(int i=0;i<count;i++) {
        int beginTime = constraints[i]._beginTime;
        int endTime = constraints[i]._endTime;
        string personName = constraints[i]._personName;
        if(personName != _personName) {
            continue;
        }
        
        hitCount++;
        
        string placeName = constraints[i]._placeName;
        CONSTRAINT constraint = constraints[i]._constraint;
        vector<Episode> episodeT = findEpisodeFromTime(beginTime,endTime, episodes);
        
        bool okFlag = false;
        
        for(int j=0;j<(int)episodeT.size();j++) {
            Episode episode = episodeT[j];
            
            if(constraint == there_is) {
                if(episode._persons[personName]._nowPlace == placeName) {
                    okFlag = true;
                    if(isShowConstraints) {
                        constraints[i].show();
                        cout << "ok" << endl;
                    }
                    break;
                }
            }
            if(constraint == there_is_no) {
                if(episode._persons[personName]._nowPlace != placeName) {
                    okFlag = true;
                    if(isShowConstraints) {
                        constraints[i].show();
                        cout << "ok" << endl;
                    }
                    break;
                }
            }
        }
        
        if(constraint == there_is && okFlag) {
            correct++;
        }
        if(constraint == there_is_no && !okFlag) {
            correct++;
        }
    }
    
    //cout << "this episode's result are " << correct << "/" << count << endl;
    if(hitCount == 0) return 1.0;
    val = (double)correct / (double)hitCount ;
    
    return val;
}

double checkEpisodePersonWithArrayWithTree(MCTREE *root,MCTREE *leaf, vector<Constraint> constraints,string _personName,Episode *episodesArray,bool isShowConstraints) {
    double val = 0;
    int count = (int)constraints.size();
    int correct = 0;
    int hitCount = 0;
    
    int episodesSize = sizeof(episodesArray) / sizeof(Episode);
    
    vector<Episode*> episodes;
    MCTREE *makeEpisodeCurrent = leaf;
    while(true) {
        episodes.insert(episodes.begin(), &makeEpisodeCurrent->episode);
        if(makeEpisodeCurrent != root) {
            makeEpisodeCurrent = makeEpisodeCurrent->parent;
            continue;
        } else {
            break;
        }
    }
    for(int i=0;i<episodesSize;i++) {
        episodes.push_back(&episodesArray[i]);
    }
    
    for(int i=0;i<count;i++) {
        int beginTime = constraints[i]._beginTime;
        int endTime = constraints[i]._endTime;
        string personName = constraints[i]._personName;
        if(personName != _personName) {
            continue;
        }
        
        hitCount++;
        
        string placeName = constraints[i]._placeName;
        CONSTRAINT constraint = constraints[i]._constraint;
        
        bool okFlag = false;
  
        for(int j=0;j<(int)episodes.size() ;j++) {
            if(episodes.at(j)->_time >= beginTime && episodes.at(j)->_time <= endTime) {
                
                if(constraint == there_is) {
                    if(episodes.at(j)->_persons[personName]._nowPlace == placeName) {
                        okFlag = true;
                        if(isShowConstraints) {
                            constraints[i].show();
                            cout << "ok" << endl;
                        }
                        break;
                    }
                }
                if(constraint == there_is_no) {
                    if(episodes.at(j)->_persons[personName]._nowPlace != placeName) {
                        okFlag = true;
                        if(isShowConstraints) {
                            constraints[i].show();
                            cout << "ok" << endl;
                        }
                        break;
                    }
                }
            
            } else {
                continue;
            }
        }
        
        if(constraint == there_is && okFlag) {
            correct++;
        }
        if(constraint == there_is_no && !okFlag) {
            correct++;
        }
        
    }
    
    //cout << "this episode's result are " << correct << "/" << count << endl;
    if(hitCount == 0) return 1.0;
    val = (double)correct / (double)hitCount ;
    
    return val;
}



bool isEqualStringWithoutCapital(string a,string b) {
    if(a.size() != b.size()) return false;
    bool flag = true;
    for(unsigned i=0;i<a.size();i++) {
        if( a.substr(i,1).c_str()[0] != b.substr(i,1).c_str()[0]) {
            flag = false;
            break;
        }
    }
    return flag;
}

bool isEqualStringWithoutOrthographicalVariant(string &a,string &b) {
    string A = removeOrthographicalVariantString(a);
    string B = removeOrthographicalVariantString(b);
    return (A == B);
}

string removeOrthographicalVariantString(string &a) {
    string dic = "";
    dic += "Pella/";
    dic += "Granicus,Granikos/";
    dic += "Sardis,Sardes/";
    dic += "Ephesus,Ephesos/";
    dic += "Miletus,Miletos,Milet/";
    dic += "Halicarnassus,Halikarnassos/";
    dic += "Side/";
    dic += "Gordion,Gordium/";
    dic += "Ankara,Ancyra/";
    dic += "Tarsus,Tarsa/";
    dic += "Issus,Issos/";
    dic += "Byblos/";
    dic += "Sidon,Saida/";
    dic += "Tyre,Tyrus,Tyros/";
    dic += "Gaza/";
    dic += "Memphis,Egypt/";
    dic += "Alexandria,Aleksandria/";
    dic += "Siwa,Siwa Oasis,Ammonium,Amonin/";
    dic += "Thebes/";
    dic += "Damascus,Damaskos/";
    dic += "Comana,Comana Pontica/";
    dic += "Nisibis,Nusaybin/";
    dic += "Gaugamela/";
    dic += "Arbela/";
    dic += "Opis/";
    dic += "Babylon/";
    dic += "Susa,Shushan/";
    dic += "Charax,Charax Spasinu,Spasinu Charax,A Susiana/";
    dic += "Persepolis/";
    dic += "Pasargadae,Pathragada/";
    dic += "Ecbatana,Ekbatana/";
    dic += "Ray,Rey,Rhagae/";
    dic += "Hecatompylos,Qumis,Hekatompylos,Saddarvazeh/";
    dic += "Herat,A Areion/";
    dic += "A Prophthasia,A Drangiana/";
    dic += "A Arachosia,A Arakhosia/";
    dic += "A Caucasus,Cabul,Kabul/";
    dic += "Drapsaca,Drapsaka/";
    dic += "Bactra,Baktra,Bactria/";
    dic += "Samarkand,Maracanda/";
    dic += "A Eschate,A Eskhate/";
    dic += "Ai Khanoum,Ai-Khanoum,Ay Khanoum,Ay-Khanoum,A Oxus/";
    dic += "A Carmania,A Karmania/";
    dic += "Bela,A Rhambacia/";
    dic += "Patala,Thatta/";
    dic += "A Indus/";
    dic += "Aornos,Aornus/";
    dic += "Texila/";
    dic += "Nicaea,Nicea/";
    dic += "Sagala,Sakala,Sangala";
    
    
    string result = a;
    /*
    string fileName = "orthographical_variants.txt";
    fstream ifs(fileName);
    if(!ifs) {
        cout << "error: not found file '" << fileName << "' @removeOrthographicalVariantString" << endl;
        exit(0);
    }
 */
    string buf;
    bool finishFlag = false;
    
    vector<string> out2 = SpritString(dic, "/");
    for(unsigned int j=0;j<out2.size() && !finishFlag;j++) {
        vector<string> out = SpritString(out2[j], ",");
        for(unsigned int i=0;i<out.size();i++) {
            if(isEqualStringWithoutCapital(out[i], a)) {
                result = out[0];
                finishFlag = true;
                break;
            }
        }
    }
    /*
    while(getline(ifs,buf) && !finishFlag) {
        vector<string> out = SpritString(buf, ",");
        for(unsigned int i=0;i<out.size();i++) {
            if(isEqualStringWithoutCapital(out[i], a)) {
                result = out[0];
                finishFlag = true;
                break;
            }
        }
    }
    */
   // ifs.close();
    return result;
}

string processingOfAlexandria(string &a) {
    string result = a;
    string alexandria = "Alexandria";
    string stringA = "A";
    string stringOf = "of";
    string stringOn = "on";
    string stringIn = "in";
    string stringThe = "the";
    vector<string> out = SpritString(a, " ");
    if(out.size() == 4) {
        if(isEqualStringWithoutCapital(out[0],alexandria) || isEqualStringWithoutCapital(out[0], stringA)) {
            if(isEqualStringWithoutCapital(out[1], stringOf) || isEqualStringWithoutCapital(out[1], stringOn) || isEqualStringWithoutCapital(out[1], stringIn)) {
                if(isEqualStringWithoutCapital(out[2], stringThe)) {
                    result = "A " + out[3];
                }
            }
        }
    }
    if(out.size() == 3) {
        if(isEqualStringWithoutCapital(out[0],alexandria) || isEqualStringWithoutCapital(out[0], stringA)) {
            if(isEqualStringWithoutCapital(out[1], stringOf) || isEqualStringWithoutCapital(out[1], stringOn) || isEqualStringWithoutCapital(out[1], stringIn)) {
                result = "A " + out[2];
            }
        }
    }
    
    if(out.size() == 2) {
        if(isEqualStringWithoutCapital(out[0],alexandria) || isEqualStringWithoutCapital(out[0], stringA)) {
            result = "A " + out[1];
        }
    }
     
    
    return result;
}

#pragma mark -
#pragma mark show

void show(int time,map<string,Place> places) {
    cout << "----------------------------" << endl;
    //cout << "time: " << time << endl;
    showBC(time);
    for(map<string,Place>::iterator it = places.begin();it != places.end();it++) {
        cout << "<" << (*it).first << ">" ;
        for(int i=0;i<(int)(*it).second._persons.size();i++) {
            Person person = (*it).second._persons[i];
            cout << "(" << person._name << "," << person._lastPlace << ")";
        }
        cout << endl;
    }
}

void showOnlyPerson(int time,map<string,Place> places) {
    cout << "----------------------------" << endl;
    //cout << "time: " << time << endl;
    showBC(time);
    for(map<string,Place>::iterator it = places.begin();it != places.end();it++) {
        if((*it).second._persons.size() <= 0) continue;
        cout << "<" << (*it).first << ">" ;
        for(int i=0;i<(int)(*it).second._persons.size();i++) {
            Person person = (*it).second._persons[i];
            cout << "(" << person._name << "," << person._lastPlace <<  ")";
        }
        cout << endl;
    }
}

void showEpisode(vector<Episode> episodes) {
    for(int i=0;i<(int)episodes.size();i++) {
        show(episodes[i]._time,episodes[i]._places);
    }
}

void showEpisodeWithPerson(vector<Episode> episodes) {
    for(int i=0;i<(int)episodes.size();i++) {
        showOnlyPerson(episodes[i]._time, episodes[i]._places);
    }
}

void showPersons(map<string,Person> &persons) {
    for(map<string,Person>::iterator it = persons.begin();it != persons.end();it++) {
        Person person = (*it).second;
        cout << "(" << person._name << "," << person._nowPlace << ")";
    }
    cout << endl;
}

void showBC(int time) {
    vector<int> bc = getBCfromTime(time);
    char str[10];
    sprintf(str,"%d", bc[0]);
    string year = string(str);
    sprintf(str,"%d", bc[1]);
    string month = string(str);
    cout << "BC." << year << " " << month << "月 (" << time << ")" << endl;
}

void outputAverageReward(string filename,int playout,double reward) {
    ofstream ofs(filename.c_str(),ios::app );
  
    ofs << playout << "," << reward << endl;
    
    ofs.close();
}

void showConstraintsPlaceDistribution(vector<Constraint> &constraints) {
    cout << "show place distribution of constraints" << endl;
    map<string,int> placeCount;
    map<string,int>::iterator it;
    for(unsigned int i=0;i<constraints.size();i++) {
        string placeName = removeOrthographicalVariantString( constraints[i]._placeName );
        it = placeCount.find(placeName);
        if(it == placeCount.end()) {
            placeCount.insert(make_pair(placeName, 1));
        } else {
            (*it).second++;
        }
    }
    
    for(it = placeCount.begin();it != placeCount.end();it++) {
        cout << (*it).first << " " << (*it).second << endl;
    }
}

void showDifferentConstraints(vector<Constraint> &a,vector<Constraint> &c) {
    int completeCount = 0;
    int notCompleteCount = 0;
    int onlyACount = 0;
    int onlyCCount = 0;
    unsigned int ia=0,ic=0;
    while(ia < a.size() && ic < c.size()) {
        if(a[ia]._id == c[ic]._id) {
            if(a[ia]._beginTime == c[ic]._beginTime && a[ia]._endTime == c[ic]._endTime && a[ia]._personName == c[ic]._personName && a[ia]._personName == c[ic]._personName && a[ia]._constraint == c[ic]._constraint) {
                completeCount++;
            } else {
                notCompleteCount++;
            }
            ia++;
            ic++;
        } else {
            if(a[ia]._id < c[ic]._id) {
                ia++;
                onlyACount++;
            } else {
                ic++;
                onlyCCount++;
            }
        }
    }
    
    cout << "completeCount:" << completeCount << "notCompleteCount:" << notCompleteCount << " onlyRuleCount:" << onlyACount << " onlyAnnotateCount:" << onlyCCount << endl;
}

void checkConstraintGenerator(string testFilename,vector<Constraint> &constraints) {
    ifstream ifs(testFilename.c_str());
    if(!ifs) {
        cout << "error:not found file '" << testFilename << "' @checkConstraintGenerator" << endl;
        exit(0);
    }
    string buf;
    int count = 0;
    int correct = 0;
    int hitcount = 0;
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf, ",");
        string BCtime = out[0];
        string month = out[1];
        string personName = out[2];
        string placeName = out[3];
        CONSTRAINT c = getEnumFromString(out[4]);
        Constraint constrain =constraints[count];
        count++;
        if(BCtime != "-" && personName != "-" && placeName != "-" && c != none) {
            hitcount++;
            cout << count << "lines" << endl;
            cout << "annotation:" << BCtime << "/" << month << "/" << personName << "/" << placeName << "/" << getStringFromEnum(c) << endl;
            constrain.show();
            int beginTime;
            int endTime;
            int bc = atoi(BCtime.c_str());
            if(month != "-") {
                int mon = getMonthFromString(month);
                beginTime = getTimeFromBC(bc, mon);
                endTime = getTimeFromBC(bc, mon);
            } else {
                beginTime = getTimeFromBC(bc,1);
                endTime = getTimeFromBC(bc, 12);
            }
            cout << "annotationTime:" << beginTime << ":" << endTime << endl;
            if(constrain._constraint == c && constrain._personName == personName && constrain._placeName == placeName && constrain._beginTime == beginTime && constrain._endTime == endTime) {
                cout << beginTime << ":" << endTime << endl;
                cout << "ok" << endl;
                correct++;
            }
        }
    }
    cout << "correct" << correct << endl;
    cout << "hit" << hitcount << endl;
}

#pragma mark -
#pragma mark calc

uint32_t xor128(void) {
    uint32_t gSeed128[4];
    uint32_t s = ((unsigned)time(NULL));
    for( int i = 1; i <= 4; i++ )
    {
        gSeed128[ i - 1 ] = s = 1812433253U * ( s ^ ( s >> 30 ) ) + i;
    }
    
    static uint32_t x = gSeed128[0];
    static uint32_t y = gSeed128[1];
    static uint32_t z = gSeed128[2];
    static uint32_t w = gSeed128[3];
    uint32_t t;
    
    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

double calcUcb1(double sumVal,int ni,int N,double c) {
    double x;
    if(ni == 0) {
        return 99999999;
    }
    x = sumVal / ni;
    return x + 2*c*sqrt(2 * log((double)N) / (double)ni);
}

vector<int> getBCfromTime(int time) {
    int year = 334 - time / 12;
    int month = time % 12 + 1;
    
    vector<int> bc;
    bc.push_back(year);
    bc.push_back(month);
    return bc;
}

int getTimeFromBC(int bc,int month) {
    if(month < 1 || month > 12) month = xor128()%12+1;
    return (334-bc)*12 + (month-1);
}

int getMonthFromString(string mString) {
    int month = -1;
    string monthString[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    vector<string>months;
    for(unsigned int i=0;i<12;i++) {
        months.push_back(monthString[i]);
    }
    for(unsigned int i=0;i<months.size();i++) {
        if(isEqualStringWithoutOrthographicalVariant(mString, months[i])) {
            month = i+1;
        }
    }
    
    return month;
}

#pragma mark -
#pragma mark tree

MCTNode makeNode(string _action,int _sum,int _count,double _ucb) {
    MCTNode node;
    node.action = _action;
    node.sumValue = _sum;
    node.count = _count;
    node.ucb = _ucb;
    return node;
}

MCTREE* makeTree(Episode _episode,MCTREE* _parent,int nowTime,int *sumCount,string action) {
    MCTREE *node = new MCTREE;
    node->count = 0;
    node->ucb = 0;
    node->sumVal = 0;
    node->parent = _parent;
    node->episode = _episode;
    node->sumCount = sumCount;
    node->childs.clear();
    node->action = action;
    node->episode._time = nowTime;
    return node;
}

void deleteTree(MCTREE *root) {
    MCTREE *current = root;
    while(root->childs.size() > 0) {
        if(current->childs.size() > 0) {
            current = current->childs[0];
            continue;
        } else {
            current = current->parent;
            delete current->childs[0];
            current->childs.erase(current->childs.begin());
            continue;
        }
    }
    
}

void showTree(MCTREE *root) {
    MCTREE *current = root;
    int depth = 0;
    showTreeSub(current, depth);
    cout << endl;
}

void showTreeSub(MCTREE *current,int depth) {
    string action = current->action;
    char str[10];
    sprintf(str,"%3f", current->ucb);
    string ucb = string(str);
    ucb = ucb.substr(0,4);
    action = action.substr(0,10);
    action = action + "(" + ucb + ")";
    action.append("-----------------");
    action = action.substr(0,15);
    
    cout << action ;
    int a = (int)current->childs.size();
    if(a == 0) {
        cout << endl;
    } else {
        for(int i=0;i<(int)current->childs.size();i++) {
            if(i==0) {
                cout << "--";
            } else  {
                for(int i=0;i<depth;i++) {
                    cout << "|                ";
                }
                cout << "+----------------";
            }
            showTreeSub(current->childs[i],depth+1);
        }
    }
}

void makeEpisodesFromTree(MCTREE *rootNode , MCTREE *leafNode,vector<Episode> &episodes) {
    MCTREE *makeEpisodeCurrent = leafNode;
    while(true) {
        episodes.insert(episodes.begin(), makeEpisodeCurrent->episode);
        // episodes.push_back(makeEpisodeCurrent->episode);
        if(makeEpisodeCurrent != rootNode) {
            makeEpisodeCurrent = makeEpisodeCurrent->parent;
            continue;
        } else {
            break;
        }
    }
}

#pragma mark -
string extractStringFromTextfileWithLine(string filename,int line) {
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "not found file '" << filename << "' @extractStringFromTextfileWithLine" << endl;
        exit(0);
    }
    
    string buf;
    for(int i=0;i<line;i++) {
        getline(ifs,buf);
    }
    return buf;
}

vector< vector<string> > extractAroundStringsFromTextfileWithString(string filename,string buf,int window) {
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "not found file '" << filename << "' @extractAroundStringFromTextfileWithString" << endl;
        exit(0);
    }
    
    vector< vector<string> > result;
    string tmp[10];
    
    string tmpBuf;
    while(getline(ifs,tmpBuf)) {
        for(int i=0;i<window;i++) {
            tmp[i] = tmp[i+1];
        }
        tmp[window] = tmpBuf;
        if(tmpBuf == buf) {
            vector<string> s;
            for(int i=0;i<window;i++) {
                s.push_back(tmp[i]);
            }
            s.push_back(tmpBuf);
            result.push_back(s);
        }
    }
    return result;
}

void outputCoreferenceTestFileFromUnknownPlaces(string outputFileName,string unknownPlaceFilename) {
    string pronoun[8] = {"the city","there","it","It","the area","here","this coast","the island"};
    
    ofstream ofs(outputFileName.c_str());
    
    ifstream ifs(unknownPlaceFilename.c_str());
    if(!ifs) {
        cout << "not found file '" << unknownPlaceFilename << "' @outputCoreferenceTestFileFromUnknownPlaces" << endl;
        exit(0);
    }
    string buf;
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf, ":");
        string name = out[0];
        int id = atoi(out[1].c_str());
        for(int i=0;i<8;i++) {
            if(pronoun[i] == name) {
                string sentence = extractStringFromTextfileWithLine("alexander_wikipedia_bc.txt", id);
                vector< vector<string> > ss = extractAroundStringsFromTextfileWithString("alexander_wikipedia.txt", sentence, 8);
                for(unsigned int j=0;j<ss.size();j++) {
                    ofs << "coreference:" << id << ":-----------------------------" << endl;
                    for(unsigned int k=0;k<ss[j].size();k++) {
                        ofs << ss[j][k] << endl;
                    }
                }
            }
        }
    }
}

void outputCoreferenceTestFileFromWikipedia(string outputFileName,vector<Constraint> &constraints,string wikipediaFilename) {
    ofstream ofs(outputFileName.c_str());
    
    for(unsigned int i=0;i<constraints.size();i++) {
        int id = constraints[i]._id;
        string buf = extractStringFromTextfileWithLine("alexander_wikipedia_bc.txt", id);
        vector< vector<string> > ss = extractAroundStringsFromTextfileWithString(wikipediaFilename, buf, 8);
        for(unsigned int j=0;j<ss.size();j++) {
            ofs << "coreference:" << id << ":-----------------------------" << endl;
            for(unsigned int k=0;k<ss[j].size();k++) {
                ofs << ss[j][k] << endl;
            }
        }
    }
    ofs.close();
}

#pragma mark -
#pragma mark debug

//おなじエージェントが複数存在したり、エージェントの位置情報とplaceのエージェント情報の一致を確認する
void assertEpisode(const vector<Episode> &episodes) {
    
    for(int j=0;j<(int)episodes.size();j++) {
        Episode episode = episodes[j];
    
        for(map<string,Place>::iterator it = episode._places.begin();it != episode._places.end();it++) {
            map<string,bool> checkPerson;
            for(map<string,Person>::iterator pit = episode._persons.begin();pit != episode._persons.end();pit++) {
                checkPerson.insert(make_pair((*pit).second._name, false));
            }
            
            Place place = (*it).second;
            for(int i=0;i<(int)place._persons.size();i++) {
                string personName = place._persons[i]._name;
                if(episode._persons.find(personName) == episode._persons.end()) {
                    cout << "episodeError: time=" << episode._time << "/place's person is not exist" << endl;
                    showEpisode(episodes);
                    exit(0);
                }
                if(episode._persons[personName]._nowPlace != place._name) {
                    cout << "episodeError: time=" << episode._time << "/place's person is not match" << endl;
                    showEpisode(episodes);
                    exit(0);
                }
                if(!checkPerson[personName]) {
                    checkPerson[personName] = true;
                } else {
                    cout << "episodeError: time=" << episode._time << "/person is present in the double" << endl;
                    showEpisode(episodes);
                    exit(0);
                }
            }
        }
    }
}
