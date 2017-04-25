/*--------------------------------------------------------------------------*/
// A simple cross-platform console application to test the glove
//
// WIN32 must be defined when compiling for Windows.
// For Visual C++ this is normally already defined.
//
// Copyright (C) 2000, 5DT <Fifth Dimension Technologies>
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#ifdef WIN32
#include <windows.h> // for Sleep
#else
//#include <unistd.h>  // for usleep
#endif
#include "../../fglove.h"
/*--------------------------------------------------------------------------*/
char *szPort = NULL;
char szPortToOpen[6];
fdGlove *pGlove = NULL;
bool showraw = false;
int glovetype = FD_GLOVENONE;
std::ofstream myfile;

int openGlove() {
	szPort = "COM1";
	strcpy(szPortToOpen, szPort);
	// open glove
	printf("Attempting to open glove on  %s .. ", szPortToOpen);
	if (NULL == (pGlove = fdOpen(szPortToOpen)))
	{
		printf("failed.\n");
		return -1;
	}
	printf("succeeded.\n");

	char *szType = "?";
	glovetype = fdGetGloveType(pGlove);
	switch (glovetype)
	{
	case FD_GLOVENONE: szType = "None"; break;
	case FD_GLOVE7:    szType = "Glove7"; break;
	case FD_GLOVE7W:   szType = "Glove7W"; break;
	case FD_GLOVE16:   szType = "Glove16"; break;
	case FD_GLOVE16W:  szType = "Glove16W"; break;
	case FD_GLOVE5U: szType = "DG5 Ultra serial"; break;
	case FD_GLOVE5UW: szType = "DG5 Ultra serial, wireless"; break;
	case FD_GLOVE5U_USB: szType = "DG5 Ultra USB"; break;
	case FD_GLOVE14U: szType = "DG14 Ultra serial"; break;
	case FD_GLOVE14UW: szType = "DG14 Ultra serial, wireless"; break;
	case FD_GLOVE14U_USB: szType = "DG14 Ultra USB"; break;
	}
	printf("glove type: %s\n", szType);
	printf("glove handedness: %s\n", fdGetGloveHand(pGlove) == FD_HAND_RIGHT ? "Right" : "Left");
	int iNumSensors = fdGetNumSensors(pGlove);
	printf("glove num sensors: %d\n", iNumSensors);
	return 0;
}

void writeRawDataToFile() {

}

void writeScaledDataToFile() {
	//TODO: write multiple lines
	myfile.open("data.csv", std::fstream::app);
	myfile << fdGetSensorScaled(pGlove, FD_THUMBNEAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_INDEXNEAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_INDEXFAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_INDEXMIDDLE) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_MIDDLENEAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_MIDDLEFAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_MIDDLERING) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_RINGNEAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_RINGFAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_RINGLITTLE) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_LITTLENEAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_LITTLEFAR) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_THUMBPALM) << ",";
	myfile << fdGetSensorScaled(pGlove, FD_WRISTBEND);
	myfile << "\n";
	myfile.close();
}

int closeGlove() {
	// Close glove
	printf("closing glove\n");
	fdClose(pGlove);
	printf("glove closed, goodbye\n");
	return 0;
}
/*
int main( int argc, char** argv )
{
	char    *szPort    = NULL;
	char	szPortToOpen[6];
	fdGlove *pGlove    = NULL;
	bool     showraw   = false;
	int      glovetype = FD_GLOVENONE;
	int      i;

	if (argc<2)
	{
		printf( "Usage: testglove <devicename> -r\n" );
		printf( "-r\tShow raw values instead of scaled\n");
	}
	else
	{
		// handle command line arguments
		for ( i=1; i<argc; i++ )
		{
			if (!strcmp(argv[i], "-r"))
				showraw = true;
			else
				szPort = "COM1";
		}
	}
	if (!szPort)
	{
#ifdef WIN32
		szPort = "COM1";
#else
		szPort = "/dev/fglove";
#endif
	}
	strcpy(szPortToOpen,szPort);
	if (strcmp(szPort,"USB") == 0)
	{
		unsigned short aPID[5];
		int nNumFound = 5;
		int nChosen = 0;
		fdScanUSB(aPID,nNumFound);
		for (int c = 0; c < nNumFound; c++)
		{
			printf("Available USB Gloves:\n");
			printf("%i - ",c);
			switch (aPID[c])
			{
			case DG14U_R:
				printf("Data Glove 14 Ultra Right\n");
				break;
			case DG14U_L:
				printf("Data Glove 14 Ultra Left\n");
				break;
			case DG5U_R:
				printf("Data Glove 5 Ultra Right\n");
				break;
			case DG5U_L:
				printf("Data Glove 5 Ultra Left\n");
				break;
			default:
				printf("Unknown\n");
			}
		}
		printf("Please enter option:");
		scanf("%i",&nChosen);
		sprintf(szPortToOpen,"USB%i",nChosen);
		fdOpen(szPortToOpen);

	}

	// Initialize glove
	printf( "Attempting to open glove on %s .. ", szPortToOpen );
	if (NULL == (pGlove = fdOpen( szPortToOpen )))
	{
		printf( "failed.\n" );
		return -1;
	}
	printf( "succeeded.\n" );

	char *szType = "?";
	glovetype = fdGetGloveType(pGlove);
	switch (glovetype)
	{
	case FD_GLOVENONE: szType = "None"; break;
	case FD_GLOVE7:    szType = "Glove7"; break;
	case FD_GLOVE7W:   szType = "Glove7W"; break;
	case FD_GLOVE16:   szType = "Glove16"; break;
	case FD_GLOVE16W:  szType = "Glove16W"; break;
	case FD_GLOVE5U: szType = "DG5 Ultra serial"; break;
	case FD_GLOVE5UW: szType = "DG5 Ultra serial, wireless"; break;
	case FD_GLOVE5U_USB: szType = "DG5 Ultra USB"; break;
	case FD_GLOVE14U: szType = "DG14 Ultra serial"; break;
	case FD_GLOVE14UW: szType = "DG14 Ultra serial, wireless"; break;
	case FD_GLOVE14U_USB: szType = "DG14 Ultra USB"; break;
	}
	printf( "glove type: %s\n", szType );
	printf( "glove handedness: %s\n", fdGetGloveHand(pGlove)==FD_HAND_RIGHT?"Right":"Left" );
	int iNumSensors = fdGetNumSensors(pGlove);
	printf( "glove num sensors: %d\n", iNumSensors );
	// Display glove info
	unsigned char buf[64];
	
	printf( "glove info: %s\n", (char*)buf );

	for ( i=0; i<10000000; i++ )
	{
		if (showraw)
		{
			if (glovetype==FD_GLOVE7 || glovetype==FD_GLOVE7W)
			{
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_THUMBNEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_INDEXNEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_MIDDLENEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_RINGNEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_LITTLENEAR));
			}
			else
			{
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_THUMBNEAR));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_THUMBFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_THUMBINDEX));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_INDEXNEAR));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_INDEXFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_INDEXMIDDLE));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_MIDDLENEAR));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_MIDDLEFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_MIDDLERING));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_RINGNEAR));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_RINGFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_RINGLITTLE));
				printf("%4d,", (int)fdGetSensorRaw(pGlove,FD_LITTLENEAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_LITTLEFAR));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_THUMBPALM));
				printf("%4d ", (int)fdGetSensorRaw(pGlove,FD_WRISTBEND));
				
			}
			printf( "r=%4d,p=%4d",
				(int)fdGetSensorRaw(pGlove,FD_ROLL),
				(int)fdGetSensorRaw(pGlove,FD_PITCH));
		}
		else // show scaled values
		{
			if (glovetype==FD_GLOVE7 || glovetype==FD_GLOVE7W)
			{
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_THUMBNEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_INDEXNEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_MIDDLENEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_RINGNEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_LITTLENEAR));
			}
			else
			{
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_THUMBNEAR));
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_THUMBFAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_THUMBINDEX));
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_INDEXNEAR));
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_INDEXFAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_INDEXMIDDLE));
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_MIDDLENEAR));
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_MIDDLEFAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_MIDDLERING));
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_RINGNEAR));
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_RINGFAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_RINGLITTLE));
				printf("%.1f,", fdGetSensorScaled(pGlove,FD_LITTLENEAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_LITTLEFAR));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_THUMBPALM));
				printf("%.1f ", fdGetSensorScaled(pGlove,FD_WRISTBEND));
				
			}
			printf( "r=%.1f,p=%.1f",
				fdGetSensorScaled(pGlove,FD_ROLL),
				fdGetSensorScaled(pGlove,FD_PITCH));
		}
		printf(" %d\n", fdGetGesture(pGlove));

		//Sleep(15);

		//usleep(15000); // fixme

	}

	// Close glove
	printf( "closing glove\n" );
	fdClose( pGlove );
	printf( "glove closed, goodbye\n" );

	return 0;
}
*/