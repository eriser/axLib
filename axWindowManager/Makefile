
LIBS = -pthread -lX11 -lGL -lGLU -lpng -lfreetype 

LIB_INCLUDE = -L/usr/X11R6/lib -L/usr/local/lib -L/home/alexarse/Desktop/axLib/
INCLUDE = -I/usr/X11R6/include -I/usr/X11R6/include/X11 -I/usr/include -I/usr/include/freetype2 -I/home/alexarse/Desktop/axLib/ -I/home/alexarse/Desktop/axLib/include
PROGRAM_NAME = main
# LIB_NAME = .a
CC = clang++ -std=c++11 -g $(INCLUDE) 
##CC = clang++ -std=c++11 -g $(INCLUDE) 
#-I/usr/include/freetype2 -L/usr/local/lib -lfreetype

OBJECTS = axWm.o
##main.o

default: 
	clang++ -o axWm axWm.cpp $(LIB_INCLUDE) $(INCLUDE) -laxlib $(LIBS) -std=c++11

startx:
	echo "/home/alexarse/Desktop/axLib/axWindowManager/axWm & /home/alexarse/Desktop/axLib/axProjects/Sidebar/main" > ~/.xinitrc

clean:
	rm -f axWm




# main.o: main.cpp main.h
# 	$(CC)  $(INCLUDE) (LIB_INCLUDE) $(INCLUDE) -laxlib main.cpp -c

# clean:
	# rm -f $(LIB_NAME)
	# rm -f $(OBJECTS)
