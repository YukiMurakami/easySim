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
        Constraint con(time,out[1],out[2],getEnumFromString(out[3]));
        constraints.push_back(con);
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
        Constraint con(time,out[1],out[2],getEnumFromString(out[3]));
        tmp.push_back(con);
    }
    
    if((int)tmp.size() < count) {
        cout << "error: number of count is bigger than constraints" << endl;
        exit(0);
    }
    
    for(int i=0;i<count;i++) {
       // int index = rand()%(tmp.size());
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
        question._time = time;
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
        int id = rand() % 2;
        if(id == 0) {
            //do nothing
        }
        if(id == 1) {
            //move
            Place place = places[person._nowPlace];
            int next = rand() % (place._nextPlaces.size());
            move(person,persons,places,places[place._nextPlaces[next]]);
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

int doActionMCTS(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints,int endTime) {
    
    clock_t start,end;
    start = clock();
    
    map<string,Person> nowPersons(persons);
    map<string,Place> nowPlaces(places);
 //   vector<Episode> nowEpisodes;
    Episode episode(0,nowPersons,nowPlaces);
 //   nowEpisodes.push_back(episode);
 
    
    int sumPlayout = 0;
    
    double cp = 0.1;
    
    vector< vector<Episode> >completeEpisodess;
    
    for(map<string,Person>::iterator it = nowPersons.begin();it != nowPersons.end();it++) {
        Person person = (*it).second;
        
        vector<Episode> completeEpisodes;
        bool isFinish = false;
        
  //      double maxValue = -100000;
        
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
        
        int playout = 600000;
        
        for(int round=0;round<playout;round++) {
            sumPlayout++;
            if(isFinish) break;
            bool downFlag = true;//trueなら木を潜り続ける
            
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
                bool hasActions = true;
                double maxUcb = -99999999;
                int maxindex = -1;
                string notFoundAction = "";
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
                        hasActions = false;
                        notFoundAction = action;
                        break;
                    } else {
                        current->childs[index]->ucb = calcUcb1(current->childs[index]->sumVal, current->childs[index]->count, *current->childs[index]->sumCount,cp);
                        if(maxUcb < current->childs[index]->ucb) {
                            maxindex = index;
                            maxUcb = current->childs[index]->ucb;
                        }
                    }
                }
                if(hasActions) {
                    current = current->childs[maxindex];
                    continue;
                } else {
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
                    
                    Episode episodes[150];
                    int I = 0;
                    while(time < endTime) {
                        time++;
                     
                        doAction(randomPersons, randomPlaces);
                     
                    //    Episode episode(time,randomPersons,randomPlaces);
                     
                        episodes[I]._persons = randomPersons;
                        episodes[I]._places = randomPlaces;
                        episodes[I]._time = time;
                        I++;
                      //  randomEpisodes.push_back(episode);
                    
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
                        cout << round << ":" << getVal << "/" << time << "/" << 1.0/time << endl;
                    }
                    /*
                    if(maxValue < getVal) {
                        maxValue = getVal;
                        completeEpisodes = randomEpisodes;
                       // cout << "name:" << person._name << " round:" << round << "val:" << getVal <<endl;
                    }
                     */
                    if(round >= playout-1 || getVal >= 1.0) {
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
                    
                    downFlag = false;
                    sumCount++;
                    break;
                }
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
        
        EpisodesOutput(completeEpisodes,"episodes.txt",person._name);
        
        deleteTree(&root);
    }
    
    //vector<Episode> resultEpisode = getFusionEpisode(completeEpisodess);
    //showEpisode(resultEpisode);
    return sumPlayout;
}


void move(Person &person,map<string,Person> &persons,map<string,Place> &places,Place &nextplace) {
    string movePersonName = person._name;
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
    
    vector<Episode> episodeT = findEpisodeFromTime(question._time, episodes);
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
        int time = constraints[i]._time;
        string personName = constraints[i]._personName;
        string placeName = constraints[i]._placeName;
        CONSTRAINT constraint = constraints[i]._constraint;
        vector<Episode> episodeT = findEpisodeFromTime(time, episodes);
        for(int j=0;j<(int)episodeT.size();j++) {
            Episode episode = episodeT[j];
         
            if(constraint == there_is) {
                if(episode._persons[personName]._nowPlace == placeName) {
                    correct++;
                    continue;
                }
            }
            if(constraint == there_is_no) {
                if(episode._persons[personName]._nowPlace != placeName) {
                    correct++;
                    continue;
                }
            }
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
        int time = constraints[i]._time;
        string personName = constraints[i]._personName;
        if(personName != _personName) {
            continue;
        } else {
            hitCount++;
        }
        
        string placeName = constraints[i]._placeName;
        CONSTRAINT constraint = constraints[i]._constraint;
        vector<Episode> episodeT = findEpisodeFromTime(time, episodes);
        for(int j=0;j<(int)episodeT.size();j++) {
            Episode episode = episodeT[j];
            
            if(constraint == there_is) {
                if(episode._persons[personName]._nowPlace == placeName) {
                    correct++;
                    if(isShowConstraints) {
                        constraints[i].show();
                        cout << "ok" << endl;
                    }
                    continue;
                }
            }
            if(constraint == there_is_no) {
                if(episode._persons[personName]._nowPlace != placeName) {
                    correct++;
                    if(isShowConstraints) {
                        constraints[i].show();
                        cout << "ok" << endl;
                    }
                    continue;
                }
            }
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
    
    for(int i=0;i<count;i++) {
        int time = constraints[i]._time;
        string personName = constraints[i]._personName;
        if(personName != _personName) {
            continue;
        } else {
            hitCount++;
        }
        
        string placeName = constraints[i]._placeName;
        CONSTRAINT constraint = constraints[i]._constraint;
        
        if(episodesArray[0]._time > time) {
            for(int j=0;j<(int)episodes.size() ;j++) {
                if(episodes.at(j)->_time == time) {
                    
                    if(constraint == there_is) {
                        if(episodes.at(j)->_persons[personName]._nowPlace == placeName) {
                            correct++;
                            if(isShowConstraints) {
                                constraints[i].show();
                                cout << "ok" << endl;
                            }
                            continue;
                        }
                    }
                    if(constraint == there_is_no) {
                        if(episodes.at(j)->_persons[personName]._nowPlace != placeName) {
                            correct++;
                            if(isShowConstraints) {
                                constraints[i].show();
                                cout << "ok" << endl;
                            }
                            continue;
                        }
                    }
                    
                } else {
                    continue;
                }
            }
        } else {
            for(int j=0;episodesArray[j]._time <= time;j++) {
                if(episodesArray[j]._time != time) continue;
                if(constraint == there_is) {
                    if(episodesArray[j]._persons[personName]._nowPlace == placeName) {
                        correct++;
                        if(isShowConstraints) {
                            constraints[i].show();
                            cout << "ok" << endl;
                        }
                        continue;
                    }
                }
                if(constraint == there_is_no) {
                    if(episodesArray[j]._persons[personName]._nowPlace != placeName) {
                        correct++;
                        if(isShowConstraints) {
                            constraints[i].show();
                            cout << "ok" << endl;
                        }
                        continue;
                    }
                }
            }
        }
        
    }
    
    //cout << "this episode's result are " << correct << "/" << count << endl;
    if(hitCount == 0) return 1.0;
    val = (double)correct / (double)hitCount ;
    
    return val;
}

double checkEpisodePersonWithArray(vector<Episode> *episodes,vector<Constraint> constraints,string _personName,Episode *episodesArray) {
    double val = 0;
    int count = (int)constraints.size();
    int correct = 0;
    int hitCount = 0;
    
    
    for(int i=0;i<count;i++) {
        int time = constraints[i]._time;
        string personName = constraints[i]._personName;
        if(personName != _personName) {
            continue;
        } else {
            hitCount++;
        }
        
        string placeName = constraints[i]._placeName;
        CONSTRAINT constraint = constraints[i]._constraint;
        
        if(episodesArray[0]._time > time) {
            for(int j=0;j<(int)episodes->size() ;j++) {
                if(episodes->at(j)._time == time) {
                    
                    if(constraint == there_is) {
                        if(episodes->at(j)._persons[personName]._nowPlace == placeName) {
                            correct++;
                            continue;
                        }
                    }
                    if(constraint == there_is_no) {
                        if(episodes->at(j)._persons[personName]._nowPlace != placeName) {
                            correct++;
                            continue;
                        }
                    }
                    
                } else {
                    continue;
                }
            }
        } else {
            for(int j=0;episodesArray[j]._time <= time;j++) {
                if(episodesArray[j]._time != time) continue;
                if(constraint == there_is) {
                    if(episodesArray[j]._persons[personName]._nowPlace == placeName) {
                        correct++;
                        continue;
                    }
                }
                if(constraint == there_is_no) {
                    if(episodesArray[j]._persons[personName]._nowPlace != placeName) {
                        correct++;
                        continue;
                    }
                }
            }
        }
        
    }
    
    //cout << "this episode's result are " << correct << "/" << count << endl;
    if(hitCount == 0) return 1.0;
    val = (double)correct / (double)hitCount ;
    
    return val;
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
            cout << "(" << person._name << "," << person._hasApples <<  ")";
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
            cout << "(" << person._name << "," << person._hasApples <<  ")";
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

#pragma mark -
#pragma mark calc

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
