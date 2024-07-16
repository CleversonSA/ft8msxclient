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

#define  __SDK_MSXVERSION__ 1

//*******************************************************
// UNAPI DEFINITIONS
//*******************************************************
//IMPORTANT: You need to check the map compiler generates to make sure this
//address do not overlap functions, variables, etc
//UNAPI requires memory buffer @ 0x8000 or higher...
#define RcvMemorySize 1024
unsigned char ucConnNumber; //hold the connection number received by UnapiHelper
//For data receive parsing
unsigned char ucRcvData[128];
__at 0x8000 unsigned char ucRcvDataMemory[]; //area to hold data sent to UNAPI, need to be in the 3rd 16K block
unsigned int uiGetSize;
Z80_registers regs; //auxiliary structure for asm function calling


//*******************************************************
// GLOBAL DEFINITIONS
//*******************************************************
#define AUTO_WAIT_CYCLES 512
unsigned char autoUpdate = 1;
unsigned char beeps = 1;
unsigned char scrolling = 1;
unsigned char cqOnly = 0;
unsigned int  autoWaitCount = 0;

//*******************************************************
// PROTOTYPES
//*******************************************************
void prepareScreen();
unsigned char connectServer(unsigned char *, unsigned char *);
void sendCommand(unsigned char, unsigned char *);
unsigned char interpretKey(unsigned char, unsigned char);
void parseReturnData(unsigned char*, unsigned int);
void parseFT8RecData(unsigned char*, unsigned int);
void beep();
void clearLineStatus();

//*******************************************************
// MAIN
//*******************************************************
int main(char** argv, int argc) 
{
  char ucTxData = 0; //where our key inputs go
	unsigned char ucRet; //return of functions
	unsigned char ucServer[128]; //will hold the name of the server we will connect
	unsigned char ucPort[6]; //will hold the port that the server accepts connections
  unsigned char ucAliveConnCount = 0; //when this is 0, check if connection is alive
  char chTextLine[128];

  uiGetSize = 0; //no bytes received yet
  
  prepareScreen();
  
  if (!InitializeTCPIP())
  {
    Print("** NO TCP STACK FOUND **\n\r");
    Print("Please start TCP stack first\n\r");
    return -1;
  }


	if (argc == 0) {
  	strcpy(ucServer,"ft8pi.local");
	}	

	if (argc != 0) {
		strcpy(ucServer,(unsigned char*)argv[0]);
  	strcpy(ucPort,(unsigned char*)argv[1]);
	} else {
		strcpy(ucPort,"6666");
	}

	Print("Trying connection...");
	Print(ucServer);
	Print(ucPort);

  ucRet = OpenSingleConnection (ucServer, ucPort, &ucConnNumber);

  if (ucRet != ERR_OK && IsConnected(ucConnNumber) == '0')
  {
    Print("** CONNECTION FAILED **\n\r");
    Print("Server was not found, check conectivity and try again!\n\r");
    return -1;
  }

  Print("\r\n** CONNECTED, PRESS ANY KEY TO START **\r\n");
  Beep();
  do {
    ucTxData = Inkey();
  } while(!ucTxData);

  prepareScreen();

  autoWaitCount = 0;

  do {
    //UNAPI Breathing just in case adapter need it
    Breath();
    ucTxData = Inkey ();

    if (ucTxData || 
        (autoUpdate == 1 && (autoWaitCount > AUTO_WAIT_CYCLES))) {
        
        if (autoWaitCount > AUTO_WAIT_CYCLES) {
            //Auto update command
            ucTxData = 'U';
            autoWaitCount = 0;
        }

       if (interpretKey(ucConnNumber, ucTxData)) {
        
          uiGetSize = RcvMemorySize;
          if (RXData(ucConnNumber, ucRcvDataMemory, &uiGetSize,0))
          {
            //Data received?
            if(uiGetSize)
            {
              parseReturnData(ucRcvDataMemory, uiGetSize);
            }
          }
          
      }
    } 

    //Exit function
    if (ucTxData == 'Q' || ucTxData == 'q') 
    {
      Beep();
      prepareScreen();
      Print("Bye! :)\n\r");
      break;
    }

    if (autoUpdate == 1) {
      
      autoWaitCount++;
    
    }

  }
  while(1);

  CloseConnection(ucConnNumber);

  return 0;
}
 

//*******************************************************
// FUNCTIONS
//*******************************************************
void beep()
{
  if (beeps == 1) {
    Beep();
  }
}

void prepareScreen()
{
  Cls();
  //       1234567890123456789012345678901234567890
  Print("\fFT8MSXClient                     v0.5.2\r\n");
  Print(  "---------------------------------------\r\n");
}


void sendCommand(unsigned char ucConnNumber, unsigned char *command)
{
  int size = strlen(command);
  unsigned char i = 0;

  for (i = 0; i<size; i++)
  {
    TxByte (ucConnNumber, command[i]);
  }
  beep();
}


unsigned char interpretKey(unsigned char ucConnNumber, unsigned char char2Send)
{

   clearLineStatus();

	 if (char2Send == 'p' || char2Send == 'P') {
			
			if (scrolling == 1) {
				
				scrolling = 0;			
				Locate(1,22);
				Print ("** PAGE SCROLL DISABLED **");
				Beep();

			} else {
				
				scrolling = 1;
				Locate(1,22);
				Print("** PAGE SCROLL ENABLED **");
				Beep();

			}
			return 0;

	 }


	 if (char2Send == 'r' || char2Send == 'R') {

			prepareScreen();
			Beep();
			return 0;

	 }

   
   if (char2Send == 'b' || char2Send == 'B') {

      if (beeps == 1) {
        
        beeps = 0;
        Locate(1,22);
        Print("** BEEPS DISABLED **");

      } else {

        beeps = 1;
        Locate(1,22);
        Print("** BEEPS ENABLED **");
        Beep();

      }

      return 0;

   }

   if (char2Send == 'a' || char2Send == 'A') {

      if (autoUpdate == 1) {
        
        autoUpdate = 0;
        Locate(1,22);
        Print("** AUTO UPDATE DISABLED **");
        autoWaitCount = 0;
        Beep();

      } else {

        autoUpdate = 1;
        Locate(1,22);
        Print("** AUTO UPDATE ENABLED **");
        autoWaitCount = 0;
        Beep();

      }

      return 0;

   }

   if (char2Send == 'w' || char2Send == 'W') {

      sendCommand(ucConnNumber, "wipe\n\r");
      
      Locate(1,22);
      Print("** WIPE LIST REQUESTED **");
      Beep();

      return 0;

   }

   if (char2Send == 'c' || char2Send == 'C') {

      if (cqOnly == 0) {

        cqOnly = 1;
        sendCommand(ucConnNumber, "cqonlyenabled\n\r");
        
        Locate(1,22);
        Print("** CQ ONLY FILTER ENABLED **");
        Beep();

        return 0;      
      }


      if (cqOnly == 1) {

        cqOnly = 0;
        sendCommand(ucConnNumber, "cqonlydisabled\n\r");
        
        Locate(1,22);
        Print("** CQ ONLY FILTER DISABLED **");
        Beep();

        return 0;
      }

   }

   if (char2Send == 'u' || char2Send == 'U') {

      sendCommand(ucConnNumber, "logs\n\r");
      return 1;

   } else {

      Locate(1,22);
      Print("** INVALID COMMAND KEY **");
      Beep();

   }
   return 0;
}


void parseReturnData(unsigned char* bufferData, unsigned int bufferSize)
{
  bufferData[bufferSize] = '\0';

  if (strcmp(bufferData, "EMPTY\n\r") == 0) {
  
    clearLineStatus();
    Locate(1,22);
    Print("** NO NEW ENTRIES! **");
    beep();
    return;

  } else if (strcmp(bufferData, "\n\r") == 0 || 
              strcmp(bufferData, " ") == 0 ||
              strcmp(bufferData, "") == 0)
  {
  
    clearLineStatus();
    Locate(1,22);
    Print("** NO DATA RECEIVED! **");
    beep();
    return;
  
  } else {

    clearLineStatus();
    
    parseFT8RecData(bufferData, bufferSize);

  }



}


void parseFT8RecData(unsigned char*bufferData, unsigned int bufferSize) {

  //prepareScreen();
  char *field = strtok(bufferData,";");
  char fmtedField[16];
	char tmpCompare[5];

  fmtedField[0] = '\0';
  int line = 5;
	char isCQ = 0;

  Locate(0,3);
  //     1234567890123456789012345678901234567890
  Print("dB    dT  Freq CQ FROM    TO      GS-RST");
  Locate(0,4);
  Print("---+-----+----+--+-------+-------+------");

  while (field != 0)
  {
    Locate(0,line);
    //     1234567890123456789012345678901234567890
    Print("                                        ");

    if (line % 21 == 0 && scrolling == 1) {

      prepareScreen();
      Locate(0,3);
      //     0123456789012345678901234567890123456789
      Print("dB    dT  Freq CQ FROM    TO      GS-RST");
			Locate(0,4);
			Print("---+-----+----+--+-------+-------+------");
      line = 5;
      Locate(1,line);

    } else if (line % 21 == 0) {

			break;

		}

    //Timestamp(ignored)
    field = strtok(NULL, ";");

    if (field != 0)
    {
      
      //Signal Status
      sprintf(fmtedField, "%s", field);
      Locate(0,line);
      Print(fmtedField);
      Locate(3,line);
      Print("|");

      field = strtok(NULL, ";");

    }

    if (field != 0)
    {
      
      //dBM
      sprintf(fmtedField, "%s", field);
      Locate(4,line);
      Print(fmtedField);
      Locate(9,line);
      Print("|");

      field = strtok(NULL, ";");

    }

    if (field != 0)
    {
      
      //Frequency
      sprintf(fmtedField, "%s", field);
      Locate(10,line);
      Print(fmtedField);
      Locate(14,line);
      Print("|");
      
      field = strtok(NULL, ";");

    }

    if (field != 0)
    {
      
      //Souce Call Sign or CQ
      sprintf(fmtedField, "%s", field);
			strncpy(tmpCompare, fmtedField,3);
			
			if (strcmp(tmpCompare, "CQ") == 0) {
				
				Locate(15,line);
				Print("CQ");
				Locate(17,line);
				Print("|");
				Locate(25,line);
				Print("|");
				isCQ = 1;

			} else {
 
				Locate(17,line);
				Print("|");
 	    	Print(fmtedField);
 	  	  Locate(25,line);
  	    Print("|");

			}
      field = strtok(NULL, ";");

    }


    if (field != 0)
    {
      
      //Destination Call Sign or from CQ
      if (isCQ == 1) {
				
				Locate(18,line);
				sprintf(fmtedField, "%s", field);
				Print(fmtedField);
				Locate(33,line);
				Print("|");

			} else {

	      sprintf(fmtedField, "%s", field);
  	    Locate(26,line);
    	  Print(fmtedField);
      	Locate(33,line);
      	Print("|");

			}

      field = strtok(NULL, ";");

    }

    if (field != 0)
    {
      
      //Grid Square Locator or Rec Signal
      sprintf(fmtedField, "%s", field);
      
      Locate(34,line);
      Print(fmtedField);
      Locate(39,line);
      Print("|");

      field = strtok(NULL, ";");

    }

    beep();
    line++;
		isCQ=0;
  }

}


void clearLineStatus()
{
  Locate(0,22);
  //     1234567890123456789012345678901234567890
  Print("                                       ");
}
