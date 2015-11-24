# Plot2DTest.make
# makefile to built plot2dtest which tests the Plot2D class
# wpf 2015-10-28 19h02min

all: ../bin/plot2dtest

../bin/plot2dtest: ../obj/Plot2DTestmain.o ../obj/Plot2DTest.o\
    ../obj/Parser.o ../obj/Post.o ../obj/Plot2D.o ../obj/util.o\
    ../obj/globalVals.o
	g++ -std=c++11 -Wall -g -gdwarf-2 -o ../bin/plot2dtest\
           ../obj/Plot2DTestmain.o ../obj/Plot2DTest.o ../obj/Parser.o\
           ../obj/Post.o ../obj/Plot2D.o ../obj/util.o ../obj/globalVals.o\
           -lPocoFoundation

../obj/Plot2DTestmain.o: Plot2DTestmain.cc Plot2DTest.h Plot2D.h Parser.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Plot2DTestmain.o\
           Plot2DTestmain.cc

../obj/Plot2DTest.o: Plot2DTest.h Plot2DTest.cc Plot2D.h Post.h\
    Parser.h util.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Plot2DTest.o Plot2DTest.cc

../obj/Post.o: Post.h Post.cc util.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Post.o Post.cc

../obj/Plot2D.o: Plot2D.h Plot2D.cc Post.h globalVals.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Plot2D.o Plot2D.cc

../obj/Parser.o: Parser.h Parser.cc Post.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Parser.o Parser.cc

../obj/util.o: util.h util.cc
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/util.o util.cc

../obj/globalVals.o: globalVals.h globalVals.cc
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/globalVals.o globalVals.cc

