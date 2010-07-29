/*
 * ============================================================================
 *
 *  Project
 *
 *  File:          testmodule.inc
 *  Type:          Module
 *  Description:   Test module designed with no other purpose other than testing project base components.
 *
 *  Copyright (C) 2009-2010  Greyscale
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 */

#define NUM_MODULES 10

/**
 * A global variable to store the module identifiers in.
 */
stock Module:g_iTestModules[NUM_MODULES];

/**
 * Plugin is loading.
 * Register TestModule.
 */
public TestModule_OnPluginStart()
{
    new moduledata2[ModuleData] = {false, false, "", "testmoduleblank", "Module designed to fail to load.", {INVALID_MODULE}};
    ModuleMgr_Register(moduledata2);
    
    new moduledata3[ModuleData] = {false, false, "Fail Module", "", "Module designed to fail to load.", {INVALID_MODULE}};
    ModuleMgr_Register(moduledata3);
    
    new moduledata4[ModuleData] = {true, false, "Depend On Me", "dependonme", "Module to depend on.", {INVALID_MODULE}};
    new Module:moduleDepend = ModuleMgr_Register(moduledata4);
    
    new Module:dependencies[8];
    dependencies[0] = moduleDepend;
    dependencies[1] = INVALID_MODULE;
    
    new moduledata5[ModuleData] = {false, false, "Depender", "depender", "Module that is depending.", {INVALID_MODULE}};
    moduledata5[ModuleData_Dependencies] = dependencies;
    ModuleMgr_Register(moduledata5);
    
    // Define all the module's data as layed out by enum ModuleData in project.inc.
    new moduledata[ModuleData];
    
    decl String:format1[32];
    for (new x = 0; x < NUM_MODULES; x++)
    {
        moduledata[ModuleData_Disabled] = !(bool:GetRandomInt(0, 50));
        moduledata[ModuleData_Hidden] = !(bool:GetRandomInt(0, 50));
        
        Format(format1, sizeof(format1), "Module_%02d", x);
        strcopy(moduledata[ModuleData_FullName], MM_DATA_FULLNAME, format1);
        
        Format(format1, sizeof(format1), "module%02d", x);
        strcopy(moduledata[ModuleData_ShortName], MM_DATA_SHORTNAME, format1);
        
        strcopy(moduledata[ModuleData_Description], MM_DATA_DESCRIPTION, "Module designed to test the project base.");
        
        moduledata[ModuleData_Dependencies][0] = INVALID_MODULE;
        
        // Register the module with this data.
        g_iTestModules[x] = ModuleMgr_Register(moduledata);
        
        // Now register the events we're going to use.
        #if defined EVENT_MANAGER
            EventMgr_RegisterEvent(g_iTestModules[x], "Event_OnEventsRegister", "TestModule_OnEventsRegister");
        #endif
        
        #if defined CONFIG_MANAGER
            //ConfigMgr_Register(g_iTestModules[x], "TestModule_ReloadConfig", "configs/project/testconfig.txt");
        #endif
    }
    
    // ************************
    // Done registering module.
    // ************************
}

/**
 * Register all events here.
 */
public TestModule_OnEventsRegister()
{
    // Now register the events we're going to use.
    #if defined EVENT_MANAGER
    
        new module = _:EventMgr_GetEventOwner() - 2;
        // This event should cause errors and not register.
        //EventMgr_RegisterEvent(g_iTestModules[module], "Event_OnAllPluginsLoaded", "TestModule_FAILFAILFAIL");
        
        EventMgr_RegisterEvent(g_iTestModules[module], "Event_OnEventsReady", "TestModule_OnEventsReady");
        EventMgr_RegisterEvent(g_iTestModules[module], "Event_OnMyModuleEnable", "TestModule_OnMyModuleEnable");
        EventMgr_RegisterEvent(g_iTestModules[module], "Event_OnMyModuleDisable", "TestModule_OnMyModuleDisable");
        EventMgr_RegisterEvent(g_iTestModules[module], "Event_OnClientPutInServer", "TestModule_OnClientPutInServer");
        EventMgr_RegisterEvent(g_iTestModules[module], "Event_OnClientDisconnect", "TestModule_OnClientDisconnect");
        
        #if defined PROJECT_GAME_CSS
            EventMgr_RegisterEvent(g_iTestModules[module], "Event_RoundStart", "TestModule_OnRoundStart");
            //EventMgr_RegisterEvent(g_iTestModules[x], "Event_OnMapStart", "TestModule_OnMapStart");
            EventMgr_RegisterEvent(g_iTestModules[module], "Event_PlayerSpawn", "TestModule_OnPlayerSpawn");
            EventMgr_RegisterEvent(g_iTestModules[module], "Event_PlayerHurt", "TestModule_OnPlayerHurt");
        #endif
    #endif
}

/**
 * All modules and events have been registered by this point.  Event priority can be changed here.
 */
public TestModule_OnEventsReady()
{
    // Test priority setting.
    /**
    if (EventMgr_GetEventOwner() == g_iTestModules[NUM_MODULES - 1])
    {
        EventMgr_PrintPriority(g_EvOnMyModuleEnable);
        EventMgr_GivePriority(g_EvOnMyModuleEnable, g_iTestModules[8], g_iTestModules[0]);
        EventMgr_PrintPriority(g_EvOnMyModuleEnable);
        EventMgr_TakePriority(g_EvOnMyModuleEnable, g_iTestModules[8], g_iTestModules[9]);
        EventMgr_PrintPriority(g_EvOnMyModuleEnable);
    }
    */
    
    PrintToServer("Events Ready.");
}

enum TestModuleConfigs
{
    String:TMConfig_SectionName[32] = 0,
    TMConfig_Int,
    Float:TMConfig_Float,
    String:TMConfig_String[32]
}

#if defined CONFIG_MANAGER
#define TM_MAX_KEYS 64
//new g_TMConfigCache[TM_MAX_KEYS][TestModuleConfigs];
#endif

/*
public TestModule_OnMapStart()
{
    #if defined CONFIG_MANAGER
        // Cache our configs.
        new count = ConfigMgr_CacheKv(g_iTestModule, CM_CONFIGINDEX_FIRST, "TestModule_ConfigCache");
        for (new x = 0; x < count; x++)
            PrintToServer("CACHED: INDEX: %d SECTION: %s INT %d FLOAT %f STRING %s", x, g_TMConfigCache[x][TMConfig_SectionName], g_TMConfigCache[x][TMConfig_Int], g_TMConfigCache[x][TMConfig_Float], g_TMConfigCache[x][TMConfig_String]);
    #endif
}

#if defined CONFIG_MANAGER
public TestModule_ReloadConfig(configindex)
{
    #if defined LOG_MANAGER
        LogMgr_Print(g_iTestModule, LogType_Normal, "TestConfigReload", "Reloaded test module's config (index %d)", configindex);
    #endif
    
    new count = ConfigMgr_CacheKv(g_iTestModule, CM_CONFIGINDEX_FIRST, "TestModule_ConfigCache");
    for (new x = 0; x < count; x++)
        PrintToServer("CACHED: INDEX: %d SECTION: %s INT %d FLOAT %f STRING %s", x, g_TMConfigCache[x][TMConfig_SectionName], g_TMConfigCache[x][TMConfig_Int], g_TMConfigCache[x][TMConfig_Float], g_TMConfigCache[x][TMConfig_String]);
}

public KvCache:TestModule_ConfigCache(Handle:kv, sectionindex, const String:sectionname[])
{
    strcopy(g_TMConfigCache[sectionindex][TMConfig_SectionName], 32, sectionname);
    
    g_TMConfigCache[sectionindex][TMConfig_Int] =      KvGetNum(kv,    "int");
    g_TMConfigCache[sectionindex][TMConfig_Float] =    KvGetFloat(kv,  "float");
    
    KvGetString(kv, "string", g_TMConfigCache[sectionindex][TMConfig_String], 32);
}
#endif
*/

#if defined EVENT_MANAGER

public Action:TestModule_OnMyModuleEnable()
{
    return Plugin_Handled;
}

public TestModule_OnMyModuleDisable()
{
    PrintToServer("Module disabled.");
}

public TestModule_OnClientPutInServer(client)
{
    new Module:curmodule = EventMgr_GetEventOwner();
    
    #if defined LOG_MANAGER
        LogMgr_Print(curmodule, LogType_Normal, "Event_OnClientPutInServer", "Client %N joined.", client);
    #endif
}

public TestModule_OnClientDisconnect(client)
{
    new Module:curmodule = EventMgr_GetEventOwner();
    
    #if defined LOG_MANAGER
        LogMgr_Print(curmodule, LogType_Normal, "Event_OnClientDisconnect", "Client %N left.", client);
    #endif
}

#if defined PROJECT_GAME_CSS

public TestModule_OnPlayerSpawn(client)
{
    new Module:curmodule = EventMgr_GetEventOwner();
    
    #if defined LOG_MANAGER
        LogMgr_Print(curmodule, LogType_Normal, "Event_PlayerSpawn", "Client %N spawned.", client);
    #endif
    
    #if defined ACCESS_MANAGER
    #if defined LOG_MANAGER
    if (AccessMgr_HasAccess(client, curmodule))
        LogMgr_Print(curmodule, LogType_Normal, "Event_PlayerSpawn", "Client %N has access to testmodule.", client);
    #endif
    #endif
}

public TestModule_OnPlayerHurt(client, attacker, health, armor, const String:weapon[], dmg_health, dmg_armor, hitgroup)
{
    new Module:curmodule = EventMgr_GetEventOwner();
    
    #if defined LOG_MANAGER
        LogMgr_Print(curmodule, LogType_Normal, "Event_PlayerHurt", "Client %N was hurt by %N health %d armor %d weapon %s dmg_health %d hitgroup %d", client, attacker, health, armor, weapon, dmg_health, hitgroup);
    #endif
}

/**
 * Round has started.
 */
public TestModule_OnRoundStart(dcell, Float:dfloat, any:darray[], String:dstring[])
{
    new Module:curmodule = EventMgr_GetEventOwner();
    
    #if defined TRANSLATIONS_MANAGER
        TransMgr_PrintText(SERVER_INDEX, MsgFormat_Both, MsgType_Server, curmodule, true, "Round has started!");
        TransMgr_PrintTextAll(false, false, MsgFormat_Both, MsgType_Chat, curmodule, true, "Round has started!");
    #endif
    
    #if defined EVENT_MANAGER
        // Switching event forward.
        EventMgr_SetEventForward(curmodule, g_EvRoundStart, "TestModule_OnRoundStart2");
    #endif
}

/**
 * Round has started. (Alternate)
 */
public TestModule_OnRoundStart2(dcell, Float:dfloat, any:darray[], String:dstring[])
{
    new Module:curmodule = EventMgr_GetEventOwner();
    
    #if defined LOG_MANAGER
        LogMgr_Print(curmodule, LogType_Normal, "Round_Start", "Round start!");
    #endif
    
    #if defined EVENT_MANAGER
        // Toggle player_hurt event
        if (EventMgr_IsEventActive(curmodule, g_EvPlayerHurt))
        {
            EventMgr_Disable(curmodule, g_EvPlayerHurt);
            PrintToServer("Disabled event player_hurt");
        }
        else
        {
            EventMgr_Enable(curmodule, g_EvPlayerHurt);
            PrintToServer("Enabled event player_hurt");
        }
    #endif
}

#endif

#endif
