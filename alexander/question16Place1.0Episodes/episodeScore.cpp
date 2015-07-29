
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

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

int main(int argc,const char * argv[]) {
    
    string filename = argv[1];
    cout << "filename:" << filename << endl;
    
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error: file not found" << endl;
        exit(0);
    }
    string buf;
    
    int scoreCount = 0;
    double sumScore = 0.0;
    double sumDepth = 0.0;
    
    while(getline(ifs,buf)) {
        vector<string> out = SpritString(buf," ");
        if(out.size() == 2) {
            vector<string> out2 = SpritString(out[0],":");
            if(out2.size() == 2) {
                double score = atof(out2[1].c_str());
                sumScore += score;
                scoreCount++;
            }
            vector<string> out3 = SpritString(out[1],":");
            if(out3.size() == 2) {
                double depth = atof(out3[1].c_str());
                sumDepth += depth;
               // scoreCount++;
            }
        }
    }
    
    ifs.close();
    
    cout << "averageScore:" << sumScore/scoreCount << endl;
    cout << "averageDepth:" << sumDepth/scoreCount << endl;
    cout << "episodeCount:" << scoreCount << endl;
    
    return 0;
}
