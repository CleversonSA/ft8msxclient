/**--
-- CustomCharmap.c
--   Custom Character Map with graphical and interted console chars
--
-- Requires SDCC and Fusion-C library to compile
-- Copyright (c) 2024 Cleverson S A ( CleversonSA )
-- All rights reserved.
--
-- Redistribution and use of this source code or any derivative works, are
-- permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice,
--    this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
-- 3. Redistributions may not be sold, nor may they be used in a commercial
--    product or activity without specific prior written permission.
-- 4. Source code of derivative works MUST be published to the public.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
-- TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
-- PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
-- CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
-- EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
-- PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
-- OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
-- WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
-- OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
-- ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--
*/
#include <stdio.h>
#include <stdlib.h>
#include "fusion-c/header/msx_fusion.h"
#include "CustomCharmap.h"

void InitCustomCharmap()
{
    int col = 0;
    char memChar = 0;
    int vmemCharInitAddr = 0;
    int vmemCharCopyAddr = 0;
    int byteCharLine = 0;

    // Define graph caracter for top-left border
    // 00000000 = 0
    // 00000000 = 0
    // 01111111 = 127
    // 01000000 = 64
    // 01011111 = 95
    // 01010000 = 80
    // 01010000 = 80
    // 01010000 = 80
    char grpChar[8] = {0, 0, 127, 64, 95, 80, 80, 80};
    char grpPos = 0;
    char grpCharPos = 0;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for top-right border
    // 00000000 = 0
    // 00000000 = 0
    // 11110000 = -16
    // 00010000 = 16
    // 11010000 = -48
    // 01010000 = 80
    // 01010000 = 80
    // 01010000 = 80
    grpChar[0] = 0;
    grpChar[1] = 0;
    grpChar[2] = -16;
    grpChar[3] = 16;
    grpChar[4] = -48;
    grpChar[5] = 80;
    grpChar[6] = 80;
    grpChar[7] = 80;
    grpPos = 0;
    grpCharPos = 1;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for column border
    // 01010000 = 40
    // 01010000 = 40
    // 01010000 = 40
    // 01010000 = 40
    // 01010000 = 40
    // 01010000 = 40
    // 01010000 = 40
    // 01010000 = 40
    grpChar[0] = 80;
    grpChar[1] = 80;
    grpChar[2] = 80;
    grpChar[3] = 80;
    grpChar[4] = 80;
    grpChar[5] = 80;
    grpChar[6] = 80;
    grpChar[7] = 80;
    grpPos = 0;
    grpCharPos = 2;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for botton-left border
    // 01010000 = 80
    // 01010000 = 80
    // 01011111 = 95
    // 01000000 = 64
    // 01111111 = 127
    // 00000000 = 0
    // 00000000 = 0
    // 00000000 = 0
    grpChar[0] = 80;
    grpChar[1] = 80;
    grpChar[2] = 95;
    grpChar[3] = 64;
    grpChar[4] = 127;
    grpChar[5] = 0;
    grpChar[6] = 0;
    grpChar[7] = 0;
    grpPos = 0;
    grpCharPos = 3;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for botton-right border
    // 01010000 = 80
    // 01010000 = 80
    // 11010000 = -48
    // 00010000 = 16
    // 11110000 = -16
    // 00000000 = 0
    // 00000000 = 0
    // 00000000 = 0
    grpChar[0] = 80;
    grpChar[1] = 80;
    grpChar[2] = -48;
    grpChar[3] = 16;
    grpChar[4] = -16;
    grpChar[5] = 0;
    grpChar[6] = 0;
    grpChar[7] = 0;
    grpPos = 0;
    grpCharPos = 4;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for center-left border
    // 01010000 = 80
    // 01010000 = 80
    // 01011111 = 95
    // 01000000 = 64
    // 01011111 = 95
    // 01010000 = 80
    // 01010000 = 80
    // 01010000 = 80
    grpChar[0] = 80;
    grpChar[1] = 80;
    grpChar[2] = 95;
    grpChar[3] = 64;
    grpChar[4] = 95;
    grpChar[5] = 80;
    grpChar[6] = 80;
    grpChar[7] = 80;
    grpPos = 0;
    grpCharPos = 5;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for center-right border
    // 01010000 = 80
    // 01010000 = 80
    // 11010000 = -48
    // 00010000 = 16
    // 11010000 = -48
    // 01010000 = 80
    // 01010000 = 80
    // 01010000 = 80
    grpChar[0] = 80;
    grpChar[1] = 80;
    grpChar[2] = -48;
    grpChar[3] = 16;
    grpChar[4] = -48;
    grpChar[5] = 80;
    grpChar[6] = 80;
    grpChar[7] = 80;
    grpPos = 0;
    grpCharPos = 6;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for center-right border
    // 00000000 = 0
    // 00000000 = 0
    // 11111111 = -1
    // 00000000 = 0
    // 11111111 = -1
    // 00000000 = 0
    // 00000000 = 0
    // 00000000 = 0
    grpChar[0] = 0;
    grpChar[1] = 0;
    grpChar[2] = -1;
    grpChar[3] = 0;
    grpChar[4] = -1;
    grpChar[5] = 0;
    grpChar[6] = 0;
    grpChar[7] = 0;
    grpPos = 0;
    grpCharPos = 7;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for top T border
    // 00000000 = 0
    // 00000000 = 0
    // 11111111 = -1
    // 00000000 = 0
    // 11011111 = -33
    // 01010000 = 80
    // 01010000 = 80
    // 01010000 = 80
    grpChar[0] = 0;
    grpChar[1] = 0;
    grpChar[2] = -1;
    grpChar[3] = 0;
    grpChar[4] = -33;
    grpChar[5] = 80;
    grpChar[6] = 80;
    grpChar[7] = 80;
    grpPos = 0;
    grpCharPos = 8;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Define graph caracter for upper T border
    // 01010000 = 80
    // 01010000 = 80
    // 11011111 = -33
    // 00000000 = 0
    // 11111111 = -1
    // 00000000 = 0
    // 00000000 = 0
    // 00000000 = 0
    grpChar[0] = 80;
    grpChar[1] = 80;
    grpChar[2] = -33;
    grpChar[3] = 0;
    grpChar[4] = -1;
    grpChar[5] = 0;
    grpChar[6] = 0;
    grpChar[7] = 0;
    grpPos = 0;
    grpCharPos = 9;
    for (grpPos = 0; grpPos < 8; grpPos++)
    {
        Vpoke(__CHARMAP_VRAM_INIT_ADDR__ + ((__CHARMAP_VRAM_CHARPOS_GRP_START__ + grpCharPos) * 8) + grpPos, grpChar[grpPos]);
    }

    // Copy the first char characters blocks to end of charmap table (almost end)
    // inverting the bit pattern of each char
    col = 0;
    for (memChar = 0x20; memChar < 0x7b; memChar++)
    {
        vmemCharInitAddr = __CHARMAP_VRAM_INIT_ADDR__ + ((int)memChar) * 8;
        vmemCharCopyAddr = __CHARMAP_VRAM_INIT_ADDR__ + (__CHARMAP_VRAM_CHARPOS_BAR_START__ + col) * 8;

        for (byteCharLine = 0; byteCharLine <= 7; byteCharLine++)
        {
            Vpoke(vmemCharCopyAddr + byteCharLine, 0xFF & (~Vpeek(vmemCharInitAddr + byteCharLine)));
        }
        col++;
    }
}

void LocateCC(int x, int y)
{
    __charmap_vpoke_pos__ = (__CHARMAP_SCREEN_WIDTH__ * y) + x;
}

void PrintInverted(char *sentence)
{
    char character;
    int col = 0;
    while (*(sentence))
    {
        character = *(sentence++);
        Vpoke(__charmap_vpoke_pos__ + col, __CHARMAP_VRAM_CHARPOS_BAR_START__ + (character - 0x20));
        col++;
    }
}

void ClsCC()
{
    Locate(0, 0);
    __charmap_vpoke_pos__ = 0;
    for (char y = 0; y < __CHARMAP_SCREEN_LINES__; y++)
    {
        for (char x = 0; x < __CHARMAP_SCREEN_WIDTH__; x++)
        {
            PrintChar(' ');
        }
    }
}

void DrawSampleWindow()
{
    Locate(0, 18);
    PrintChar(__CHARMAP_GRPBOX_TOP_LEFT_);
    Locate(1, 18);
    PrintChar(__CHARMAP_GRPBOX_LINE_);
    Locate(2, 18);
    PrintChar(__CHARMAP_GRPBOX_T_TOP_);
    Locate(3, 18);
    PrintChar(__CHARMAP_GRPBOX_LINE_);
    Locate(4, 18);
    PrintChar(__CHARMAP_GRPBOX_TOP_RIGHT_);

    Vpoke((40 * 19), 0Xf6);
    Vpoke((40 * 19) + 1, 0Xf8);
    Vpoke((40 * 19) + 2, 0Xfa);
    Vpoke((40 * 19) + 3, 0Xf8);
    Vpoke((40 * 19) + 4, 0Xf7);
    Vpoke((40 * 20), 0Xf3);
    Vpoke((40 * 20) + 4, 0Xf3);
    Vpoke((40 * 21), 0Xf4);
    Vpoke((40 * 21) + 1, 0Xf8);
    Vpoke((40 * 21) + 2, 0Xf8);
    Vpoke((40 * 21) + 3, 0Xf8);
    Vpoke((40 * 21) + 4, 0Xf5);
}

void DrawCharMap()
{
    for (int f = 0; f <= 15; f++)
    {
        Locate(f * 2 + 2, 1);
        printf("%i", f);
        Locate(0, f + 2);
        printf("%i", f);
    }
    int a = 0;
    int x = 0;
    int y = 0;
    for (x = 0; x <= 15; x++)
    {
        for (y = 0; y <= 15; y++)
        {
            Locate(x * 2 + 2, y + 2);
            a = x + (y * 16);
            if (a < 32)
                continue;
            printf("%c", (char)a);
        }
    }
}