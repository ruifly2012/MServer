#include "llog.h"
#include "../lua/leventloop.h"

llog::llog( lua_State *L )
    : L ( L )
{
    _wts = 0;
}

llog::~llog()
{
    assert( "log thread not exit yet",!_run );
}

int32 llog::stop ()
{
    if ( !thread::_run )
    {
        ERROR( "try to stop a inactive log thread" );
        return 0;
    }

    thread::stop();

    return 0;
}

int32 llog::start()
{
    /* 设定多少秒写入一次 */
    int32 sec  = luaL_optinteger( L,1,5 );
    int32 usec = luaL_optinteger( L,2,0 );
    thread::start( sec,usec );

    return 0;
}

int32 llog::write()
{
    if ( !thread::_run )
    {
        return luaL_error( L,"log thread inactive" );
    }
    
    size_t len = 0;
    const char *name = luaL_checkstring( L,1 );
    const char *str  = luaL_checklstring( L,2,&len );

    class leventloop *ev = leventloop::instance();
    
    /* 时间必须取主循环的帧，不能取即时的时间戳 */
    lock();
    _log.write( ev->now(),name,str,len );
    unlock();

    return 0;
}

void llog::routine( notify_t msg )
{
    switch ( msg )
    {
        case ERROR : 
            assert( "main thread shold not notify error",false );
            return;
            break ;
        case EXIT  : return;break;
        case NONE  : break;
        case MSG   :
            assert( "log thread do't need msg notify",false );
            return;
            break ;
    }

    bool wfl = true;
    while ( wfl )
    {
        wfl = false;

        lock();
        class log_file *plf = _log.get_log_file( _wts );
        unlock();

        if ( plf )
        {
            plf->flush(); /* 写入磁盘 */
            wfl = true;
        }
    }

    /* 清理不必要的缓存 */
    lock();
    _log.remove_empty( _wts );
    unlock();

    ++_wts;
}

bool llog::cleanup()
{
    /* 线程终止，所有日志入磁盘 */
    /* 不应该再有新日志进来，可以全程锁定 */
    ++_wts;
    bool wfl = true;
    
    lock();
    while ( wfl )
    {
        wfl = false;
        class log_file *plf = _log.get_log_file( _wts );
        if ( plf )
        {
            plf->flush(); /* 写入磁盘 */
            wfl = true;
        }
    }
    unlock();
}

int32 llog::mkdir_p( lua_State *L )
{
    const char *path = luaL_checkstring( L,1 );
    if ( !log::mkdir_p( path ) )
    {
        lua_pushboolean( L,0 );
    }
    else
    {
        lua_pushboolean( L,1 );
    }
    
    return 1;
}
