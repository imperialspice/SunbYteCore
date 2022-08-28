//
// Created by middleton on 8/25/22.
//
#include <vector>
#include <string>


std::vector<std::string> break_path(const std::string& path) {
    std::vector<std::string> rtn;
    bool search = true;
    int pos = 0;
    int oldpos = 0;
    while (search) {
        pos = path.find(" ", oldpos);
        if(pos == -1) {
            rtn.emplace_back(path.substr(oldpos, rtn.size() - oldpos));
            break;
        }
        rtn.emplace_back(path.substr(oldpos, pos-oldpos));
        oldpos = pos+1;
    }
    return rtn;
}

int main(){
    std::string test = "settings tracking/limit Int 10";
    auto b = break_path(test);
    b.size();

}