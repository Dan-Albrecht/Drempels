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

#ifndef GEISS_VIDEO
#define GEISS_VIDEO 1

#define y_inc 18

//#define SAT 1

bool TryLockSurface(LPDIRECTDRAWSURFACE lpDDsurf, LPDDSURFACEDESC pDDSD)
{
    HRESULT hr;

    ZeroMemory(pDDSD, sizeof(DDSURFACEDESC));
    pDDSD->dwSize = sizeof(DDSURFACEDESC);
    
    hr = lpDDsurf->Lock(NULL, pDDSD, DDLOCK_WAIT, NULL);
    if (hr != DD_OK)
    {
        if (hr == DDERR_SURFACELOST)
        {
            hr = lpDDsurf->Restore();
            if (hr != DD_OK)
            {
                dumpmsg("trylock: could not restore lost surface!");
                return false;
            }
            hr = lpDDsurf->Lock(NULL, pDDSD, DDLOCK_WAIT, NULL);
            if (hr != DD_OK)
            {
                dumpmsg("trylock: could not lock restored surface!");
                return false;
            }
        }
        else
        {
            dumpmsg("trylock: could not lock surface; error code:");
            if (hr==DDERR_INVALIDOBJECT) 
                dumpmsg("DDERR_INVALIDOBJECT  ");
            else if (hr==DDERR_INVALIDPARAMS) 
                dumpmsg("DDERR_INVALIDPARAMS  ");
            else if (hr==DDERR_OUTOFMEMORY) 
                dumpmsg("DDERR_OUTOFMEMORY  ");
            else if (hr==DDERR_SURFACEBUSY) 
                dumpmsg("DDERR_SURFACEBUSY  ");
            else if (hr==DDERR_SURFACELOST) 
                dumpmsg("DDERR_SURFACELOST  ");
            else if (hr==DDERR_WASSTILLDRAWING) 
                dumpmsg("DDERR_WASSTILLDRAWING");
            return false;
        }
    }

    return true;
}


/*
-problem here is that it doesn't support stretching!
-also, now src AND dest can have width != pitch
-will always be going from 32 bits to 16, 24, or 32 bits
-need the following cases:
    -32 -> 16 1:1
    -32 -> 16 1:2*
    -32 -> 16 1:4*
    -32 -> 24 1:1
    -32 -> 24 1:2*
    -32 -> 24 1:4*
    -32 -> 32 1:1
    -32 -> 32 1:2*
    -32 -> 32 1:4*
  * means new code will have to be written
*/

bool Merge_All_VS_To_Backbuffer(
        LPDIRECTDRAWSURFACE lpDDSsrc,
        LPDIRECTDRAWSURFACE lpDDSdest)
{
	// requires:
	//
	//	 -lpDDSsrc is 32 bit ARGB surface
	//	 -lpDDSdest can be any bit depth, but it size must be
	//		the same, double, or quadruple that of lpDDSsrc

	DDSURFACEDESC ddsd_src;
	ZeroMemory(&ddsd_src, sizeof(ddsd_src));
	ddsd_src.dwSize = sizeof(ddsd_src);
	
	DDSURFACEDESC ddsd_dest;
	ZeroMemory(&ddsd_dest, sizeof(ddsd_dest));
	ddsd_dest.dwSize = sizeof(ddsd_dest);

    if (!TryLockSurface(lpDDSsrc, &ddsd_src))
    {
        dumpmsg("merge_all_vs: failed to lock lpDDSsrc");
        return false;
    }
    
    if (!TryLockSurface(lpDDSdest, &ddsd_dest))
    {
        lpDDSsrc->Unlock(NULL);
        dumpmsg("merge_all_vs: failed to lock lpDDSdest");
        return false;
    }

	//stuff
	unsigned char *src  = (unsigned char *)ddsd_src.lpSurface;
	unsigned char *dest = (unsigned char *)ddsd_dest.lpSurface;
	WORD *worddest      = (WORD *)ddsd_dest.lpSurface;
	DWORD *dworddest    = (DWORD *)ddsd_dest.lpSurface;

    unsigned __int16 FXW_DIV_2 = ddsd_src.dwWidth/2;
    unsigned __int16 FXW_DIV_4 = ddsd_src.dwWidth/4;
    unsigned __int16 FXW_DIV_8 = ddsd_src.dwWidth/8;
    unsigned __int16 FXW_DIV_16 = ddsd_src.dwWidth/16;
    
	int extra_bytes_per_scanline_s = ddsd_src.lPitch - ddsd_src.dwWidth*4;
	int extra_bytes_per_scanline_d = ddsd_dest.lPitch - ddsd_dest.dwWidth*(iDispBits/8);
	int bytes_per_scanline_d = ddsd_dest.lPitch;

	if (intframe == 1)
	{
		char buf[64];
		sprintf(buf, "32 bits -> %d bits: %d %d %d / %d %d", 
			iDispBits,
			extra_bytes_per_scanline_s,
			extra_bytes_per_scanline_d,
			bytes_per_scanline_d,
			ddsd_src.dwWidth,
			ddsd_dest.dwWidth);
		dumpmsg(buf);
	}

	// we also do 1:1 case for windowed mode, since 
	// lpDDSBack (the destination) is the same (small)
	// size as the VS's...
    //if (procsize == 0 || (rendermode==1 || rendermode==2))
	if (ddsd_src.dwWidth == ddsd_dest.dwWidth)
    {
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //                 1:1 (NO STRETCH)
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        if (iDispBits==16)
	    {
		    if (VIDEO_CARD_555==1)
		    __asm
		    {
			    mov        edi, worddest
                mov        eax, src
			    
			    // [edi]           is start pt. on back surface
			    // [eax]           points to VS1
			    // ecx is the loop counter
                // ebx holds linesize_over_FXW (times 4)

			    mov        ecx, FXH2

			    Scanline_Loop16b:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_2

				    PixelChunk_Loop16b:

                        MOVQ       mm0, [eax]     // read 64 bits [RGBxRGBx]

                        MOVQ       mm1, mm0
                        MOVQ       mm2, mm0       //|????????rrrrrrrr|ggggggggbbbbbbbb|????????bbbbbbbb|ggggggggrrrrrrrr|

                        PSRLD      mm0, 16+3      //|0000000000000000|00000000000rrrrr|0000000000000000|00000000000rrrrr|
                        PSRLD      mm1,  8+3      //|0000000000000000|000rrrrrrrrggggg|0000000000000000|0000000000gggggg|
                        PSRLD      mm2,    3      //|00000000000rrrrr|rrrggggggggbbbbb|0000000000000000|00000000000bbbbb|
                        PSLLD      mm0, 32-5      //|rrrrr00000000000|0000000000000000|0000000000000000|00000000000rrrrr|
                        PSLLD      mm1, 32-5      //|ggggg00000000000|0000000000000000|0000000000000000|0000000000gggggg|
                        PSLLD      mm2, 32-5      //|bbbbb00000000000|0000000000000000|0000000000000000|00000000000bbbbb|
                        PSRLD      mm0, 1         //|0rrrrr0000000000|0000000000000000|0000000000000000|00000000000rrrrr|
                        PSRLD      mm1, 1+5       //|000000ggggg00000|0000000000000000|0000000000000000|0000000000gggggg|
                        PSRLD      mm2, 1+5+5     //|00000000000bbbbb|0000000000000000|0000000000000000|00000000000bbbbb|

                        POR        mm0, mm1
                        POR        mm0, mm2       //|0rrrrrgggggbbbbb|0000000000000000|rrrrrggggggbbbbb|0000000000000000|
                    
                        MOVQ       mm1, mm0
                        PSRLQ      mm0, 16
                        PSRLQ      mm1, 32 
                        POR        mm0, mm1       //|????????????????|????????????????|0rrrrrgggggbbbbb|0rrrrrgggggbbbbb|

                        MOVD       [edi], mm0     // puts lower 32 bits to memory (2 dest. pixels)
                
                        ADD        eax, 8
                        ADD        edi, 4

					    dec    cx
					    jnz    PixelChunk_Loop16b
				    
				    add   eax, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d         
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop16b

			    EMMS
		    }
            else
		    __asm
		    {
			    mov        edi, worddest
                mov        eax, src
			    
			    // [edi]           is start pt. on back surface
			    // [eax]           points to VS1
			    // ecx is the loop counter
                // ebx holds linesize_over_FXW (times 4)

			    mov        ecx, FXH2

			    Scanline_Loop16a:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_2

				    PixelChunk_Loop16a:

                        MOVQ       mm0, [eax]     // read 64 bits [RGBxRGBx]

                                                  //MM0 (high half)                      MM1 (high half)                      MM2 (high half)
                                                  //-----------------------------------  -----------------------------------  -----------------------------------
                                                  //|........rrrrrrrr|ggggggggbbbbbbbb|
                        MOVQ       mm1, mm0       //|........rrrrrrrr|ggggggggbbbbbbbb|  |........rrrrrrrr|ggggggggbbbbbbbb|
                        PSRLW      mm0, 3         //|000........rrrrr|000ggggggggbbbbb|  |........rrrrrrrr|ggggggggbbbbbbbb|
                        PSRLW      mm1, 10        //|000........rrrrr|000ggggggggbbbbb|  |0000000000......|0000000000gggggg|
                        PSLLW      mm0, 11        //|rrrrr00000000000|bbbbb00000000000|  |0000000000......|0000000000gggggg|
                        PSLLD      mm1, 21        //|rrrrr00000000000|bbbbb00000000000|  |00000gggggg00000|0000000000000000|
                        MOVQ       mm2, mm0       //|rrrrr00000000000|bbbbb00000000000|  |00000gggggg00000|0000000000000000|  |rrrrr00000000000|bbbbb00000000000|
                        POR        mm0, mm1       //|rrrrrgggggg00000|bbbbb00000000000|                                       |rrrrr00000000000|bbbbb00000000000|
                        PSLLD      mm2, 5         //|rrrrrgggggg00000|bbbbb00000000000|                                       |00000000000bbbbb|0000000000000000|
                        POR        mm0, mm2       //|rrrrrggggggbbbbb|bbbbb00000000000|rrrrrggggggbbbbb|bbbbb00000000000|

                        PSRLD      mm0, 16        //|0000000000000000|rrrrrggggggbbbbb|0000000000000000|rrrrrggggggbbbbb|
                        MOVQ       mm1, mm0
                        PSRLQ      mm1, 16
                        POR        mm0, mm1

                        MOVD       [edi], mm0     // puts lower 32 bits to memory (2 16-bit RGB quantities == 2 pixels)
                
                        ADD        eax, 8//16//8
                        ADD        edi, 4//8//4

					    dec    cx
					    jnz    PixelChunk_Loop16a
				    
				    add   eax, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop16a

			    EMMS
		    }
	    }
        else if (iDispBits==24)
        {
		    __asm
		    {
			    mov        edi, dworddest
                mov        eax, src
                mov        esi, eax
			    
			    // [esi]           points to VS1
			    // [edi]           is start pt. on back surface
			    // ecx is the loop counter

			    mov        ecx, FXH2

			    Scanline_Loop24:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_8

				    PixelChunk_Loop24:

                        mov  eax, dword ptr [esi]
                        mov  ebx, dword ptr [esi+4]
                        mov  edx, dword ptr [esi+8]
                    
                        // fill high byte of EAX w/ 'r' of pixel #2
                        rol  eax, 8
                        mov  al, bl
                        ror  eax, 8

                        // fill high word of EBX with 'g' and 'b' of pixel #3
                        rol  ebx, 8
                        mov  bx, dx
                        ror  ebx, 16

                        // fill bottom 3 bytes of edx with value of pixel #4
                        mov  edx, [esi+12]
                        rol  edx, 8
                        mov  dl, [esi+10]    //!

                        // write four pixels, packed into 3     [eax:ebx:edx]
                        mov  [edi], eax
                        mov  [edi+4], ebx
                        mov  [edi+8], edx

                        add esi, 16
                        add edi, 12

                                mov  eax, dword ptr [esi]
                                mov  ebx, dword ptr [esi+4]
                                mov  edx, dword ptr [esi+8]
                    
                                // fill high byte of EAX w/ 'r' of pixel #2
                                rol  eax, 8
                                mov  al, bl
                                ror  eax, 8

                                // fill high word of EBX with 'g' and 'b' of pixel #3
                                rol  ebx, 8
                                mov  bx, dx
                                ror  ebx, 16

                                // fill bottom 3 bytes of edx with value of pixel #4
                                mov  edx, [esi+12]
                                rol  edx, 8
                                mov  dl, [esi+10]    //!

                                // write four pixels, packed into 3     [eax:ebx:edx]
                                mov  [edi], eax
                                mov  [edi+4], ebx
                                mov  [edi+8], edx

                                add esi, 16
                                add edi, 12

					    dec    cx
					    jnz    PixelChunk_Loop24
				    
				    add   esi, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop24

			    EMMS
		    }
        }
	    else if (iDispBits==32)
	    {
		    __asm
		    {
			    mov        edi, dworddest
                mov        eax, src
			    
			    // [edi]           is start pt. on back surface
			    // [eax]           points to VS1
			    // ecx is the loop counter
                // ebx holds linesize_over_FXW (times 4)

			    mov        ecx, FXH2

			    Scanline_Loop32:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_16

				    PixelChunk_Loop32:

					    // load, saturate & store the red, green, & blue values
                        MOVQ       mm0, [eax]       // read 2 bgrx pixels
                        MOVQ       mm1, [eax+8]     // read 2 bgrx pixels
                        MOVQ       mm2, [eax+16]    // read 2 bgrx pixels
                        MOVQ       mm3, [eax+24]    // read 2 bgrx pixels
                        MOVQ       mm4, [eax+32]    // read 2 bgrx pixels
                        MOVQ       mm5, [eax+40]    // read 2 bgrx pixels
                        MOVQ       mm6, [eax+48]    // read 2 bgrx pixels
                        MOVQ       mm7, [eax+56]    // read 2 bgrx pixels
                    
                        MOVQ       [edi], mm0       
                        MOVQ       [edi+8], mm1
                        MOVQ       [edi+16], mm2
                        MOVQ       [edi+24], mm3    // write result to back surface
                        MOVQ       [edi+32], mm4
                        MOVQ       [edi+40], mm5
                        MOVQ       [edi+48], mm6
                        MOVQ       [edi+56], mm7
                    
                        ADD        eax, 64
                        ADD        edi, 64

					    dec    cx
					    jnz    PixelChunk_Loop32
				    
				    add   eax, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop32

			    EMMS
		    }
	    }
    }
    //else if (procsize == 1)
	else if (ddsd_src.dwWidth*2 == ddsd_dest.dwWidth)
    {
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //                  DOUBLE STRETCH
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        if (iDispBits==16)
	    {
		    if (VIDEO_CARD_555==1)	// 5-5-5
		    __asm
		    {
			    mov        edi, worddest
                mov        eax, src
			    
			    // [edi]           is start pt. on back surface
			    // [eax]           points to VS1
			    // ecx is the loop counter

			    mov        ecx, FXH2

				// for pixel stretching:
				mov        ebx, bytes_per_scanline_d

			    Scanline_Loop16bd:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_2

				    PixelChunk_Loop16bd:

                        MOVQ       mm0, [eax]     // read 64 bits [RGBxRGBx]

                        MOVQ       mm1, mm0
                        MOVQ       mm2, mm0       //|????????rrrrrrrr|ggggggggbbbbbbbb|????????bbbbbbbb|ggggggggrrrrrrrr|

                        PSRLD      mm0, 16+3      //|0000000000000000|00000000000rrrrr|0000000000000000|00000000000rrrrr|
                        PSRLD      mm1,  8+3      //|0000000000000000|000rrrrrrrrggggg|0000000000000000|0000000000gggggg|
                        PSRLD      mm2,    3      //|00000000000rrrrr|rrrggggggggbbbbb|0000000000000000|00000000000bbbbb|
                        PSLLD      mm0, 32-5      //|rrrrr00000000000|0000000000000000|0000000000000000|00000000000rrrrr|
                        PSLLD      mm1, 32-5      //|ggggg00000000000|0000000000000000|0000000000000000|0000000000gggggg|
                        PSLLD      mm2, 32-5      //|bbbbb00000000000|0000000000000000|0000000000000000|00000000000bbbbb|
                        PSRLD      mm0, 1         //|0rrrrr0000000000|0000000000000000|0000000000000000|00000000000rrrrr|
                        PSRLD      mm1, 1+5       //|000000ggggg00000|0000000000000000|0000000000000000|0000000000gggggg|
                        PSRLD      mm2, 1+5+5     //|00000000000bbbbb|0000000000000000|0000000000000000|00000000000bbbbb|

                        POR        mm0, mm1
                        POR        mm0, mm2       //|0rrrrrgggggbbbbb|0000000000000000|rrrrrggggggbbbbb|0000000000000000|
                    
                        MOVQ       mm1, mm0
                        PSRLQ      mm0, 16
                        POR        mm0, mm1       

                        MOVQ       [edi], mm0     // puts lower 32 bits to memory (2 16-bit RGB quantities == 2 pixels)
                        MOVQ       [edi+ebx], mm0     // puts lower 32 bits to memory (2 16-bit RGB quantities == 2 pixels)
                
                        ADD        eax, 8
                        ADD        edi, 8

					    dec    cx
					    jnz    PixelChunk_Loop16bd
				    
				    add   eax, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
					add   edi, bytes_per_scanline_d
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop16bd

			    EMMS
		    }
            else	// 5-6-5
		    __asm
		    {
			    mov        edi, worddest
                mov        eax, src
			    
			    // [edi]           is start pt. on back surface
			    // [eax]           points to VS1
			    // ecx is the loop counter
                // ebx holds linesize_over_FXW (times 4)

			    mov        ecx, FXH2

				// for pixel stretching:
				mov        ebx, bytes_per_scanline_d

			    Scanline_Loop16ad:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_2

				    PixelChunk_Loop16ad:

                        MOVQ       mm0, [eax]     // read 64 bits [RGBxRGBx]

                                                  //MM0 (high half)                      MM1 (high half)                      MM2 (high half)
                                                  //-----------------------------------  -----------------------------------  -----------------------------------
                                                  //|........rrrrrrrr|ggggggggbbbbbbbb|
                        MOVQ       mm1, mm0       //|........rrrrrrrr|ggggggggbbbbbbbb|  |........rrrrrrrr|ggggggggbbbbbbbb|
                        PSRLW      mm0, 3         //|000........rrrrr|000ggggggggbbbbb|  |........rrrrrrrr|ggggggggbbbbbbbb|
                        PSRLW      mm1, 10        //|000........rrrrr|000ggggggggbbbbb|  |0000000000......|0000000000gggggg|
                        PSLLW      mm0, 11        //|rrrrr00000000000|bbbbb00000000000|  |0000000000......|0000000000gggggg|
                        PSLLD      mm1, 21        //|rrrrr00000000000|bbbbb00000000000|  |00000gggggg00000|0000000000000000|
                        MOVQ       mm2, mm0       //|rrrrr00000000000|bbbbb00000000000|  |00000gggggg00000|0000000000000000|  |rrrrr00000000000|bbbbb00000000000|
                        POR        mm0, mm1       //|rrrrrgggggg00000|bbbbb00000000000|                                       |rrrrr00000000000|bbbbb00000000000|
                        PSLLD      mm2, 5         //|rrrrrgggggg00000|bbbbb00000000000|                                       |00000000000bbbbb|0000000000000000|
                        POR        mm0, mm2       //|rrrrrggggggbbbbb|bbbbb00000000000|rrrrrggggggbbbbb|bbbbb00000000000|

                        PSRLD      mm0, 16        //|0000000000000000|rrrrrggggggbbbbb|0000000000000000|rrrrrggggggbbbbb|
                        MOVQ       mm1, mm0
                        PSLLD      mm1, 16
                        POR        mm0, mm1

                        MOVQ       [edi], mm0
                        MOVQ       [edi+ebx], mm0
                
                        ADD        eax, 8//16//8
                        ADD        edi, 8//8//4

					    dec    cx
					    jnz    PixelChunk_Loop16ad
				    
				    add   eax, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
				    add   edi, bytes_per_scanline_d
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop16ad

			    EMMS
		    }
	    }
        else if (iDispBits==24)
        {
		    __asm
		    {
			    mov        edi, dworddest
                mov        eax, src
                mov        esi, eax
			    
			    // [esi]           points to VS1
			    // [edi]           is start pt. on back surface
			    // ecx is the loop counter

			    mov        ecx, FXH2

				// for pixel stretching:
				mov        ebx, bytes_per_scanline_d

			    Scanline_Loop24d:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_4

				    PixelChunk_Loop24d:

					//         mm0mm0mm|mm1mm1mm|mm3mm3mm
					// input:  -BGR-CHS|-DIT-EJU  
					// output: hsBGRBGR|TDITchsc|ejuejuDI

						movq   mm0, [esi]
						movq   mm2, [esi+8]
						movq   mm1, mm0
						movq   mm3, mm2
						psllq mm0, 40		
						psllq mm1, 8 		
						psllq mm2, 40		
						psllq mm3, 8 		
						psrlq mm0, 40		// 00000BGR
						psrlq mm1, 40		// 00000CHS
						psrlq mm2, 40		// 00000DIT
						psrlq mm3, 40		// 00000EJU
						movq  mm4, mm0
						movq  mm5, mm1
						movq  mm6, mm2
						movq  mm7, mm3
						
						psllq mm7, 40		// EJU00000
						psllq mm4, 24		// 00BGR000
						psllq mm3, 16		// 000EJU00
						psllq mm1, 8		// 0000CHS0
						psllq mm2, 32		// 0DIT0000
						por mm0, mm4		// 00BGRBGR
						por mm3, mm7		// EJUEJU00
						movq mm4, mm5		// 00000CHS
						movq mm7, mm6		// 00000DIT
						por mm1, mm2		// 0DITCHS0
						psllq mm5, 48		// HS000000
						psrlq mm6, 8 		// 000000DI
						psrlq mm4, 16		// 0000000C
						psllq mm7, 56		// T0000000
						por mm0, mm5		// HSBGRBGR
						por mm3, mm6		// EJUEJUDI
						por mm1, mm4		// 0DITCHSC
						por mm1, mm7		// TDITCHSC

                        movq  [edi], mm0
                        movq  [edi+8], mm1
                        movq  [edi+16], mm3
                        movq  [edi+ebx], mm0
                        movq  [edi+ebx+8], mm1
                        movq  [edi+ebx+16], mm3

                        add esi, 16
                        add edi, 24

					    dec    cx
					    jnz    PixelChunk_Loop24d
				    
				    add   esi, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
				    add   edi, ebx
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop24d

			    EMMS
		    }
        }
	    else if (iDispBits==32)
	    {
		    __asm
		    {
			    mov        edi, dworddest
                mov        eax, src
			    
			    // [edi]           is start pt. on back surface
			    // [eax]           points to VS1
			    // ecx is the loop counter
                // ebx holds linesize_over_FXW (times 4)

			    mov        ecx, FXH2

				// for pixel stretching:
				mov        ebx, bytes_per_scanline_d

			    Scanline_Loop32d:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_4

				    PixelChunk_Loop32d:

					    // load, saturate & store the red, green, & blue values
                        MOVQ       mm0, [eax]       // read 2 bgrx pixels
                        MOVQ       mm1, [eax+8]     // read 2 bgrx pixels
						MOVQ       mm2, mm0
						MOVQ       mm3, mm1
						PSLLQ      mm0, 32
						PSLLQ      mm1, 32
						PSRLQ      mm2, 32
						PSRLQ      mm3, 32
						MOVQ       mm4, mm0
						MOVQ       mm5, mm1
						MOVQ       mm6, mm2
						MOVQ       mm7, mm3
						PSRLQ      mm0, 32
						PSRLQ      mm1, 32
						PSLLQ      mm2, 32
						PSLLQ      mm3, 32
						POR        mm0, mm4
						POR        mm1, mm5
						POR        mm2, mm6
						POR        mm3, mm7
                    
                        MOVQ       [edi], mm0       
                        MOVQ       [edi+8], mm2
                        MOVQ       [edi+16], mm1
                        MOVQ       [edi+24], mm3    // write result to back surface
                        MOVQ       [edi+ebx], mm0       
                        MOVQ       [edi+ebx+8], mm2
                        MOVQ       [edi+ebx+16], mm1
                        MOVQ       [edi+ebx+24], mm3    // write result to back surface
                    
                        ADD        eax, 16
                        ADD        edi, 32 

					    dec    cx
					    jnz    PixelChunk_Loop32d
				    
				    add   eax, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
				    add   edi, bytes_per_scanline_d
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop32d

			    EMMS
		    }
	    }
    }
	//else if (procsize==2)
	else if (ddsd_src.dwWidth*4 == ddsd_dest.dwWidth)
	{
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //                   QUAD STRETCH
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        if (iDispBits==16)
	    {
		    if (VIDEO_CARD_555==1)	// 5-5-5
		    __asm
		    {
			    mov        edi, worddest
                mov        esi, src
			    
			    // [edi]           is start pt. on back surface
			    // [esi]           points to VS1
			    // ecx is the loop counter

			    mov        ecx, FXH2

				// for pixel stretching:
				mov        ebx, bytes_per_scanline_d
				mov        edx, bytes_per_scanline_d
				add        edx, bytes_per_scanline_d
				add        edx, bytes_per_scanline_d

			    Scanline_Loop16bq:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_2

				    PixelChunk_Loop16bq:

                        MOVQ       mm0, [esi]     // read 64 bits [RGBxRGBx]

                        MOVQ       mm1, mm0
                        MOVQ       mm2, mm0       //|????????rrrrrrrr|ggggggggbbbbbbbb|????????bbbbbbbb|ggggggggrrrrrrrr|

                        PSRLD      mm0, 16+3      //|0000000000000000|00000000000rrrrr|0000000000000000|00000000000rrrrr|
                        PSRLD      mm1,  8+3      //|0000000000000000|000rrrrrrrrggggg|0000000000000000|0000000000gggggg|
                        PSRLD      mm2,    3      //|00000000000rrrrr|rrrggggggggbbbbb|0000000000000000|00000000000bbbbb|
                        PSLLD      mm0, 32-5      //|rrrrr00000000000|0000000000000000|0000000000000000|00000000000rrrrr|
                        PSLLD      mm1, 32-5      //|ggggg00000000000|0000000000000000|0000000000000000|0000000000gggggg|
                        PSLLD      mm2, 32-5      //|bbbbb00000000000|0000000000000000|0000000000000000|00000000000bbbbb|
                        PSRLD      mm0, 1         //|0rrrrr0000000000|0000000000000000|0000000000000000|00000000000rrrrr|
                        PSRLD      mm1, 1+5       //|000000ggggg00000|0000000000000000|0000000000000000|0000000000gggggg|
                        PSRLD      mm2, 1+5+5     //|00000000000bbbbb|0000000000000000|0000000000000000|00000000000bbbbb|

                        POR        mm0, mm1
                        POR        mm0, mm2       //|0rrrrrgggggbbbbb|0000000000000000|rrrrrggggggbbbbb|0000000000000000|

						MOVQ       mm7, mm0
						PSRLD      mm0, 16
						
						// set up quad-stretched left pixel in mm6
						// set up quad-stretched right pixel in mm0
						POR        mm0, mm7		  //|pL|pL|pR|pR|
						MOVQ       mm6, mm0
						PSRLQ      mm0, 32
						PSLLQ      mm6, 32
						MOVQ       mm1, mm0
						MOVQ       mm5, mm6
						PSLLQ      mm1, 32
						PSRLQ      mm5, 32
						POR        mm0, mm1
						POR        mm6, mm5
						
	                    MOVQ       [edi], mm6     
                        MOVQ       [edi+8], mm0     
                        MOVQ       [edi+ebx], mm6 
                        MOVQ       [edi+ebx+8], mm0 
                        MOVQ       [edi+ebx*2], mm6 
                        MOVQ       [edi+ebx*2+8], mm0 
                        MOVQ       [edi+edx], mm6 
                        MOVQ       [edi+edx+8], mm0 
                
                        ADD        esi, 8
                        ADD        edi, 16

					    dec    cx
					    jnz    PixelChunk_Loop16bq
				    
				    add   esi, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
					add   edi, edx
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop16bq

			    EMMS
		    }
            else	// 5-6-5
		    __asm
		    {
			    mov        edi, worddest
                mov        esi, src
			    
			    // [edi]           is start pt. on back surface
			    // [esi]           points to VS1
			    // ecx is the loop counter

			    mov        ecx, FXH2

				// for pixel stretching:
				mov        ebx, bytes_per_scanline_d
				mov        edx, bytes_per_scanline_d
				add        edx, bytes_per_scanline_d
				add        edx, bytes_per_scanline_d

			    Scanline_Loop16aq:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_2

				    PixelChunk_Loop16aq:

                        MOVQ       mm0, [esi]     // read 64 bits [RGBxRGBx]

                                                  //MM0 (high half)                      MM1 (high half)                      MM2 (high half)
                                                  //-----------------------------------  -----------------------------------  -----------------------------------
                                                  //|........rrrrrrrr|ggggggggbbbbbbbb|
                        MOVQ       mm1, mm0       //|........rrrrrrrr|ggggggggbbbbbbbb|  |........rrrrrrrr|ggggggggbbbbbbbb|
                        PSRLW      mm0, 3         //|000........rrrrr|000ggggggggbbbbb|  |........rrrrrrrr|ggggggggbbbbbbbb|
                        PSRLW      mm1, 10        //|000........rrrrr|000ggggggggbbbbb|  |0000000000......|0000000000gggggg|
                        PSLLW      mm0, 11        //|rrrrr00000000000|bbbbb00000000000|  |0000000000......|0000000000gggggg|
                        PSLLD      mm1, 21        //|rrrrr00000000000|bbbbb00000000000|  |00000gggggg00000|0000000000000000|
                        MOVQ       mm2, mm0       //|rrrrr00000000000|bbbbb00000000000|  |00000gggggg00000|0000000000000000|  |rrrrr00000000000|bbbbb00000000000|
                        POR        mm0, mm1       //|rrrrrgggggg00000|bbbbb00000000000|                                       |rrrrr00000000000|bbbbb00000000000|
                        PSLLD      mm2, 5         //|rrrrrgggggg00000|bbbbb00000000000|                                       |00000000000bbbbb|0000000000000000|
                        POR        mm0, mm2       //|rrrrrggggggbbbbb|bbbbb00000000000|rrrrrggggggbbbbb|bbbbb00000000000|

                        PSRLD      mm0, 16        //|0000000000000000|rrrrrggggggbbbbb|0000000000000000|rrrrrggggggbbbbb|
						MOVQ       mm7, mm0
                        PSLLD      mm0, 16        //|rrrrrggggggbbbbb|0000000000000000|rrrrrggggggbbbbb|0000000000000000|
    
						// set up quad-stretched left pixel in mm6
						// set up quad-stretched right pixel in mm0
						POR        mm0, mm7		  //|pL|pL|pR|pR|
						MOVQ       mm6, mm0
						PSRLQ      mm0, 32
						PSLLQ      mm6, 32
						MOVQ       mm1, mm0
						MOVQ       mm5, mm6
						PSLLQ      mm1, 32
						PSRLQ      mm5, 32
						POR        mm0, mm1
						POR        mm6, mm5
						
	                    MOVQ       [edi], mm6     
                        MOVQ       [edi+8], mm0     
                        MOVQ       [edi+ebx], mm6 
                        MOVQ       [edi+ebx+8], mm0 
                        MOVQ       [edi+ebx*2], mm6 
                        MOVQ       [edi+ebx*2+8], mm0 
                        MOVQ       [edi+edx], mm6 
                        MOVQ       [edi+edx+8], mm0 
                
                        ADD        esi, 8
                        ADD        edi, 16

					    dec    cx
					    jnz    PixelChunk_Loop16aq
				    
				    add   esi, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
					add   edi, edx
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop16aq

			    EMMS
		    }
	    }
	    else if (iDispBits==24)
		{
		    __asm
		    {
			    mov        edi, dworddest
                mov        eax, src
                mov        esi, eax
			    
			    // [esi]           points to VS1
			    // [edi]           is start pt. on back surface
			    // ecx is the loop counter

			    mov        ecx, FXH2

				// for pixel stretching:
				mov        ebx, bytes_per_scanline_d
				mov        edx, bytes_per_scanline_d
				add        edx, bytes_per_scanline_d
				add        edx, bytes_per_scanline_d

			    Scanline_Loop24q:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_2

				    PixelChunk_Loop24q:

					//         mm0mm0mm|mm1mm1mm|mm3mm3mm
					// input:  -BGR-CHS|
					// output: GRBGRBGR|SCHSBGRB|CHSCHSCH

						movq   mm0, [esi]
						movq   mm1, mm0
						psllq mm0, 40		
						psllq mm1, 8 		
						psrlq mm0, 40		// 00000BGR
						psrlq mm1, 40		// 00000CHS
						movq  mm2, mm0
						movq  mm3, mm1
						psllq mm0, 24
						psllq mm1, 24
						por   mm0, mm2		// 00BGRBGR
						por   mm1, mm3		// 00CHSCHS
						
						movq  mm4, mm0		// 00BGRBGR
						movq  mm5, mm1		// 00CHSCHS
						movq  mm6, mm0		// 00BGRBGR
						movq  mm7, mm1		// 00CHSCHS
						movq  mm2, mm0		// 00BGRBGR
						movq  mm3, mm1		// 00CHSCHS
	
						psllq mm4, 48       // GR000000
						psrlq mm5, 32       // 000000CH
						psllq mm7, 16       // CHSCHS00
						psrlq mm2, 16		// 0000BGRB
						psllq mm3, 32       // SCHS0000

						por   mm0, mm4		// GRBGRBGR
						por   mm7, mm5		// CHSCHSCH
						por   mm2, mm3		// SCHSBGRB

                        movq  [edi], mm0
                        movq  [edi+8], mm2
                        movq  [edi+16], mm7
                        movq  [edi+ebx], mm0
                        movq  [edi+ebx+8], mm2
                        movq  [edi+ebx+16], mm7
                        movq  [edi+ebx*2], mm0
                        movq  [edi+ebx*2+8], mm2
                        movq  [edi+ebx*2+16], mm7
                        movq  [edi+edx], mm0
                        movq  [edi+edx+8], mm2
                        movq  [edi+edx+16], mm7

                        add esi, 8
                        add edi, 24

					    dec    cx
					    jnz    PixelChunk_Loop24q
				    
				    add   esi, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
				    add   edi, edx
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop24q

			    EMMS
		    }
		}
	    else if (iDispBits==32)
	    {
		    __asm
		    {
			    mov        edi, dworddest
                mov        eax, src
			    
			    // [edi]           is start pt. on back surface
			    // [eax]           points to VS1
			    // ecx is the loop counter
                // ebx holds linesize_over_FXW (times 4)

			    mov        ecx, FXH2

				// for pixel stretching:
				mov        ebx, bytes_per_scanline_d
				mov        edx, bytes_per_scanline_d
				add        edx, bytes_per_scanline_d
				add        edx, bytes_per_scanline_d

			    Scanline_Loop32q:

				    // upper word of ECX: # of scanlines left
				    // lower word of ECX: # of pixels left in current scanline.

				    shl   ecx, 16
				    mov    cx, FXW_DIV_4

				    PixelChunk_Loop32q:

					    // load, saturate & store the red, green, & blue values
                        MOVQ       mm0, [eax]       // read 2 bgrx pixels
                        MOVQ       mm1, [eax+8]     // read 2 bgrx pixels
						MOVQ       mm2, mm0
						MOVQ       mm3, mm1
						PSLLQ      mm0, 32
						PSLLQ      mm1, 32
						PSRLQ      mm2, 32
						PSRLQ      mm3, 32
						MOVQ       mm4, mm0
						MOVQ       mm5, mm1
						MOVQ       mm6, mm2
						MOVQ       mm7, mm3
						PSRLQ      mm0, 32
						PSRLQ      mm1, 32
						PSLLQ      mm2, 32
						PSLLQ      mm3, 32
						POR        mm0, mm4
						POR        mm1, mm5
						POR        mm2, mm6
						POR        mm3, mm7
                    
                        MOVQ       [edi], mm0       
                        MOVQ       [edi+8], mm0
                        MOVQ       [edi+16], mm2
                        MOVQ       [edi+24], mm2    // write result to back surface
                        MOVQ       [edi+32], mm1       
                        MOVQ       [edi+40], mm1
                        MOVQ       [edi+48], mm3
                        MOVQ       [edi+56], mm3    // write result to back surface

                        MOVQ       [edi+ebx], mm0       
                        MOVQ       [edi+ebx+8], mm0
                        MOVQ       [edi+ebx+16], mm2
                        MOVQ       [edi+ebx+24], mm2    // write result to back surface
                        MOVQ       [edi+ebx+32], mm1       
                        MOVQ       [edi+ebx+40], mm1
                        MOVQ       [edi+ebx+48], mm3
                        MOVQ       [edi+ebx+56], mm3    // write result to back surface

                        MOVQ       [edi+ebx*2], mm0       
                        MOVQ       [edi+ebx*2+8], mm0
                        MOVQ       [edi+ebx*2+16], mm2
                        MOVQ       [edi+ebx*2+24], mm2    // write result to back surface
                        MOVQ       [edi+ebx*2+32], mm1       
                        MOVQ       [edi+ebx*2+40], mm1
                        MOVQ       [edi+ebx*2+48], mm3
                        MOVQ       [edi+ebx*2+56], mm3    // write result to back surface

                        MOVQ       [edi+edx], mm0       
                        MOVQ       [edi+edx+8], mm0
                        MOVQ       [edi+edx+16], mm2
                        MOVQ       [edi+edx+24], mm2    // write result to back surface
                        MOVQ       [edi+edx+32], mm1       
                        MOVQ       [edi+edx+40], mm1
                        MOVQ       [edi+edx+48], mm3
                        MOVQ       [edi+edx+56], mm3    // write result to back surface
                    
                        ADD        eax, 16
                        ADD        edi, 64 

					    dec    cx
					    jnz    PixelChunk_Loop32q

				    add   eax, extra_bytes_per_scanline_s
				    add   edi, extra_bytes_per_scanline_d
				    add   edi, edx
				    shr   ecx, 16
				    dec   cx
				    jnz   Scanline_Loop32q

			    EMMS
		    }
	    }
	}


    if (lpDDSsrc->Unlock(NULL) != DD_OK)
    {
        dumpmsg("merge_all_VS: error unlocking lpDDSsrc!");
    }

    if (lpDDSdest->Unlock(NULL) != DD_OK)
    {
        dumpmsg("merge_all_VS: error unlocking lpDDSdest!");
    }

    return true;
}







void Put_FPS_To_Backbuffer(COLORREF *fg, COLORREF *bg, int y_pos, bool bRightJustify)
{
    HDC hdc;
	HRESULT ddrval;
	char szFPS[64];

	UINT uFormat = 0;
	if (bRightJustify) uFormat |= DT_RIGHT;

	if (fps == 0) 
		sprintf(szFPS, " fps: [calibrating] ");
	else
		//sprintf(szFPS, " fps: %4.2f, honest: %4.2f, sleep: %4.2f ms ", fps, honest_fps, sleep_amount);
		sprintf(szFPS, " fps: %4.2f ", honest_fps);

  
	if (lpDDSMsg && (ddrval = lpDDSMsg->GetDC(&hdc)) == DD_OK)
	{
		SetBkColor( hdc, RGB( 0, 0, 0 ) );
		SetBkMode( hdc, TRANSPARENT );
		for (int i=0; i<2; i++)
		{
			if (i==0)
				SetTextColor( hdc, *bg );
			else
				SetTextColor( hdc, *fg );
			//TextOut( hdc, 1-i, y_pos+1-i, szFPS, lstrlen(szFPS) );
            RECT r;
			if (rendermode==3)
				SetRect(&r, 1-i, y_pos+1-i, FXW2-i, FXH2);
			else
				SetRect(&r, 1-i, y_pos+1-i, FXW-i, FXH);
            DrawText(hdc, szFPS, lstrlen(szFPS), &r, uFormat);
		}
		lpDDSMsg->ReleaseDC(hdc);
	}
}




void Put_Msg_To_Backbuffer(char *str, COLORREF *fg, COLORREF *bg, int y_pos, bool bRightJustify)
{
    HDC hdc;
	HRESULT ddrval;
	
	UINT uFormat = DT_PATH_ELLIPSIS;
	if (bRightJustify) uFormat |= DT_RIGHT;

	if (lpDDSMsg && (ddrval = lpDDSMsg->GetDC(&hdc)) == DD_OK)
	{
		SetBkColor( hdc, RGB( 0, 0, 0 ) );
		SetBkMode( hdc, TRANSPARENT );
		for (int i=0; i<2; i++)
		{
			if (i==0)
				SetTextColor( hdc, *bg );
			else
				SetTextColor( hdc, *fg );
			//TextOut( hdc, 1-i, y_pos+1-i, str, lstrlen(str) );
            RECT r;
			if (rendermode==3)
				SetRect(&r, 1-i, y_pos+1-i, FXW2-i, FXH2);
			else
				SetRect(&r, 1-i, y_pos+1-i, FXW-i, FXH);
            DrawText(hdc, str, lstrlen(str), &r, uFormat);
		}
		lpDDSMsg->ReleaseDC(hdc);
	}
}


void Put_Helpmsg_To_Backbuffer(COLORREF *fg, COLORREF *bg)
{
    HDC hdc;
	HRESULT ddrval;
	int indent = 20;
	int y_pos = y_inc;

	//y_pos = min(y_pos, FXH - y_inc*10 - indent);

	if (lpDDSMsg && (ddrval = lpDDSMsg->GetDC(&hdc)) == DD_OK)
	{
		SetBkColor( hdc, RGB( 0, 0, 0 ) );
		SetBkMode( hdc, TRANSPARENT );
		for (int i=0; i<2; i++)
		{
			if (i==0)
				SetTextColor( hdc, *bg );
			else
				SetTextColor( hdc, *fg );
			TextOut( hdc, indent, y_pos, szH1, lstrlen(szH1) );
			TextOut( hdc, indent, y_pos+y_inc*1, szH2, lstrlen(szH2) );
			TextOut( hdc, indent, y_pos+y_inc*2, szH3, lstrlen(szH3) );
			TextOut( hdc, indent, y_pos+y_inc*3, szH4, lstrlen(szH4) );
			TextOut( hdc, indent, y_pos+y_inc*4, szH5, lstrlen(szH5) );
			TextOut( hdc, indent, y_pos+y_inc*5, szH6, lstrlen(szH6) );
			TextOut( hdc, indent, y_pos+y_inc*6, szH7, lstrlen(szH7) );
			TextOut( hdc, indent, y_pos+y_inc*7, szH8, lstrlen(szH8) );
			TextOut( hdc, indent, y_pos+y_inc*8, szH9, lstrlen(szH9) );
            if (szH10[0])
			    TextOut( hdc, indent, y_pos+y_inc*9, szH10, lstrlen(szH10) );
            if (szH11[0])
			    TextOut( hdc, indent, y_pos+y_inc*10, szH11, lstrlen(szH11) );
			y_pos--;
			indent--;
		}
		lpDDSMsg->ReleaseDC(hdc);
	}
}





void FX_Screenshot(int fr)
{
  // looks to the arrays VS_R[], VS_G[], VS_B[] to write 
  // a 24-bit dynamically-sized TGA.  Below is special fix for VidEdit
  // (...VidEdit requires at least 320x200 image size)
//              if (xneg>0) xneg=0;
//              if (xpos<639) xpos=639;
//              if (yneg>0) yneg=0;
//              if (ypos<199) ypos=199;

	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	HRESULT ddrval;
	ddrval = lpDDSVS[(intframe+1)%2]->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
	if (ddrval == DD_OK)
	{
		FILE *outfile;

		char filename[512];
		sprintf(filename, "c:\\dr%06d.tga", fr);
		//sprintf(temp, "dr%06d.tga", fr);
		//strcpy(filename, winpath);
		//strcat(filename, temp);
		outfile = fopen(filename, "wb");

		int x_span = FXW2, y_span = FXH2;
		unsigned long k_end = FXH2*FXW2;

		if (iDispBits==8)
		{
		  // header for 8-bit targa
		  fprintf(outfile, "%c%c%c%c%c%c",  0,  1,  1,  0,  0,  0);
		  fprintf(outfile, "%c%c%c%c%c%c",  1, 24,  0,  0,  0,  0);
		  fprintf(outfile, "%c%c%c%c%c%c", x_span % 256, x_span/256, y_span % 256, y_span/256, 8, 8);
		}
		else
		{
		  // header for 24-bit targa
		  fprintf(outfile, "%c%c%c%c%c%c",  0,  0,  2,  0,  0,  0);
		  fprintf(outfile, "%c%c%c%c%c%c",  0,  0,  0,  0,  0,  0);
		  fprintf(outfile, "%c%c%c%c%c%c", x_span % 256, x_span/256, y_span % 256, y_span/256, 32, 32);
		}



		if (iDispBits==8)
		{
		  /*
		  for (k=0; k<256; k++)
			  fprintf(outfile, "%c%c%c", ape2[k].peGreen, ape2[k].peBlue, ape2[k].peRed);

		  fwrite(VS1, FXW*FXH, 1, outfile);
		  */
		}
		else
		{
		  fwrite(ddsd.lpSurface, FXW2*FXH2*4, 1, outfile);
		  /*
		  k_end *= 4;
		  for (k=FX_YCUT_HIDE*FXW*4; k<k_end; k += FXW*4)
		  {
			  fprintf(outfile, "%c%c%c", VS1[k], VS1[k+1], VS1[k+2]);
		  }
		  */
		}


		fclose(outfile);
	}

	lpDDSVS[(intframe+1)%2]->Unlock(NULL);
}



#endif // GEISS_VIDEO