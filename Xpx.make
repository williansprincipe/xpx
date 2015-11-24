# Xpx.make
# makefile to built xpx which stands for Xpxotoelectron
# wpf 2015-11-23 11h05min

all: ../bin/xpx

../bin/xpx: ../obj/Xpxmain.o ../obj/Xpx.o\
    ../obj/LNLSFile.o ../obj/Region.o ../obj/Plot2D.o \
    ../obj/Parser.o ../obj/Post.o ../obj/util.o\
    ../obj/Settings.o ../obj/globalVals.o ../obj/Help.o
	g++ -std=c++11 -Wall -g -gdwarf-2 -o ../bin/xpx\
           ../obj/Xpxmain.o ../obj/Xpx.o ../obj/LNLSFile.o\
           ../obj/Region.o ../obj/Plot2D.o ../obj/Parser.o\
           ../obj/Post.o ../obj/util.o ../obj/Settings.o ../obj/globalVals.o\
           ../obj/Help.o -lPocoFoundation -lgsl -lgslcblas -lm

../obj/Xpxmain.o: Xpxmain.cc Xpx.h Parser.h Post.h Help.h\
    Settings.h # Regoin.h # I don't understand this LNLSFile.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Xpxmain.o Xpxmain.cc

../obj/Xpx.o: Xpx.h Xpx.cc LNLSFile.h Post.h\
    Plot2D.h Parser.h util.h globalVals.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Xpx.o Xpx.cc

../obj/LNLSFile.o: LNLSFile.h LNLSFile.cc Post.h Plot2D.h globalVals.h util.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/LNLSFile.o LNLSFile.cc

../obj/Region.o: Region.h Region.cc Post.h Plot2D.h globalVals.h util.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Region.o Region.cc

../obj/Plot2D.o: Plot2D.h Plot2D.cc Post.h globalVals.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Plot2D.o Plot2D.cc

../obj/Help.o: Help.h Help.cc Post.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Help.o Help.cc

../obj/Post.o: Post.h Post.cc util.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Post.o Post.cc

../obj/Parser.o: Parser.h Parser.cc Post.h globalVals.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Parser.o Parser.cc

../obj/util.o: util.h util.cc globalVals.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/util.o util.cc

../obj/Settings.o: Settings.h Settings.cc
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Settings.o Settings.cc

../obj/globalVals.o: globalVals.h globalVals.cc
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/globalVals.o globalVals.cc

