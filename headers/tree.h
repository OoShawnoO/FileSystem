#include "types.h"
#include "filesystem.h"
#include "user.h"
#include <map>

struct DirNode{
    filesystem_c* dir;
    map<string,filesystem_c*> son;
};

void Traverse(const DirNode&);

void AppendFile(DirNode&,filesystem_c*);

void RemoveFile(DirNode&,string);

