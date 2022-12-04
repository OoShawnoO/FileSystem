# Project File System

- 目的:以内存作为存储区，模拟 UNIX 的文件系统结构，简单的文件系统，并提供以下 的文件和目录操作接口：创建文件、删除文件、打开文件、关闭文件、读文件、写文件。 



设计思路:

```c++
class filesystem_c; // 作为 文件类file_c \ 目录类dir_c 的基类 提供共同接口
class file_c;		// 文件类 使用 vector<string>存储数据 二进制数据使用 base64.encode 转换后存储
class dir_c;		// 目录类 map<string,filesystem_c*> 保存目录下的文件和文件夹索引
class user_c;		// 用户类 采用自增id = 1 root用户 groupid = 1 管理权限

权限的实现：
    预留      预留    chexec   chwrite  chread   exec    write   read
    0         0         0         0        0      0        0      0
    128       64        32        16       8      4        2      1

union attribute_u{
    unsigned char attr;
    struct{
        unsigned char read:1;       //读权限
        unsigned char write:1;      //写权限
        unsigned char exec:1;       //执行权限     
        unsigned char chread:1;     //修改读权限
        unsigned char chwrite:1;    //修改写权限
        unsigned char chexec:1;     //修改执行权限
        unsigned char reserve:2;    //预留
    }attr_s;
}
```



