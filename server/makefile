main : main.o server.o check.o modes.o recvfrom_client.o database.o sendto_client.o send_recv.o handle.o wav_parser.o user.o friend_handle.o
	gcc -o main main.o server.o check.o modes.o recvfrom_client.o send_recv.o  handle.o sendto_client.o -lpthread -lm database.o -L/usr/lib/mysql -lmysqlclient wav_parser.o user.o friend_handle.o
server.o : server.c server.h head.h const_value.h
	gcc -c server.c
check.o : check.h check.c head.h database.h sendto_client.h
	gcc -c check.c
modes.o : modes.h modes.c recvfrom_client.h head.h
	gcc -c modes.c
recvfrom_client.o : recvfrom_client.h recvfrom_client.c head.h
	gcc -c recvfrom_client.c
database.o : database.c database.h head.h
	gcc -c database.c -I/usr/include/mysql -lmysqlclient
sendto_client.o : sendto_client.c sendto_client.h head.h wav_parser.h
	gcc -c sendto_client.c
send_recv.o : send_recv.h send_recv.c head.h
	gcc -c send_recv.c
handle.o : handle.c handle.h head.h
	gcc -c handle.c
wav_parser.o : wav_parser.c wav_parser.h head.h
	gcc -c wav_parser.c
user.o : user.h user.c head.h const_value.h
	gcc -c user.c
friend_handle.o : friend_handle.c friend_handle.h head.h const_value.h handle.h
	gcc -c friend_handle.c
clean:
	rm *.o
