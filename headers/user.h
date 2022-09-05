#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class user_c{
private:
    string name;
public:
    user_c();
    ~user_c();
    string getname();
};

#endif