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

#pragma mark -
#pragma mark initalize

void initAgent(map<string,Person> &persons,map<string,Place> &places,string filename) {
    persons.clear();
    places.clear();
    
    ifstream ifs(filename);
    string buf;
    if(!ifs) {
        cout << "not found initialize file" << endl;
        exit(0);
    }
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf, ":");
        if(out[0] == "human") {
            vector<string> names = SpritString(out[1],",");
            for(int i=0;i<names.size();i++) {
                Person person;
                person._name = names[i];
                persons.insert(make_pair(person._name,person));
            }
        }
        if(out[0] == "place") {
            vector<string> names = SpritString(out[1],",");
            for(int i=0;i<names.size();i++) {
                Place place;
                place._name = names[i];
                places.insert(make_pair(place._name, place));
            }
        }
        if(persons.find(out[0]) != persons.end()) {
            if(places.find(out[1]) != places.end()) {
                persons[out[0]]._nowPlace = out[1];
                places[out[1]]._persons.push_back(persons[out[0]]);
            }
        }
        if(places.find(out[0]) != places.end()) {
            vector<string> names = SpritString(out[1], ",");
            for(int i=0;i<names.size();i++) {
                if(places.find(names[i]) != places.end()) {
                    places[out[0]]._nextPlaces.push_back(names[i]);
                }
            }
        }
    }
}

void initConstraints(vector<Constraint> &constraints,string filename) {
    constraints.clear();
    
    ifstream ifs(filename);
    string buf;
    if(!ifs) {
        cout << "not found constraint file" << endl;
        exit(0);
    }
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf, ",");
        int time = atoi(out[0].c_str());
        Constraint con(time,out[1],out[2],getEnumFromString(out[3]));
        constraints.push_back(con);
    }
}

void initQuestion(Question &question,string filename) {
    ifstream ifs(filename);
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
        for(int i=0;i<places[person._nowPlace]._nextPlaces.size();i++) {
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
            for(int i=0;i<personsUCB[p].size();i++) {
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
            for(int i=0;i<personsUCB[p].size();i++) {
                if(personsUCB[p][i].action == selectedAction) {
                    index = i;
                    break;
                }
            }
            
            string personName = person._name;
            double val = checkEpisodePerson(nowEpisodes, constraints,personName);
            
            personsUCB[p][index].count++;
            personsUCB[p][index].sumValue += val;
            
            int totalCount = 0;
            for(int i=0;i<personsUCB[p].size();i++) {
                totalCount += personsUCB[p][i].count;
            }
            for(int i=0;i<personsUCB[p].size();i++) {
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
        for(int i=0;i<personsUCB[p].size();i++) {
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

void doActionMCTS(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints,vector< vector<string> > &actionList) {
    map<string,Person> nowPersons(persons);
    map<string,Place> nowPlaces(places);
    vector<Episode> nowEpisodes;
    Episode episode(0,nowPersons,nowPlaces);
    nowEpisodes.push_back(episode);
    
    double cp = 0.1;
    
    int endTime = 10;
    
    vector< vector<Episode> >completeEpisodess;
    
    for(map<string,Person>::iterator it = nowPersons.begin();it != nowPersons.end();it++) {
        Person person = (*it).second;
        
        vector<Episode> completeEpisodes;
        bool isFinish = false;
        
        double maxValue = -100000;
        
        //rootノードを作成
        int sumCount = 0;
        MCTREE root;
        root.count = 0;
        root.ucb = 0;
        root.sumVal = 0;
        root.places = nowPlaces;
        root.persons = nowPersons;
        root.parent = NULL;
        root.time = 0;
        root.episodes = nowEpisodes;
        root.sumCount = &sumCount;
        root.action = "nothing";
        root.childs.clear();
        
        MCTREE *current = &root;
        
        int playout = 10000;
        
        for(int round=0;round<playout;round++) {
            if(isFinish) break;
            bool downFlag = true;//trueなら木を潜り続ける
            
            //showTree(&root);
            
            double getVal = 0;//取得した報酬
            
            while(downFlag && !isFinish) {
                //取れるアクションを取得
                
                if(current->time >= endTime) {
                    downFlag = false;
                    getVal = checkEpisodePerson(current->episodes, constraints, person._name);
                    break;
                }
                vector<string> actions;
                actions.push_back("nothing");
                for(int i=0;i<current->places[current->persons[person._name]._nowPlace]._nextPlaces.size();i++) {
                    string next = places[current->persons[person._name]._nowPlace]._nextPlaces[i];
                    actions.push_back("move:"+next);
                }
                //各アクションの対応する子ノードが全部存在すれば、UCB値を比較することにする
                bool hasActions = true;
                double maxUcb = -99999999;
                int maxindex = -1;
                string notFoundAction = "";
                for(int i=0;i<actions.size();i++) {
                    string action = actions[i];
                    //actionと一致する子ノードがあるかどうか調べる
                    int index = -1;
                    for(int j=0;j<current->childs.size();j++) {
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
                    MCTREE* child = makeTree(current->persons, current->places, current->episodes, current, current->time + 1, current->sumCount, notFoundAction);
                    current->childs.push_back(child);
                    current = child;
                    assertEpisode(current->episodes);
                    if(notFoundAction.substr(0,5) == "move:") {
                        string next = notFoundAction.substr(5);
                        //cout << "before move" << endl;
                        //showPersons(current->persons);
                        //show(current->time, current->places);
                        move(person,current->persons,current->places,current->places[next]);
                        //cout << "after move" << endl;
                        //showPersons(current->persons);
                        //show(current->time, current->places);
                    }
                    Episode tmpEpisode(current->time,current->persons,current->places);
                    current->episodes.push_back(tmpEpisode);
                    assertEpisode(current->episodes);
                    //あとはランダムに実行
                    map<string,Person> randomPersons(current->persons);
                    map<string,Place> randomPlaces(current->places);
                    vector<Episode> randomEpisodes(current->episodes);
                    int time = current->time;
                    while(time < 10) {
                        time++;
                        doAction(randomPersons, randomPlaces);
                        Episode episode(time,randomPersons,randomPlaces);
                        randomEpisodes.push_back(episode);
                    }
                   // showEpisode(randomEpisodes);
                    getVal = checkEpisodePerson(randomEpisodes, constraints, person._name);
                    if(maxValue < getVal) {
                        maxValue = getVal;
                        completeEpisodes = randomEpisodes;
                        cout << "name:" << person._name << " round:" << round << "val:" << getVal <<endl;
                    }
                    if(getVal == 1.0) {
                        completeEpisodes = randomEpisodes;
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
        
        //showEpisode(getOnlyPersonEpisode(person._name, completeEpisodes));
        completeEpisodess.push_back(getOnlyPersonEpisode(person._name, completeEpisodes));
    }
    
    showEpisode(getFusionEpisode(completeEpisodess));
}


void move(Person &person,map<string,Person> &persons,map<string,Place> &places,Place &nextplace) {
    string movePersonName = person._name;
    Place *place = &places[persons[movePersonName]._nowPlace];

  //  cout << "move:" << person._name << "(" << place->_name << "▶︎" << nextplace._name << ")" << endl;
    
    for(int i=0;i<place->_persons.size();i++) {
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
    for(int i=0;i<episodeT.size();i++) {
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
        for(int j=0;j<episodeT.size();j++) {
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

double checkEpisodePerson(vector<Episode> episodes,vector<Constraint> constraints,string _personName) {
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
        for(int j=0;j<episodeT.size();j++) {
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
    if(hitCount == 0) return 1.0;
    val = (double)correct / (double)hitCount ;
    
    return val;
}

#pragma mark -
#pragma mark show

void show(int time,map<string,Place> places) {
    cout << "----------------------------" << endl;
    cout << "time: " << time << endl;
    for(map<string,Place>::iterator it = places.begin();it != places.end();it++) {
        cout << "<" << (*it).first << ">" ;
        for(int i=0;i<(*it).second._persons.size();i++) {
            Person person = (*it).second._persons[i];
            cout << "(" << person._name << "," << person._hasApples <<  ")";
        }
        cout << endl;
    }
}

void showEpisode(vector<Episode> episodes) {
    for(int i=0;i<episodes.size();i++) {
        show(episodes[i]._time,episodes[i]._places);
    }
}

void showPersons(map<string,Person> &persons) {
    for(map<string,Person>::iterator it = persons.begin();it != persons.end();it++) {
        Person person = (*it).second;
        cout << "(" << person._name << "," << person._nowPlace << ")";
    }
    cout << endl;
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

MCTREE* makeTree(map<string,Person> &_persons,map<string,Place> &_places,vector<Episode> _nowEpisodes,MCTREE* _parent,int nowTime,int *sumCount,string action) {
    MCTREE *node = new MCTREE;
    node->count = 0;
    node->ucb = 0;
    node->sumVal = 0;
    map<string,Place> places(_places);
    map<string,Person> persons(_persons);
    node->places = places;
    node->persons = persons;
    node->parent = _parent;
    node->time = nowTime;
    node->episodes = _nowEpisodes;
    node->sumCount = sumCount;
    node->childs.clear();
    node->action = action;
    return node;
}

void showTree(MCTREE *root) {
    MCTREE *current = root;
    int depth = 0;
    showTreeSub(current, depth);
    cout << endl;
}

void showTreeSub(MCTREE *current,int depth) {
    string action = current->action;
    string ucb = to_string(current->ucb);
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
        for(int i=0;i<current->childs.size();i++) {
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

#pragma mark -
#pragma mark debug

//おなじエージェントが複数存在したり、エージェントの位置情報とplaceのエージェント情報の一致を確認する
void assertEpisode(vector<Episode> episodes) {
    
    for(int j=0;j<episodes.size();j++) {
        Episode episode = episodes[j];
    
        for(map<string,Place>::iterator it = episode._places.begin();it != episode._places.end();it++) {
            map<string,bool> checkPerson;
            for(map<string,Person>::iterator pit = episode._persons.begin();pit != episode._persons.end();pit++) {
                checkPerson.insert(make_pair((*pit).second._name, false));
            }
            
            Place place = (*it).second;
            for(int i=0;i<place._persons.size();i++) {
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
