/*
 * ============================================================================
 *
 *  Project
 *
 *  File:          testmodule.inc
 *  Type:          Base
 *  Description:   Test module designed with no other purpose but to test the project base.
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

/**
 * A global variable to store this module's ID in.
 */
new g_iTestModuleID;

/**
 * Plugin is loading.
 * Register TestModule.
 */
public TestModule_OnPluginStart()
{
    // Define all the module's data as layed out by enum ModuleData in project.inc.
    new moduledata[ModuleData];
    moduledata[ModuleData_Disabled] = false;
    strcopy(moduledata[ModuleData_FullName], sizeof(moduledata[ModuleData_FullName]), "Test Module");
    strcopy(moduledata[ModuleData_ShortName], sizeof(moduledata[ModuleData_ShortName]), "testmodule");
    
    // Register the module with this data.
    g_iTestModuleID = ModuleManager_Register(moduledata);
    
    // Now register the events we're going to use.
    EventManager_RegisterEvent(g_iTestModuleID, Event_RoundStart, "TestModule_OnRoundStart");
}

/**
 * Round has started.
 */
public TestModule_OnRoundStart()
{
    decl String:fullname[32];
    ModuleManager_ReadString(g_iTestModuleID, ModuleData_FullName, fullname, sizeof(fullname));
    PrintToServer("[%s] Round started!", fullname);
}