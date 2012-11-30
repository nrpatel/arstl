UNAME := $(shell uname -s)

OBJS = stlparser.o
CC = g++
CFLAGS = -O3 -Wall -DGL_GLEXT_PROTOTYPES -I/usr/local/include
LDFLAGS =  -lGLEW -L/usr/local/lib -laruco -lopencv_core -lopencv_imgproc -lopencv_features2d -lopencv_gpu -lopencv_calib3d -lopencv_objdetect -lopencv_video -lopencv_highgui -lopencv_ml -lopencv_legacy -lopencv_contrib -lopencv_flann
GL_LDFLAGS =  -lglut

ifeq ($(UNAME),Darwin)
        GL_LDFLAGS = -framework OpenGL -framework GLUT
endif

.PHONY: all
all: arstl

arstl: arstl.o $(OBJS)
	$(CC) -o $@ arstl.o $(OBJS) $(CFLAGS) $(GL_LDFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) arstl.o arstl
