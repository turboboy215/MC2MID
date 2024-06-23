/*Mark Cooksey (GB/GBC) to TXT converter*/
/*By Will Trowbridge*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define bankSize 16384

unsigned short ReadLE16(unsigned char* Data);

FILE *rom, *txt;
long bank;
char offsetString[100];
long offset;
long tablePtrLoc;
long tableOffset;
long macroPtrLoc;
long macroOffset;
long sfxPtrLoc;
long sfxOffset;
int i;
char outfile[1000000];
const int MagicBytes[10] = { 0x6F, 0x26, 0x00, 0x29, 0x54, 0x5D, 0x29, 0x29, 0x19, 0x11 };
const int MacroFindOld[13] = { 0xE1, 0x11, 0xFE, 0xFF, 0x19, 0x3E, 0x01, 0x22, 0x03, 0x0A, 0xCB, 0x27, 0x11 };
const int MacroFindNew[5] = { 0xCB, 0x27, 0x30, 0x06, 0x11 };
const int SFXFind[5] = { 0xCB, 0x27, 0x85, 0x6F, 0x30 };
unsigned long macroList[500];
unsigned long macroListOrd[500][2];
long ptrs[4];
long stepPtr;
long nextPtr;
long endPtr;
int songNum;
long bankAmt;
int highestMacro = 1;
unsigned static char* romData;

/*Function prototypes*/
void song2txt(int songNum, long ptrList[4], long nextPtr);

void getMacroList(unsigned long list[], long offset, long sfxTable);
void orderMacroList(unsigned long list1[], unsigned long list2[500][2]);

/*Convert little-endian pointer to big-endian*/
unsigned short ReadLE16(unsigned char* Data)
{
	return (Data[0] << 0) | (Data[1] << 8);
}


int main(int args, char* argv[])
{
	printf("Mark Cooksey (GB/GBC) to TXT converter\n");
	if (args != 3)
	{
		printf("Usage: MC2TXT <rom> <bank>");
		return -1;
	}
	else
	{
		if ((rom = fopen(argv[1], "rb")) == NULL)
		{
			printf("ERROR: Unable to open file %s!\n", argv[1]);
			exit(1);
		}
		else
		{
			bank = strtol(argv[2], NULL, 16);
			if (bank != 1)
			{
				bankAmt = bankSize;
			}
			else
			{
				bankAmt = 0;
			}
		}
		fseek(rom, ((bank - 1) * bankSize), SEEK_SET);
		romData = (unsigned char*)malloc(bankSize);
		fread(romData, 1, bankSize, rom);
		fclose(rom);

		/*Try to search the bank for song table loader*/
		for (i = 0; i < bankSize; i++)
		{
			if (!memcmp(&romData[i], MagicBytes, 10))
			{
				tablePtrLoc = bankAmt + i + 10;
				printf("Found pointer to song table at address 0x%04x!\n", tablePtrLoc);
				tableOffset = ReadLE16(&romData[tablePtrLoc - bankAmt]);
				printf("Song table starts at 0x%04x...\n", tableOffset);
				break;
			}
		}

		/*Search for sound effects table*/
		for (i = 0; i < bankSize; i++)
		{
			if (!memcmp(&romData[i], SFXFind, 5))
			{
				sfxPtrLoc = bankAmt + i - 2;
				printf("Found pointer to sound effects table at address 0x%04x!\n", sfxPtrLoc);
				sfxOffset = ReadLE16(&romData[sfxPtrLoc - bankAmt]);
				printf("Sound effects table starts at 0x%04x...\n", sfxOffset);
				break;
			}
		}

		/*Now try to search the bank for macro table loader*/
		for (i = 0; i < bankSize; i++)
		{
			/*First, try old method (games before 1999)*/
			if (!memcmp(&romData[i], MacroFindOld, 13))
			{
				macroPtrLoc = bankAmt + i + 13;
				printf("Found pointer to macro table at address 0x%04x!\n", macroPtrLoc);
				macroOffset = ReadLE16(&romData[macroPtrLoc - bankAmt]);
				printf("Macro table starts at 0x%04x...\n", macroOffset);
				getMacroList(macroList, macroOffset, sfxOffset);
				break;
			}
			
			/*Now try new method (games from 1999-)*/
			else if (!memcmp(&romData[i], MacroFindNew, 5))
			{
				macroPtrLoc = bankAmt + i + 5;
				printf("Found pointer to macro table at address 0x%04x!\n", macroPtrLoc);
				macroOffset = ReadLE16(&romData[macroPtrLoc - bankAmt]);
				printf("Macro table starts at 0x%04x...\n", macroOffset);
				getMacroList(macroList, macroOffset, sfxOffset);
				break;
			}
		}
		if (tableOffset != NULL)
		{
			songNum = 1;
			i = tableOffset;
			while ((nextPtr = ReadLE16(&romData[i + 10 - bankAmt])) >= ptrs[0] && (nextPtr = ReadLE16(&romData[i + 10 - bankAmt])) != 9839)
			{
				ptrs[0] = ReadLE16(&romData[i - bankAmt]);
				printf("Song %i channel 1: 0x%04x\n", songNum, ptrs[0]);
				ptrs[1] = ReadLE16(&romData[i + 2 - bankAmt]);
				printf("Song %i channel 2: 0x%04x\n", songNum, ptrs[1]);
				ptrs[2] = ReadLE16(&romData[i + 4 - bankAmt]);
				printf("Song %i channel 3: 0x%04x\n", songNum, ptrs[2]);
				ptrs[3] = ReadLE16(&romData[i + 6 - bankAmt]);
				printf("Song %i channel 4: 0x%04x\n", songNum, ptrs[3]);
				stepPtr = ReadLE16(&romData[i + 8 - bankAmt]);
				printf("Song %i step table: 0x%04x\n", songNum, stepPtr);
				endPtr = nextPtr - bankAmt;
				song2txt(songNum, ptrs, endPtr);
				i += 10;
				songNum++;
			}
			ptrs[0] = ReadLE16(&romData[i - bankAmt]);
			printf("Song %i channel 1: 0x%04x\n", songNum, ptrs[0]);
			ptrs[1] = ReadLE16(&romData[i + 2 - bankAmt]);
			printf("Song %i channel 2: 0x%04x\n", songNum, ptrs[1]);
			ptrs[2] = ReadLE16(&romData[i + 4 - bankAmt]);
			printf("Song %i channel 3: 0x%04x\n", songNum, ptrs[2]);
			ptrs[3] = ReadLE16(&romData[i + 6 - bankAmt]);
			printf("Song %i channel 4: 0x%04x\n", songNum, ptrs[3]);
			stepPtr = ReadLE16(&romData[i + 8 - bankAmt]);
			printf("Song %i step table: 0x%04x\n", songNum, stepPtr);
			endPtr = bankSize;
			song2txt(songNum, ptrs, endPtr);

		}
		else
		{
			printf("ERROR: Magic bytes not found!\n");
			exit(-1);
		}
		printf("The operation was successfully completed!\n");
	}
}

/*Convert the song data to TXT*/
void song2txt(int songNum, long ptrList[4], long nextPtr)
{
	unsigned char command[4];
	unsigned int curPos;
	unsigned int curChan;
	int hadNoteYet = 0;
	int hadMacroYet = 0;
	unsigned char lowNibble;
	unsigned char highNibble;
	int curMacro = 1;
	int transpose;
	int end = 0;
	int j;

	sprintf(outfile, "song%d.txt", songNum);
	if ((txt = fopen(outfile, "wb")) == NULL)
	{
		printf("ERROR: Unable to write to file song%d.txt!\n", songNum);
		exit(2);
	}
	else
	{
		curChan = 1;
		curPos = ptrList[0]-bankAmt;
		fprintf(txt, "Channel 1:\n");
		command[0] = romData[curPos];
		command[1] = romData[curPos + 1];
		command[2] = romData[curPos + 2];
		command[3] = romData[curPos + 3];
		while (curPos < nextPtr && end == 0)
		{
			command[0] = romData[curPos];
			command[1] = romData[curPos + 1];
			command[2] = romData[curPos + 2];
			command[3] = romData[curPos + 3];
			if (curPos == ptrList[1]-bankAmt)
			{
				fprintf(txt, "\nChannel 2:\n");
			}
			if (curPos == ptrList[2] - bankAmt)
			{
				fprintf(txt, "\nChannel 3:\n");
			}
			if (curPos == ptrList[3] - bankAmt)
			{
				fprintf(txt, "\nChannel 4:\n");
			}
			for (j = 0; j < 500; j++)
			{
				if (curPos == macroList[j])
				{
					if (j <= highestMacro)
					{
						fprintf(txt, "\nMacro %i:\n", j + 1);
						curMacro = j + 1;
					}
				}
			}
			switch (command[0])
			{
			case 0x60:
				lowNibble = (command[1] >> 4);
				highNibble = (command[1] & 15);
				fprintf(txt, "0x60: Rest - Instrument: %i, Length: %i\n", lowNibble, highNibble);
				curPos+=2;
				break;
			case 0x61:
				fprintf(txt, "0x61: Silence/pause channel\n");
				curPos++;
				break;
			case 0x62:
				fprintf(txt, "0x62: Go to track loop point: 0x%04x\n", (ReadLE16(&romData[curPos + 1])));
				curPos+=3;
				break;
			case 0x63:
				fprintf(txt, "0x63: Change noise channel type? Parameter: %01x\n", (ReadLE16(&romData[curPos+1])));
				curPos+=2;
				break;
			case 0x64:
				transpose = command[2];
				if (transpose > 127)
				{
					transpose = transpose + -256;
				}
				fprintf(txt, "0x64: Call macro:\nMacro %i, transpose value: %d, Repeat times: %i\n", (command[1] + 1), transpose, command[3]);
				curPos+=4;
				break;
			case 0x65:
				fprintf(txt, "0x65: End of macro\n");
				if (curMacro != highestMacro)
				{
					curPos++;
				}
				else
				{
					end = 1;
				}
				break;
			case 0x66:
				fprintf(txt, "0x66: Set 'has played' flag to %01x\n", command[1]);
				curPos+=2;
				break;
			case 0x67:
				fprintf(txt, "0x67: Set all panning: %01x\n", command[1]);
				curPos+=2;
				break;
			case 0x68:
				fprintf(txt, "0x68: Set step table: %01x\n", (ReadLE16(&romData[curPos + 1])));
				curPos+=3;
				break;
			case 0x69:
				fprintf(txt, "0x69: Set song tempo: %i\n", command[1]);
				curPos+=2;
				break;
			case 0x6A:
				fprintf(txt, "0x6A: Set channel 1 panning: %01x\n", command[1]);
				curPos+=2;
				break;
			case 0x6B:
				fprintf(txt, "0x6B: Set channel 2 panning: %01x\n", command[1]);
				curPos += 2;
				break;
			case 0x6C:
				fprintf(txt, "0x6C: Set channel 3 panning: %01x\n", command[1]);
				curPos += 2;
				break;
			case 0x6D:
				fprintf(txt, "0x6D: Set channel 4 panning: %01x\n", command[1]);
				curPos += 2;
				break;
			default:
				lowNibble = (command[1] >> 4);
				highNibble = (command[1] & 15);
				fprintf(txt, "Note value: %i, Instrument: %i, Length: %i\n", command[0], lowNibble, highNibble);
				curPos += 2;
				break;

			}
		}
		fclose(txt);
	}
}

/*Get all the macro pointers from the table, and try to find the end of the table*/
void getMacroList(unsigned long list[], long offset, long sfxTable)
{
	int j;
	unsigned long curValue;
	unsigned long curValue2;
	unsigned long tempCurValue;
	long newOffset = offset;
	long offset2 = offset - bankAmt;
	long initialValue = (ReadLE16(&romData[newOffset - bankAmt]));
	
	for (j = 0; j < 500; j++)
	{
		curValue = (ReadLE16(&romData[newOffset-bankAmt]))-bankAmt;
		curValue2 = (ReadLE16(&romData[newOffset - bankAmt]));
		if (curValue2 != sfxTable && curValue2 >= bankAmt && curValue2 != 65535)
		{
			/*Workaround for Kirikou (and possibly other games with "**PLANET**" padding (i.e. games developed by Planet)*/
			if (curValue2 == 21573)
			{
				tempCurValue = (ReadLE16(&romData[newOffset+2 - bankAmt]));
				if (tempCurValue == 10794)
				{
					list[j] = NULL;
					highestMacro = j - 1;
					break;
				}
			}
			list[j] = curValue;
			newOffset += 2;
		}
		else
		{
			highestMacro = j;
			break;
		}


	}
}
