snshell: snshell.o snshell_functions.o snshell_ui.o snshell_applications.o ini.o
	gcc -O2 -o snshell snshell.o snshell_functions.o snshell_ui.o snshell_applications.o ini.o
	strip snshell

snshell.o: snshell.c
	gcc -c snshell.c

snshell_ui.o: snshell_ui.c
	gcc -c snshell_ui.c

snshell_functions.o: snshell_functions.c
	gcc -c snshell_functions.c

snshell_applications.o: snshell_applications.c
	gcc -c snshell_applications.c

ini.o: ini.c
	gcc -c ini.c

install:
	if [ ! -d "/opt/snshell" ]; then mkdir /opt/snshell; fi
	cp snshell /opt/snshell/snshell
	cp -r snshell_data /opt/snshell/

clean:
	rm -f snshell
	rm -f *.o
