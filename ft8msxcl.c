//*******************************************************
//
//   FT8MSXClient
//
// Copyright 2023 Cleverson SA PU5TOS
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//*******************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UnapiHelper.h"
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/asm.h"
#include "CustomCharmap.h"

#define __SDK_MSXVERSION__ 1

//*******************************************************
// UNAPI DEFINITIONS
//*******************************************************
// IMPORTANT: You need to check the map compiler generates to make sure this
// address do not overlap functions, variables, etc
// UNAPI requires memory buffer @ 0x8000 or higher...
#define RcvMemorySize 1024
unsigned char ucConnNumber = 1; // hold the connection number received by UnapiHelper
// For data receive parsing
unsigned char ucRcvData[128];
__at 0x8000 unsigned char ucRcvDataMemory[]; // area to hold data sent to UNAPI, need to be in the 3rd 16K block
unsigned int uiGetSize;
Z80_registers regs; // auxiliary structure for asm function calling

//*******************************************************
// GLOBAL DEFINITIONS
//*******************************************************
#define AUTO_WAIT_CYCLES 512
unsigned char autoUpdate = 1;
unsigned char beeps = 1;
unsigned char scrolling = 1;
unsigned char cqOnly = 0;
unsigned int autoWaitCount = 0;
unsigned int emulatorMode = 0;

//*******************************************************
// PROTOTYPES
//*******************************************************
void prepareScreen();
unsigned char connectServer(unsigned char *, unsigned char *);
void sendCommand(unsigned char, unsigned char *);
unsigned char interpretKey(unsigned char, unsigned char);
void parseReturnData(unsigned char *, unsigned int);
void parseFT8RecData(unsigned char *, unsigned int);
void beep();
void clearLineStatus();
void sleep(int);
void showLineStatus(char *);
void cleanLineTable(int);

//*******************************************************
// MAIN
//*******************************************************
int main(char **argv, int argc)
{
  char ucTxData = 0;                  // where our key inputs go
  unsigned char ucRet;                // return of functions
  unsigned char emulatedString[1024]; // for developing testing
  unsigned char ucServer[128];        // will hold the name of the server we will connect
  unsigned char ucPort[6];            // will hold the port that the server accepts connections
  unsigned char ucAliveConnCount = 0; // when this is 0, check if connection is alive
  char chTextLine[128];

  uiGetSize = 0; // no bytes received yet

  // prepareScreen();

  ClsCC();
  Screen(0);
  Width(40);

  InitCustomCharmap();
  LocateCC(0, 0);
  prepareScreen();
  Locate(1, 2);

  if (argc == 0)
  {
    strcpy(ucServer, "ft8pi.local");
  }
  else if (argc != 0)
  {

    if (strcmp((unsigned char *)argv[0], "/E") == 0)
    {
      Print("Emulated mode activated!");
      emulatorMode = 1;
      strcpy(emulatedString, (unsigned char *)argv[1]);
    }
    else if (strcmp((unsigned char *)argv[0], "/LOREM") == 0)
    {
      Print("Emulated static data activated!");
      emulatorMode = 1;
      strcpy(emulatedString, "1732214355;-15;0.1;1620;IS0SWW;PP5CF;GG52;1732214325;-14;0.2;1621;IS0SWW;PP5CF;GG52;1732214160;-13;0.6;1512;IT9DID;PY2ROE;RR73;1732214100;-14;0.6;1513;CQ;PY2ROE;GG56;1732214070;-17;0.6;1513;CQ;PY2ROE;GG56;1732214040;-12;0.6;1513;CQ;PY2ROE;GG56;1732213980;-15;0.6;1514;CQ;PY2ROE;GG56;1732213965;-18;0.2;1322;A61SD;PY2NL;-11;1732213950;-20;0.6;1514;CQ;PY2ROE;GG56;1732213890;-15;0.6;1514;ZS6MJM;PY2ROE;RR73;1732213890;-16;0.2;1988;J88BTI;PY2CT;-11;1732213830;-17;0.2;1987;J88BTI;PY2CT;-11;1732213830;-16;0.6;1515;G8BCG;PY2ROE;RR73;1732213740;-17;0.6;1514;ZS6MJM;PY2ROE;-12;1732213680;-18;0.6;1515;G8BCG;PY2ROE;RR73;1732213680;-16;0.2;1987;J88BTI;PY2CT;-14;1732213560;-18;0.2;1987;J88BTI;PY2CT;-09;");
    }
    else
    {
      strcpy(ucServer, (unsigned char *)argv[0]);
      strcpy(ucPort, (unsigned char *)argv[1]);
    }
  }
  else
  {
    strcpy(ucPort, "6666");
  }

  if (emulatorMode == 0)
  {
    if (!InitializeTCPIP())
    {
      Locate(1, 2);
      Print("** NO TCP STACK FOUND **");
      Locate(1, 3);
      Print("Please start TCP stack first");
      sleep(2);
      ClsCC();
      return 0;
    }

    showLineStatus("Trying connection...");
    Locate(1, 3);
    Print("Server connection:");
    Locate(1, 4);
    Print("Host:");
    Locate(7, 4);
    Print(ucServer);
    Locate(1, 5);
    Print("Port:");
    Locate(7, 5);
    Print(ucPort);

    ucRet = OpenSingleConnection(ucServer, ucPort, &ucConnNumber);

    if (ucRet != ERR_OK && IsConnected(ucConnNumber) == '0')
    {
      showLineStatus("** CONNECTION FAILED **");
      Locate(1, 5);
      Print("Server was not found!");
      Locate(1, 6);
      Print("Check conectivity and try again!");
      sleep(2);
      ClsCC();
      return 0;
    }
  }

  showLineStatus("** CONNECTED, PRESS ANY KEY TO START **");
  Beep();
  do
  {
    ucTxData = Inkey();
  } while (!ucTxData);

  prepareScreen();

  autoWaitCount = 0;

  do
  {
    if (emulatorMode == 0)
    {
      // UNAPI Breathing just in case adapter need it
      Breath();
    }
    ucTxData = Inkey();

    if (ucTxData ||
        (autoUpdate == 1 && (autoWaitCount > AUTO_WAIT_CYCLES)))
    {

      if (autoWaitCount > AUTO_WAIT_CYCLES)
      {
        // Auto update command
        ucTxData = 'U';
        autoWaitCount = 0;
      }

      if (interpretKey(ucConnNumber, ucTxData))
      {
        if (emulatorMode == 0)
        {
          uiGetSize = RcvMemorySize;
          if (RXData(ucConnNumber, ucRcvDataMemory, &uiGetSize, 0))
          {
            // Data received?
            if (uiGetSize)
            {
              parseReturnData(ucRcvDataMemory, uiGetSize);
            }
          }
        }
        else
        {
          uiGetSize = strlen(emulatedString);
          strcpy(ucRcvDataMemory, emulatedString);
          parseReturnData(ucRcvDataMemory, uiGetSize);
        }
      }
    }

    // Exit function
    if (ucTxData == 'Q' || ucTxData == 'q')
    {
      Beep();
      prepareScreen();
      Locate(1, 2);
      Print("Bye! :)");
      sleep(1);
      ClsCC();
      Locate(0, 0);
      break;
    }

    if (autoUpdate == 1)
    {
      autoWaitCount++;
    }

  } while (1);

  if (emulatorMode == 0)
  {
    CloseConnection(ucConnNumber);
  }

  return 0;
}

//*******************************************************
// FUNCTIONS
//*******************************************************

void sleep(int times)
{
  int count = 0;
  char modCount = 0;

  if (times < 0)
  {
    return;
  }

  for (count = 0; count < AUTO_WAIT_CYCLES; count++)
  {
    // wait
    LocateCC(0, 22);
    modCount = (char)count % 4;
    switch (modCount)
    {
    case 1:
      PrintInverted("W   . . .");
      break;
    case 2:
      PrintInverted(" a  . .  ");
      break;
    case 3:
      PrintInverted("  i .    ");
      break;
    default:
      PrintInverted("   t     ");
      break;
    }
  }

  times = times - 1;
  sleep(times);
}

void beep()
{
  if (beeps == 1)
  {
    Beep();
  }
}

void prepareScreen()
{
  ClsCC();
  //             1234567890123456789012345678901234567890
  LocateCC(0, 0);
  PrintInverted(" FT8MSXClient                   v0.6.0  ");
  for (char y = 1; y < __CHARMAP_SCREEN_LINES__ - 1; y++)
  {
    Locate(0, y);
    PrintChar(__CHARMAP_GRPBOX_COLUMN_);
    Locate(__CHARMAP_SCREEN_WIDTH__, y);
    PrintChar(__CHARMAP_GRPBOX_COLUMN_);
  }
  clearLineStatus();
}

void sendCommand(unsigned char ucConnNumber, unsigned char *command)
{
  int size = strlen(command);
  unsigned char i = 0;

  for (i = 0; i < size; i++)
  {
    if (emulatorMode == 0)
    {
      TxByte(ucConnNumber, command[i]);
    }
  }
  beep();
}

unsigned char interpretKey(unsigned char ucConnNumber, unsigned char char2Send)
{

  clearLineStatus();

  if (char2Send == 'p' || char2Send == 'P')
  {

    if (scrolling == 1)
    {

      scrolling = 0;
      showLineStatus("** PAGE SCROLL DISABLED **");
      Beep();
    }
    else
    {

      scrolling = 1;
      showLineStatus("** PAGE SCROLL ENABLED **");
      Beep();
    }
    return 0;
  }

  if (char2Send == 'r' || char2Send == 'R')
  {

    prepareScreen();
    Beep();
    return 0;
  }

  if (char2Send == 'b' || char2Send == 'B')
  {

    if (beeps == 1)
    {

      beeps = 0;
      showLineStatus("** BEEPS DISABLED **");
    }
    else
    {

      beeps = 1;
      showLineStatus("** BEEPS ENABLED **");
      Beep();
    }

    return 0;
  }

  if (char2Send == 'a' || char2Send == 'A')
  {

    if (autoUpdate == 1)
    {

      autoUpdate = 0;
      showLineStatus("** AUTO UPDATE DISABLED **");
      autoWaitCount = 0;
      Beep();
    }
    else
    {

      autoUpdate = 1;
      showLineStatus("** AUTO UPDATE ENABLED **");
      autoWaitCount = 0;
      Beep();
    }

    return 0;
  }

  if (char2Send == 'w' || char2Send == 'W')
  {

    sendCommand(ucConnNumber, "wipe\n\r");

    showLineStatus("** WIPE LIST REQUESTED **");
    Beep();

    return 0;
  }

  if (char2Send == 'c' || char2Send == 'C')
  {

    if (cqOnly == 0)
    {

      cqOnly = 1;
      sendCommand(ucConnNumber, "cqonlyenabled\n\r");

      showLineStatus("** CQ ONLY FILTER ENABLED **");
      Beep();

      return 0;
    }

    if (cqOnly == 1)
    {

      cqOnly = 0;
      sendCommand(ucConnNumber, "cqonlydisabled\n\r");

      showLineStatus("** CQ ONLY FILTER DISABLED **");
      Beep();

      return 0;
    }
  }

  if (char2Send == 'u' || char2Send == 'U')
  {

    sendCommand(ucConnNumber, "logs\n\r");
    return 1;
  }
  else
  {

    showLineStatus("** INVALID COMMAND KEY **");
    Beep();
  }
  return 0;
}

void parseReturnData(unsigned char *bufferData, unsigned int bufferSize)
{
  bufferData[bufferSize] = '\0';

  if (strcmp(bufferData, "EMPTY\n\r") == 0)
  {

    clearLineStatus();
    showLineStatus("** NO NEW ENTRIES! **");
    beep();
    return;
  }
  else if (strcmp(bufferData, "\n\r") == 0 ||
           strcmp(bufferData, " ") == 0 ||
           strcmp(bufferData, "") == 0)
  {

    clearLineStatus();
    showLineStatus("** NO DATA RECEIVED! **");
    beep();
    return;
  }
  else
  {

    clearLineStatus();

    parseFT8RecData(bufferData, bufferSize);
  }
}

void printTableHeader()
{
  Locate(1, 1);
  //     1234567890123456789012345678901234567890
  Print("dB   dT  Freq CQ TO       FROM   GSRST");
  //     ---+----+----+--+------+------+-----
  Locate(0, 2);
  char tableLine[] = {__CHARMAP_GRPBOX_COLUMN_LEFT_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_T_TOP_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_T_TOP_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_T_TOP_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_T_TOP_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_T_TOP_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_T_TOP_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_LINE_,
                      __CHARMAP_GRPBOX_COLUMN_RIGHT_,
                      '\0'};
  Print(tableLine);
  for (char l = 3; l < 22; l++)
  {
    Locate(4, l);
    PrintChar(__CHARMAP_GRPBOX_COLUMN_);
    Locate(9, l);
    PrintChar(__CHARMAP_GRPBOX_COLUMN_);
    Locate(14, l);
    PrintChar(__CHARMAP_GRPBOX_COLUMN_);
    Locate(17, l);
    PrintChar(__CHARMAP_GRPBOX_COLUMN_);
    Locate(26, l);
    PrintChar(__CHARMAP_GRPBOX_COLUMN_);
    Locate(34, l);
    PrintChar(__CHARMAP_GRPBOX_COLUMN_);
  }
}

void cleanLineTable(int line)
{
  Locate(1, line);
  Print("   ");
  Locate(5, line);
  Print("    ");
  Locate(10, line);
  Print("    ");
  Locate(15, line);
  Print("  ");
  Locate(18, line);
  Print("        ");
  Locate(27, line);
  Print("      ");
  Locate(35, line);
  Print("    ");
}

void parseFT8RecData(unsigned char *bufferData, unsigned int bufferSize)
{

  // prepareScreen();
  char *field = strtok(bufferData, ";");
  char fmtedField[16];
  char tmpCompare[5];

  fmtedField[0] = '\0';
  int line = 3;
  char isCQ = 0;

  printTableHeader();

  while (field != 0)
  {
    cleanLineTable(line);
    if (line % 21 == 0 && scrolling == 1)
    {

      prepareScreen();
      printTableHeader();
      line = 3;
      Locate(1, line);
    }
    else if (line % 21 == 0)
    {

      break;
    }

    // Timestamp(ignored)
    field = strtok(NULL, ";");

    if (field != 0)
    {

      // Signal Status
      sprintf(fmtedField, "%s", field);
      Locate(1, line);
      Print(fmtedField);

      field = strtok(NULL, ";");
    }

    if (field != 0)
    {

      // dBM
      sprintf(fmtedField, "%s", field);
      Locate(5, line);
      Print(fmtedField);

      field = strtok(NULL, ";");
    }

    if (field != 0)
    {

      // Frequency
      sprintf(fmtedField, "%s", field);
      Locate(10, line);
      Print(fmtedField);

      field = strtok(NULL, ";");
    }

    if (field != 0)
    {

      // Souce Call Sign or CQ
      sprintf(fmtedField, "%s", field);
      strncpy(tmpCompare, fmtedField, 3);

      if (strcmp(tmpCompare, "CQ") == 0)
      {

        Locate(15, line);
        Print("CQ");
        isCQ = 1;
      }
      else
      {
        Locate(18, line);
        Print(fmtedField);
      }
      field = strtok(NULL, ";");
    }

    if (field != 0)
    {

      // Destination Call Sign or from CQ
      if (isCQ == 1)
      {

        Locate(27, line);
        sprintf(fmtedField, "%s", field);
        Print(fmtedField);
      }
      else
      {

        sprintf(fmtedField, "%s", field);
        Locate(27, line);
        Print(fmtedField);
      }

      field = strtok(NULL, ";");
    }

    if (field != 0)
    {

      // Grid Square Locator or Rec Signal
      sprintf(fmtedField, "%s", field);

      Locate(35, line);
      Print(fmtedField);

      field = strtok(NULL, ";");
    }

    beep();
    line++;
    isCQ = 0;
  }

  clearLineStatus();
  sleep(2);
}

void showLineStatus(char *status)
{
  LocateCC(0, 22);
  PrintInverted(status);
}

void clearLineStatus()
{
  LocateCC(0, 22);
  //             1234567890123456789012345678901234567890
  PrintInverted("                                        ");
}
