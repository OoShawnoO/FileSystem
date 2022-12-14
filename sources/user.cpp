#include "user.h"
#include "filesystem.h"

#include <fstream>

extern vector<user_c*> users;
extern user_c* user;

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

unsigned char user_c::ucount = 1;
unsigned char user_c::gcount = 1;

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
    gid = gcount++;
    uid = ucount++;

    error = NO;
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
    if (gid == 1 || uid == 1)
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
    if (gid == 1 || uid == 1)
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
    if (gid == 1 || uid == 1)
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
    if (gid == 1 || uid == 1)
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

//???1???pwd - ?????????????????????????????????
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
//???2???ls - ???????????????????????????????????????
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
//???3???cd - ?????????????????????
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
            pos = dynamic_cast<dir_c*>(pos->get_contents()[".."]);
            args[0].erase(0, 3);
            split(args[0], '/', vc);
        }
        else if (args[0][1] == '.')
        {
            pos = dynamic_cast<dir_c*>(pos->get_contents()[".."]);
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
        if (a == 0)
        {
            break;
        }
        map<string,filesystem_c*>& contents = pos->get_contents();
        if (contents.find(x) != contents.end())
        {
            if (contents[x]->get_filetype() == DIR)
            {
                pos = dynamic_cast<dir_c*>(contents[x]);
            }
            else if(contents[x]->get_filetype() == LINK 
            && dynamic_cast<link_c*>(contents[x])->get_real()->get_filetype() == DIR){
                pos = dynamic_cast<dir_c*>(dynamic_cast<link_c*>(contents[x])->get_real());
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
// ?????????????????????????????? ??????????????????????????????
bool user_c::TEMP(vector<string>& args,function<void(user_c*,string)> cdarg,ATTRIBUTE attr){
    auto dir = get_current_dir();
    string name = args[0];
    if (args[0].find("/") != string::npos)
    {
        name = args[0].substr(args[0].rfind("/") + 1);
        args[0] = args[0].substr(0, args[0].rfind("/"));
        if(args[0] == "") {args[0] = "/";}
        if (cd(args))
        {
            if ((dynamic_cast<filesystem_c*>(get_current_dir()))->permission(this, attr))
            {
                cdarg(this,name);
                set_current_dir(dir);
                if(get_error()!=NO) return false;
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
            cdarg(this,name);
            if(get_error()!=NO) return false;
            return true;
        }
        else
        {
            set_error(PERMISSION);
            return false;
        }
    }
}

bool user_c::TEMP(vector<string>& args,function<filesystem_c*(user_c*,string,filesystem_c*)> cdarg,function<void(user_c*,string,string,filesystem_c*)> destarg,ATTRIBUTE attr){
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
        if(args[0] == "") {args[0] = "/";}
        if (cd(vsrc))
        {
            if ((dynamic_cast<filesystem_c*>(get_current_dir()))->permission(this, READ))
            {
                // TODO ???????????????
                copy = cdarg(this,src,copy);
                set_current_dir(dir);
                if(get_error()!=NO) return false;
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
            // TODO ???????????????
            copy = cdarg(this,src,copy);
            if(get_error()!=NO) return false;
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
                // TODO ????????????????????????????????????
                destarg(this,dest,src,copy);
                set_current_dir(dir);
                if(get_error()!=NO) return false;
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
            // TODO ????????????????????????????????????
            destarg(this,dest,src,copy);
            if(get_error()!=NO) return false;
        }
        else
        {
            set_error(PERMISSION);
            return false;
        }
    }
    return true;
}
//???4???mkdir - ????????????
bool user_c::mkdir(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        dir_c *newdir = new dir_c(user, name, dynamic_cast<filesystem_c*>(user->get_current_dir()));
    },ATTRIBUTE::WRITE);
}
//???5???rmdir - ????????????(?????????)
bool user_c::rmdir(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* filesystem = user->get_current_dir()->get_contents()[name];
        if(filesystem != NULL){
            delete dynamic_cast<dir_c *>(filesystem);
        }
        else{
            user->set_error(NOTFOUND);
        }
    },ATTRIBUTE::WRITE);
}
//???6???touch - ???????????????
bool user_c::touch(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        file_c *newfile = new file_c(user, name, dynamic_cast<filesystem_c*>(user->get_current_dir()));
    },ATTRIBUTE::WRITE);
}
//???7???cp - ????????????????????????????????????????????? cp source[args[0]] -> dest[args[1]]
bool user_c::cp(vector<string> &args)
{
    #ifdef __Qt__
    return TEMP(args,[](user_c* user,string name){
        user->copyfilesystem = user->get_current_dir()->get_contents()[name];
        FILETYPE type = user->copyfilesystem->get_filetype();
        if ( type == DIR)
        {
            user->copyfilesystem = new dir_c(*dynamic_cast<dir_c *>(user->copyfilesystem));
        }
        else if (type == BINARY 
        || type == TEXT 
        || type == UNKNOWN
        )
        {
            user->copyfilesystem = new file_c(*dynamic_cast<file_c *>(user->copyfilesystem));
        }
        else if(type == LINK){
            user->copyfilesystem = new link_c(*dynamic_cast<link_c*>(user->copyfilesystem));
        }
        else
        {
        }
    });
    #else
    return TEMP(args,[](user_c* user,string src,filesystem_c* copy)->filesystem_c*{
        auto filesystem = user->get_current_dir()->get_contents()[src];
        FILETYPE type = filesystem->get_filetype();
        if ( type == DIR)
        {
            copy = new dir_c(*dynamic_cast<dir_c *>(filesystem));
        }
        else if (type == BINARY 
        || type == TEXT 
        || type == UNKNOWN
        )
        {
            copy = new file_c(*dynamic_cast<file_c *>(filesystem));
        }
        else if(type == LINK){
            copy = new link_c(*dynamic_cast<link_c*>(filesystem));
        }
        else
        {
        }
        return copy;
    },[](user_c* user,string dest,string src,filesystem_c* copy){
        if (dest == "")
        {
            dest = src;
        }
        copy->set_name(dest);
        copy->set_parent(user->get_current_dir());
        
    },ATTRIBUTE::WRITE);
    #endif
}
//???8???rm - ?????????????????????
bool user_c::rm(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* filesystem = user->get_current_dir()->get_contents()[name];
        if(filesystem != NULL){
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
        }else{
            user->set_error(NOTFOUND);
        }
    },
    WRITE);
}
//???9???mv - ?????????????????????????????????
bool user_c::mv(vector<string> &args)
{
#ifdef __Qt__
    vector<string> v(args.begin(),args.begin()+1);
    vector<string> v2(args.begin()+1,args.end());
    if(cp(args) == false) return false;
    if(paste(v2) == false) return false;
    if(rm(v) == false) return false;
    return true;
#else
    vector<string> v(args.begin(),args.begin()+1);
    if(cp(args) == false) return false;
    if(rm(v) == false) return false;
    return true;
#endif
}
//???10???cat - ??????????????????
bool user_c::cat(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f->type == LINK) f = dynamic_cast<link_c*>(f)->get_real();
        if(f == NULL){user->set_error(NOTFOUND);}
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT);}
        else{
            file_c* file = dynamic_cast<file_c*>(f);
            vector<string>& v = file->get_mem();
            vector<string>::iterator it = v.begin();
            for(;it != v.end() && it != v.begin() + 10;it ++){
                cout << *it <<endl;
            }
            if(it == v.begin()+10) user->set_read_cursor(10);
            else user->set_read_cursor(0);
        }
    });
}
//???11???more - ???????????????
bool user_c::more(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND);}
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT);}
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
    });
}
//???12???echo - ????????????????????????
bool user_c::echo(vector<string> &args)
{
    if(args.size() > 0 && args.size() < 2){
        // cout << args[0] << endl;
        color_cout(HIGHTLIGHT,F_MAGENTA,args[0]); 
        cout << endl;
        return true;
    }
    else if(args.size() >= 2)
    {
        return dup2(args);
    }
    return false;
}
//???13???head - ????????????????????????
bool user_c::head(vector<string> &args)
{
    return cat(args);
}
//???14???tail - ???????????????????????????
bool user_c::tail(vector<string> &args)
{
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND); }
        else if(f->get_filetype() != TEXT) {user->set_error(NOTTEXT); }
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
    });
}
//???15???> / >> - ???????????????/??????
bool user_c::dup2(vector<string> &args)
{
    if(args[1] != ">" && args[1] != ">>")
    {
        error = ERROR::CMD;
        return false;
    }
    vector<string> farg(args.begin()+2,args.begin()+3);
    return TEMP(farg,[=](user_c* user,string name){
        auto files = user->get_current_dir()->get_contents()[name];
        dynamic_cast<file_c*>(files)->get_mem().push_back(args[0]);
    },ATTRIBUTE::WRITE);
    

}
//???16???ln - ?????????
bool user_c::ln(vector<string> &args)
{
    return TEMP(args,[](user_c* user,string src,filesystem_c* copy)->filesystem_c*{
        copy = user->get_current_dir()->get_contents()[src];
        if(copy == NULL){user->set_error(NOTFOUND);}
        return copy;
    },[](user_c* user,string dest,string src,filesystem_c* copy){
        link_c* link = new link_c(user,dest,dynamic_cast<filesystem_c*>(user->get_current_dir()));
        link->set_real(copy);
    },ATTRIBUTE::WRITE);
}
//???17???history - ?????????????????????????????????
bool user_c::history(vector<string> &args)
{
    int count = 1;
    for(auto it : his){
        cout << count++ << "\t" << it << endl;
    }
}
// (18) vim - ?????????
bool user_c::vim(vector<string>& args){
    return TEMP(args,
    [](user_c* user,string name){
        filesystem_c* fs = user->get_current_dir()->get_contents()[name];
        if(fs->get_filetype() == LINK){
            fs = dynamic_cast<link_c*>(user->get_current_dir()->get_contents()[name])->get_real();
        }

        if( fs->get_filetype() == TEXT){
            file_c* f = dynamic_cast<file_c*>(fs);
            f->set_update_time(get_time());
            vector<string> v = f->get_mem();
            ofstream out(name);
            for(auto x : v){
                out << x;
            }
            out.close();
            
            #ifdef __unix__
                #ifdef __Qt__
                system((string("gedit ")+name).c_str());
                #else
                system((string("vim ")+name).c_str());
                #endif
            #elif _WIN32
            system((string("start notepad ")+name).c_str());
            #endif
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
            #ifdef __unix__
            system((string("rm ")+name).c_str());
            #elif _WIN32
            system((string("del ")+name).c_str());
            #endif
        }
        
    },ATTRIBUTE::WRITE);
}
// (19) stat - ????????????
bool user_c::stat(vector<string>& args){
    return TEMP(
        args,
        [](user_c* user,string name){
            filesystem_c* filesystem = user->get_current_dir()->get_contents()[name];
            if(filesystem == NULL){user->set_error(NOTFOUND);}
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
            cout << "Creater_uid : " << (int)filesystem->get_createid() << "\t";
            cout << "Owner_uid : " << (int)filesystem->get_ownerid() << "\t";
            cout << "Owenr_gid : " << (int)filesystem->get_ownergid() << "\t" <<endl;
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

#ifdef __Qt__
bool user_c::paste(vector<string>& args){
    if(copyfilesystem != NULL){
        return TEMP(args,[](user_c* user,string name){
            if (name == "")
            {
                name = user->copyfilesystem->get_name();
            }
            user->copyfilesystem->set_name(name);
            user->copyfilesystem->set_parent(user->get_current_dir(),false);
            user->copyfilesystem = nullptr;
        });
    }
    else{
        return false;
    }
}
//(21) get_pos - ??????????????????
vector<string> user_c::get_pos(filesystem_c* filesystem){
    vector<string> vs;
    dir_c* d = dynamic_cast<dir_c*>(filesystem->get_parent());
    auto dir = dynamic_cast<filesystem_c*>(d);
    if (dynamic_cast<dir_c*>(dir) == root)
    {
        vs.push_back("/");
        return vs;
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
        vs.push_back(*it);
    }
    vs.push_back(filesystem->get_name());
    return vs;
}
#endif
bool user_c::myid(vector<string>&)
{
    cout << "current user id = " << (int)uid << endl;
    return true;
}

bool user_c::mygid(vector<string>&)
{
    cout << "current use group id = " << (int)gid << endl;
    return true;
}

bool user_c::login(vector<string>& args)
{
    for(auto u : users)
    {
        if(u->name == args[0])
        {
            dir_c* current_dir = user->get_current_dir();
            user = u;
            u->current_dir = current_dir;
            return true;
        }
    }
    error = NOUSER;
    return false;
}

bool user_c::quit(vector<string>&)
{
    color_cout(HIGHTLIGHT,F_MAGENTA,"Bye " + name + "!");
    cout << endl;
    exit(0);
}

bool user_c::chmod(vector<string>& args)
{
    vector<string> tmp{args[0]};
    return TEMP(tmp,
    [=](user_c* user,string name){
        filesystem_c* f = user->get_current_dir()->get_contents()[name];
        if(f == NULL){user->set_error(NOTFOUND);return;}
        else
        {
            unsigned char attr = (unsigned char)atoi(args[1].c_str());
            if(attr == 0 && args[1] != "0") {user->set_error(ERROR::CMD); return;}
            if(attr > 63){user->set_error(ERROR::CMD);return;}
            if(attr & 0x1 || attr & 0x8)
            {
                if(!f->permission(this,CHREAD))
                {
                    user->set_error(PERMISSION);
                    return;
                }
            }
            else if(attr & 0x2 || attr & 0x10)
            {
                if(!f->permission(this,CHWRITE))
                {
                    user->set_error(PERMISSION);
                    return;
                }
            }
            else if(attr & 0x4 || attr & 0x20)
            {
                if(!f->permission(this,CHEXEC))
                {
                    user->set_error(PERMISSION);
                    return;
                }
            }
            
            f->set_other_permission(attr);
        }
    });
}