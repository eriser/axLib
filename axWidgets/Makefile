
LIBS = -pthread -lX11 -lGL -lGLU -lpng -lfreetype 

LIB_INCLUDE = -L/usr/X11R6/lib -L/usr/local/lib
INCLUDE = -I/usr/X11R6/include -I/usr/X11R6/include/X11 -I/usr/include -I/usr/include/freetype2 -I/home/alexarse/Desktop/axLib/include
PROGRAM_NAME = main
LIB_NAME = libaxcore.a
CC = clang++ -std=c++11 -g $(INCLUDE) 

OBJECTS = axNumberBox.o axPopupMenu.o axSlider.o axButton.o axGrid.o axMenu.o axKnob.o axScrollBar.o


default: $(OBJECTS)
	#ar rcs $(LIB_NAME) $(OBJECTS_LIST)
	# $(CC)  $(LIB_INCLUDE) $(INCLUDE) $(LIBS) $(OBJECTS) -o $(PROGRAM_NAME) 

# -cvq 
# library: $(OBJECTS)
	# ar rcs libaxlib.a $(OBJECTS)

axNumberBox.o: axNumberBox.h axNumberBox.cpp
	$(CC) $(INCLUDE) axNumberBox.cpp -c

axPopupMenu.o: axPopupMenu.h axPopupMenu.cpp
	$(CC) $(INCLUDE) axPopupMenu.cpp -c

axSlider.o: axSlider.h axSlider.cpp
	$(CC) $(INCLUDE) axSlider.cpp -c

axButton.o: axButton.h axButton.cpp
	$(CC) $(INCLUDE) axButton.cpp -c

axGrid.o: axGrid.h axGrid.cpp
	$(CC) $(INCLUDE) axGrid.cpp -c

axMenu.o: axMenu.h axMenu.cpp
	$(CC) $(INCLUDE) axMenu.cpp -c

axKnob.o: axKnob.h axKnob.cpp
	$(CC) $(INCLUDE) axKnob.cpp -c

axScrollBar.o: axScrollBar.h axScrollBar.cpp
	$(CC) $(INCLUDE) axScrollBar.cpp -c

clean:
	rm -f $(LIB_NAME)
	rm -f $(OBJECTS)
