#ifndef __LOCK_H
#define __LOCK_H

#include <pthread.h>

class CLock
{

public:
    CLock(void);
    ~CLock(void);


public:
    void Lock(void);

    void UnLock(void);

private:
    pthread_mutex_t m_Mutex;

};




#endif