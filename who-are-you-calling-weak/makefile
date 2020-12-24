all: Output-1.txt Output-2.txt Output-3.txt Output-4.txt

Output-1.txt : shared-ptr-from-ptr.cpp common.ipp
	g++ shared-ptr-from-ptr.cpp
	./a.out >Output-1.txt
	rm a.out

Output-2.txt : shared-ptr-make_shared.cpp common.ipp
	g++ shared-ptr-make_shared.cpp
	./a.out >Output-2.txt
	rm a.out

Output-3.txt : weak-ptr-from-ptr.cpp common.ipp
	g++ weak-ptr-from-ptr.cpp
	./a.out >Output-3.txt
	rm a.out

Output-4.txt : weak-ptr-make_shared.cpp common.ipp
	g++ weak-ptr-make_shared.cpp
	./a.out >Output-4.txt
	rm a.out

