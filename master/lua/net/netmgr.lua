-- netmgr.lua
-- 2015-09-14
-- xzc

-- 网络事件管理

local Net_mgr = oo.singleton( nil,... )

-- 构造函数
function Net_mgr:__init()
    self.conn = {}
    setmetatable( self.conn,{ __mode = "v" } )  --保证连接对象能被释放
end

-- push一个连接对象
function Net_mgr:push( conn )
    self.conn[conn.fd] = conn
end

-- accept事件
function Net_mgr:accept_event( fd,... )
    local conn = self.conn[fd]
    if not conn then
        ELOG( "event_cb no connnect object found" )
        return
    end

    local result,sk = xpcall( conn.on_accept,__G__TRACKBACK__,conn,... )
    return sk
end

-- read事件
function Net_mgr:read_event( fd,... )
    local conn = self.conn[fd]
    if not conn then
        ELOG( "event_cb no connnect object found" )
        return
    end
    
    xpcall( conn.on_read,__G__TRACKBACK__,conn,... )
end

-- disconnect事件
function Net_mgr:disconnect_event( fd,... )
    local conn = self.conn[fd]
    if not conn then
        ELOG( "event_cb no connnect object found" )
        return
    end
    
    xpcall( conn.on_disconnect,__G__TRACKBACK__,conn,... )
end

-- connected事件
function Net_mgr:connected_event( fd,... )
    local conn = self.conn[fd]
    if not conn then
        ELOG( "event_cb no connnect object found" )
        return
    end
    
    xpcall( conn.on_connected,__G__TRACKBACK__,conn,... )
end

local net_mgr = Net_mgr()    -- 创建单例

return net_mgr
