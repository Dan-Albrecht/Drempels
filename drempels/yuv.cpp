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

#include "yuv.h"

bool CopyRGBSurfaceToYUVSurfaceMMX(
        LPDDSURFACEDESC pddsd1,
        LPDDSURFACEDESC pddsd2,
		fourcc_enum eOverlayFormat)
{
    if (pddsd1->dwWidth != pddsd2->dwWidth) 
        return false;
    if (pddsd1->dwHeight != pddsd2->dwHeight) 
        return false;
    
    DWORD  w = pddsd1->dwWidth;
    DWORD  h = pddsd1->dwHeight;
    LONG   pitch1 = pddsd1->lPitch;
    LONG   pitch2 = pddsd2->lPitch;
    unsigned __int32 *pPixels1 = (unsigned __int32 *)pddsd1->lpSurface;
    unsigned __int32 *pPixels2 = (unsigned __int32 *)pddsd2->lpSurface;
	signed __int16 cm1[4];
	signed __int16 cm2[4];
	signed __int16 cm3[4];
	signed __int16 cm4[4];
	int loops_per_scanline = w/2;
	int extra_bytes_per_scanline_src  = pitch1 - w*4;
	int extra_bytes_per_scanline_dest = pitch2 - w*2;

    if (eOverlayFormat == UYVY) // U Y V Y
    {
		// swap 0<->1, and 2<->3
		cm1[1] = 77/2;  cm1[0] = -38/2;   cm1[3] = 77/2;  cm1[2] = 110/2;
		cm2[1] = 150/2; cm2[0] = -74/2;   cm2[3] = 150/2; cm2[2] = -92/2;
		cm3[1] = 29/2;  cm3[0] = 112/2;   cm3[3] = 29/2;  cm3[2] = -18/2;
		cm4[1] = 0;     cm4[0] = 32768/2; cm4[3] = 0;     cm4[2] = 32768/2;
    }
    else    // Y U Y 2
    {
		// (laptop)
		cm1[0] = 77/2;  cm1[1] = -38/2;   cm1[2] = 77/2;  cm1[3] = 110/2;
		cm2[0] = 150/2; cm2[1] = -74/2;   cm2[2] = 150/2; cm2[3] = -92/2;
		cm3[0] = 29/2;  cm3[1] = 112/2;   cm3[2] = 29/2;  cm3[3] = -18/2;
		cm4[0] = 0;     cm4[1] = 32768/2; cm4[2] = 0;     cm4[3] = 32768/2;
    }
    
	__asm
	{
		mov   edx, h

		mov   esi, pPixels1
		mov   edi, pPixels2
		sub   edi, 4			// pre-subtract
		movq  mm4, cm4
		movq  mm5, cm1
		movq  mm6, cm2
		movq  mm7, cm3

		ALIGN 8
		yuvscanlineloop:

			mov   ecx, loops_per_scanline

			ALIGN 8
			yuvloop:

                /*
                // prefetch
			    test     ecx, 0x000001ff
			    jnz      PROCESS_PIXEL_MMX32           // every 256th pixel do some prefetches

						 mov      ebx, 2*256                 // need to prefetch 256*6 bytes
                        ALIGN 8
					    LOAD_ESI_ARRAY_MMX32:
						 mov      eax, [ebx+esi]
						 mov      eax, [ebx+esi+32]
						 sub      ebx, 64
						 jnz      LOAD_ESI_ARRAY_MMX32

                ALIGN 8
				PROCESS_PIXEL_MMX32:
                */

				// read in 2 pixels
				movq mm0, qword ptr [esi]	// -- b1 g1 r1 -- b2 g2 r2
				movq mm1, qword ptr [esi]	// -- b1 g1 r1 -- b2 g2 r2
				movq mm2, qword ptr [esi]	// -- b1 g1 r1 -- b2 g2 r2

                // quick reference:
                // punpcklbw mm7, mm7     // abcdefgh -> eeffgghh
                // punpcklbw mm7, mm0     // abcdefgh -> 0e0f0g0h (if mm0 is zero)
				// packuswb mm7, mm7      // 0a0b0g0r -> abgrabgr ?

				// step 1: get to this state:
				//	mm0: r1   r1   r2   r2
				//	mm1: g1   g1   g2   g2
				//	mm2: b1   b1   b2   b2
				//  mm3: junk
				//  mm4: 0    32k  0    32k
				//  mm5: c1r1 c1r2 c2r1 c2r2
				//  mm6: c1g1 c1g2 c2g1 c2g2
				//  mm7: c1b1 c1b2 c2b1 c2b2

				// NOTE: the shifts of 8, 16, and 24 below are
				//       correct (vs. 0-8-16) but might be in
				//       backwards order!

				pslld mm0, 8
				pslld mm1, 16
				pslld mm2, 24
				psrld mm0, 24    // 00 00 00 r1 00 00 00 r2
				psrld mm1, 24    // 00 00 00 g1 00 00 00 g2
				psrld mm2, 24    // 00 00 00 b1 00 00 00 b2
				
                movq  mm3, mm0
				pslld mm0, 16
				por   mm0, mm3   // 00 r1 00 r1 00 r2 00 r2

				movq  mm3, mm1
				pslld mm1, 16
				por   mm1, mm3   // 00 g1 00 g1 00 g2 00 g2

				movq  mm3, mm2
				pslld mm2, 16
				por   mm2, mm3   // 00 b1 00 b1 00 b2 00 b2

				// step 2: multiply to get to this state:
				//	mm0: r1*c1r1  r1*c1r2  r2*c2r1  r2*c2r2
				//	mm1: g1*c1g1  g1*c1g2  g2*c2g1  g2*c2g2
				//	mm2: b1*c1b1  b1*c1b2  b2*c2b1  b2*c2b2
				//  mm4: 0    32k  0    32k
				pmullw mm0, mm5
				 add  edi, 4
				pmullw mm1, mm6
				 add  esi, 8
				pmullw mm2, mm7
				
				// step 3: add to get to this state:
				//	mm0: d1*256   d2*256   d3*256   d4*256
				paddsw mm0, mm4
				paddsw mm0, mm1
				paddsw mm0, mm2

				psrlw    mm0, 7
				packuswb mm0, mm0    // bytes: abgrabgr
				movd     dword ptr [edi], mm0  // store 
				
				loop yuvloop

			// scanline complete
			add esi, extra_bytes_per_scanline_src
			add edi, extra_bytes_per_scanline_dest

			dec edx
			jnz yuvscanlineloop

		emms
	}
   
    return true;
}

/*
oops - only works w/YUV overlays!!!

bool CopyRGBSurfaceToYUVSurfaceMMXAndSaturate(
        LPDDSURFACEDESC pddsd1,
        LPDDSURFACEDESC pddsd2,
		fourcc_enum eOverlayFormat,
		int saturation)
{
	// saturation: 0 = none, 255 = full
	if (saturation==0)
		return CopyRGBSurfaceToYUVSurfaceMMX(pddsd1, pddsd2, eOverlayFormat);
    
	if (pddsd1->dwWidth != pddsd2->dwWidth) 
        return false;
    if (pddsd1->dwHeight != pddsd2->dwHeight) 
        return false;
    
    DWORD  w = pddsd1->dwWidth;
    DWORD  h = pddsd1->dwHeight;
    LONG   pitch1 = pddsd1->lPitch;
    LONG   pitch2 = pddsd2->lPitch;
    unsigned __int32 *pPixels1 = (unsigned __int32 *)pddsd1->lpSurface;
    unsigned __int32 *pPixels2 = (unsigned __int32 *)pddsd2->lpSurface;
	signed __int16 cm1[4];
	signed __int16 cm2[4];
	signed __int16 cm3[4];
	signed __int16 cm4[4];
	int loops_per_scanline = w/2;
	int extra_bytes_per_scanline_src  = pitch1 - w*4;
	int extra_bytes_per_scanline_dest = pitch2 - w*2;
	unsigned __int16 m_sat[4] = { saturation, saturation, saturation, saturation };

    if (eOverlayFormat == UYVY) // U Y V Y
    {
		// swap 0<->1, and 2<->3
		cm1[1] = 77/2;  cm1[0] = -38/2;   cm1[3] = 77/2;  cm1[2] = 110/2;
		cm2[1] = 150/2; cm2[0] = -74/2;   cm2[3] = 150/2; cm2[2] = -92/2;
		cm3[1] = 29/2;  cm3[0] = 112/2;   cm3[3] = 29/2;  cm3[2] = -18/2;
		cm4[1] = 0;     cm4[0] = 32768/2; cm4[3] = 0;     cm4[2] = 32768/2;
    }
    else    // Y U Y 2
    {
		// (laptop)
		cm1[0] = 77/2;  cm1[1] = -38/2;   cm1[2] = 77/2;  cm1[3] = 110/2;
		cm2[0] = 150/2; cm2[1] = -74/2;   cm2[2] = 150/2; cm2[3] = -92/2;
		cm3[0] = 29/2;  cm3[1] = 112/2;   cm3[2] = 29/2;  cm3[3] = -18/2;
		cm4[0] = 0;     cm4[1] = 32768/2; cm4[2] = 0;     cm4[3] = 32768/2;
    }
    
	__asm
	{
		mov   edx, h

		mov   esi, pPixels1
		mov   edi, pPixels2
		sub   edi, 4			// pre-subtract
		movq  mm4, cm4
		movq  mm5, cm1
		movq  mm6, cm2
		movq  mm7, cm3

		yuvscanlineloop:

			mov   ecx, loops_per_scanline

			yuvloop:

				// read in 2 pixels
				movq mm0, qword ptr [esi]	// -- b1 g1 r1 -- b2 g2 r2
				//movq mm1, qword ptr [esi]	// -- b1 g1 r1 -- b2 g2 r2
				//movq mm2, qword ptr [esi]	// -- b1 g1 r1 -- b2 g2 r2

				movq mm1, qword ptr [esi]
				pmullw mm0, m_sat
				paddusw mm0, mm1
				movq mm1, mm0
				movq mm2, mm0

				//paddusb mm0, mm0
				//paddusb mm1, mm1
				//paddusb mm2, mm2

				// step 1: get to this state:
				//	mm0: r1   r1   r2   r2
				//	mm1: g1   g1   g2   g2
				//	mm2: b1   b1   b2   b2
				//  mm3: junk
				//  mm4: 0    32k  0    32k
				//  mm5: c1r1 c1r2 c2r1 c2r2
				//  mm6: c1g1 c1g2 c2g1 c2g2
				//  mm7: c1b1 c1b2 c2b1 c2b2

				// NOTE: the shifts of 8, 16, and 24 below are
				//       correct (vs. 0-8-16) but might be in
				//       backwards order!
				pslld mm0, 8
				psrld mm0, 24
				movq  mm3, mm0
				pslld mm0, 16
				por   mm0, mm3   // 00 r1 00 r1 00 r2 00 r2

				pslld mm1, 16
				psrld mm1, 24
				 add  edi, 4
				movq  mm3, mm1
				pslld mm1, 16
				por   mm1, mm3   // 00 g1 00 g1 00 g2 00 g2

				pslld mm2, 24
				psrld mm2, 24
				 add  esi, 8
				movq  mm3, mm2
				pslld mm2, 16
				por   mm2, mm3   // 00 b1 00 b1 00 b2 00 b2

				// step 2: multiply to get to this state:
				//	mm0: r1*c1r1  r1*c1r2  r2*c2r1  r2*c2r2
				//	mm1: g1*c1g1  g1*c1g2  g2*c2g1  g2*c2g2
				//	mm2: b1*c1b1  b1*c1b2  b2*c2b1  b2*c2b2
				//  mm4: 0    32k  0    32k
				pmullw mm0, mm5
				pmullw mm1, mm6
				pmullw mm2, mm7
				
				// step 3: add to get to this state:
				//	mm0: d1*256   d2*256   d3*256   d4*256
				paddsw mm0, mm4
				paddsw mm0, mm1
				paddsw mm0, mm2

				psrlw    mm0, 7
				packuswb mm0, mm0    // bytes: abgrabgr
				movd     dword ptr [edi], mm0  // store 
				
				loop yuvloop

			// scanline complete
			add esi, extra_bytes_per_scanline_src
			add edi, extra_bytes_per_scanline_dest

			dec edx
			jnz yuvscanlineloop

		emms
	}
   
    return true;
}
*/


bool CopyRGBSurfaceToYUVSurface(
        LPDDSURFACEDESC pddsd1,
        LPDDSURFACEDESC pddsd2,
		fourcc_enum eOverlayFormat)
{
    if (pddsd1->dwWidth != pddsd2->dwWidth) 
        return false;
    if (pddsd1->dwHeight != pddsd2->dwHeight) 
        return false;
    
    DWORD  w = pddsd1->dwWidth;
    DWORD  h = pddsd1->dwHeight;
    LONG   pitch1 = pddsd1->lPitch;
    LONG   pitch2 = pddsd2->lPitch;
    unsigned __int32 *pPixels1 = (unsigned __int32 *)pddsd1->lpSurface;
    unsigned __int32 *pPixels2 = (unsigned __int32 *)pddsd2->lpSurface;
    unsigned __int32 color1;
    LONG   offset1 = 0;
    LONG   offset2 = 0;
    unsigned int    R, G, B, i1, i2, i3, i4;
    BYTE            yuv[4];

    if (eOverlayFormat == UYVY) // U Y V Y
    {
        i1 = 1;
        i2 = 0;
        i3 = 3;
        i4 = 2;
    }
    else    // Y U Y 2
    {
		// (laptop)
        i1 = 0;
        i2 = 1;
        i3 = 2;
        i4 = 3;
    }
    
    // Go through the image 2 pixels at a time and convert to YUV
    for (unsigned int y=0; y<h; y++)
    {
        offset1 = y*pitch1/4;
        offset2 = y*pitch2/4;

		for (unsigned int x=0; x<w; x+=2)
        {
            color1 = pPixels1[offset1++];
            B = (color1) & 0xFF;
            G = (color1 >> 8) & 0xFF;
            R = (color1 >> 16) & 0xFF;
			//G = R; B = R;
            //yuv[i1] = (BYTE)(0.299*R + 0.587*G + 0.114*B);
            //yuv[i2] = (BYTE)(128.0 - 0.147*R - 0.289*G + 0.436*B);
            yuv[i1] = (77*R + 150*G + 29*B) >> 8;
            yuv[i2] = (32768 - 38*R - 74*G + 112*B) >> 8;
            
            color1 = pPixels1[offset1++];
            B = (color1) & 0xFF;
            G = (color1 >> 8) & 0xFF;
            R = (color1 >> 16) & 0xFF;
			//G = R; B = R;
            //yuv[i3] = (BYTE)(0.299*R + 0.587*G + 0.114*B);
            //yuv[i4] = (BYTE)(128.0 + 0.36*R - 0.29*G - 0.07*B);
            //yuv[i4] = (BYTE)(128.0 + 0.43*R - 0.36*G - 0.07*B);
            yuv[i3] = (77*R + 150*G + 29*B) >> 8;
            yuv[i4] = (32768 + 110*R - 92*G - 18*B) >> 8;

            pPixels2[offset2++] = *((unsigned __int32 *)yuv);
        }
    }    

    return true;
}
