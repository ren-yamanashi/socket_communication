runMain:
	gcc src/main.c
	./a.out

compileServer:
	gcc src/server.c -o src/server.exe

compileClient:
	gcc src/client.c -o src/client.exe

runServer:
	./src/server.exe

runClient:
	./src/client.exe