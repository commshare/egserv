
#include "util.hpp"

using namespace std;
using namespace util;

int main(int argc, char* argv[])
{
    string line;
    vector<string> sv ;
    int mode;
    char sep;
    enum{
        SPACEMODE,
        SEPMODE
    };
    mode = SPACEMODE;
    
    while(1){
        sv.clear();
        getline(cin, line);
        if(mode == SEPMODE)
            split(line, sv, sep);
        else
            split(line, sv);
        if(sv.size() == 0)
            continue;
        if(sv.size() == 2 && sv[0] == "sep"){
            cout << "switch to sep mode" << endl;
            mode = SEPMODE;
            sep = sv[1][0];
        }
        if(sv[0] == "q")
            break;
        for(auto& it : sv)
            cout << it << endl;
    }
}
