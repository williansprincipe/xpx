# PostTest.make
# makefile to built posttest which tests the Post class
# wpf 2015-10-28 19h02min

all: ../bin/posttest

../bin/posttest: ../obj/PostTestmain.o ../obj/PostTest.o ../obj/Parser.o\
           ../obj/Post.o ../obj/util.o
	g++ -std=c++11 -Wall -g -gdwarf-2 -o ../bin/posttest\
           ../obj/PostTestmain.o ../obj/PostTest.o ../obj/Parser.o\
           ../obj/Post.o ../obj/util.o -lPocoFoundation

../obj/PostTestmain.o: PostTestmain.cc PostTest.h Post.h Parser.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/PostTestmain.o PostTestmain.cc

../obj/PostTest.o: PostTest.h PostTest.cc Post.h util.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/PostTest.o PostTest.cc

../obj/Post.o: Post.h Post.cc globalVals.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Post.o Post.cc

../obj/Parser.o: Parser.h Parser.cc Post.h
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/Parser.o Parser.cc

../obj/util.o: util.h util.cc
	g++ -std=c++11 -Wall -g -gdwarf-2 -c -o ../obj/util.o util.cc

