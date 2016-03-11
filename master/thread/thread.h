#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

#include "../ev/ev_watcher.h"
#include "../global/global.h"

class thread
{
public:
    thread();
    virtual ~thread();
    
    void stop ();
    bool start( int32 sec = 1,int32 usec = 0 );
    
    pthread_t get_id();
public:
    typedef enum
    {
        ERROR  = -1,
        NONE   =  0,
        EXIT   =  1,
        MSG    =  2
    }notify_t;
protected:
    virtual bool cleanup();       /* 子线程清理 */
    virtual bool initlization();  /* 子线程初始化 */
    
    void notify_child( notify_t msg );     /* 通知子线程 */
    void notify_parent( notify_t msg );     /* 通知主线程 */
    
    virtual void routine( notify_t msg ) = 0;    /* 子线程通知处理 */
    virtual notification( notify_t msg ) = 0;    /* 主线程收到通知 */
    
    static void *start_routine( void *arg );
    
    inline void lock() { pthread_mutex_lock( &_mutex ); }
    inline void unlock() { pthread_mutex_unlock( &_mutex ); }
private:
    void io_cb( ev_io &w,int32 revents );
private:
    int32 _fd[2]  ;
    ev_io _watcher;
    pthread_t _thread;
    volatile bool _run;
    volatile bool _join;
    pthread_mutex_t _mutex;
};

#endif /* __THREAD_H__ */
