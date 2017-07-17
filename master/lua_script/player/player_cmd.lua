-- 玩家相关协议处理

local CS = CS
local SC = SC

local g_command_mgr = g_command_mgr
local g_network_mgr = g_network_mgr
local g_player_mgr  = g_player_mgr


local function player_ping( srv_conn,pid,pkt )
    srv_conn:send_clt_pkt( pid,SC.PLAYER_PING,{ time = ev:time() } )
end

local function player_enter( srv_conn,pid,pkt )
    g_player_mgr:on_enter_world( pid,pkt )
end

local function player_offline( srv_conn,pkt )
    g_player_mgr:on_player_offline( pkt )
end

local function player_login_otherwhere( srv_conn,pkt )
    g_player_mgr:on_login_otherwhere( pkt.pid )
end

-- 这里注册系统模块的协议处理
if "gateway" == Main.srvname then
    g_command_mgr:clt_register( CS.PLAYER_LOGIN,
        g_account_mgr.player_login(g_account_mgr),true )
    g_command_mgr:clt_register( CS.PLAYER_CREATE,
        g_account_mgr.create_role(g_account_mgr),true )
end

if "world" == Main.srvname then
    g_command_mgr:clt_register( CS.PLAYER_PING,player_ping )
    g_command_mgr:clt_register( CS.PLAYER_ENTER,player_enter )

    g_command_mgr:srv_register( SS.PLAYER_OFFLINE,player_offline )
    g_command_mgr:srv_register( SS.PLAYER_OTHERWHERE,player_login_otherwhere )
end