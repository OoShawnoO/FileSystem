#include "user.h"
#include "filesystem.h"

#include <fstream>

static tm *get_time(){
        time_t timer;
        timer = time(NULL);
        tm *t = localtime(&timer);
        return t;
}

void process_attr(attribute_u attr){
    if(attr.attr_s.read == 1) { cout << "r ";}
    else { cout << "- " ;}
    if(attr.attr_s.write == 1) { cout << "w ";}
    else { cout << "- ";}
    if(attr.attr_s.exec == 1) { cout << "x ";}
    else { cout << "- ";}
    if(attr.attr_s.chread == 1) { cout << "cr ";}
    else { cout << "- ";}
    if(attr.attr_s.chwrite == 1) { cout << "cw ";}
    else { cout << "- ";}
    if(attr.attr_s.chexec == 1) { cout << "cx " ;}
    else { cout << "- ";}
}

unsigned char user_c::ucount = 0;
unsigned char user_c::gcount = 0;

unsigned char user_c::get_ucount()
{
    return ucount;
}
unsigned char user_c::get_gcount()
{
    return gcount;
}

user_c::user_c(string _name)
{
    name = _name;
    gid = get_gcount();
    uid = get_ucount();
    gcount++;
    ucount++;
    error = NO;
    
    functions = *new map<string, bool (user_c::*)(std::vector<std::string> &)>;
    functions["ls"] = &user_c::ls;
    functions["pwd"] = &user_c::pwd;
    functions["cd"] = &user_c::cd;
    functions["mkdir"] = &user_c::mkdir;
    functions["rm"] = &user_c::rm;
    functions["cp"] = &user_c::cp;
    functions["touch"] = &user_c::touch;
    functions["rmdir"] = &user_c::rmdir;
    functions["mv"] = &user_c::mv;
    functions["cat"] = &user_c::cat;
    functions["more"] = &user_c::more;
    functions["echo"] = &user_c::echo;
    functions["head"] = &user_c::head;
    functions["tail"] = &user_c::tail;
    functions["dup2"] = &user_c::dup2;
    functions["ln"] = &user_c::ln;
    functions["history"] = &user_c::history;
    functions["vim"] = &user_c::vim;
    functions["stat"] = &user_c::stat;
}

user_c::~user_c()
{
}

string user_c::get_name() const { return name; }
unsigned char user_c::get_uid() const { return uid; }
unsigned char user_c::get_gid() const { return gid; }
ERROR user_c::get_error() const { return error; }
dir_c *user_c::get_current_dir() { return current_dir; }
int user_c::get_read_cursor() const{ return read_cursor;}
vector<string>& user_c::get_history() {return his;}

void user_c::set_name(string _name) { name = _name; }
bool user_c::set_uid(unsigned char _uid)
{
    if (gid == 1)
    {
        uid = _uid;
        return true;
    }
    else
    {
        error = PERMISSION;
        return false;
    }
}
bool user_c::set_gid(unsigned char _gid)
{
    if (gid == 1)
    {
        gid = _gid;
        return true;
    }
    else
    {
        error = PERMISSION;
        return false;
    }
}
bool user_c::set_uid(unsigned char _uid, user_c &user)
{
    if (gid == 1)
    {
        user.uid = _uid;
        return true;
    }
    else
    {
        error = PERMISSION;
        return false;
    }
};
bool user_c::set_gid(unsigned char _gid, user_c &user)
{
    if (gid == 1)
    {
        user.gid = _gid;
        return true;
    }
    else
    {
        error = PERMISSION;
        return false;
    }
};
void user_c::set_error(ERROR _error) { error = _error; }
void user_c::set_current_dir(dir_c *dir) { current_dir = dir; }
void user_c::set_read_cursor(int cursor) { read_cursor = cursor; }

extern dir_c *root;
extern user_c *user;

//（1）pwd - 显示当前目录的绝对路径
bool user_c::pwd(vector<string> &)
{
    auto dir = dynamic_cast<filesystem_c*>(current_dir);
    if (dynamic_cast<dir_c*>(dir) == root)
    {
        cout << "/" << endl;
        return true;
    }
    vector<string> v;
    while (dir->get_name() != "/")
    {
        v.push_back(dir->get_name());
        dir = dynamic_cast<filesystem_c*>(dir)->parent;
        v.push_back("/");
    }
    for (vector<string>::reverse_iterator it = v.rbegin(); it != v.rend(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
//（2）ls - 显示当前路径下的文件和目录
bool user_c::ls(vector<string> &args)
{
    for (auto x : get_current_dir()->get_contents())
    {
        switch (x.second->get_filetype())
        {
        case DIR:
        {
            color_cout(HIGHTLIGHT, F_YELLOW, x.first);
            break;
        }
        case BINARY:
        {
            color_cout(HIGHTLIGHT, F_CYAN, x.first);
            break;
        }
        case LINK:
        {
            color_cout(HIGHTLIGHT,F_RED,x.first);
            break;
        }
        case TEXT:
        {
            color_cout(HIGHTLIGHT, F_GREEN, x.first);
            break;
        }
        default:
        {
            color_cout(HIGHTLIGHT, F_MAGENTA, x.first);
            break;
        }
        }
        cout << " ";
    }
    cout << endl;
}
//（3）cd - 切换至指定目录
bool user_c::cd(vector<string> &args)
{
    auto pos = get_current_dir();
    vector<string> vc;
    switch (args[0][0])
    {
    case '/':
    {
        pos = root;
        args[0].erase(0, 1);
        split(args[0], '/', vc);
        break;
    }
    case '.':
    {
        if (args[0][1] == '.' && args[0].size() > 2)
        {
            pos = dynamic_cast<dir_c*>(((dynamic_cast<filesystem_c*>(pos))->get_parent()));
            args[0].erase(0, 3);
            split(args[0], '/', vc);
        }
        else if (args[0][1] == '.')
        {
            pos = dynamic_cast<dir_c*>(((dynamic_cast<filesystem_c*>(pos))->get_parent()));
        }
        else
        {
            args[0].erase(0, 2);
            split(args[0], '/', vc);
        }
        break;
    }
    default:
    {
        split(args[0], '/', vc);
        break;
    }
    }
    for (auto x : vc)
    {
        char a = x.c_str()[0];
        if (a == NULL)
        {
            break;
        }

        if (pos->get_contents().find(x) != pos->get_contents().end())
        {
            if (pos->get_contents()[x]->get_filetype() == DIR)
            {
                pos = dynamic_cast<dir_c*>(pos->get_contents()[x]);
            }
            else if(pos->get_contents()[x]->get_filetype() == LINK 
            && dynamic_cast<link_c*>(pos->get_contents()[x])->get_real()->get_filetype() == DIR){
                pos = dynamic_cast<dir_c*>(dynamic_cast<link_c*>(pos->get_contents()[x])->get_real());
            }
            else
            {
                set_error(NOTDIR);
            }
        }
        else
        {
            set_error(NOTFOUND);
        }
    }
    if (get_error() == NO)
    {
        set_current_dir(pos);
        return true;
    }
    else
    {
        return false;
    }
}
// 以下函数重复代码过多 进行进一步封装为接口
bool user_c::TEMP(vector<string>& args,function<void(user_c*,string)> cdarg,
function<void(user_c*,string)> noarg,ATTRIBUTE attr){
    auto dir = get_current_dir();
    string name = args[0];
    if (args[0].find("/") != string::npos)
    {
        name = args[0].substr(args[0].rfind("/") + 1);
        args[0] = args[0].substr(0, args[0].rfind("/"));

        if (cd(args))
        {
            if ((dynamic_cast<filesystem_c*>(get_current_dir()))->permission(this, attr))
            {
                cdarg(this,name);
                set_current_dir(dir);
                return true;
            }
            else
            {
                set_error(PERMISSION);
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((dynamic_cast<filesystem_c*>(dir))->permission(this, attr))
        {
            noarg(this,name);
            return true;
        }
        else
        {
            set_error(PERMISSION);
            return false;
        }
    }
}
//（4）mkdir - 创建目录
bool user_c::mkdir(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        dir_c *newdir = new dir_c(user, name, dynamic_cast<filesystem_c*>(user->get_current_dir()));
    },
    [](user_c* user,string name){
        dir_c *newdir = new dir_c(user, name, dynamic_cast<filesystem_c*>(user->get_current_dir()));
    });
}
//（5）rmdir - 删除目录(空目录)
bool user_c::rmdir(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        delete dynamic_cast<dir_c *>(user->get_current_dir()->get_contents()[name]);
    },
    [](user_c* user,string name){
        delete dynamic_cast<dir_c *>(user->get_current_dir()->get_contents()[name]);
    }
    );
}
//（6）touch - 创建空文件
bool user_c::touch(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        file_c *newfile = new file_c(user, name, dynamic_cast<filesystem_c*>(user->get_current_dir()));
    },
    [](user_c* user,string name){
        file_c *newfile = new file_c(user, name, dynamic_cast<filesystem_c*>(user->get_current_dir()));
    },
    WRITE
    );
}
//（7）cp - 拷贝文件或目录到指定文件或目录 cp source[args[0]] -> dest[args[1]]
bool user_c::cp(vector<string> &args)
{
    auto dir = get_current_dir();
    filesystem_c *copy = nullptr;
    string src = args[0];
    string dest = args[1];
    vector<string> vsrc(args.begin(), args.begin() + 1);
    vector<string> vdest(args.begin() + 1, args.begin() + 2);
    if (vsrc[0].find("/") != string::npos)
    {
        src = vsrc[0].substr(vsrc[0].rfind("/") + 1);
        vsrc[0] = vsrc[0].substr(0, vsrc[0].rfind("/"));
        if (cd(vsrc))
        {
            if ((dynamic_cast<filesystem_c*>(get_current_dir()))->permission(this, READ))
            {
                // TODO 拷贝该文件
                auto filesystem = get_current_dir()->get_contents()[src];
                if (filesystem->get_filetype() == DIR)
                {
                    copy = new dir_c(*dynamic_cast<dir_c *>(filesystem));
                }
                else if (filesystem->get_filetype() == BINARY 
                || filesystem->get_filetype() == TEXT 
                || filesystem->get_filetype() == UNKNOWN
                )
                {
                    copy = new file_c(*dynamic_cast<file_c *>(filesystem));
                }
                else if(filesystem->get_filetype() == LINK){
                    copy = new link_c(*dynamic_cast<link_c*>(filesystem));
                }
                else
                {
                    return false;
                }
                set_current_dir(dir);
            }
            else
            {
                set_error(PERMISSION);
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((dynamic_cast<filesystem_c*>(dir))->permission(this, READ))
        {
            // TODO 拷贝该文件
            auto filesystem = get_current_dir()->get_contents()[src];
            if(filesystem == NULL){set_error(NOTFOUND);return false;}
            if (filesystem->get_filetype() == DIR)
            {
                copy = new dir_c(*dynamic_cast<dir_c *>(filesystem));
            }
            else if (filesystem->get_filetype() == BINARY 
            || filesystem->get_filetype() == TEXT 
            || filesystem->get_filetype() == UNKNOWN
            )
            {
                copy = new file_c(*dynamic_cast<file_c *>(filesystem));
            }
            else if(filesystem->get_filetype() == LINK){
                copy = new link_c(*dynamic_cast<link_c*>(filesystem));
            }
            else
            {
                return false;
            }
        }
        else
        {
            set_error(PERMISSION);
            return false;
        }
    }

    if (vdest[0].find("/") != string::npos)
    {
        dest = vdest[0].substr(vdest[0].rfind("/") + 1);
        vdest[0] = vdest[0].substr(0, vdest[0].rfind("/"));
        if (cd(vdest))
        {
            if ((dynamic_cast<filesystem_c*>(get_current_dir()))->permission(this, WRITE))
            {
                // TODO 将拷贝的文件放在该目录下
                if (dest == "")
                {
                    dest = src;
                }
                copy->set_name(dest);
                copy->set_parent(get_current_dir(),false);
                set_current_dir(dir);
            }
            else
            {
                set_error(PERMISSION);
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((dynamic_cast<filesystem_c*>(dir))->permission(this, WRITE))
        {
            // TODO 将拷贝的文件放在该目录下
            if (dest == "")
            {
                dest = src;
            }
            copy->set_name(dest);
            copy->set_parent(get_current_dir(),false);
        }
        else
        {
            set_error(PERMISSION);
            return false;
        }
    }
}
//（8）rm - 删除文件或目录
bool user_c::rm(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* filesystem = user->get_current_dir()->get_contents()[name];
        
        FILETYPE type = filesystem->get_filetype();
        if( type == DIR){
            delete dynamic_cast<dir_c *>(filesystem);
        }else if(type == BINARY || type == TEXT || type == UNKNOWN){
            user->get_current_dir()->get_contents().erase(name);
            user->get_current_dir()->set_update_time(get_time());
            delete dynamic_cast<file_c *>(filesystem);   
        }else if(type == LINK){
            user->get_current_dir()->get_contents().erase(name);
            user->get_current_dir()->set_update_time(get_time());
            delete dynamic_cast<link_c*>(filesystem);
        }
    },
    [](user_c* user,string name){
        filesystem_c* filesystem = user->get_current_dir()->get_contents()[name];
        
        FILETYPE type = filesystem->get_filetype();
        if( type == DIR){
            delete dynamic_cast<dir_c *>(filesystem);
        }else if(type == BINARY || type == TEXT || type == UNKNOWN){
            user->get_current_dir()->get_contents().erase(name);
            user->get_current_dir()->set_update_time(get_time());
            delete dynamic_cast<file_c *>(filesystem);
        }else if(type == LINK){
            user->get_current_dir()->get_contents().erase(name);
            user->get_current_dir()->set_update_time(get_time());
            delete dynamic_cast<link_c*>(filesystem);
        }
    },
    WRITE
    );
}
//（9）mv - 移动文件与目录或重命名
bool user_c::mv(vector<string> &args)
{
    vector<string> v(args.begin(),args.begin()+1);
    if(cp(args) == false) return false;
    if(rm(v) == false) return false;
    return true;
}
//（10）cat - 查看文件内容
bool user_c::cat(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND); return false;}
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT); return false;}
        else{
            file_c* file = dynamic_cast<file_c*>(user->get_current_dir()->get_contents()[name]);
            vector<string>& v = file->get_mem();
            vector<string>::iterator it = v.begin();
            for(;it != v.end() && it != v.begin() + 10;it ++){
                cout << *it <<endl;
            }
            if(it == v.begin()+10) user->set_read_cursor(10);
            else user->set_read_cursor(0);
        }
    },
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND); return false;}
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT); return false;}
        else{
            file_c* file = dynamic_cast<file_c*>(user->get_current_dir()->get_contents()[name]);
            vector<string>& v = file->get_mem();
            vector<string>::iterator it = v.begin();
            for(;it != v.end() && it != v.begin() + 10;it ++){
                cout << *it <<endl;
            }
            if(it == v.begin()+10) user->set_read_cursor(10);
            else user->set_read_cursor(0);
        }
    }
    );
}
//（11）more - 文本过滤器
bool user_c::more(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND); return false;}
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT); return false;}
        else{
            file_c* file = dynamic_cast<file_c*>(user->get_current_dir()->get_contents()[name]);
            vector<string>& v = file->get_mem();
            vector<string>::iterator it = v.begin() + user->get_read_cursor();
            for(;it != v.end() && it != v.begin() + 10 + user->get_read_cursor();it ++){
                cout << *it;
            }
            if(it == v.begin()+10+user->get_read_cursor()) user->set_read_cursor(10+user->get_read_cursor());
            else user->set_read_cursor(0);
        }
    },
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND); return false;}
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT); return false;}
        else{
            file_c* file = dynamic_cast<file_c*>(user->get_current_dir()->get_contents()[name]);
            vector<string>& v = file->get_mem();
            vector<string>::iterator it = v.begin() + user->get_read_cursor();
            for(;it != v.end() && it != v.begin() + 10 + user->get_read_cursor();it ++){
                cout << *it;
            }
            if(it == v.begin()+10+user->get_read_cursor()) user->set_read_cursor(10+user->get_read_cursor());
            else user->set_read_cursor(0);
        }
    }
    );
}
//（12）echo - 输出内容到控制台
bool user_c::echo(vector<string> &args)
{
    if(args.size() > 0){
        cout << args[0] << endl;
        return true;
    }
    return false;
}
//（13）head - 显示文件开头部分
bool user_c::head(vector<string> &args)
{
    return cat(args);
}
//（14）tail - 显示文件尾部的部分
bool user_c::tail(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND); return false;}
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT); return false;}
        else{
            file_c* file = dynamic_cast<file_c*>(user->get_current_dir()->get_contents()[name]);
            vector<string>& v = file->get_mem();
            vector<string>::iterator it;
            if(v.size() > 10){
                it = v.end() - 10;
            }else{
                it = v.end() - v.size();   
            }
            
            for(;it != v.end();it ++){
                cout << *it <<endl;
            }
        }
    },
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND); return false;}
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT); return false;}
        else{
            file_c* file = dynamic_cast<file_c*>(user->get_current_dir()->get_contents()[name]);
            vector<string>& v = file->get_mem();
            vector<string>::iterator it;
            if(v.size() > 10){
                it = v.end() - 10;
            }else{
                it = v.end() - v.size();   
            }
            
            for(;it != v.end();it ++){
                cout << *it <<endl;
            }
        }
    }
    );
}
//（15）> / >> - 输出重定向/追加
bool user_c::dup2(vector<string> &args)
{
    
}
//（16）ln - 软链接
bool user_c::ln(vector<string> &args)
{
    auto dir = get_current_dir();
    filesystem_c *copy = nullptr;
    string src = args[0];
    string dest = args[1];
    vector<string> vsrc(args.begin(), args.begin() + 1);
    vector<string> vdest(args.begin() + 1, args.begin() + 2);
    if (vsrc[0].find("/") != string::npos)
    {
        src = vsrc[0].substr(vsrc[0].rfind("/") + 1);
        vsrc[0] = vsrc[0].substr(0, vsrc[0].rfind("/"));
        if (cd(vsrc))
        {
            if ((dynamic_cast<filesystem_c*>(get_current_dir()))->permission(this, WRITE))
            {
                copy = get_current_dir()->get_contents()[src];
                if(copy == NULL){set_error(NOTFOUND);return false;}
                set_current_dir(dir);
            }
            else
            {
                set_error(PERMISSION);
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((dynamic_cast<filesystem_c*>(dir))->permission(this, WRITE))
        {
            copy = get_current_dir()->get_contents()[src];
            if(copy == NULL){set_error(NOTFOUND);return false;}
        }
        else
        {
            set_error(PERMISSION);
            return false;
        }
    }

    if (vdest[0].find("/") != string::npos)
    {
        dest = vdest[0].substr(vdest[0].rfind("/") + 1);
        vdest[0] = vdest[0].substr(0, vdest[0].rfind("/"));
        if (cd(vdest))
        {
            if ((dynamic_cast<filesystem_c*>(get_current_dir()))->permission(this, WRITE))
            {
                link_c* link = new link_c(this,dest,dynamic_cast<filesystem_c*>(get_current_dir()));
                link->set_real(copy);
                set_current_dir(dir);
            }
            else
            {
                set_error(PERMISSION);
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((dynamic_cast<filesystem_c*>(dir))->permission(this, WRITE))
        {
            // TODO 将LINK放在该目录下
            link_c* link = new link_c(this,dest,dynamic_cast<filesystem_c*>(get_current_dir()));
            link->set_real(copy);
        }
        else
        {
            set_error(PERMISSION);
            return false;
        }
    }
}
//（17）history - 查看执行过的的历史命令
bool user_c::history(vector<string> &args)
{
    int count = 1;
    for(auto it : his){
        cout << count++ << "\t" << it << endl;
    }
}
// (18) vim - 写文件
bool user_c::vim(vector<string>& args){
    TEMP(args,
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(user->get_current_dir()->get_contents()[name]->get_filetype() == LINK 
        && dynamic_cast<link_c*>(user->get_current_dir()->get_contents()[name])->get_real()->get_filetype() == TEXT
        ){
            f = dynamic_cast<file_c*>(dynamic_cast<link_c*>(user->get_current_dir()->get_contents()[name])->get_real());
        }

        if( f->get_filetype() == TEXT){
            file_c* f = dynamic_cast<file_c*>(user->get_current_dir()->get_contents()[name]);
            f->set_update_time(get_time());
            vector<string> v = f->get_mem();
            ofstream out(name);
            for(auto x : v){
                out << x;
            }
            out.close();
            system((string("vim ")+name).c_str());
            v.clear();
            ifstream in(name);
            while(!in.eof()){
                string line;
                getline(in,line);
                line = line + "\n";
                if(line!="\n"){
                    v.push_back(line);
                }
            }
            in.close();
            f->get_mem() = v;
            system((string("rm ")+name).c_str());
        }
        
    },
    [](user_c* user,string name){
        file_c* f = dynamic_cast<file_c*>(user->get_current_dir()->get_contents()[name]);
        f->set_update_time(get_time());
        vector<string> v = f->get_mem();
        ofstream out(name);
        for(auto x : v){
            out << x;
            out << "\n";
        }
        out.close();
        system((string("vim ")+name).c_str());
        v.clear();
        ifstream in(name);
        while(!in.eof()){
            string line;
            getline(in,line);
            line = line + "\n";
            if(line!="\n"){
                v.push_back(line);
            }
        }
        in.close();
        f->get_mem() = v;
        system((string("rm ")+name).c_str());
    }
    );
}
// (19) stat - 文件属性
bool user_c::stat(vector<string>& args){
    return TEMP(
        args,
        [](user_c* user,string name){
            filesystem_c* filesystem = user->get_current_dir()->get_contents()[name];
            if(filesystem == NULL){user->set_error(NOTFOUND);return false;}
            cout << "FILE : " << filesystem->get_name() << "\t" << endl;
            if(filesystem->get_filetype() == DIR){
                cout << "Size : " << sizeof(dir_c) + sizeof(filesystem_c) << "\t" ;
            }else if(filesystem->get_filetype() == LINK){
                cout << "Size : " << sizeof(link_c) + sizeof(filesystem_c) << "\t" ;
            }else{
                cout << "Size : " << sizeof(file_c) + sizeof(filesystem_c) 
                                    + sizeof(dynamic_cast<file_c*>(filesystem)->get_mem()) << "\t" ;
            }
            cout << "Type : ";
            switch(filesystem->get_filetype()){
                case DIR : {
                    cout << "Dir";
                    break;
                }
                case BINARY : {
                    cout << "Binary";
                    break;
                }
                case TEXT : {
                    cout << "Text";
                    break;
                }
                case LINK : {
                    cout << "Link";
                    break;
                }
                case UNKNOWN : {
                    cout << "Unknown";
                    break;
                }
                default : {
                    break;
                }
            }
            cout << "\t" << endl;
            cout << "Link : " << filesystem->get_lcount() << endl;
            cout << "Creater_uid : " << filesystem->get_createid() << "\t";
            cout << "Owner_uid : " << filesystem->get_ownerid() << "\t";
            cout << "Owenr_gid : " << filesystem->get_ownergid() << "\t" <<endl;
            tm t = filesystem->get_create_time();
            cout << "Create Time : " << t.tm_year + 1900 << "-" << t.tm_mon + 1 << "-" << t.tm_mday << " " ;
            if(t.tm_hour > 10) { cout << t.tm_hour << ":";}
            else{cout << "0" << t.tm_hour << ":";}
            if(t.tm_min > 10) { cout << t.tm_min << ":";}
            else{cout << "0" << t.tm_min << ":";}
            if(t.tm_sec > 10) { cout << t.tm_sec <<endl;}
            else{cout << "0" << t.tm_sec << endl;}
            t = filesystem->get_update_time();
            cout << "Update Time : " << t.tm_year + 1900 << "-" << t.tm_mon + 1 << "-" << t.tm_mday << " " ;
            if(t.tm_hour > 10) { cout << t.tm_hour << ":";}
            else{cout << "0" << t.tm_hour << ":";}
            if(t.tm_min > 10) { cout << t.tm_min << ":";}
            else{cout << "0" << t.tm_min << ":";}
            if(t.tm_sec > 10) { cout << t.tm_sec <<endl;}
            else{cout << "0" << t.tm_sec << endl;}
            
            t = filesystem->get_visit_time();
            cout << "Visit Time : " << t.tm_year + 1900 << "-" << t.tm_mon + 1 << "-" << t.tm_mday << " " ;
            if(t.tm_hour > 10) { cout << t.tm_hour << ":";}
            else{cout << "0" << t.tm_hour << ":";}
            if(t.tm_min > 10) { cout << t.tm_min << ":";}
            else{cout << "0" << t.tm_min << ":";}
            if(t.tm_sec > 10) { cout << t.tm_sec <<endl;}
            else{cout << "0" << t.tm_sec << endl;}
            
            attribute_u attr = filesystem->get_owner_permission();
            cout << "Owner Permission : ";
            process_attr(attr);
            cout << endl;
            attr = filesystem->get_group_permission();
            cout << "Owner Group Permission : ";
            process_attr(attr);
            cout << endl;
            attr = filesystem->get_other_permission();
            cout << "Other Permission : ";
            process_attr(attr);
            cout << endl;
        },
        [](user_c* user,string name){
            filesystem_c* filesystem = user->get_current_dir()->get_contents()[name];
            if(filesystem == NULL){user->set_error(NOTFOUND);return false;}
            cout << "FILE : " << filesystem->get_name() << "\t" << endl;
            if(filesystem->get_filetype() == DIR){
                cout << "Size : " << sizeof(dir_c) + sizeof(filesystem_c) << "\t" ;
            }else if(filesystem->get_filetype() == LINK){
                cout << "Size : " << sizeof(link_c) + sizeof(filesystem_c) << "\t" ;
            }else{
                int size = 0;
                vector<string> mem = dynamic_cast<file_c*>(filesystem)->get_mem();
                for(auto i : mem){
                    size += i.capacity();
                }
                cout << "Size : " << sizeof(file_c) + sizeof(filesystem_c) + size << "\t";
            }
            cout << "Type : ";
            switch(filesystem->get_filetype()){
                case DIR : {
                    cout << "Dir";
                    break;
                }
                case BINARY : {
                    cout << "Binary";
                    break;
                }
                case TEXT : {
                    cout << "Text";
                    break;
                }
                case LINK : {
                    cout << "Link";
                    break;
                }
                case UNKNOWN : {
                    cout << "Unknown";
                    break;
                }
                default : {
                    break;
                }
            }
            cout << "\t" << endl;
            cout << "Link : " << filesystem->get_lcount() << endl;
            cout << "Creater_uid : " << filesystem->get_createid() << "\t";
            cout << "Owner_uid : " << filesystem->get_ownerid() << "\t";
            cout << "Owenr_gid : " << filesystem->get_ownergid() << "\t" <<endl;
            tm t = filesystem->get_create_time();
            cout << "Create Time : " << t.tm_year + 1900 << "-" << t.tm_mon + 1 << "-" << t.tm_mday << " " ;
            if(t.tm_hour > 10) { cout << t.tm_hour << ":";}
            else{cout << "0" << t.tm_hour << ":";}
            if(t.tm_min > 10) { cout << t.tm_min << ":";}
            else{cout << "0" << t.tm_min << ":";}
            if(t.tm_sec > 10) { cout << t.tm_sec <<endl;}
            else{cout << "0" << t.tm_sec << endl;}
            
            t = filesystem->get_update_time();
            cout << "Update Time : " << t.tm_year + 1900 << "-" << t.tm_mon + 1 << "-" << t.tm_mday << " " ;
            if(t.tm_hour > 10) { cout << t.tm_hour << ":";}
            else{cout << "0" << t.tm_hour << ":";}
            if(t.tm_min > 10) { cout << t.tm_min << ":";}
            else{cout << "0" << t.tm_min << ":";}
            if(t.tm_sec > 10) { cout << t.tm_sec <<endl;}
            else{cout << "0" << t.tm_sec << endl;}
            
            t = filesystem->get_visit_time();
            cout << "Visit Time : " << t.tm_year + 1900 << "-" << t.tm_mon + 1 << "-" << t.tm_mday << " " ;
            if(t.tm_hour > 10) { cout << t.tm_hour << ":";}
            else{cout << "0" << t.tm_hour << ":";}
            if(t.tm_min > 10) { cout << t.tm_min << ":";}
            else{cout << "0" << t.tm_min << ":";}
            if(t.tm_sec > 10) { cout << t.tm_sec <<endl;}
            else{cout << "0" << t.tm_sec << endl;}
            
            attribute_u attr = filesystem->get_owner_permission();
            cout << "Owner Permission : ";
            process_attr(attr);
            cout << endl;
            attr = filesystem->get_group_permission();
            cout << "Owner Group Permission : ";
            process_attr(attr);
            cout << endl;
            attr = filesystem->get_other_permission();
            cout << "Other Permission : ";
            process_attr(attr);
            cout << endl;
        },
        READ
    );
}