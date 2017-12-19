/*
	snshell application launcher
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

#include <sys/types.h>
#include <dirent.h>

#include <sys/stat.h>
#include <unistd.h>

// seconds to wait in sleepmode for a *
#define SS_SLEEPTIME 10

typedef struct
{
    const char* version;
    const char* name;
    const char* description;
    const char* command;
    int needsParam;
} applicationConfiguration;


/*
	sleepmode
*/
void snshell_app_sleepMode()
{
    printText("ENTERING SLEEP MODE...");
    snshell_cls();
    printText("TERMINAL SLEEPMODE\nUSE CTRL+C OR CLOSE WINDOW TO EXIT...\n\n");

    while (1) {
        sleep (SS_SLEEPTIME);
        printf ("*");
        fflush (NULL);
    }

}

/*
	loads the configuration of an external app
*/
static int snshell_app_handler(void* user, const char* section, const char* name, const char* value) 
{
    applicationConfiguration* pconfig = (applicationConfiguration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("application", "version")) {
        pconfig->version = strdup(value);
    } else if (MATCH("application", "name")) {
        pconfig->name = strdup(value);
    } else if (MATCH("application", "description")) {
        pconfig->description = strdup(value);
    } else if (MATCH("application", "command")) {
        pconfig->command = strdup(value);
    } else if (MATCH("application", "needsParam")) {
        pconfig->needsParam = atoi(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

/*
	loads an external app (through an app config placed in 
	/opt/snshell/snshell_data/applications/) or shows its configuration.
*/
int snshell_app_loadApp(char *applicationName)
{
	//check help command:

	int getInfo = 0;
	char filename[255];
	strcpy(filename, "/opt/snshell/snshell_data/applications/");

	if (strstr(applicationName, "help ") != NULL) {
		strcat(filename, applicationName+5);
		getInfo = 1;
	} else {
		strcat(filename, applicationName);
	}


	FILE *file;
	if (file = fopen(filename, "r")) {
		fclose(file);
		applicationConfiguration config;

    	if (ini_parse(filename, snshell_app_handler, &config) < 0) {
    		return 0;
    	} else {
    		if (getInfo == 1) {
    			printText("\nApplication Information:", 0);
    			printText("\n - Name: ", 0);
    			printText(config.name, 0);
    			printText("\n - Version: ", 0);
    			printText(config.version, 0);
    			printText("\n - Description: ", 0);
    			printText(config.description, 0);
    			printText("\n\n", 0);
    		} else {
	    		printText("\nStarting Application \"", 0);
    			printText(config.name, 0);
    			printText("\"... \n", 0);

    			char applicationCommand[255];
    			strcpy(applicationCommand, config.command);

    			if (config.needsParam == 1) {
    				char inputText[255];

    				fputs(" => Application needs additional parameters: ", stdout);
					fflush(stdout);
					if ( fgets(inputText, sizeof inputText, stdin) != NULL ) {
						char *newline = strchr(inputText, '\n'); /* search for newline character */
						if ( newline != NULL ) {
							*newline = '\0'; /* overwrite trailing newline */
						}
					}
					strcat(applicationCommand, " ");
					strcat(applicationCommand, inputText);
    			}

    			printText("\n", 1);
    			system(applicationCommand);

    			printText("\nApplication stopped.\n", 0);
    		}
    	}

		return 1;
	}
    return 0;
}