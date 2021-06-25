# Replacing `bool` Values

## Introduction

When used in the context of programming, the term 'Dimensional Analysis' refers 
to the technique of defining types to represent the kinds of values used in the 
program. With the appropriate operations between objects of those types defined 
the compiler can check the expressions in the code to make sure they are valid.  
This is not generally possible if you rely on using the fundamental types like 
`int` or `double`.

For instance, say you have a program that deals with distances, durations, and 
speeds. It should be obvious that adding or subtracting a distance and a speed 
are invalid operations, but the compiler would not be able to tell you that this 
code is incorrect:

    double distance = 10;
    double speed = 2;
    double duration = distance - speed;

However, if you have types Distance, Duration, and Speed, with only the valid 
operations between them defined, the compiler can issue an error for this code:

    Distance distance = 10;
    Speed speed = 2;
    Duration duration = distance - speed;

To be useable when using this technique most types need to be defined as classes 
or structures. There are libraries available for many languages that make this 
task easier - a recent (2018) survey of them for many languages can be found in 
[Preussner].

However, if you would normally think of using a `bool` variable to hold the 
value, there are several mechanisms available in the C++ language that can be 
used instead, with no need for library support.  We will outline some of them in 
this article, as well as try to explain why you might choose to do so.

When reading the problem descriptions and suggested solutions below, and 
wondering if you want to use them, it is worth applying what I call the TLAMP 
principle. Pronounced 'tee lamp', it stands for Think Like A Maintenance 
Programmer. What may seem obvious to you when first writing a piece of code can 
look completely opaque to someone doing maintenance work on that code in the 
future. They want the code to be as clear as possible on first reading.  That 
later programmer could be yourself in six months - when you haven't looked at 
the code for that length of time what seemed obvious when you were writing it 
may not be so later.

## Why bother when `bool`s are so simple?

You might ask why we would bother replacing a `bool` value with some other 
mechanism when `bool`s are so simple to use. In this section, we will outline 
some of the problems with using `bool`s that make it worthwhile to at least 
consider doing so.

Many of these problems arise because programmers decide to use `bool` variables 
or parameters just because the value being represented can only take two values.  
If you get into the habit of only using `bool` for values that are going to be 
used in boolean expressions, you can avoid them to a large extent.

To illustrate some of the problems we will use the following example[1].

Imagine a water company wants a system written to monitor and control its water 
network. There is a large amount of equipment on the network, such as sensors 
for measuring things like flow rate, temperature, chemical concentrations, and 
also control equipment such as valves and pumps to allow the flows in the 
network to be controlled.  This network has evolved over many years, and the 
equipment is from different manufacturers and of different ages, with a variety 
of protocols used to talk to it.

The initial analysis leads to a design in which the connections to this 
equipment are handled by a Connection base class which provides a standard 
interface, with a set of classes derived from Connection that handle the details 
of each protocol. There is a factory function, called `CreateConnection`, which 
returns an object of the correct class for each connection. Each class is 
designed to handle either input or output on the connection. The initial design 
for the CreateConnection function interface looks like the following:

    ConnectionPtr CreateConnection( std::string_view id
                                  , bool is_output);

The `is_output` parameter determines whether an output (`true`) or input 
(`false`) connection is being created.

An additional requirement is for some users to have elevated permissions on some 
connections. This allows for operations like controlling pump speeds to alter 
flow rates, for instance. To handle this, a second `bool` parameter is added to 
indicate if the user is privileged or not.

During testing of the system, it is found that some parts of the network are so 
old that they only support 7-bit data. As a result, communications over these 
connections have to be encoded from binary to ASCII. To indicate this a further 
`bool` parameter is added to the function, called `is_encoded`, to indicate if 
this encoding is required or not.

Finally, a security review of the system raises concerns that some of the 
connections go over public networks, and a requirement is made that those 
connections need to be encrypted. A final parameter is added to the function 
called `is_encrypted` which indicates if the connection needs to be encrypted or 
not.

The final prototype for the function now looks as follow:

    ConnectionPtr CreateConnection( std::string_view id
                                  , bool is_output
                                  , bool is_authorised
                                  , bool is_encoded
                                  , bool is_encrypted);

### The Meaning Of True

Or rather, the meaning of `true`. And, indeed, `false`. In many cases where 
a variable can take just two values, and so at first looks like a good candidate 
to use a `bool`, it is not obvious which value should map to `true` and which to 
`false`.

The `is_output` parameter in the `CreateConnection` function is a perfect 
example of this. The parameter allows the caller of the function to determine if 
an outgoing or incoming connection is required, but other than the name of the 
parameter there is nothing that indicates which of those is selected by passing 
`true` and which by passing `false`.

You could argue that the name of the parameter shows how it is used, but that 
relies on anyone reading the code either knowing the prototype because they have 
seen if before, or else are willing to look it up. Neither of which is 
guaranteed to be done by a maintenance programmer who is under pressure to get 
a fix out quickly.

### All `bool`s Look The Same

In many cases, the `bool` values do match what we would expect for a given 
parameter, but they can still be problematic, especially if you have more than 
one `bool` in the parameter list. This is because all `bool`s look the same to 
the compiler.

The `CreateConnection` function illustrates this problem. If we ignore the 
problem with it outlined above, it is reasonable that the `is_output` parameter 
is the first `bool` in the list, as the direction of the connection is the most 
important property it has.

Good arguments could be made for any order of the other three `bool` parameters 
however - the one chosen here has arisen simply because of the order the 
requirement for them came up in the development process.  For instance, it could 
be argued that the `is_encoded` and `is_encrypted` parameters are the wrong way 
around for an outbound connection, as encryption occurs before encoding when 
sending a message.

Unless a programmer knows the function prototype off by heart, it would be easy 
for them to get the parameter order wrong, and the compiler won't warn about it.  
Only extensive testing will ensure all calls are correct.

What can be even more confusing for someone reading the code later is if it uses 
named variables for the parameters, but gets them in the wrong order.  For 
instance, consider the following code:

    bool is_encoded   = /* code that sets value to true */;
    bool is_encrypted = /* code that sets value to true */;
    ...
    auto connptr = CreateConnection(id, is_output, is_authorised, is_encrypted, 
                                    is_encoded);

This will work, in the sense of giving the expected result, because the 
`is_encoded` and `is_encrypted` variables have the same value. However, if one 
of those values needs to change later, or someone copies the code elsewhere and 
changed one of the values, the result would be incorrect, but it wouldn't be 
obvious why unless the person reading the code recognises that the last two 
parameters are in the wrong order.

The compiler cannot report this problem because it just sees the types of 
parameters passed in. The names of the variables are relevant only to tell it 
where to read the parameter value from - it doesn't check that they match the 
names in the function prototype.

Note: This problem doesn't just apply to the `bool` type of course - lists of 
parameters all with the same type can be problematic when trying to work out 
what each parameter means. This article doesn't deal with that situation but it 
is worth being aware of it.

### Conversions To And From `bool`

The built-in C++ scalar types all implicitly convert to and from the `bool` 
type.  This implicit conversion is useful when writing code that tests that 
a value is not zero or a null pointer.

Some classes in the standard library also provide an `operator bool` to test 
that an object is in a valid state - for instance, the `std::basic_ios` class 
that is the base of many iostreams classes class provides one to check if an 
error has occurred on the stream.

Another use for this implicit conversion is in the `!!` pseudo-operator, which 
can be used to return the `bool` equivalent of an expression[2] in any cases 
where automatic conversion doesn't happen.

However, this implicit conversion can cause problems if it happens when you are 
not expecting it. For instance when calling a function, if you pass a scalar 
value in a parameter that expects a `bool`, it will be converted.

Consider the code in listing _conversion-1.cpp_. The two `PrintArgs` functions 
simply output their prototype and the values they have been called with. The 
second one allows the `bool` parameter to be defaulted, hence why the `short` is 
placed before it in the parameter list.

Unfortunately, when this program is compiled, the line labelled `// 3` fails to 
compile. The output in _conversion-1.out_ shows the errors when the code is 
compiled with the GCC on my Linux system.

The problem arises during the overload resolution process to decide which 
function should be called. The full details of overload resolution are complex 
(see [CppRef1]) but the case here is relatively simple.  An important point is 
that an integer with no suffix in the code has type `int` so the `2` in the 
problematic call has type `int`.

When the compiler sees the call in the line labelled `// 3`, it first finds all 
the declared functions named `PrintArgs` and adds them to the overload set. It 
then checks each one to see if it matches the arguments given.  This proceeds as 
follows:

-   For the two-parameter function, the `"Abc"` can be converted to 
a `std::string`, so the first argument matches the first parameter.  The `2` is 
an `int`, and it can be implicitly converted to the `bool` type of the second 
parameter.  Both arguments match the function parameters, so the function is 
a candidate.

-   For the three-parameter function, the `"Abc"` is a match as above. The `2` 
is an `int`, and that can be implicitly converted to a `short` using a narrowing 
conversion.  The third argument is missing but the parameter has a default 
value, so it is ignored in the matching. The arguments match the parameter list 
for this function, so it is also a candidate.

At this point, the overload resolution process is done, and we still have two 
candidates with no way to pick between them, and hence the call is ambiguous.  

To solve the ambiguity the programmer changes the second definition so it looks 
like the one in listing _conversion-2.cpp_. Unfortunately, the default value for 
the `bool` parameter can no longer be used, but the ambiguity no longer occurs.

The program now compiles without any problems and appears to run fine as well, 
producing the output in _conversion-2.out_. However, looking closely at the 
output shows that the output from the lines labelled `// 3` and `// 4` do not 
match the arguments in the code. This is again because of implicit conversions.

In the case of the call in line `// 3`, the `2` is converted from `int` to 
`bool`, ending up with the value `true`.

In the case of the call in line `// 4`, the `2` in the second argument is again 
converted from `int` to the `bool` value `true`, and the `true` in the third 
argument is converted from `bool` to `short`, ending up with the value `1`.

This kind of bug can arise if you change the interface of a function and rely on 
the compiler to catch any calls with incorrect arguments. As can be seen in this 
example, it does not always issue warnings or errors for calls that you should 
have changed.  A refactoring tool may be able to find them, or you might simply 
have to check each call by hand.

This kind of problem with implicit conversions can arise in other cases, but the 
one going to or from a `bool` is more insidious because the values of a `bool` 
are fundamentally different from the values of a scalar type, in that they are 
logical truth values, not numbers. The fact that the C++ spec dictates that 
`false` maps to a value of 0 and `true` maps to a value of 1 when converted to 
a number is just a convention to allow the conversion to occur.  Other languages 
don't allow such conversion, or if they do they use different mappings[3].

It may not matter to you if an `int` gets converted to a `short` as long as the 
value doesn't change, but with a `bool` you are going from a logical value to 
a number or from a number to a logical value, which is a more fundamental 
change, and one that may well make no sense in the context of the code.

### More Than Two Values

It might sound trite to say it, but a `bool` value can only hold two different 
values. This may become a problem if you realise that a parameter needs to hold 
more than two values.

For instance, in our water company  example, the binary-to-ASCII encoding on 
some connections might need doing using UUencoding [UUencode], while others 
might use Base64 [Base64].

With just two values for `is_encoded` and one of those used to indicate no 
encoding is required, you cannot represent those two different types of encoding 
in the parameter. You have two options in this case - either add another 
parameter to give the encoding or else convert the `bool` parameter to some 
other kind that can represent three (or more) values.  The first extends the 
function interface even more, and the second has all the possible problems 
associated with conversion to/from `bool` given above.

## Alternatives To `bool`

We have seen why you might want to avoid using `bool` variables and parameters, 
now we will show some methods that you can use to do so. As mentioned 
previously, all of these are available from the core language, with no library 
support required.

Some of these methods are designed primarily for replacing function parameters, 
while the others are more general and can be used to replace variables as well.

### Split One Function Into Two (Or More)

Rather than having a single function with different functionality selected by 
passing a `bool` parameter, split the functionality into two different 
functions, with their names indicating what is being done. Any common 
functionality can be split off into a third function that the two new functions 
call.

This is particularly useful for the case where it is not obvious what the 
mapping from the `true` or `false` values to the selected functionality is.

In our water company example, rather than passing the `is_output` parameter, you 
would instead create functions called `CreateOutboundConnection` and 
`CreateInboundConnection`, where the names indicate what type of connection is 
being created.

This method is fine for replacing one or maybe two parameters. The problem with 
doing more than that is that each additional parameter replaced doubles the 
number of new functions required. Also, with descriptive function names, they  
can get unmanageably long very quickly.

### Using A Flags Variable

This method involves replacing one or more `bool` values with a variable holding 
a collection of single-bit fields. This will generally be an integer value or 
a `std::bitset`.

An example of a flags variable in the standard library is the mode parameter of 
the `std::ifstream` and `std::ofstream` constructors, which uses the 
`std::ios_base::openmode` type.

When using this method with an integer, you would normally define a set of 
constants, one for each flag value.  The value of each constant has its 
particular flag bit set to 1, all other bits set to 0, so the constant 
represents the flag being turned on.  You then use normal binary operations to 
turn on the flags and to test if they are turned on or not.

You can do the same when using a `std::bitset`, but you also have the option of 
accessing individual bits using the `[]` operator or the `test()` function, 
which take the position of the bit in the bitset to check and return true if it 
is set to 1, else false.

One advantage of using a flag variable is that the user just has to turn on the 
flags they want, and all the others default to off. On the other hand, it is 
awkward to explicitly say that a flag is turned off, should you wish to do so.

If you find a flag needs more than two values, you just need to increase the 
size of the field and adjust the constants appropriately. If you are using 
a bitset, the direct bit access through `[]` or `test()` could not be used in 
this case.

A useful trick in case this might happen is to not make bitfields adjacent to 
each other when they are first defined. For instance with four flags in a four 
byte integer, set the fields up as the lowest bit in each byte. That way if you 
do need to increase the number of values represented by a flag, you won't have 
to change any of the constants that don't relate to that flag.

### Using A Flags Structure

This method uses a structure to hold the flags. The structure members can be 
either `bool`s or single-bit bitfields.

If using this method, you can directly set the individual fields to turn the 
flag on or off. For the bitfields version you would usually use 0 for off and 
1 for on.

If using the bitfield version you need to define them as unsigned, as they are 
just one bit wide.  If they are defined as signed then setting the value to 
1 will end up with it being treated as -1.  Listing _bitfield-1.cpp_ illustrates 
this.  Checking the output in _bitfield-1.out_ you can see that structure with 
`int` fields outputs `-1` for each one, while the structure with `unsigned int` 
values outputs `1` for them.

If you don't want to create a variable of the structure type to pass to 
a function you can use an initializer-list as the parameter and the structure 
will be created for you. Listing _bitfield-2.cpp_ shows examples of both types. 

The advantage of setting up a variable before passing it to the function is that 
someone reading the code later can see exactly which flags are being set, 
whereas when using an initializer list they have to know what the structure 
looks like to know which flags are being set.

When using the bitfield version, if you need to extend a field to hold more than 
two fields you can just extend its width. For the `bool` version, you can just 
replace the `bool` with a different type.

### Using Enums

This method simply uses enums with two enumerators defined.  Using appropriate 
names means the values can be self-documenting. Either scoped or unscoped enums 
can be used.

Unscoped enums have the disadvantage that the enumerators are defined in the 
scope enclosing the enum, so you cannot have the same enumerator name in two 
enums that will be used at the same time. On the other hand, it does mean that 
the enumerators can be used with no qualification.

For scoped enums the enumerators are defined in the scope of the enum, so two 
enums can have enumerators with the same name if that makes sense. This does 
mean that they have to be qualified with the enum name when used.

If an unscoped enum is passed as a function parameter that expects an integer, 
the value in the enum variable will be converted to an integer.  This does not 
happen for a scoped enum - no conversion takes place.

Listing _conversion-3.cpp_ is the scoped enum equivalent of _conversion-1.cpp_.  
This version compiles with no ambiguous function calls detected, and if you run 
the resulting program you will see that the `PrintArgs` functions called in each 
case are the correct ones. The output for the program is shown in 
_conversion-3.out_.

#### C++20 And `using enum`

The point was made above that when using scoped enums you need need to precede 
the enumeration name with the scoped enum name.  This has been addressed in 
C++20 with the addition of the `using enum` construct to pull all the names in 
the named enum into the current scope.

A brief description of this facility can be found at [CppRef2] - look for 
_Using-enum-declaration_. The facility was added by the paper P1099r5 [P1099r5], 
and a fuller description of it can be found by reading that (brief) paper.

As of the time of writing (April 2021), the C++20 language features pages for 
GCC (at version 11) and MSVC (at VS 2019 16.4) show this feature as being 
implemented. The equivalent Clang page shows this feature has not yet 
implemented.

## Problems Versus Suggested Alternatives

In this section, we will check if the suggested alternatives solve any of the 
problems outlined.

### The Meaning Of True

Splitting into two functions works, as long as you use sensible names for the 
new functions

Using a flags variable mostly works, as long as you use sensible names for the 
constants representing the flags. As noted in the description it is not as 
simple to explicitly indicate the flag is turned off.

Using a flags structure works as long as the structure members have sensible 
names.  Unlike the case above, it is also simple to set the correct member to 
indicate the flag is turned off

Using enums works as long as the enumerators have sensible names.

### All `bool`s Look Alike

Splitting into two functions can work if you only have two `bool` parameters, 
but any more than that and it becomes impractical.

Using a flags variable or a flags structure works as we no longer have multiple 
variables.

Using enums works because all enums are distinct from each other.

### Conversions To And From `bool`

Splitting into two functions works for the parameter that has been removed, 
although any remaining `bool` parameters being passed could still suffer from 
conversion.

Using a flags variable held in an integer can undergo all the normal integer 
conversions, so it does not solve this problem.

Using a flags variable held in a `std::bitset` is better because you cannot 
assign an integer to a `bitset` or vice versa. Note however that you can 
initialize a `bitset` with an integer, so passing an integer to a function when 
it expects a `bitset` will use the integer to initialize the `bitset`.

Using a flags structure works as structs do not implicitly convert to anything 
else.

Using unscoped enums partially solves the conversion problem. An integer or 
floating-point type cannot be converted to the enum type implicitly[4].  On the 
other hand, values of the enum type are implicitly convertible to integral 
types.

Using scoped enums solves the implicit conversion problem completely[4][5].

### More Than Two Values

Splitting into two functions could solve this problem as you just need to add 
a function for each new value. If your functions are handling two conditions 
then you'll need a new function for each possible new combination, so it may be 
worth redesigning at this point to stop the number of functions from exploding.

Using a flags variable works as you can just increase the number of bits each 
flag uses to represent its value. You do have to be careful that the constants 
for different flags don't overlap each other.

Using a flags structure works by allowing you to easily determine the size of 
each member of the structure. Unlike for the flags variable above you do not 
need to keep fields separated manually.

Using enum types works as you just need to add new enumerators for the new 
values. If using unscoped enums you have to be careful not to create any name 
clashes with enumerators belonging to other unscoped enum types.

## Potential Disadvantages With Suggested Alternatives

This section will discuss some potential disadvantages with the suggested 
alternatives, and hopefully show that they are either not a problem or else the 
pros outweigh the cons.

### More Verbose Code

All of the alternatives suggested make the code more verbose. For most of them 
this is simply a case of replacing code like

    if (x) { ... }

with an explicit test like

    if (x == value) { ... }

It could be argued that making the test explicit does make the code more 
self-documenting, so should not be seen as a disadvantage.

The alternative using constants to define flag bits, either in an integer or 
a `std::bitset`, does have code that looks more complicated, as you have to use 
a binary 'and' to isolate the flag bit and test if it is set, like

    if ((x & flagbit) == flagbit)

or if you are happy to rely on the implicit conversion to `bool` you can use

    if (x & flagbit)

instead. Neither is as clear as the simple test against a value. On the other 
hand with the `std::bitset` you can use the `[]` operator or `test` function to 
check a bit at a position.

### Namespace Pollution

All of the suggested alternatives insert new entities into the current 
namespace, whether functions, constants, or types. All of those entities 
introduce new names into the current namespace which wouldn't need to exist if 
you just used `bool` values.  This will cause problems if they clash with any 
names already in that namespace.

Of course, this isn't specific to this case - it occurs whenever you add new 
entities to a scope, so do whatever you normally would to get around it.

An easy solution is to add the new entities in their own namespace.  This does 
mean that the names need the namespace as an extra qualifier, but you can use 
a `using` declaration to bring the name into the current namespace. If the new 
entities are only used in a single `\*.cpp` file you can put them in an 
anonymous namespace in that file and you won't even need the extra qualifier.

### Size And Speed Of Compiled Programs

A common concern when using the alternatives is that the code will be larger 
and/or slower than when using `bool`s. This should not be a concern as modern 
compilers are intelligent enough to recognise what the code is doing and 
optimizing it appropriately.

Sample code to show this can be found on [BitBucket], [GitHub], or [GitLab], 
depending on your preferred supplier. The various _\*.cpp_ files each 
demonstrate one alternative, except the _bools.cpp_ one which shows the original 
form with `bool` variables.

The _find-medians.sh_ shell script in that directory runs all the programs and 
captures the runtimes, then works out the median and mode runtimes for each one.  
Running this script on my main machine gives the runtimes shown in Table 1 for 
code optimized with `-O3`.

As can be seen, the runtimes for the optimized programs are virtually identical 
for all the programs. This shows that you don't lose much if any speed when 
using the alternatives.

As far as code size is concerned, for the optimized code the program sizes range 
from 17160 bytes for _functions.opt_ to 17320 for _bitset-consts.opt_. The 
_bools.cpp_ file is 17272 bytes. So there is little difference in code size 
either.

## So no more `bool`s then?

It might seem that this article is saying that you shouldn't use `bool` values 
in your programs at all. This is not the intention.

One target is the use of `bool`s in what might be termed long-range code. What 
do we mean by `long-range` code?

Calling a function is long-range, as you are leaving the current function's 
scope and entering the called one. You should think carefully before using 
`bool`s as parameters of functions. As this article has tried to show, there are 
alternatives which can be both safer and clearer, with little or no loss of 
program speed.

Code in a single function could also be considered long-range if the whole usage 
cannot be seen on a single screenful of code[6].  Using a `bool` to store the 
result of a logical operation which is used in the immediately following code is 
fine, as it's obvious what is going on. Even if the value is only used once, if 
it simplifies a condition expression it can still be valid to do so.

Another target is the use of `bool` for class member variables. This is an ideal 
case for using one of the alternatives, especially enums.  Classes provide their 
own scope, so the potential for namespace pollution is immediately reduced. And 
if the member variable is private (as they should normally be), all the code 
using it will be written by the class maintainer, so the users of the class 
won't have to handle it at all.

So in summary, if the use of the `bool` would be obvious from the immediate 
context of the code, it is fine to use it. In all other cases, consider using
an alternative. This article provides several such alternatives as a starting 
point.

## References

[Base64] https://en.wikipedia.org/wiki/Base64

[BitBucket] https://bitbucket.org/dustycorner/articles/src/master/replacing-bool-values/testcode

[CppRef1] https://en.cppreference.com/w/cpp/language/overload_resolution

[CppRef2] https://en.cppreference.com/w/cpp/language/enum

[GitHub] https://github.com/dustycorner/articles/tree/master/replacing-bool-values/testcode

[GitLab] https://gitlab.com/dustycorner/articles/-/tree/master/replacing-bool-values/testcode

[P1099r5] Gašper Ažman and Jonathan Müller, _Using Enum_, http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1099r5.html

[Preussner] Dimensional Analysis in Programming Languages, https://gmpreussner.com/research/dimensional-analysis-in-programming-languages

[UUencode] https://en.wikipedia.org/wiki/Uuencoding

## Footnotes

[1] This example may seem contrived, but I once worked on a system that had many 
functions with three or four `bool` parameters. A lot of the calls were done 
using literal values for some or all of the parameters, and only checking the 
surrounding code could confirm whether the values were correct.

[2] I have seen this pseudo-operator referred to as the 'normalise operator'.  
The way it works is by relying on the right-to-left binding of the `!` operator.  
The right-hand `!` applies to the operand, forcing it to the `bool` equivalent 
and then negating the result.  The left-hand `!` then applies to the resulting 
value and negates it again, giving us back the `bool` equivalent of the original 
operand.

[3] Anyone old enough to have used one of the microcomputers released during the 
1980s home computer boom might remember that the BASIC built into many of them 
used -1 for the 'true' value, presumably because the representation of that 
value has all bits set to 1. Sinclair Basic, as used on the ZX81 and Spectrum, 
went its own way and used 1 for the 'true' value.

[4] Although you can use an explicit cast, such as a `static_cast`, to convert 
integer, floating-point, or enumeration values to an enum type, whether unscoped 
or scoped.

[5] Scoped enum values can be converted to integer values using a `static_cast` 
though.

[6] And by a single screenful of code I don't mean using huge monitors and small 
fonts to get 150+ lines of code on a screen at a time. Think more like 40 to 50 
lines maximum, so a quick scan up and down is easy to do.

## Tables

### Table 1 Optimized Runtimes

|Test Case|Median|Mode|
|:--------|:----:|:--:|
|Bools|387|387|
|Bitset with constants|387|388|
|Bitset with indexing|387|388|
|Scoped enum|387|387|
|Unscoped enum|387|387|
|Functions|382|382|
|Integer flags|387|387|
|Struct with bitfields|387|386|
|Struct with bools|387|387|

