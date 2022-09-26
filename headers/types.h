#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class user_c;
class filesystem_c;
class dir_c;
class file_c;

extern dir_c *root;
extern user_c *user;

enum FILETYPE
{
    BINARY = 0,
    TEXT,
    DIR,
    LINK,
    UNKNOWN,
};

enum ERROR
{
    PERMISSION = 0,
    NO,
    NOTDIR,
    NOTFOUND,
    NOTTEXT,
    CMD,
};

/*
    预留      预留    chexec   chwrite  chread   exec    write   read
    0         0         0         0        0      0        0      0
    128       64        32        16       8      4        2      1
*/

union attribute_u
{
    unsigned char attr;
    struct
    {
        unsigned char read : 1;    //读权限
        unsigned char write : 1;   //写权限
        unsigned char exec : 1;    //执行权限
        unsigned char chread : 1;  //修改读权限
        unsigned char chwrite : 1; //修改写权限
        unsigned char chexec : 1;  //修改执行权限
        unsigned char reserve : 2; //预留
    } attr_s;
};

enum ATTRIBUTE
{
    READ = 0,
    WRITE,
    EXEC,
    CHREAD,
    CHWRITE,
    CHEXEC,
};

/*
显示方式
0（默认值）、1（高亮）、22（非粗体）、4（下划线）、24（非下划线）、5（闪烁）、25（非闪烁）、7（反显）、27（非反显）

前景色
30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（蓝色）、35（洋红）、36（青色）、37（白色）

背景色
40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、45（洋红）、46（青色）、47（白色）

*/

/* LAYOUT TYPE */
#define DEFAULT 0
#define HIGHTLIGHT 1
#define NOBOLd 22
#define UNDERLINE 4
#define NOUNDERLINE 24
#define BLING 5
#define NOBLING 25
/* FONT COLOR TYPE */
#define F_BLACK 30
#define F_RED 31
#define F_GREEN 32
#define F_YELLOW 33
#define F_BLUE 34
#define F_MAGENTA 35
#define F_CYAN 36
#define F_WHITE 37
/* BACKGROUND COLOR TYPE*/
#define B_BLACK 40
#define B_RED 41
#define B_GREEN 42
#define B_YELLOW 43
#define B_BLUE 44
#define B_MAGENTA 45
#define B_CYAN 46
#define B_WHITE 47

void color_cout(int layout, int color, string s);
void split(const string &str, char splitchar, vector<string> &res);
void uerror(ERROR);

#endif