# Who Are You Calling Weak?

_The surprising strength of `weak_ptr`s_

Management of memory allocated from the heap is one of the fundamental processes 
in program development.  Many programs beyond the simplest toy or test programs 
will need heap memory to do their work.

Modern programming languages often take the responsibility for handling heap 
memory away from the programmer, with techniques like garbage collection 
operating in the background. While this makes life easier in many ways for the 
programmer, it can make it hard to determine when memory is going to be 
released, and some GC systems can cause unpredictable slowdowns when the garbage 
collector runs.

C++ on the other hand exposes heap management to the programmer, giving finer 
control over when the memory will be allocated and deallocated.  The use of raw 
pointers to allocated memory can lead to well known problems, with the necessity 
to take care that allocated memory is released in every possible path through 
a program, especially in the presence of exceptions.  To get around these 
problems, smart pointer classes have been developed that take control of the 
ownership of the pointed to memory, so the programmer doesn't need to worry 
about it.

In the time before the C++11 standard was released, one of the commonest smart 
pointer classes in use was the Boost `shared_ptr` class. This class has been 
present in Boost since at least the 1.31.0 release (the earliest release for 
which the Boost website has documentation). Ultimately it formed the basis of 
the `std::shared_ptr` class added to the C++11 standard. This article is about 
the `std::shared_ptr` class, but in general it also applies if you are still 
using the Boost version.

## How `shared_ptr` works

This section describes how the `shared_ptr` objects work under-the-hood. You can 
skip it if you are already familiar with this.

The primary facility offered by `shared_ptr`s is that they allow shared 
ownership of an object.  What this means is that you can have multiple pointers 
to the object, and as long as at least one is pointing to it, it will not be 
deleted.

In order to offer this shared ownership facility, the `shared_ptr` class uses 
a reference count for each object pointed to. Each time a `shared_ptr` to the 
object is set, the reference count is incremented. When a `shared_ptr` goes out 
of scope, the reference count is decremented. Only when the reference count goes 
to zero is the referenced object deleted. Also the memory is usually reclaimed, 
but see later for why this might not happen immediately.

The reference count for an object obviously has to be stored somewhere. Some
classes that use reference counting to handle object ownership are intrusive,
i.e. they store the reference count within the owned object itself. The 
`shared_ptr` class is non-intrusive, which means they don't need the object
pointed to to hold the reference count. Instead they use a control block that
is allocated separately to the memory allocated for the pointed-to object.
Figure 1(a) illustrates this.

## The `make_shared` template

Allocating memory is generally an expensive operation, so allocating two
blocks of memory when creating a new object pointed to by a `shared_ptr`
effectively doubles the time to carry out the operation. To avoid this double
allocation, the template `make_shared` can be used. The C++11 standard
states that `make_shared` should perform no more than one memory allocation.
Figure 1(b) illustrates what this would look like in memory. As `make_shared` 
has other advantages over using `new`, such as better exception safety, you 
would generally prefer to use it.

It is worth being aware of this behaviour of the `make_shared` template with 
regards to increasing the size of the block allocated when creating a new object 
on the heap. For instance, if you are using a memory allocator that uses slab 
allocation, you need to take account of the size of the control block when 
working out which slab size the object will use if you use a `shared_ptr` to 
point to it.

## Examples of `shared_ptr` memory usage

The following examples illustrate how memory is allocated when creating an 
object and using a `shared_ptr` to store the address. The code was compiled on 
a 64-bit Arch Linux system using GCC 8.3.0.

The file _common.ipp_ is used in all examples to provide overrides for global 
`new` and `delete` operators so we can log when memory is allocated and 
deallocated, and the DataHolder class for the objects we create. This is 
a 'large' class so it is too big for any small-object optimization that the 
implementation of `shared_ptr` may use.

### Creation with `operator new`

The code in _shared-ptr-from-ptr.cpp_ allows us to examine how memory is 
allocated and dealloacted when creating an object on the heap using `new` and 
storing the returned pointer in a `shared_ptr`.

Sample output from this code is shown in Output-1.

Line 2 is the memory allocation for the DataHolder object, and it is constructed 
at line 3.

Line 4 is the memory allocation for the `shared_ptr` control block. As can be 
seen in line 5, the `shared_ptr` is pointing to the memory allocated at line 2, 
i.e. the memory for the object.

The following three lines show the destruction of the DataHolder followed by the 
deallocation of the memory previously allocated. Note that the memory for the 
object is deallocated before the memory for the control block.

Line 9 shows that the object destruction and memory deallocation occurs at the 
end of the inner block, as would be expected by the `shared_ptr` going out of 
scope when the inner block ends.

### Creation with `make_shared`

The code in _shared-ptr-make_shared.cpp_ allows us to examine how memory is 
allocated and dealloacted when creating an object on the heap using 
`make_shared`.

Sample output from this code is shown in Output-2.

Comparing this to Output-1 shows some obvious differences.

Firstly, there is only one allocation of memory from the heap, at line 2. The 
memory allocated is not equal to the size of the control block and object block 
from the previous example because there is some optimization of memory usage .
occuring

Second, on line 4, you can see that the `shared_ptr` does not point to the start 
of the block allocated in line 2, but part way through. This is because the 
initial bytes of the memory are used by the control block, and the object 
pointed to starts after the control block.

Finally, line 6 shows there is just one memory deallocation, matching the single 
memory allocation from line 2, and line 7 shows that the object destruction and 
memory deallocation occur at the end of the inner block.

## Using `weak_ptr`

The automatic deallocation of the object pointed to when the last reference to 
it goes out of scope is a major advantage of `shared_ptr`. However, problems 
arise if you have two objects which have `shared_ptr` members and which directly 
or indirectly reference each other. This is because the `shared_ptr` members 
will only be destroyed, and hence decrease the reference count of their 
pointed-to objects, when the containing object is destroyed. But if object A has 
a `shared_ptr` pointing to object B, and object B has a `shared_ptr` pointing to 
object A, the reference counts in the respective `shared_ptr`s will never be 
decremented to 0, so A and B will never be deleted.

For instance, if you have a data structure representing a graph where the 
pointers between nodes in the graph are `shared_ptr`s, as soon as you have 
a cycle in the graph, the graph objects in the cycle will never be deleted.

One way to get around this problem is to explicitly call `reset()` on the 
`shared_ptr` member. This causes it to no longer point to an object, so breaking 
the cycle explicitly.

Another way to break the cycle is to use `weak_ptr` instead of `shared_ptr`.  
A `weak_ptr` provides _potential_ ownership, not _actual_ ownership. What this 
means is that if the only pointers pointing to an object are `weak_ptr`s, the 
object can be deleted.

In order to actually use an object pointed to by a `weak_ptr` you have to call 
`lock` on the pointer to obtain a `shared_ptr` to the object, and from then you 
just treat it as normal. If the result of the `lock` call is a `shared_ptr` that 
points to nothing, it means the object pointed to has been destroyed, and cannot 
be used.

## How `weak_ptr` works

So that the `lock` call can work, there has to be some bookkeeping data 
associated with an object that exists as long as there is a `weak_ptr` that 
could potentially refer to the object. This bookkeeping data is generally kept 
in the control block used by the `shared_ptr` to hold the reference count.

An additional count indicating how many `weak_ptr`s point to the object is held,
and as long as the weak reference count is above zero the control block will not 
be deleted, even if the pointed-to object is deleted as a result of the normal 
reference count going to zero.  

## Effect of `weak_ptr` on allocated memory

The effect of using `weak_ptr`s on allocated memory usage can be seen in the 
following examples.

### Creation using `operator new`

The code in _weak-ptr-from-ptr.cpp_ shows how using `weak_ptr` affects memory 
used by a `shared_ptr` created using `new`.

Sample output from this code is shown in Output-3. Compare this to the 
corresponding output with no `weak_ptr` given in Output-1.

Lines 1-5 are identical except for the memory addresses. The only thing of note 
here is that the initial creation of the `weak_ptr` does not allocate any 
memory, because `weak_ptr`s do not have their own control blocks.

Lines 6 and 7 show the `weak_ptr` being assigned from the `shared_ptr`. As 
expected, the reference count is not updated after the `weak_ptr` is pointed to 
the object. Under the hood the weak reference count will have been updated, but 
the public interface of `shared_ptr` does not allow access to the weak reference 
count to confirm this.

The interesting part is lines 8-11. Lines 8 and 9 show that when the inner block 
is exited, the DataHolder that was created is destroyed and its memory released 
just as happens in lines 6 and 7 in Output-1.  But for the control block, if you 
compare lines 10 and 11 to Output-1 lines 8 and 9, you will see that it is not 
deallocated when we exit the inner block.  This is because the weak reference 
count is not zero as the `weak_ptr` is still referencing it.  Only when the 
program exits and the `weak_ptr` is destroyed, setting the weak reference count 
to zero, is the control block destroyed.

### Creation using `make_shared`

The code in _weak-ptr-make_shared.cpp_ shows how using `weak_ptr` affects memory 
used by a `shared_ptr` created using `make_shared`.

Sample output from this code is shown in Output-4. Compare this to the 
corresponding output with no `weak_ptr` given in Output-2.

Once again, the initial lines 1-4 from the two outputs are identical, and lines 
5 and 6 in Output-4 show that assigning a `shared_ptr` to a `weak_ptr` doesn't 
affect the use count of the `shared_ptr`.

The interesting part is again when the `shared_ptr` goes out of scope at the end 
of the inner block, leading to the DataHolder being destroyed. As can be seen at 
line 7, the DataHolder is destroyed when the inner block is exited. Once again 
the memory allocated is not released until the program exits and the `weak_ptr` 
is destroyed, so releasing the control block.

Importantly, note that even though the memory for the referenced object is not 
needed once the last `shared_ptr` pointing to it has gone out of scope, it 
cannot be released until the control block is also destroyed. This is because 
the allocated memory block cannot be partially released, but only as a complete 
block. 

## Conclusion: `weak_ptr`s are stronger than you might think

As can be seen, `weak_ptr`s are only 'weak' in the sense of not preventing an 
object being destroyed while they still point at its control block. They still 
prevent some or all of the memory allocated when creating the object from being 
released until all the `weak_ptr`s pointing to it have been destroyed.

Whether this is an important consideration when designing your program is 
obviously dependent on a number of factors.

If you are in an environment with huge amounts of memory, where memory 
consumption is only a secondary concern, you probably wouldn't need to worry 
about it.

If you just use `weak_ptr`s to hold parts of a logical structure together, such 
as a graph where you use `weak_ptr`s to hold some of the links between the 
nodes, and the whole structure is destroyed in one go, you probably won't be 
concerned because all the `weak_ptr`s will be destroyed at the same time, so no 
memory will be held onto.

On the other hand, if you are working in a restricted memory environment where 
you need control over how much memory is allocated at any time, you would need 
to be aware of how `weak_ptr`s hold onto memory, even if it is just the control 
block associated with the objects pointed to, and more so if you use 
`make_shared` to create the objects in the first place.
