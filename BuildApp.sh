gcc -c "./App.c" -o "./App.o"
gcc -c "./Monitor.c" -o "./Monitor.o"

gcc "./Monitor.o" "./App.o" -o "./App.out"
