#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

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

int main() {
    vector< pair<int,double> > values;
    ifstream ifs("dataEschate50.txt");
    string buf;
    int count = 0;
    bool firstFlag = true;
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf,",");
        int playout = atoi(out[0].c_str());
        double value = atof(out[1].c_str());
        
        if(firstFlag) {
            values.push_back(make_pair(playout,value));
        } else {
            values[playout/1000].second += value;
        }
        
        if(playout == 999000) {
            count++;
            firstFlag = false;
        }
    }
    ifs.close();
    

    ofstream ofs("outputEschate50.csv");

    for(unsigned int i=0;i<values.size();i++) {
        if(i % 5 == 0) {
            cout << values[i].first << "," << values[i].second / count << endl;
            ofs << values[i].first << "," << values[i].second / count << endl;
        }
    }

    ofs.close();
    
    cout << count << endl;

    return 0;
}
