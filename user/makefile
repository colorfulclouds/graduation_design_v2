main : user_log.o log_display.o user.o connect_server.o sendto_server.o recvfrom_server.o grocery_box.o check_info.o send_recv.o   music.o music_shell.o play_queue.o play_go.o plays_go.o download_queue.o upload_queue.o friend_go.o
	gcc -o main user_log.o log_display.o user.o connect_server.o sendto_server.o recvfrom_server.o send_recv.o music.o grocery_box.o check_info.o -lpthread   -lm -lasound music_shell.o play_queue.o play_go.o plays_go.o download_queue.o upload_queue.o friend_go.o
log_display.o : log_display.c log_display.h const_value.h head.h recvfrom_server.h music.h music_shell.h
	gcc -c log_display.c
user.o : user.c user.h const_value.h head.h
	gcc -c user.c
user_log.o : const_value.h head.h user.h user_log.c
	gcc -c user_log.c
connect_server.o : connect_server.c connect_server.h head.h
	gcc -c connect_server.c
sendto_server.o : head.h sendto_server.h
	gcc -c sendto_server.c
recvfrom_server.o : head.h recvfrom_server.h
	gcc -c recvfrom_server.c
grocery_box.o : grocery_box.c grocery_box.h head.h const_value.h
	gcc -c grocery_box.c
check_info.o : check_info.c check_info.h head.h const_value.h
	gcc -c check_info.c
send_recv.o : send_recv.h send_recv.c head.h
	gcc -c send_recv.c
music.o : music.c music.h head.h sendto_server.h
	gcc -c music.c
music_shell.o : music_shell.c music_shell.h head.h const_value.h
	gcc -c music_shell.c
play_queue.o : play_queue.c play_queue.h const_value.h head.h music_shell.h music.h
	gcc -c play_queue.c
play_go.o : play_go.h play_go.c head.h music_shell.h music.h play_queue.h
	gcc -c play_go.c
plays_go.o : plays_go.c plays_go.h head.h const_value.h
	gcc -c plays_go.c
download_queue.o : download_queue.c download_queue.h head.h const_value.h
	gcc -c download_queue.c
upload_queue.o : upload_queue.c upload_queue.o head.h const_value.h
	gcc -c upload_queue.c
friend_go.o : friend_go.c friend_go.h const_value.h head.h music_shell.h
	gcc -c friend_go.c
clean:
	rm *.o