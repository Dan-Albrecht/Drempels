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

#ifndef GEISS_BLITS_H
#define GEISS_BLITS_H 1

#include "ddraw.h"

typedef enum fourcc_enum {
    RGB_OVERLAY,    // using RGB overlay (use Blt)
    UYVY,           // YUV mode #1 (manual blit)
    YUY2,           // YUV mode #2 (manual blit)
	MISC,			// no pixelformat was specified, but it worked (?!)
    NATIVE          // card's own fourcc code (use Blt & cross fingers)
};

bool CopyRGBSurfaceToYUVSurfaceMMX(
        LPDDSURFACEDESC pddsd1,
        LPDDSURFACEDESC pddsd2,
		fourcc_enum eOverlayFormat);

bool CopyRGBSurfaceToYUVSurface(
        LPDDSURFACEDESC pddsd1,
        LPDDSURFACEDESC pddsd2,
		fourcc_enum eOverlayFormat);


#endif
