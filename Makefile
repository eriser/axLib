
LIBS = -pthread -lX11 -lGL -lGLU -lpng -lfreetype 

LIB_INCLUDE = -L/usr/X11R6/lib -L/usr/local/lib
INCLUDE = -I/usr/X11R6/include -I/usr/X11R6/include/X11 -I/usr/include -I/usr/include/freetype2 -I include/
PROGRAM_NAME = main
LIB_NAME = libaxcore.a
CC = clang++ -std=c++11 -g $(INCLUDE) 
##CC = clang++ -std=c++11 -g $(INCLUDE) 
#-I/usr/include/freetype2 -L/usr/local/lib -lfreetype

WIDGETS = axWidgets/axButton.o axWidgets/axGrid.o \
		  axWidgets/axSlider.o axWidgets/axNumberBox.o \
		  axWidgets/axPopupMenu.o axWidgets/axMenu.o \
		  axWidgets/axKnob.o axWidgets/axScrollBar.o

OBJECTS = axCoreX11.o axUtils.o axC++.o 				\
		  axApp.o axCore.o axColor.o axGC.o 			\
		  axImage.o axFont.o axPanel.o axManager.o 		\
		  axWindow.o axWindowTree.o  axObject.o			

# default: $(OBJECTS)
# 	#ar rcs $(LIB_NAME) $(OBJECTS_LIST)
# 	make -C axWidgets
# 	$(CC)  $(LIB_INCLUDE) $(INCLUDE) $(LIBS) $(OBJECTS) $(WIDGETS) -o $(PROGRAM_NAME) 

default: $(OBJECTS)
	make -C axWidgets/
	ar rcs libaxlib.a $(OBJECTS) $(WIDGETS)

axUtils.o: include/axUtils.h axUtils.cpp
	$(CC) axUtils.cpp -c

axCoreX11.o: include/axCoreX11.h axCoreX11.cpp
	$(CC) axCoreX11.cpp -c

axC++.o: include/axC++.h axC++.cpp
	$(CC) axC++.cpp -c

axApp.o: include/axApp.h axApp.cpp
	$(CC) axApp.cpp -c

axGrid.o: include/axGrid.h axGrid.cpp
	$(CC) axGrid.cpp -c

axCore.o: include/axCore.h axCore.cpp
	$(CC) axCore.cpp -c

axColor.o: include/axColor.h axColor.cpp
	$(CC) axColor.cpp -c

axGC.o: include/axGC.h axGC.cpp
	$(CC) $(INCLUDE) axGC.cpp -c 

axImage.o: include/axImage.h axImage.cpp
	$(CC) axImage.cpp -c

axFont.o: include/axFont.h axFont.cpp
	$(CC) axFont.cpp -c 

axID.o: include/axID.h axID.cpp
	$(CC) axID.cpp -c

axPanel.o: include/axPanel.h axPanel.cpp
	$(CC) axPanel.cpp -c

axManager.o: include/axManager.h axManager.cpp
	$(CC) axManager.cpp -c

axWindow.o: include/axWindow.h axWindow.cpp
	$(CC) axWindow.cpp -c

axWindowTree.o: include/axWindowTree.h axWindowTree.cpp
	$(CC) axWindowTree.cpp -c

axObject.o: include/axObject.h axObject.cpp
	$(CC) axObject.cpp -c

# main.o: main.cpp main.h
# 	$(CC) main.cpp -c

clean:
	rm -f $(LIB_NAME)
	rm -f $(OBJECTS)
