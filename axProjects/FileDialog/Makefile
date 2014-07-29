
LIBS = -pthread -lX11 -lGL -lGLU -lpng -lfreetype 

LIB_INCLUDE = -L/usr/X11R6/lib -L/usr/local/lib -L/home/alexarse/Desktop/axLib/
INCLUDE = -I/usr/X11R6/include -I/usr/X11R6/include/X11 -I/usr/include -I/usr/include/freetype2 -I/home/alexarse/Desktop/axLib/ -I/home/alexarse/Desktop/axLib/include
PROGRAM_NAME = main
# LIB_NAME = .a
CC = clang++ -std=c++11 -g $(INCLUDE) 
##CC = clang++ -std=c++11 -g $(INCLUDE) 
#-I/usr/include/freetype2 -L/usr/local/lib -lfreetype

OBJECTS = main.o
##main.o

# g++ -o prog1 main.cpp Hash.o -std=c++0x
default: 
	#ar rcs $(LIB_NAME) $(OBJECTS_LIST)
	clang++ -o main main.cpp $(LIB_INCLUDE) $(INCLUDE) -laxlib $(LIBS) -std=c++11

# library: $(OBJECTS)
# 	ar -cvq libaxlib.a $(OBJECTS_LIST)

# axUtils.o: axUtils.h axUtils.cpp
# 	$(CC) axUtils.cpp -c

# axCoreX11.o: axCoreX11.h axCoreX11.cpp
# 	$(CC) axCoreX11.cpp -c

# axC++.o: axC++.h axC++.cpp
# 	$(CC) axC++.cpp -c

# axApp.o: axApp.h axApp.cpp
# 	$(CC) axApp.cpp -c

# axGrid.o: axGrid.h axGrid.cpp
# 	$(CC) axGrid.cpp -c

# axCore.o: axCore.h axCore.cpp
# 	$(CC) axCore.cpp -c

# axColor.o: axColor.h axColor.cpp
# 	$(CC) axColor.cpp -c

# axGC.o: axGC.h axGC.cpp
# 	$(CC) $(INCLUDE) axGC.cpp -c 

# axImage.o: axImage.h axImage.cpp
# 	$(CC) axImage.cpp -c

# axFont.o: axFont.h axFont.cpp
# 	$(CC) axFont.cpp -c 

# axID.o: axID.h axID.cpp
# 	$(CC) axID.cpp -c

# axPanel.o: axPanel.h axPanel.cpp
# 	$(CC) axPanel.cpp -c

# axManager.o: axManager.h axManager.cpp
# 	$(CC) axManager.cpp -c

# axWindow.o: axWindow.h axWindow.cpp
# 	$(CC) axWindow.cpp -c

# axWindowTree.o: axWindowTree.h axWindowTree.cpp
# 	$(CC) axWindowTree.cpp -c

# axButton.o: axButton.h axButton.cpp
# 	$(CC) axButton.cpp -c

main.o: main.cpp main.h
	$(CC)  $(INCLUDE) (LIB_INCLUDE) $(INCLUDE) -laxlib main.cpp -c

# clean:
	# rm -f $(LIB_NAME)
	# rm -f $(OBJECTS)
