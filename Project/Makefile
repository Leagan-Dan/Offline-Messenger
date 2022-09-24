s1: 
	gcc server.c -l sqlite3 -o server
	./server
c1:
	gcc client.c -o client
	./client 127.0.0.1 7777
clean:
	rm -f *~ client server
