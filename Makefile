runMain:
	gcc src/main.c
	./a.out

runServer:
	gcc src/server.c -o src/server.exe

runClient:
	gcc src/client.c -o src/client.exe

execServer:
	./src/server.exe

execClient:
	./src/client.exe