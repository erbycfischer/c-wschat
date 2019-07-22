all:
	clang -std=c11 -o server main.c chat_packets.c
	mv ./server ./output/server
	chmod +x ./output/server

clean:
	rm ./output/server