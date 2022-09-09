#include "filesystem.h"

/*
    time_t timer;
    timer = time(NULL);
    create_time = localtime(&timer);
    *update_time = *create_time;
    *visit_time = *create_time;
*/

tm* get_time(){
        time_t timer;
        timer = time(NULL);
        tm* t = localtime(&timer);
        return t;
}


filesystem_c::filesystem_c(
        string _name,
        unsigned char owenr,
        unsigned char group,
        unsigned char other,
        const user_c user,
        FILETYPE _type
){      
        name = _name;
        if(_type!=UNKNOWN) set_filetype(_type);
        else{set_filetype();}
        set_owner_permission(owenr);
        set_group_permission(group);
        set_other_permission(other);
        set_createid(user.get_uid());
        set_ownerid(user.get_uid());
        tm* t = get_time();
        create_time = new tm;
        update_time = new tm;
        visit_time = new tm;
        set_create_time(t);
        set_update_time(t);
        set_visit_time(t);
}

filesystem_c::~filesystem_c(){

}

inline FILETYPE filesystem_c::get_filetype() const{return type;}
inline attribute_u filesystem_c::get_owner_permission() const{return owner_attr_u;}
inline attribute_u filesystem_c::get_group_permission() const{return group_attr_u;}
inline attribute_u filesystem_c::get_other_permission() const{return other_attr_u;}
inline string filesystem_c::get_name() const{return name;}
inline unsigned char filesystem_c::get_createid() const{return createrid;}
inline unsigned char filesystem_c::get_ownerid() const{return ownerid;}
inline unsigned char filesystem_c::get_ownergid() const{return ownergid;}
inline tm filesystem_c::get_create_time() const{return *create_time;}
inline tm filesystem_c::get_update_time() const{return *update_time;}
inline tm filesystem_c::get_visit_time() const{return *visit_time;}
inline filesystem_c* filesystem_c::get_parent(){return parent;}

void filesystem_c::set_filetype(FILETYPE newtype){type = newtype;}
void filesystem_c::set_filetype(){
        size_t pos = name.rfind('.');
        if(pos!=string::npos){
                string s = name.substr(pos);
                if(s == ".txt") type = TEXT;
                else if(s == ".doc") type = TEXT;
                else if(s == ".html") type = TEXT;
                else type = BINARY;
        }else{
                type = BINARY;   
        }
}
void filesystem_c::set_owner_permission(attribute_u attr){owner_attr_u = attr;}
void filesystem_c::set_owner_permission(unsigned char attr){owner_attr_u.attr = attr;}
void filesystem_c::set_group_permission(attribute_u attr){group_attr_u = attr;}
void filesystem_c::set_group_permission(unsigned char attr){group_attr_u.attr = attr;}
void filesystem_c::set_other_permission(attribute_u attr){other_attr_u = attr;}
void filesystem_c::set_other_permission(unsigned char attr){other_attr_u.attr = attr;}
void filesystem_c::set_name(string _name){name = _name;}
void filesystem_c::set_createid(unsigned char _createrid){createrid = _createrid;}
void filesystem_c::set_ownerid(unsigned char _ownerid){ownerid = _ownerid;}
void filesystem_c::set_ownergid(unsigned char gid){ownergid = gid;}
void filesystem_c::set_create_time(tm* newcreatetime){*create_time = *newcreatetime;}
void filesystem_c::set_update_time(tm* newupdatetime){*update_time = *newupdatetime;}
void filesystem_c::set_visit_time(tm* newvisittime){*visit_time = *newvisittime;}
void filesystem_c::set_parent(filesystem_c* _parent){
        parent->get_contents().erase(this->get_name()) ;
        parent = _parent;
        parent->get_contents()[get_name()] = this;
}


/*
        if(user.get_uid() == ownerid){
                if(owner_attr_u.attr_s.read) return true;
                else return false;
        }else if(user.get_gid() == ownergid){
                if(group_attr_u.attr_s.read) return true;
                else return false;
        }else{
                if(other_attr_u.attr_s.read) return true;
                else return false;
        }
*/
bool filesystem_c::permission(user_c& user,ATTRIBUTE attr) const{
        unsigned char u,g,o;
        switch(attr){
                case READ : {
                        u = owner_attr_u.attr_s.read;
                        g = group_attr_u.attr_s.read;
                        o = other_attr_u.attr_s.read;
                }
                case WRITE : {
                        u = owner_attr_u.attr_s.write;
                        g = group_attr_u.attr_s.write;
                        o = other_attr_u.attr_s.write;
                }
                case EXEC : {
                        u = owner_attr_u.attr_s.exec;
                        g = group_attr_u.attr_s.exec;
                        o = other_attr_u.attr_s.exec;
                }
                case CHREAD : {
                        u = owner_attr_u.attr_s.chread;
                        g = group_attr_u.attr_s.chread;
                        o = other_attr_u.attr_s.chread;
                }
                case CHWRITE : {
                        u = owner_attr_u.attr_s.chwrite;
                        g = group_attr_u.attr_s.chwrite;
                        o = other_attr_u.attr_s.chwrite;
                }
                case CHEXEC : {
                        u = owner_attr_u.attr_s.chexec;
                        g = group_attr_u.attr_s.chexec;
                        o = other_attr_u.attr_s.chexec;
                }
                default : {
                        return false;
                }
        }
        if(user.get_uid() == ownerid || user.get_gid() == 1){
                if(u) return true;
                else return false;
        }else if(user.get_gid() == ownergid || user.get_gid() == 1){
                if(g) return true;
                else return false;
        }else{
                if(o) return true;
                else return false;
        }
}

long filesystem_c::get_size() const{
        
}
vector<string>& filesystem_c::get_mem(){
        
}
map<string,filesystem_c*>& filesystem_c::get_contents(){

}


file_c::file_c(
        const user_c& user,string name,
        unsigned char owner_permission,
        unsigned char group_permission,
        unsigned char other_permission
):filesystem_c(name,owner_permission,group_permission,other_permission,user,UNKNOWN)
{
        mem = *(new vector<string>);
}

file_c::~file_c(){
        delete &mem;
}

long file_c::get_size() const{
        return size;
}
vector<string>& file_c::get_mem(){
        return mem;
}

dir_c::dir_c(
        const user_c& user,string name,
        unsigned char owner_permission,
        unsigned char group_permission,
        unsigned char other_permission
):filesystem_c(name,owner_permission,group_permission,other_permission,user,DIR)
{
        contents = *(new map<string,filesystem_c*>);
        contents["."] = this;
        if(this->get_name() == "/"){
                contents[".."] = this;
        }else{
                contents[".."] = this->parent;
        }
        
}

dir_c::~dir_c(){
        delete &contents;
}

map<string,filesystem_c*>& dir_c::get_contents(){
        return contents;
}
