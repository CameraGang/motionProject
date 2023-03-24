OUTFILE = motionProject
OUTDIR = $(HOME)/project

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = gcc
CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L -Wshadow -pthread
FILES =  main.c camera.c recordBuffer.c helper.c
LIBS = $(shell pkg-config --cflags libavformat libavcodec libswresample libswscale libavutil)

all:
	$(CC_C) $(CFLAGS) $(FILES)  -o $(OUTDIR)/$(OUTFILE) -lavformat -lavcodec -lavutil -lswscale

run:
	$(OUTDIR)/$(OUTFILE) | ffmpeg -vcodec mjpeg -i pipe:0 -f mjpeg udp://192.168.7.1:1234

delete:
	rm rec_*

clean:
	rm $(OUTDIR)/$(OUTFILE)
