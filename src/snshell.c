/*
 Simple Network Shell
 ====================

 An Remote-Shell for simple I/O without full access to the remote machine through SSH.
*/

#include <stdio.h>
#include <stdlib.h>

#define SS_PRINT_CONNECTION_INFO_FIRST 1


/*
	basic user prompt
*/
int snshell_userPrompt()
{
    char inputText[255];
    int exit = 0;

    while(!exit) {

        fputs("> ", stdout);
        fflush(stdout);
        if ( fgets(inputText, sizeof inputText, stdin) != NULL )
        {
            char *newline = strchr(inputText, '\n'); /* search for newline character */
            if ( newline != NULL )
            {
                *newline = '\0'; /* overwrite trailing newline */
            }
        }

        if (strcmp(inputText, "help") == 0) {
          snsell_misc_displayText("/opt/snshell/snshell_data/text/help");
        } else if (strcmp(inputText, "sleepmode") == 0) {
            snshell_app_sleepMode();
		} else if (strcmp(inputText, "version") == 0) {
            snsell_misc_displayText("/opt/snshell/snshell_data/text/version");
        } else if (strcmp(inputText, "exit") == 0) {
            exit = 1;
            printText("CLOSING CONNECTION...\n", 0);
        } else if (!snshell_app_loadApp(inputText)) {
            printText("INVALID COMMAND. (TRY \"help\")\n", 0);
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc == 2) {
      printf("Param: %s\n", argv[1]);
    }

    snshell_cls();

    snshell_print_header(0); //header und connectioninfo

    if (snshell_misc_check_os() == 1) {

        printText("LOGGED IN. (\"help\" FOR INFORMATIONS)\n", 0);

        snshell_userPrompt();
    } else {
        printText("\nERROR: LOGGING OUT.", 0);
    }
    return 0;
}
