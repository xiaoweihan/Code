#include "Lock.h"
#include <iostream>

using namespace std;

CLock::CLock(void)
{
    if (0 != pthread_mutex_init(&m_Mutex,NULL))
    {
        cout << "pthread_mutex_init failed." << endl;
    }
}

CLock::~CLock(void)
{
    pthread_mutex_destroy(&m_Mutex);
}

void CLock::Lock(void)
{
    pthread_mutex_lock(&m_Mutex);
}

void CLock::UnLock(void)
{
    pthread_mutex_unlock(&m_Mutex);    
}