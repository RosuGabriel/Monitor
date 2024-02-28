gcc -c "./Menu.c" -o "./Menu.o"
gcc -c "./Monitor.c" -o "./Monitor.o"

gcc "./Monitor.o" "./MainMenu.o" -o "./MenuApp.out"