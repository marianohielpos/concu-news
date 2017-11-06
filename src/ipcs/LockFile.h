#ifndef LOCK_H_
#define LOCK_H_

#include <fcntl.h>
#include <iostream>


class LockFile {

private:
    struct flock fl;
    int fd;
    std::string nombre;

public:
    LockFile ( const std::string nombre );
    ~LockFile();

    int tomarLock ();
    int liberarLock ();
    ssize_t escribir ( const void* buffer,const ssize_t buffsize ) const;
};


#endif //LOCK_H_
