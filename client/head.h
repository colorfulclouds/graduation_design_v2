//HEAD
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <error.h>
#include <signal.h>
#include <pthread.h>
#include <alsa/asoundlib.h> 
#include <assert.h>
#include <sys/types.h>
#include <regex.h>
#include <dirent.h>
#include <time.h>
#include <math.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>

#include <termios.h>	//终端相关