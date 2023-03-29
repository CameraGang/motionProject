OUTFILE = motionProject
OUTDIR = $(HOME)/project

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = gcc
CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L -Wshadow -pthread
FILES =  main.c camera.c record.c helper.c buzzer.c led.c sharedFunc.c
LIBS = $(shell pkg-config --cflags libavformat libavcodec libswresample libswscale libavutil)

all:
	$(CC_C) $(CFLAGS) $(FILES)  -o $(OUTDIR)/$(OUTFILE) -lavformat -lavcodec -lavutil -lswscale

run:
	$(OUTDIR)/$(OUTFILE) | ffmpeg -vcodec mjpeg -i pipe:0 -f mjpeg udp://192.168.7.1:1234

delete:
	rm rec_*

clean:
	rm $(OUTDIR)/$(OUTFILE)




# TEST MAKEFILE: 
# OUTFILE = buzzer
# OUTDIR = $(HOME)/cmpt433/public/myApps

# CROSS_COMPILE = arm-linux-gnueabihf-
# CC_C = $(CROSS_COMPILE)gcc
# CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread

# app:
# 	$(CC_C) $(CFLAGS) buzzer.c led.c helper.c sharedFunc.c test.c -o $(OUTDIR)/$(OUTFILE)

# clean:
# 	rm $(OUTDIR)/$(OUTFILE)