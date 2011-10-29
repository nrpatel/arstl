OBJS = 
CC = g++
CFLAGS = -O3 -Wall -DGL_GLEXT_PROTOTYPES -I/usr/local/include
LDFLAGS = -lGL -lGLU -lglut -L/usr/local/lib -laruco -lopencv_core -lopencv_imgproc -lopencv_features2d -lopencv_gpu -lopencv_calib3d -lopencv_objdetect -lopencv_video -lopencv_highgui -lopencv_ml -lopencv_legacy -lopencv_contrib -lopencv_flann

.PHONY: all
all: arstl

arstl: arstl.o $(OBJS)
	$(CC) -o $@ arstl.o $(OBJS) $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) arstl.o arstl
