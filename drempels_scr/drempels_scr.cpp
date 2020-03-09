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

#include "stdafx.h"
#include "stdlib.h"
#include "shellapi.h"

enum TScrMode {smNone,smConfig,smPassword,smPreview,smSaver,smApp};
TScrMode ScrMode=smNone;

#define NAME "Drempels"
#define TITLE "Drempels"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// don't allow 2 copies to run at once (can happen w/screensavers)
	if (FindWindow(NAME, TITLE))
	{
		return 1;
	}
			
	char *c=GetCommandLine();
	
	ScrMode = smNone;
	
	if (*c=='\"') {c++; while (*c!=0 && *c!='\"') c++;} else {while (*c!=0 && *c!=' ') c++;}
	if (*c!=0) c++;
	while (*c==' ') c++;
	if (*c==0) {ScrMode=smApp; }
	else
	{ 
		if (*c=='-' || *c=='/') c++;
		if (*c=='p' || *c=='P' || *c=='l' || *c=='L')
		{ 
			c++; while (*c==' ' || *c==':') c++;
			ScrMode=smPreview;
		}
		else if (*c=='s' || *c=='S') 
        {
            ScrMode=smSaver; 
        }
		else if (*c=='c' || *c=='C') 
        {
            c++; 
            
            while (*c==' ' || *c==':') c++; 
                        
            ScrMode=smConfig;
        }
		else if (*c=='a' || *c=='A') 
        {
            c++; 
            while (*c==' ' || *c==':') c++; 
            
            ScrMode=smPassword;
        }
	}
	
	char szWinDir[512] = "";
	GetWindowsDirectory(szWinDir, 511);

	char szFilename[512] = "drempels.exe";

	char szFilenameWithPath[512];
	strcpy(szFilenameWithPath, szWinDir);
	if (strlen(szFilenameWithPath) > 0 &&
		szFilenameWithPath[strlen(szFilenameWithPath)-1] != '\\')
	{
		strcat(szFilenameWithPath, "\\");
	}
	strcat(szFilenameWithPath, szFilename);

	if (ScrMode == smConfig || ScrMode == smSaver)
	{
		if (::GetFileAttributes(szFilenameWithPath) == -1)
		{
			char buf[1024];
			wsprintf(buf, "Could not find the following required file:\r\n\r\n     %s\r\n\r\nDrempels can not run without this file; you should reinstall Drempels.", szFilenameWithPath);
			MessageBox(NULL, buf, "MISSING DREMPELS.EXE", MB_OK);
		}
	}

	switch(ScrMode)
	{
	case smConfig:
		ShellExecute(NULL, "open", "drempels.exe", "/c", szWinDir, SW_SHOWNORMAL);
		break;
	case smSaver:
		ShellExecute(NULL, "open", "drempels.exe", "/s", szWinDir, SW_SHOWNORMAL);
		break;
	default:
		exit(0);
		break;
	}	

	return 0;
}



