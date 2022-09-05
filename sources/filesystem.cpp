#include "filesystem.h"

file_c::file_c(user_c user,string _name,unsigned char permission){

    time_t timer;
    timer = time(NULL);
    create_time = localtime(&timer);
    *update_time = *create_time;
    *visit_time = *create_time;

    name = _name;

}