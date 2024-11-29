compiler=gcc
client_source=client.c
server_source=server.c
client_build=lab_3_client
server_build=lab_3_server

all: client server

client:
	$(compiler) $(client_source) -o $(client_build)

server:
	$(compiler) $(server_source) -o $(server_build)
