#ifndef MAIN_H
#define MAIN_H

#include "types.h"
#include "filesystem.h"
using namespace std;

string cmd;
vector<string> params;
vector<user_c*> users;

void split(const string& str,vector<string>& res){
    string strs = str + ' ';
    int pos = strs.find(' ');
    while(pos != string::npos){
        string temp = strs.substr(0,pos);
        res.push_back(temp);
        strs = strs.substr(pos+1);
        pos = strs.find(' ');
    }
}

void color_cout(int layout,int color,string s){
    cout << "\e["<< layout << ";" << color << "m" << s << "\e[0m";
}

void uerror(ERROR e){
    switch (e)
    {
        case PERMISSION : {
            color_cout(HIGHTLIGHT,F_RED,"ERROR : Permission denied .");
            break;
        }

        default : {
            break;
        }

    }
}

#endif