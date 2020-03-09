/* 
 *  This source file is part of Drempels, a program that falls
 *  under the Gnu Public License (GPL) open-source license.  
 *  Use and distribution of this code is regulated by law under 
 *  this license.  For license details, visit:
 *    http://www.gnu.org/copyleft/gpl.html
 * 
 *  The Drempels open-source project is accessible at 
 *  sourceforge.net; the direct URL is:
 *    http://sourceforge.net/projects/drempels/
 *  
 *  Drempels was originally created by Ryan M. Geiss in 2001
 *  and was open-sourced in February 2005.  The original
 *  Drempels homepage is available here:
 *    http://www.geisswerks.com/drempels/
 *
 */

#ifndef GEISS_SYSTEM_STUFF
#define GEISS_SYSTEM_STUFF 1

#include <windows.h>
#include <math.h>
 
// This function evaluates whether the floating-point
// control Word is set to single precision/round to nearest/
// exceptions disabled. If not, the
// function changes the control Word to set them and returns
// TRUE, putting the old control Word value in the passback
// location pointed to by pwOldCW.
BOOL MungeFPCW( WORD *pwOldCW )
{
    BOOL ret = FALSE;
    WORD wTemp, wSave;
 
    __asm fstcw wSave
    if (wSave & 0x300 ||            // Not single mode
        0x3f != (wSave & 0x3f) ||   // Exceptions enabled
        wSave & 0xC00)              // Not round to nearest mode
    {
        __asm
        {
            mov ax, wSave
            and ax, not 300h    ;; single mode
            or  ax, 3fh         ;; disable all exceptions
            and ax, not 0xC00   ;; round to nearest mode
            mov wTemp, ax
            fldcw   wTemp
        }
        ret = TRUE;
    }
    *pwOldCW = wSave;
    return ret;
}
 

void RestoreFPCW(WORD wSave)
{
    __asm fldcw wSave
}
 

bool CheckMMXTechnology()
{
    bool retval = TRUE;
    DWORD RegEDX;

    __try {
        __asm {
            mov eax, 1
            cpuid
            mov RegEDX, edx
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        retval = FALSE;
    }

    if (retval == FALSE) return FALSE;  // processor does not support CPUID

    if (RegEDX & 0x800000)          // bit 23 is set for MMX technology
    {
       __try { __asm emms }          // try executing the MMX instruction "emms"
       __except(EXCEPTION_EXECUTE_HANDLER) { retval = FALSE; }
    }

    else
            return FALSE;           // processor supports CPUID but does not support MMX technology

    // if retval == 0 here, it means the processor has MMX technology but
    // floating-point emulation is on; so MMX technology is unavailable

    return retval;
}

// --------------------------------------------------------------------------

void ReadConfigRegistry()
{ 
	LONG res; 
	HKEY skey; 
	DWORD valtype, valsize, val;
	
	res=RegOpenKeyEx(HKEY_CURRENT_USER,APPREGPATH,0,KEY_ALL_ACCESS,&skey);
	if (res!=ERROR_SUCCESS) return;
	
	
	valsize=sizeof(val); 
		
	// ------------ read VIDEO_CARD_555, FXW, FXH, and iDispBits --------------
	
	res=RegQueryValueEx(skey,"Z_16bit555",0,&valtype,(LPBYTE)&val,&valsize);	 
	if (res==ERROR_SUCCESS) VIDEO_CARD_555=val; else dumpmsg("error reading reg: Z_16bit555");// { ScrMode = smConfig; g_bFirstRun = true; }
	res=RegQueryValueEx(skey,"Z_FXW",0,&valtype,(LPBYTE)&val,&valsize);	
	if (res==ERROR_SUCCESS) FXW=val; else dumpmsg("error reading reg: Z_FXW");// { ScrMode = smConfig; g_bFirstRun = true; }
	res=RegQueryValueEx(skey,"Z_FXH",0,&valtype,(LPBYTE)&val,&valsize);	
	if (res==ERROR_SUCCESS) FXH=val; else dumpmsg("error reading reg: Z_FXH");// { ScrMode = smConfig; g_bFirstRun = true; }
	res=RegQueryValueEx(skey,"Z_BitDepth",0,&valtype,(LPBYTE)&val,&valsize);	 
	if (res==ERROR_SUCCESS) iDispBits=val; else dumpmsg("error reading reg: Z_BitDepth");// { ScrMode = smConfig; g_bFirstRun = true; }
	
	res=RegQueryValueEx(skey,"bDebugMode",0,&valtype,(LPBYTE)&val,&valsize);	 
	if (res==ERROR_SUCCESS) g_bDebugMode=val; else dumpmsg("error reading reg: bDebugMode");// { ScrMode = smConfig; g_bFirstRun = true; }
	res=RegQueryValueEx(skey,"bSuppressHelpMsg",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) g_bSuppressHelpMsg=val; else dumpmsg("error reading reg: bSuppressHelpMsg");// { ScrMode = smConfig; g_bFirstRun = true; }
	res=RegQueryValueEx(skey,"bSuppressAllMsg",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) g_bSuppressAllMsg=val; else dumpmsg("error reading reg: bSuppressAllMsg");// { ScrMode = smConfig; g_bFirstRun = true; }
	
	if (!g_bFirstRun)
	{
		// volatile keys here - those who might exist from a previous version,
		//   but the meaning of their values has changed, so we want to force
		//   the value to take the default!
		res=RegQueryValueEx(skey,"anim_speed",0,&valtype,(LPBYTE)&val,&valsize);	 
		if (res==ERROR_SUCCESS) anim_speed=val*0.001f; else dumpmsg("error reading reg: anim_speed");
	}
	res=RegQueryValueEx(skey,"time_between_textures",0,&valtype,(LPBYTE)&val,&valsize);	
	if (res==ERROR_SUCCESS) time_between_textures=val*0.001f; else dumpmsg("error reading reg: time_between_textures");
	res=RegQueryValueEx(skey,"texture_fade_time",0,&valtype,(LPBYTE)&val,&valsize);	
	if (res==ERROR_SUCCESS) texture_fade_time=val*0.001f; else dumpmsg("error reading reg: texture_fade_time");
	res=RegQueryValueEx(skey,"master_zoom",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) master_zoom=val*0.001f; else dumpmsg("error reading reg: master_zoom");
	res=RegQueryValueEx(skey,"mode_switch_speed_multiplier",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) mode_switch_speed_multiplier=val*0.001f; else dumpmsg("error reading reg: mode_switch_speed_multiplier");
	res=RegQueryValueEx(skey,"motion_blur",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) motion_blur=val; else dumpmsg("error reading reg: motion_blur");
	res=RegQueryValueEx(skey,"limit_fps",0,&valtype,(LPBYTE)&val,&valsize);	
	if (res==ERROR_SUCCESS) limit_fps=val; else dumpmsg("error reading reg: limit_fps");
	res=RegQueryValueEx(skey,"time_between_subdirs",0,&valtype,(LPBYTE)&val,&valsize);	
	if (res==ERROR_SUCCESS) g_nTimeBetweenSubdirs=val; else dumpmsg("error reading reg: time_between_subdirs");

	res=RegQueryValueEx(skey,"launch_at_startup",0,&valtype,(LPBYTE)&val,&valsize);	 
	if (res==ERROR_SUCCESS) g_bRunDrempelsAtStartup=val; else dumpmsg("error reading reg: launch_at_startup");
	res=RegQueryValueEx(skey,"exit_on_mouse_move",0,&valtype,(LPBYTE)&val,&valsize);	 
	if (res==ERROR_SUCCESS) g_bExitOnMouseMove=val; else dumpmsg("error reading reg: exit_on_mouse_move");
	res=RegQueryValueEx(skey,"high_quality",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) high_quality=val; else dumpmsg("error reading reg: high_quality");
	res=RegQueryValueEx(skey,"rendermode_as_saver",0,&valtype,(LPBYTE)&val,&valsize);	 
	if (res==ERROR_SUCCESS) rendermode_as_saver=val; else dumpmsg("error reading reg: rendermode");
	res=RegQueryValueEx(skey,"rendermode_as_app",0,&valtype,(LPBYTE)&val,&valsize);	 
	if (res==ERROR_SUCCESS) rendermode_as_app=val; else dumpmsg("error reading reg: rendermode");
	res=RegQueryValueEx(skey,"procsize_as_saver",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) procsize_as_saver=val; else dumpmsg("error reading reg: procsize");
	res=RegQueryValueEx(skey,"procsize_as_app",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) procsize_as_app=val; else dumpmsg("error reading reg: procsize");
	//res=RegQueryValueEx(skey,"hide_ctrl_window",0,&valtype,(LPBYTE)&val,&valsize);   
	//if (res==ERROR_SUCCESS) g_bHideCtrlWindow=val; else dumpmsg("error reading reg: g_bHideCtrlWindow");

	res=RegQueryValueEx(skey,"colorkey_r",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) key_R=val; else dumpmsg("error reading reg: colorkey_r");
	res=RegQueryValueEx(skey,"colorkey_g",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) key_G=val; else dumpmsg("error reading reg: colorkey_g");
	res=RegQueryValueEx(skey,"colorkey_b",0,&valtype,(LPBYTE)&val,&valsize);   
	if (res==ERROR_SUCCESS) key_B=val; else dumpmsg("error reading reg: colorkey_b");



	if (g_bIsRunningAsSaver)
	{
		procsize   = procsize_as_saver;
		rendermode = rendermode_as_saver;
	}
	else
	{
		procsize   = procsize_as_app;
		rendermode = rendermode_as_app;
	}

	switch(procsize)
	{
	case 0:
		FXW2 = FXW;
		FXH2 = FXH;
		break;
	case 1:
		FXW2 = FXW/2;
		FXH2 = FXH/2;
		break;
	case 2:
		FXW2 = FXW/4;
		FXH2 = FXH/4;
		break;
	default:
		procsize = 0;
		FXW2 = FXW;
		FXH2 = FXH;
		break;
	}
	
	char szTempTexPath[MAX_PATH];
	long iLen = MAX_PATH;
	res=RegQueryValue(skey, "szTexPath", szTempTexPath, &iLen);
	if (res==ERROR_SUCCESS) strcpy(szTexPath, szTempTexPath); else dumpmsg("error reading reg: szTexPath");
    strcpy(szTexPathCurrent, szTexPath);

    g_fTimeTilSubdirSwitch = (float)g_nTimeBetweenSubdirs;
	
	if (ScrMode==smSaver && (FXW<320 || FXW%4 != 0 || FXH<200 || FXH%4 != 0 || iDispBits%8!=0))
		dumpmsg("invalid reg params: width<320, height<200, or iDispBits%8 or FXW%4 or FXH%4 nonzero.");// { ScrMode = smConfig; g_bFirstRun = true; }
	
	
	RegCloseKey(skey);
}  

// --------------------------------------------------------------------------

void WriteConfigRegistry()
{ 
	LONG res; 
	HKEY skey; 
	DWORD val, disp;
	
	
	res=RegCreateKeyEx(HKEY_CURRENT_USER,APPREGPATH,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&skey,&disp);
	if (res!=ERROR_SUCCESS) return;
	
	val=CURRENT_VERSION;
	RegSetValueEx(skey,"Version",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	
	val=VIDEO_CARD_555;
	RegSetValueEx(skey,"Z_16bit555",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val=FXW;
	RegSetValueEx(skey,"Z_FXW",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val=FXH;
	RegSetValueEx(skey,"Z_FXH",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val=iDispBits;
	RegSetValueEx(skey,"Z_BitDepth",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	
	val=g_bDebugMode;
	RegSetValueEx(skey,"bDebugMode",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val=g_bSuppressHelpMsg;
	RegSetValueEx(skey,"bSuppressHelpMsg",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val=g_bSuppressAllMsg;
	RegSetValueEx(skey,"bSuppressAllMsg",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	
	val = time_between_textures*1000;
	RegSetValueEx(skey,"time_between_textures",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = texture_fade_time*1000;
	RegSetValueEx(skey,"texture_fade_time",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = master_zoom*1000;
	RegSetValueEx(skey,"master_zoom",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = mode_switch_speed_multiplier*1000;
	RegSetValueEx(skey,"mode_switch_speed_multiplier",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = anim_speed*1000;
	RegSetValueEx(skey,"anim_speed",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = motion_blur;
	RegSetValueEx(skey,"motion_blur",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = limit_fps;
	RegSetValueEx(skey,"limit_fps",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = g_nTimeBetweenSubdirs;
	RegSetValueEx(skey,"time_between_subdirs",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	
	val = g_bRunDrempelsAtStartup;
	RegSetValueEx(skey,"launch_at_startup",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = g_bExitOnMouseMove;
	RegSetValueEx(skey,"exit_on_mouse_move",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = high_quality;
	RegSetValueEx(skey,"high_quality",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = rendermode_as_saver;
	RegSetValueEx(skey,"rendermode_as_saver",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = rendermode_as_app;
	RegSetValueEx(skey,"rendermode_as_app",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = procsize_as_saver;
	RegSetValueEx(skey,"procsize_as_saver",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = procsize_as_app;
	RegSetValueEx(skey,"procsize_as_app",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	//val = g_bHideCtrlWindow;
	//RegSetValueEx(skey,"hide_ctrl_window",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	
	val = key_R;
	RegSetValueEx(skey,"colorkey_r",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = key_G;
	RegSetValueEx(skey,"colorkey_g",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));
	val = key_B;
	RegSetValueEx(skey,"colorkey_b",0,REG_DWORD,(CONST BYTE*)&val,sizeof(val));

	RegSetValue(skey, "szTexPath", REG_SZ, szTexPath, strlen(szTexPath));
	
	RegCloseKey(skey);
}






#endif