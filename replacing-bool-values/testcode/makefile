# The following suppresses the 'Nothing to be done' line that is output by make.  
# By making 'all' a phony target that depends on the real target, we can make 
# sure it always has something to do (the ':' is actually a shell no-op), so the 
# message goes away and doesn't clutter up the output from the find-medians.sh 
# script.
# 
# Credit where credit is due, I found this at the following StackOverflow 
# question:
# https://stackoverflow.com/questions/3417391/suppress-nothing-to-be-done-for-all

.PHONY: all

all : real_all
	@:

real_all: \
   	bools.out \
	functions.out \
	ints.out \
	bitset-consts.out \
	bitset-pos.out \
	struct-bitfields.out \
	struct-bools.out \
	enum-unscoped.out \
	enum-scoped.out

clean:
	@rm -f *.asm *.noopt *.opt *.out *.times medians.txt medians.noopt.txt && echo "All cleaned up"

%.out : %.cpp
	@echo Making $@
	@g++ -S $< -o $*.noopt.asm
	@g++ $< -lfmt -o $*.noopt
	@g++ -S -O3 $< -o $*.opt.asm
	@g++ -O3 -lfmt $< -o $*.opt
	@./$*.opt >$@ 2>>/dev/null

bools.out : bools.cpp

functions.out : functions.cpp

ints.out : ints.cpp

bitset-consts.out : bitset-consts.cpp

bitset-pos.out : bitset-pos.cpp

struct-bitfields.out : struct-bitfields.cpp

struct-bools.out : struct-bools.cpp
