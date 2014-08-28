
LIBS = -pthread -lX11 -lGL -lGLU -lpng -lfreetype -lrt -lasound
# gcc -lrt -lasound -ljack -lpthread -o YOUR_BINARY main.c libportaudio.a
LIB_INCLUDE = -L/usr/X11R6/lib -L/usr/local/lib -L/home/alexarse/Desktop/axLib/ -L/usr/lib/


LIBSNDFILE_INCLUDE = -I /usr/include 
LIBSNDFILE_FLAG = `pkg-config --cflags sndfile`
LIBSNDFILE = `pkg-config --libs sndfile`

INCLUDE = -I/usr/X11R6/include 						\
		  -I/usr/X11R6/include/X11 					\
		  -I/usr/include 							\
		  -I/usr/include/freetype2 					\
		  -I/home/alexarse/Desktop/axLib/ 			\
		  -I/home/alexarse/Desktop/axLib/include 	\
		  -I/home/alexarse/Desktop/axLib/axProjects/MidiSequencer/portaudio/include

PROGRAM_NAME = main
# LIB_NAME = .a
CC = clang++ -std=c++11 -g $(INCLUDE) 
##CC = clang++ -std=c++11 -g $(INCLUDE) 
#-I/usr/include/freetype2 -L/usr/local/lib -lfreetype

OBJECTS = AudioWaveform.o MidiTrack.o axAudio.o axAudioBuffer.o axMidi.o DrumSampler.o axAudioEnvelope.o axAudioFilter.o main.o
##main.o


# g++ -o prog1 main.cpp Hash.o -std=c++0x
default: $(OBJECTS)
	clang++ -o main $(OBJECTS) libportaudio.a libportmidi.a libporttime.a $(LIB_INCLUDE) $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE) -laxlib $(LIBS) $(LIBSNDFILE)
	# clang++ -o main main.cpp  AudioWaveform.cpp MidiTrack.cpp axAudio.cpp axAudioBuffer.cpp libportaudio.a $(LIB_INCLUDE) $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE) -laxlib $(LIBS) $(LIBSNDFILE) -std=c++11

# library: $(OBJECTS)
# 	ar -cvq libaxlib.a $(OBJECTS_LIST)

main.o: main.h main.cpp
	$(CC) main.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)

AudioWaveform.o: AudioWaveform.h AudioWaveform.cpp
	$(CC) AudioWaveform.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)

MidiTrack.o: MidiTrack.h MidiTrack.cpp
	$(CC) MidiTrack.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)

axAudio.o: axAudio.h axAudio.cpp
	$(CC) axAudio.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)

axAudioBuffer.o: axAudioBuffer.h axAudioBuffer.cpp
	$(CC) axAudioBuffer.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)

DrumSampler.o: DrumSampler.h DrumSampler.cpp
	$(CC) DrumSampler.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)

axAudioEnvelope.o: axAudioEnvelope.h axAudioEnvelope.cpp
	$(CC) axAudioEnvelope.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)

axAudioFilter.o: axAudioFilter.h axAudioFilter.cpp
	$(CC) axAudioFilter.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)

axMidi.o: axMidi.h axMidi.cpp
	$(CC) axMidi.cpp -c $(INCLUDE) $(LIBSNDFILE_FLAG) $(LIBSNDFILE_INCLUDE)
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


clean:
	rm -f $(PROGRAM_NAME)
	rm -f $(OBJECTS)
