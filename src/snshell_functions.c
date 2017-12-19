/*

	main functions for snshell

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <time.h>

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

//time to wait between chars in printText() and printCenteredText()
#define SCREEN_CHAR_SET_TIME 20000

//default filename to check if installation is all fine
#define CHECK_FILE_NAME "/opt/snshell/snshell_data/text/version"

/*
	Prints some text sloooowly ;)
*/
void printText(char *dynamicText, int fast)
{
    int i;
    for (i = 0; dynamicText[i] != 0; i++){
        printf("%c", dynamicText[i]);

		if (fast != 1) {
           	usleep(SCREEN_CHAR_SET_TIME);
        }

        fflush (NULL);
    }
}

/*
	Prints some text sloooowly - and centerd
*/
void printCenteredText(char *dynamicText, int fast)
{
    int paddingRight = 0;

    if (snshell_getTerminalSize(1) >= strlen(dynamicText)) {
        paddingRight = (snshell_getTerminalSize(1) - strlen(dynamicText)) / 2;
        int i;
        for (i = 0; i < paddingRight; i++) {
            printText(" ", 1);
        }
    }

    printText(dynamicText, fast);
}

/*
	like it says; loads an file into the ram
*/
int snsell_misc_loadFileToMemory(const char *filename, char **result)
{
	int size = 0;
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		*result = NULL;
		return -1; // -1 means file opening fail
	}
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	*result = (char *)malloc(size+1);
	if (size != fread(*result, sizeof(char), size, f)) {
		free(*result);
		return -2; // -2 means file reading fail
	}
	fclose(f);
	(*result)[size] = 0;
	return size;
}

/*
	Loads an txt and prints it the retro way.
*/
void snsell_misc_displayText(const char *filename, int fast)
{
	char *content;
	int size;
	size = snsell_misc_loadFileToMemory(filename, &content);
	if (size < 0) 	{
			puts("Error loading file\n");
	} else {
		printText(content, 0);
		printText("\n", 1);
	}
}

/*
	'boot' sequence.
*/
int snshell_misc_check_os()
{
	int startUpOk = 1;

	char *ssh_connection;
    char *ssh_client;
    char *ssh_tty;
    char *ssh_user;

    int fast = 0;

    ssh_connection = getenv ("SSH_CONNECTION");
    ssh_client = getenv ("SSH_CLIENT");
    ssh_tty = getenv ("SSH_TTY");
    ssh_user = getenv("USER");

    printText("=== System Startup ===", fast);
    printText("\n", fast);
    printText("Checking Environment...\n", fast);

	/* !File System Check */
	if (startUpOk) {
	    printText(" - File System: ", 1);
	    char *tmp_content;
		int tmp_size;
		tmp_size = snsell_misc_loadFileToMemory(CHECK_FILE_NAME, &tmp_size);
		if (tmp_size < 0) 	{
			printText("FAILED!", fast);
			startUpOk = 0;
		} else {
			printText("PASSED", fast);
		}
	    printText("\n", 1);
	}

	if (startUpOk) {
		/* time and Date */

		time_t rawtime;
  		struct tm * timeinfo;
  		char buffer [80];

  		time ( &rawtime );
  		timeinfo = localtime ( &rawtime );
  		strftime (buffer,80,"%d.%m.%Y - %H:%M:%S",timeinfo);

	    printText(" - Local Time: ", 1);
        printText(buffer, fast);
		printText("\n", fast);

		/* Accestype and User */
	    printText(" - Access Type: ", 1);
        if (ssh_connection) {
            printText("REMOTE\n", fast);
            printText("    - Connection: ", fast);
            printText(ssh_connection, fast);
            printText("\n", fast);

            if (ssh_client) {
	            printText("    - Client: ", fast);
	            printText(ssh_client, fast);
	            printText("\n", fast);
	        }

	        if (ssh_tty) {
	            printText("    - Terminal: ", fast);
	            printText(ssh_tty, fast);
	            printText("\n", fast);
	        }
        } else {
            printText("LOCAL\n", fast);
        }

        if (ssh_user) {
            printText(" - Username: ", 1);
            printText(ssh_user, fast);
            printText("\n", fast);
        }
    }

    if (startUpOk) {
    	printText("\nStartup complete.\n", fast);
    }
    printText("\n", fast);

    return startUpOk;
}

/*
	substring wrapper
*/
char* snshell_misc_substring(const char* str, size_t begin, size_t len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
    return 0;

  return strndup(str + begin, len);
}

/*
	gets the dir of the application
*/
char snshell_getApplicationDir()
{
  char cCurrentPath[FILENAME_MAX];

  if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
    return 0;
  }

  cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
  return cCurrentPath;
}
