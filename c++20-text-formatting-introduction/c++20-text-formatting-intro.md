# C++20 Text Formatting - An Introduction

Much of the talk about the C++20 standard has focused on the 'big four' items, 
i.e. modules, concepts, coroutines, and ranges. This tends to obscure other 
improvements and additions that may have a bigger impact on the general 
programmer who spends their lives working on application code.

One such addition is the new text formatting library, `std::format`.  This 
brings a more modern approach to text formatting, akin to Python's `str.format`. 
This article is intended as a brief introduction to the library, outlining the 
main items that allow you to produce formatted text.

The original proposal for the library [P0645] was written by Victor Zverovich 
and was based on his `{fmt}` library [fmtlib]. It was subsequently extended and 
modified by further proposals. A brief history can be found in a blog post 
[Zverovich2019].

This article deals with the formatting of fundamental types and strings. A later 
article will describe how you can write formatters for your own types.

## Current Implementation Status

At the time of writing (September 2021), support for `std::format` in major 
compilers is patchy.

The C++ library support page for GCC [GCClib] indicates that support is not yet 
available. A query to the libstdc++ mailing list received the response that no 
work on implementing it was currently known.

For Clang, work is being carried out on an implementation, and the progress can 
be found at [ClangFormat]. It is expected that full support will be available in 
Clang 14, due for release in 2022.

For MSVC, the C++ library support page [MSVClib] indicates that support is 
available for `std::format`, but with a caveat that to use it you currently need 
to pass the `/std:c++latest` flag, because of ongoing work on the standard.

Given the above, the code samples in this article were compiled using the 
`{fmt}` library, version 8.0.1. This version provides `std::format` compatible 
output.  Versions of `{fmt}` before 8.0.0 had some differences, especially 
regarding some floating-point formatting and locale handling.

To convert the listings to use the standard library when available, replace the 
`#include <fmt/format.h>` with `#include <format>`, and remove the `using 
namespace fmt` line. One small wrinkle is that `{fmt}` has its own `string_view` 
class, so on the rare occasions when we use `string_view` in the examples, it is 
always qualified with the `std` namespace.

# Text Formatting Functions

This section describes the main `std::format` functions. These are all you need 
if you just want to produce formatted text.

## `format`

The first function provided by `std::format` is called `format`. [Listing 
1](code/basic-formatting.cpp) gives an example of how you would use it, along 
with lines that produce the same output using `printf` and iostreams. Output of 
this program is given in [Figure 1](output/basic-formatting.out).

As can be seen from the listing, the interface to the `format` function is 
similar to the one for `printf`. It takes a _format string_ specifying the 
format of the data to be output, followed by a list of values to use to replace 
fields defined in the string.  In `printf` the fields to be replaced are 
indicated by preceding the format instructions with `%`, while in `format` they 
are delimited by `{` and `}` characters.

Looking at the strings passed to `format` in the listing, it is obvious that 
there is nothing in the replacement fields that indicates the types of values to 
be output.  Unlike `printf`, `format` knows what types of arguments have been 
passed.  This is because it is defined as a template function with the following 
signature:

    template<class... Args>
    string format(string_view fmt, const Args&... args);

The `fmt` argument is the _format string_ specifying what the output should look 
like. The `args` arguments are the values we want to output. Note that `format` 
returns a string, so to output it you need to write the string somewhere 
- in the example, we simply send it to `cout`.

The _format string_ syntax will be described in more detail later, but for now, 
it is sufficient to know that the `{}` items output the corresponding value from 
`args` using its default formatting.

Because `format` knows what the types of each argument are, if you try to use 
incompatible formatting with a value it will throw an exception. [Listing 
2](code/bad-format.cpp) demonstrates this, where we give an integer argument, 
but the format type is a string one. This function produces the following 
output:

    Caught format_error: invalid type specifier

This contrasts with `printf`, which in all likelihood will at best output 
garbage with no indication why, and at worst can crash your program.

## `format_to` and `format_to_n`

The `format` function always returns a new string on each call. This is  
a problem if you want your output to be built up in several stages, as you would 
have to store each string produced and then stitch them all together at the end 
when outputting them.

To avoid this, you can use the `format_to` function. This appends the formatted 
text to the given output. The signature for this function is as follows:

    template<class Out, class... Args>
    Out format_to(Out out, string_view fmt, const Args&... args);

The first parameter, `out`, is an output iterator, which has to model 
`OutputIterator<const char&>`. The formatted output is sent to this output 
iterator. The function returns the iterator past the end of the written text.

[Listing 3](code/format_to.cpp) shows how you might use `format_to` to output 
all the values in a vector. The output is a `back_insert_iterator<string>`, 
which matches the constraint, and appends the formatted values to the end of the 
string. Output from this program is in [Figure 2](output/format_to.out).

If you also need to limit the number of characters written, use the 
`format_to_n` function. The signature for this function is similar to that for 
`format_to`, as follows:

    template<class Out, class... Args>
    format_to_n_result<Out> format_to_n(Out out, iter_difference_t<Out> n,
                    string_view fmt, const Args&...  args);

This takes the maximum number of characters to write in parameter `n`.  The 
return value of this function is a `format_to_n_result` structure, which 
contains the following members:

`out` - Holds the output iterator past the text written.

`size` - Holds the size that the formatted string would have had, before any 
potential truncation to a length of `n`. This can be used to detect if the 
output has been truncated, by checking if the value is greater than the `n` 
passed in.

The `VecOut` function in [Listing 4)(code/format_to_n.cpp) is similar to the one 
in Listing 3, but this time it limits the number of characters written for each 
value to 5. As can be seen from the output in [Figure 
3](output/format_to_n.out), the third value in `v2` is truncated from 1000000 to 
10000 - probably something you'd only want to do if you were putting together 
a toy program to illustrate how the `format_to_n` function works.

## `formatted_size`

If you need to know how many characters would be output for a particular format 
string and set of arguments, you can call `formatted_size`. This can be used if 
you want to create a buffer of the right size to accept the output. The function 
has the following signature:

    template<class... Args>
    size_t formatted_size(string_view fmt, const Args&... args);

The `size_t` value returned gives the length that the formatted string would 
have with the given arguments. If you are using this to create a buffer to write 
a C-style string to, remember that the value returned would not include any 
terminating '\0' character unless you include it in the _format string_.

[Listing 5](code/formatted_size.cpp) illustrates the use of `formatted_size`.  
The output is in [Figure 4](code/formatted_size.out). It may appear that the 
length output is incorrect but remember that the terminating newline character 
is included in the _format string_.

## Wide-character Support

The functions described above all deal with classes (`string`, `string_view`, 
the output iterators) that use `char` to represent the characters being handled.  
If you need to use `wchar_t` characters, there is an overload for each of the 
functions which take or return the appropriate class using `wchar_t`. For 
instance, the `format` function that uses `wchar_t` has the following signature:

    template<class... Args>
    wstring format(wstring_view fmt, const Args&... args);

Note that as of the C++20 standard, `std::format` does not handle any of the 
`charN_t` types (e.g. char16_t, char32_t).

## Error Reporting

Any errors detected by `std::format` are reported by throwing objects of the class 
`format_error`. This is derived from the `std::runtime_error` class so it has 
a `what` function that returns the error string passed when the exception is 
created. Listing 2, presented previously, shows an example of catching 
a `format_error`.

# Format string

The _format strings_ used by the `std::format` functions consist of _escape 
sequences_, _replacement fields_, and other text. They are based on the style 
used by the Python `str.format`, for anyone familiar with that. A similar style 
is also used in the .NET family of languages, and in Rust.

## Escape Sequences

The two _escape sequences_ recognised are `{{` and `}}`, which are replaced by 
`{` and `}` respectively. You would use them if you need a literal `{` or `}` in 
the output.

Obviously this is distinct from the normal string escapes that the compiler 
requires if you want to insert special characters in the string, such as `\n`.  
By the time the `std::format` functions see the string, these will have already 
been replaced by the compiler.

## Replacement Fields

A _replacement field_ controls how the values passed to the `std::format` 
function are formatted. A replacement field has the following general format:

>'`{`' \[_arg-id_\] \['`:`' _format-spec_\] '`}`'

where:

_arg-id_
> If given, this specifies the index of the argument in the value list that is 
> output by the _replacement field_. Argument indexes start at 0 for the first 
> argument after the _format string_.

_format-spec_
> Gives the format specification to be applied to the value being handled. Note 
> that if you give a _format-spec_, you have to precede it with a `:`, even if 
> you do not give an _arg-id_.

Later sections will give more details on _arg-ids_ and _format-specs_.

Examples of valid _replacement fields_ are `{}`, `{0}`, `{:10d}`, `{1:s}`.

## Other Text

Any text that isn't part of a replacement field or an escape sequence is output 
literally as it appears in the _format string_.

# Argument IDs

The first item in a _replacement field_ is an optional _arg-id_. This specifies 
the index of the value in the argument list that you want to use for that 
_replacement field_. Argument index values start at 0.

If not specified, the arguments are simply used in the order that they appear in 
the function call. This is known as automatic argument numbering. For instance, 
in Listing 1 the `format` call has no _arg-ids_, so the arguments are just used 
in the order `i`, `f`, `s`.

A given _format string_ cannot have a mix of manual and automatic argument 
numbering. If you use an _arg-id_ for one _replacement field_ you have to use 
_arg-ids_ for all _replacement fields_ in the _format string_.

A simple use for this argument numbering can be seen in [Listing 
6](code/repeated-param.cpp), where it is used to output the same value in three 
different bases, along with lines that do the same thing for both `printf` and 
iostreams. The output from this is in [Figure 5](output/repeated-param.out).

Another important use for this facility will be described later in the section 
**Internationalization**.

Note that the _format string_ does not have to specify _arg-ids_ for all the 
arguments passed to the function. Any that are not given will simply be ignored.  
An example of this is shown in [Listing 7](code/unused-args.cpp), with the 
output in [Figure 6](output/unused-args.out). 

# Format Specifications

The standard _format-spec_ has the following general format[1]:

\[\[_fill_\] _align_\] \[_sign_\] \[`#`\] \[`0`\] \[_width_\] \[_prec_\] \[`L`\] \[_type_\]

There should be no spaces between each item in the _format-spec_. Also, every 
item is optional, except that if _fill_ is specified, it must be immediately 
followed by _align_. If _align_ is given any '`0`' will be ignored.

Anyone familiar with `printf` format strings will see that `std::format` uses 
a very similar style. However, there are some significant differences, so the 
following sections describe each item in the above format in detail, except for 
the '`L`' character, which will be left until the section on 
internationalization.

Note that like `printf` format specifiers, but unlike many iostreams 
manipulators, the values given in a _format-spec_ only apply to the current 
field and don't affect any later fields.

The _type_ option is called the presentation type. The valid values for each 
fundamental type are given below, along with a description of what effect they 
have. Remember that, unlike `printf`, `std::format` knows the type of value 
being output, so if you just want the default format for that value, you can 
omit the _type_ option.

## Text Alignment and Fill

The _align_ value is a single character that gives the alignment to use for the 
current field. It can have any of the values `<`, `>`, or `^`. The meaning of 
these is as follows:

>`<` - The value is left-justified in the field width. This is the default for 
string fields.

>`>` - The value is right-justified in the field width. This is the default for 
numeric fields.

>`^` - The value is centred in the field width. Any padding will be distributed 
evenly on the left and right sides of the value. If an odd number of padding 
characters is needed, the extra one will always be on the right.

If the first character in the _format-spec_ is immediately followed by one of 
the alignment characters, that first character is treated as the fill character 
to use if the field needs padding. A _fill_ character **must** be followed by 
a valid _align_ character. You cannot use either of the characters `{` or `}` as 
fill characters.

Note: The _fill_ and _align_ values only make sense if you also specify 
a _width_ value, although it is not an error to specify them without one.

[Listing 8](code/align-fill.cpp) shows the effect of the _align_ and _fill_ 
values.  Output is in [Figure 7](output/align-fill.out).

## Sign, #, and 0

The _sign_ value specifies how the sign for an arithmetic type is to be output.  
It can take the following values:

>`+` - A sign should always be output for both negative and non-negative values.

>`-` - A sign should only be output for negative values. This is the default.

>(space) - A sign should be output for negative values, and a space for 
non-negative values.

The `#` character indicates that the alternative form should be used for output 
of the given value. The meaning of this is described under the appropriate 
section below.

The `0` character is only valid when also specifying a _width_ value. If present 
it pads the field with `0` characters after any sign character and/or base 
indicator. If an _align_ value is present, any `0` character is ignored.

Note that the _sign_, `#`, and `0` values are only valid for arithmetic types,
and for `bool` or `char` (`wchar_t` in wide string functions) when an integer 
presentation type is specified for them (see later).

[Listing 9](code/sign-zero.cpp) shows the effect of the _sign_ and `0` values.  
Output is shown in [Figure 8](output/sign-zero.out). The effect of the `#` value 
will be shown in examples in the arithmetic type sections.

## Width and Precision

The _width_ value can be used to give the minimum width for a field. If the 
output value needs more characters than the specified width it will be displayed 
in full, not truncated to the width. If you need the value to be truncated to 
a certain width you can use the `format_to_n` function to output the value, with 
the guarantee that only the given number of characters at most will be written.

The value given for the _width_ field depends on whether you are hard-coding the 
width in the string, or need it to be specified dynamically at runtime. If it is 
to be hard-coded it should be given as a literal positive decimal number. If you 
need to specify the width dynamically at runtime, you use a nested replacement 
field, which looks like `{}` or `{`_n_`}`.

[Listing 10](code/widths.cpp) demonstrates the use of the _width_ value, using 
both literal values and nested replacement fields with automatic and manual 
numbering.  As shown in [Figure 9](output/widths.out), if the value is wider 
than the given _width_, the _width_ value is ignored and the field is wide 
enough to display the full value.

The _prec_ value is formed of a decimal point followed by the precision, which 
like the _width_ field can be a literal positive decimal number or a nested 
replacement field.

The _prec_ value is only valid for floating-point or string fields. It has 
different meanings for the two types and will be described in the relevant 
section below.

If using a nested replacement field for either _width_ or _prec_, you must use 
the same numbering type as for the _arg-ids_, e.g. if using manual numbering for 
_arg-ids_ you must also use it for nested replacement fields.

If you use automatic numbering, the _arg-ids_ are assigned based on the count
of `{` characters up to that point, so the _width_ and/or _prec_ values come
after the value they apply to. This contrasts with `printf`, where if using the
`*` to indicate the value is read from the argument list, the values for
_width_ and _prec_ appear before the value they apply to.

## Integer Presentation Types

The available integer presentation types are given below. Where relevant, the 
effect of selecting the alternate form using the `#` flag is also listed. Note 
that any sign character will always precede the prefix added in alternate form.

* `d` - Decimal format. This is the default if no presentation type is given.

* `b`, `B` - Binary format. For alternate form, the value is prefixed with `0b` 
for `b`, and `0B` for `B`.

* `o` - Octal format. For alternate form, the value is prefixed with `0` as long 
as it is non-zero. For example, 7 outputs as `07`, but 0 outputs as `0`.

* `x`, `X` - Hecadecimal format. The case of digits above 9 matches the case of 
the presentation type. For alternate form, the value is prefixed with `0x` for 
`x`, or `0X` for `X`.

* `c` - Outputs the character with the code value given by the integer. 
A `format_error` will be thrown if the value is not a valid code value for the 
character type of the _format string_.

[Listing 11](code/integer-format.cpp) gives examples of outputting using all the 
presentation types, with and without alternate form where that is relevant. The 
output is shown in [Figure 10](output/integer-format.out).

## Floating-point Presentation Types

The available floating-point presentation types are given below.

* `e` - Outputs the value in scientific notation. If no _prec_ value is given, 
it defaults to 6.

* `f` - Outputs the value in fixed-point notation. If no _prec_ value is given, 
it defaults to 6.

* `g` - Outputs the value in general notation, which picks between `e` and `g` 
form. The rules are slightly arcane but are the same as used for `g` when used 
with `printf`. If no _prec_ value is given, it defaults to 6.

* `a` - Outputs the value using scientific notation, but with the number 
represented in hexadecimal. Because `e` is a valid hex digit, the exponent is 
indicated with a `p` character.

If no presentation type is given, the output depends on whether a _prec_ value 
is given or not. If _prec_ is present the output is the same as using `g`. If 
_prec_ is not present, the output is in either fixed-point or scientific 
notation, depending on which gives the shortest output that still guarantees 
that reading the value in again will give the same value as was written out.

If the floating-point value represents infinity or NaN, the values 'inf' and 
'nan' will be output respectively. They will be preceded by the appropriate sign 
character if required. Specifying `#` does not cause a base prefix to be output 
for infinity or NaN.

The `e`, `f`, `g`, and `a` presentation types have equivalent `E`, `F`, `G`, and 
`A` types which perform the same, but output any alphabetic characters in 
uppercase rather than lowercase. For the `f` and `F` types this only affects 
output of infinity and NaN.

If the `#` character is used to select the alternate form, it causes a decimal 
point character to always be output, even if there are no digits after it. This 
does not apply to infinity and NaN values.

[Listing 12](code/float-format.cpp) gives examples of outputting using the 
lowercase presentation types, and how _prec_ and alternate form affects the 
output. Output is given in [Figure 11](output/float-format.out).

[Listing 13](code/float-inf-nan.cpp) gives examples of outputting infinity and 
NaN values. Because all presentation types give the same output for infinity and 
NaN, we only give the output for types `f` and `F`. Output is given in  [Figure 
12](output/float-inf-nan.out).

## Character Presentation Types

The default presentation type for `char` and `wchar_t` is `c`. It simply copies 
the character to the output.

You can also use the integer presentation types `b`, `B`, `d`, `o`, `x`, and 
`X`. They write the integer value of the character code to the output, and take 
account of the alternate form flag `#` if relevant.

[Listing 14](code/char-format.cpp) gives examples of outputting characters.  
Output is given in [Figure 13](output/char-format.out).

## String Presentation Types

String formatting works for `std::string` and `std::string_view` as well as the 
various `char *` types.

The only presentation type for strings is `s`, which is also the default if not 
given. The default alignment for string fields is left-justified.

If a precision value is specified with _prec_, and it is smaller than the string 
length, it causes only the first _prec_ characters from the string to be output.  
This has the effect of reducing the effective length of the string when checking 
against any _width_ parameter.

[Listing 15](code/string-format.cpp) shows examples of outputting various types 
of string, as well as the interaction between _width_ and _prec_ values. Output 
is shown in [Figure 14](output/string-format.out).

## Bool Presentation Types

The default `bool` presentation type is `s`, which outputs **true** or 
**false**.

You can also use the integer presentation types `b`, `B`, `d`, `o`, `x`, or `X`.  
These behave like the same types for integers, treating `false` as 0 and `true` 
as 1.

You can also use `c` as a `bool` presentation type. It will output the 
characters with values 0x1 and 0x0 for `true` and `false`, which may not be what 
you expect (or particularly useful).

[Listing 16](code/bool-format.cpp) is an example of formatting `bool`s, with the 
output in [Figure 15](output/bool-format.out).

The line that uses the `c` presentation type appears to print nothing, but if 
you send the output to a file and then examine the output using a program that 
displays the actual bytes in the file, you will see that the characters with 
codes 0x0 and 0x1 have been output. For instance, you can use `od -c` on a Unix
or Linux box.

## Pointer Presentation Types

The only _type_ value available for pointers is '`p`'. It can be omitted and 
`std::format` will deduce the type from the argument. Note that if the pointer 
is to one of the `char` types, it will be treated as a string, not as a pointer.  
If you want to output the actual pointer value you need to cast it to a `void*`.

Pointer values are output in hexadecimal, with the prefix '0x' added. and digits 
`a` to `f` in lowercase. The output is right-justified by default, just like 
arithmetic types.

Note that the C++20 standard specifies that only pointers for which 
`std::is_void_t` returns true can be output by `std::format`, which in practice 
means you need to cast any pointers to `void*`.  [Listing 
17](code/pointer-format.cpp) shows examples of pointer formatting, and does 
exactly that. Sample output is in [Figure 16](output/pointer-format.out).

# Internationalization

Internationalization, or i18n as it is commonly written, is the process of 
writing a program so its output can be used natively by people speaking 
different languages and with different conventions for writing things like 
numbers and dates.

By default, `std::format` takes no account of the current locale when outputting 
values. The reasons for this are described in the original proposal [P0645] in 
the section **Locale support**. In contrast, iostreams takes account of the 
locale on all output, even if it is set to the default.

## Format Strings

The ability to use manual argument numbering in _format strings_ to reorder 
arguments is useful when using translated output. Allowing arguments to appear 
in a different order in the output can make for grammatically correct output in 
a given language.

Rather than hard-coding the _format strings_ in your code, you could use 
a mechanism that provides the correct translated string to use, with manual 
argument numbers inserted. Libraries exist that make it easier to use such 
translated strings - for instance, GNU's `gettext` library [gettext] takes 
a string and looks up the translated version of it, as long as the translations 
have been provided in the correct format.

## Locale-aware Formatting

In a _format-spec_, the `L` modifier can be used to specify that the field 
should be output in a locale-aware fashion. Without this modifier, the locale is 
ignored.  You can use this for output of numeric values, and also `bool` when 
doing string format output. When used with `bool` it changes the **true** and 
**false** values to the appropriate `numpunct::truename` and 
`numpunct::falsename` instead.

The various output functions described earlier use the global locale when doing 
locale-aware formatting. You can change the global locale with a function call 
like the following:

    std::locale::global(std::locale("de_DE"));

This will set the global locale to the one for Germany, `de_DE`.

If you only want to change the locale for a single function call, there are 
overloads of the various output functions that take the locale as their first 
parameter. For instance, the `format` function has the following overload:

    template<class... Args>
    string format(const std::locale& loc, string_view fmt, const Args&... args);

[Listing 18](code/locale.cpp) shows examples of using locale-aware output, using 
both global locales and function-specific ones. Output from this program is 
shown in [Figure 17](output/locale.out).

# Avoiding code bloat

The formatting functions are all template functions.  This means that each time 
one of the functions is used with a new set of argument types, a new template 
instantiation will be generated.  This could quickly lead to unacceptable code 
bloat if these functions did the actual work of generating the output values. 

To avoid this problem, the `format` and `format_to` functions call helper 
functions to do the actual formatting work. These helper functions have names 
formed by adding '`v`' to the start of the name of the calling function. For 
instance, `format` calls `vformat`, which has the following signature:

    string vformat(string_view fmt, format_args args);

The `format_args` argument is a container of type-erased values, the details of 
which are probably only interesting to library authors. They are certainly 
outside the scope of this article.

There will only be a single instantiation of the `vformat` function, and one 
`vformat_to` instantiation for each type of output iterator.

The actual work of doing the formatting is done by these helper functions, so 
the amount of code generated for each call to `format` or `format_to` is 
considerably reduced.

These functions are part of the `std::format` public API, so you can use them 
yourself if you want to. [Listing 19](code/vlog.cpp), which is based on code 
presented in [P0645], shows one such use, with a logging function that takes any 
number of arguments. The output from this function is in [Figure 
18](output/vlog.out).  The call to `vlog_error` uses the `make_format_args` 
function to generate the `format_args` structure.

# Conclusion

Hopefully this article has given you a taster of what `std::format` can do. If 
you want to start trying it out you can use `{fmt}` as a good proxy for it until 
library authors catch up with the standard.

In my own projects I am already using the `std::format` compatible parts of 
`{fmt}`, and in general find it easier and clearer than the equivalent iostreams 
code.

In future articles I intend to explore how to create formatters for your own 
user defined types, and also how to convert from existing uses of iostreams and 
`printf`-family functions to `std::format`.

# Acknowledgements

I'd like to extend my thanks to Victor Zverovich for responding quickly to my 
various queries whilst writing this article, and also for reviewing draft 
versions of the article, making many useful suggestions for improvement. I'd 
also like to thank the _Overload_ reviewers for making useful suggestions for 
improvements to the article. Any errors and ambiguities that remain are solely 
my responsibility.

# Appendix 1: Comparison Of `format` And `printf` Format Specifications

Both `std::format` and `printf` use format specifications to define how a field 
is to be output. Although they are similar in many cases, there are enough 
differences to make it worth outlining them here. The following list has the 
`printf` items on the left and gives the `std::format` equivalent if there is 
one.

* `-` - Replaced by the `<`, `^`, and `>` flags to specify alignment.  
  Repurposed as a sign specifier

* `+`, space - These have the same meaning and have been joined as sign
  specifiers by `-`.

* `#` - Has the same meaning.

* `0` - Has the same meaning.

+ `*` - Used in `printf` to say the width or precision is specified at run-time.  
  Replaced by nested replacement fields.  See Note 1 below on argument ordering 
differences when converting these.

* `d` - In `printf` it specifies a signed integer. In `std::format` it specifies 
any integer, but as it is the default it can be omitted, except when outputting 
the integer value of a character or `bool`.

* `h`, `l`, `ll`, `z`, `j`, `t` - Not used. In `printf` they specify the size of 
  integer being output. `std::format` is type aware so these are not needed.

* `hh` - Not used. In `printf` it specifies the value is a char to be output as 
  a numeric value. Use `d` in `std::format` to do the same thing.

* `i`, `u` - Not used. Replaced by `d`.

* `L` - Not used. In `printf` it specifies a `long double` value is being 
  passed, but `std::format` is type aware. The `L` character has been repurposed 
  to say the field should take account of the current locale when output.

* `n` - Not used. In `printf` it saves the number of characters output so far to 
  an integer pointed to by the argument. Use `formatted_size` as a replacement.

* `c`, `p`, `s` - Have the same meaning, but as they are the default output type 
  for their argument type, they can be omitted.

* `a`, `A`, `e`, `E`, `f`, `F`, `g`, `G`, `o`, `x`, `X` - All have the same 
meaning.

Note 1: If using dynamic values for width or precision, and you are using 
automatic parameter numbering in `std::format`, the order of assigning parameter 
numbers when parsing the string means the width and precision values come after 
the value being output, whereas in the `printf`-family functions they come 
before the value.

Note 2: POSIX adds positional parameters to the `printf` format specification.  
These are specified using `%n$` for value fields, or `*m$` for width and 
precision fields - e.g `%1$*2$,*3$f`.  The `std::format` format specification 
already supports these using manual numbering mode.

Table 1 shows examples of `printf` formatting and the equivalent `std::format` 
version.

<table>
<caption>Table 1</caption>
<tr>
<th><pre>printf</code></th>
<th><pre>std::format</code></th>
</tr>
<tr>
<td>
<pre>
//  Integer output
int iv = -1;
short sv = -2;
long lv = -3;
long long llv = -4;
printf("%i, %hi, %li, %lli\n", iv, sv, lv, llv);
unsigned int uiv = 1;
unsigned short usv = 2;
unsigned long ulv = 3;
unsigned long long ullv = 4;
printf("%u, %hu, %lu, %llu\n", uiv, usv, ulv, ullv);
</code>
</td>
<td>
<pre>
//  Integer output
int iv = -1;
short sv = -2;
long lv = -3;
long long llv = -4;
cout << format("{}, {}, {}, {}\n", iv, sv, lv, llv);
unsigned int uiv = 1;
unsigned short usv = 2;
unsigned long ulv = 3;
unsigned long long ullv = 4;
cout << format("{}, {}, {}, {}\n", uiv, usv, ulv, ullv);
</code>
</td>
</tr>
<tr>
<td>
<pre>
//  Floating-point output
float f = 1.234;
double d = 2.345;
long double ld = 3.456;
printf("%g %g %Lg\n", f, d, ld);
</code>
</td>
<td>
<pre>
//  Floating-point output
float f = 1.234;
double d = 2.345;
long double ld = 3.456;
cout << format("{} {} {}\n", f, d, ld);
</code>
</td>
</tr>
<tr>
<td>
<pre>
//  Character output
char c = 'A';
printf("%c %hhd\n", c, c);
</code>
</td>
<td>
<pre>
//  Character output
char c = 'A';
cout << format("{} {:d}\n", c, c);
</code>
</td>
</tr>
<tr>
<td>
<pre>
// String output
const char* cptr = "Mary had a little lamb";
std::string str(cptr);
std::string_view strview(str);
printf("%s\n", cptr);
printf("%s\n", str.c_str());
// printf cannot handle std::string_view
</code>
</td>
<td>
<pre>
// String output
const char* cptr = "Mary had a little lamb";
std::string str(cptr);
std::string_view strview(str);
cout << format("{}\n", cptr);
cout << format("{}\n", str);
cout << format("{}\n", strview);
</code>
</td>
</tr>
<tr>
<td>
<pre>
// Pointer output
printf("%p\n", static_cast<const void*>(cptr));
</code>
</td>
<td>
<pre>
// Pointer output
cout << format("{}\n", static_cast<const void*>(cptr));
</code>
</td>
</tr>
<tr>
<td>
<pre>
//  Field Alignment
printf("[%-10d] [%10d]\n", iv, iv);
// printf doesn't support centred alignment
</code>
</td>
<td>
<pre>
//  Field Alignment
cout << format("[{:<10}] [{:>10}]\n", iv, iv);
cout << format("[{:^10}]\n", iv);
</code>
</td>
</tr>
<tr>
<td>
<pre>
//  Sign Field
printf("[%+d] [% d] [%+d] [% d]\n", -1, -1, 1, 1);
// printf does not support '-' as a sign specifier
</code>
</td>
<td>
<pre>
//  Sign Field
cout << format("[{:+}] [{: }] [{:+}] [{: }]\n", -1, -1, 1, 1);
cout << format("[{:-}] [{:-}]\n", -1, 1);
</code>
</td>
</tr>
<tr>
<td>
<pre>
// Run-time width and precision
int width=10, prec=5;
double val=123.456;
printf("%*.*f\n", width, prec, val);
</code>
</td>
<td>
<pre>
// Run-time width and precision
int width=10, prec=5;
double val=123.456;
cout << format("{:{}.{}f}\n", val, width, prec);
</code>
</td>
</tr>
<tr>
<td>
<pre>
//  Positional Parameters
float fv = 123.456;
printf("%1$*2$.*3$f\n", fv, 10, 5);
</code>
</td>
<td>
<pre>
//  Positional Parameters
float fv = 123.456;
cout << format("{0:{1}.{2}f}\n", fv, 10, 5);
</code>
</td>
</tr>
</table>

# Appendix 2: `std::format` and `{fmt}`

As previously mentioned, `std::format` is based on the `{fmt}` library. However, 
`{fmt}` offers a number of extra facilities that are not in the C++20 version of 
`std::format`. This appendix gives a brief description of the main ones.

## Direct Output To A Terminal

Both iostreams and the `printf`-family functions have the ability to write 
directly to the terminal, either using `std::cout` or the `printf` function 
itself. The only way to do this in `std::format` in C++20 is to use `format_to` 
with a `back_inserter` attached to `std::cout`, but this is not recommended as 
it leads to slow performance. This is why the examples in this article write the 
strings produced to `std::cout`.

The `{fmt}` library provides a `print` function to do this work. It can in fact 
write to any `std::FILE*` stream, defaulting to `stdout` if none is specified in 
the function call. A proposal to add this to C++23 has been made [P2093].

## Named Arguments

The `{fmt}` library supports named arguments, so you can specify an argument in 
the replacement field by name as well as position.

## Output of Value Ranges

There are a number of utility functions provided in `{fmt}`. One of the most 
useful is `fmt::join`, which can be used to output ranges of values in a single 
operation, with a given separator string between each value. The ranges output 
can be tuples, initializer_lists, any container to which `std::begin` and 
`std::end` can be applied, or any range specified by begin and end iterators.

# References

[ClangFormat] Libc++ Format Status, https://libcxx.llvm.org//Status/Format.html

[fmtlib] `{fmt}` library, https://github.com/fmtlib/fmt

[GCClib] GCC library support, https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2020

[gettext] GNU gettext library, https://www.gnu.org/software/libc/manual/html_node/Translation-with-gettext.html

[MSVClib] MSVC C++ library support, https://docs.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance

[P0645] _Text Formatting_, Victor Zverovich, 2019, http://wg21.link/P0645

[P2093] _Formatted Output_, Victor Zverovish, 2021, http://wg21.link/P2093

[Zverovich2019] _std::format in C++20_, Victor Zverovich, https://www.zverovich.net/2019/07/23/std-format-cpp20.html

# Footnotes

[1] This section describes the standard _format-spec_ defined by `std::format` 
for formatting fundamental types, `string`s, and `string_view`s. Other types 
like `std::chrono` have their own _format-spec_ definitions, and user-defined 
types can also define their own.
