#ifndef MAIN_H
#define MAIN_H

#include "types.h"
#include "filesystem.h"
using namespace std;

string cmd;
vector<string> params;
vector<user_c*> users;

extern user_c* user;
extern dir_c* root;


void split(const string& str,char splitchar,vector<string>& res);

void color_cout(int layout,int color,string s);

void uerror(ERROR e);

#endif