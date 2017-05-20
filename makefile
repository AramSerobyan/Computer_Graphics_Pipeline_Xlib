EXE = Scan
OBJS = Scan_Bilinear_Resize.o PointList.o Polygon.o Triangle.o MVPTransform.o #Sphere.o
FILES = Scan_Bilinear_Resize.cpp PointList.cpp PointList.h Polygon.cpp Polygon.h Triangle.cpp Triangle.h MVPTransform.cpp MVPTransform.h #Sphere.cpp Sphere.h
INCLUDES = /usr/X11R6/include -I /usr/include -I $(shell pwd)
LIB = /usr/X11R6/lib
LFLAGS = -lX11
CC = g++

.SUFFIXES: .c
prog: ${EXE}

${EXE}: ${OBJS}
	${CC} -L${LIB} -o $@ ${OBJS} ${LFLAGS}

${OBJS}:${FILES}
	 ${CC} -I${INCLUDES} -c ${FILES}
#another way to do it

#.c.o:
#	 ${CC} -I${INCLUDES} -c $<
#
#${EXE}: ${OBJS}
#	${CC} -L${LIB} -o $@ ${OBJS} ${LFLAGS}
#
clean:
	rm -f ${OBJS} ${EXE}
	make
