#include<stdio.h>
// our dictionary
#define WORD_world  world,
#define WORD_new    new,
#define WORD_brave  brave,
#define WORD_hello  hello,
#define WORD_Hello  Hello,

// the classics
#define COMMA(...)  ,
#define FIRST(a, ...)  a

// apply function f for each argument recursively with tail
#define FOREACHTAIL_1(f,a)      f(a,)
#define FOREACHTAIL_2(f,a,...)  f(a,FOREACHTAIL_1(f,__VA_ARGS__)) 
#define FOREACHTAIL_3(f,a,...)  f(a,FOREACHTAIL_2(f,__VA_ARGS__)) 
#define FOREACHTAIL_4(f,a,...)  f(a,FOREACHTAIL_3(f,__VA_ARGS__)) 
#define FOREACHTAIL_N(_4,_3,_2,_1,N,...)  \
        FOREACHTAIL_##N
#define FOREACHTAIL(f,...) \
        FOREACHTAIL_N(__VA_ARGS__,4,3,2,1)(f,__VA_ARGS__)

// if there are two arguments, expand to true. Otherwise false.
#define IFTWO_N(_0,_1,N,...)     N
#define IFTWO(true, false, ...)  IFTWO_N(__VA_ARGS__, true, false)

// If empty, expand to true, otherwise false.
// https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
#define IFEMPTY(true, false, ...)  IFTWO(true, false, COMMA __VA_ARGS__ ())

// Join arguments with `_`.
#define JOIN_U(a, b)      a##_##b
#define JOIN_TWO_IN(a,b)  IFEMPTY(FIRST, JOIN_U, b)(a, b)
#define JOIN_TWO(a,b)     JOIN_TWO_IN(a,b)
#define JOIN(...)         FOREACHTAIL(JOIN_TWO, __VA_ARGS__)

// Append WORD_ to each argument and join arguments with spaces.
#define WORD_             /* the last one expands to empty */
#define WORDS_TWO(a, b)   WORD_##a b
#define WORDS(...)        FOREACHTAIL(WORDS_TWO, __VA_ARGS__)

#define MAGIC_MACRO(a)  JOIN(WORDS(WORDS(WORDS(WORDS(WORDS(WORDS(WORDS(WORDS(WORDS(a))))))))))

#define STRFY1(...) STRFY2(__VA_ARGS__)
#define STRFY2(...) STRFY(__VA_ARGS__)
#define STRFY(...) #__VA_ARGS__


int main() {
	printf(STRFY1(MAGIC_MACRO(hello brave new  world )));
}
