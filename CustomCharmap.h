/**--
-- CustomCharmap.h
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

// Address of VRAM where the character definitions start
// For Screen 0 and Width(40). For 80 is 4096
#define __CHARMAP_VRAM_INIT_ADDR__ 2048
#define __CHARMAP_VRAM_CHARPOS_BAR_START__ 0x96
#define __CHARMAP_VRAM_CHARPOS_GRP_START__ 0xf1

// Custom Box characters shortcuts
#define __CHARMAP_GRPBOX_TOP_LEFT_ 0xf1
#define __CHARMAP_GRPBOX_LINE_ 0xf8
#define __CHARMAP_GRPBOX_T_TOP_ 0xf9
#define __CHARMAP_GRPBOX_TOP_RIGHT_ 0xf2
#define __CHARMAP_GRPBOX_COLUMN_LEFT_ 0xf6
#define __CHARMAP_GRPBOX_T_BOTTOM_ 0xfa
#define __CHARMAP_GRPBOX_COLUMN_RIGHT_ 0xf7
#define __CHARMAP_GRPBOX_COLUMN_ 0xf3
#define __CHARMAP_GRPBOX_BOTTOM_LEFT_ 0xf4
#define __CHARMAP_GRPBOX_BOTTOM_RIGHT_ 0xf5

// Defines in linear mode the X and Y coordinates for Vpoke
#define __CHARMAP_SCREEN_WIDTH__ 40
#define __CHARMAP_SCREEN_LINES__ 23

int __charmap_vpoke_pos__ = 0;

void InitCustomCharmap();
void whiteBar();
void DrawCharMap();
void DrawSampleWindow();
void LocateCC(int, int);
void PrintInverted(char *);
void ClsCC();