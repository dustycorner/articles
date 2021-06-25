all: \
   	bitfield-1.out \
  	bitfield-2.out \
 	conversion-1.out \
 	conversion-2.out \
 	conversion-3.out

bitfield-1.out : bitfield-1.cpp
	g++ bitfield-1.cpp
	./a.out >bitfield-1.out
	rm a.out

bitfield-2.out : bitfield-2.cpp
	g++ bitfield-2.cpp
	./a.out >bitfield-2.out
	rm a.out

conversion-1.out : conversion-1.cpp
	-g++ conversion-1.cpp 2>conversion-1.out

conversion-2.out : conversion-2.cpp
	g++ conversion-2.cpp
	./a.out >conversion-2.out
	rm a.out

conversion-3.out : conversion-3.cpp
	g++ conversion-3.cpp
	./a.out >conversion-3.out
	rm a.out
