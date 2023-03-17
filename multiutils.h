// DISCLAIMER NOT MY CODE
// apply function f for each argument recursively with tail
int TIMES_CALLED = 0;
#define FOREACHTAIL_1(f,...)      f(__VA_ARGS__,)
#define FOREACHTAIL_2(f,a,...)  f(a,FOREACHTAIL_1(f,__VA_ARGS__)) 
#define FOREACHTAIL_3(f,a,...)  f(a,FOREACHTAIL_2(f,__VA_ARGS__)) 
#define FOREACHTAIL_4(f,a,...)  f(a,FOREACHTAIL_3(f,__VA_ARGS__)) 
#define FOREACHTAIL_5(f,a,...)  f(a,FOREACHTAIL_4(f,__VA_ARGS__)) 
#define FOREACHTAIL_6(f,a,...)  f(a,FOREACHTAIL_5(f,__VA_ARGS__)) 
#define FOREACHTAIL_7(f,a,...)  f(a,FOREACHTAIL_6(f,__VA_ARGS__)) 
#define FOREACHTAIL_8(f,a,...)  f(a,FOREACHTAIL_7(f,__VA_ARGS__)) 
#define FOREACHTAIL_N(_8,_7,_6,_5,_4,_3,_2,_1,N,...)  \
        FOREACHTAIL_##N
#define FOREACHTAIL(f,...) \
        FOREACHTAIL_N(__VA_ARGS__,8,7,6,5,4,3,2,1)(f,__VA_ARGS__)
// END NOT MY CODE PORTION
// supports up to 8 

#define ARR_Y 100
#define NUM_1 1,
#define NUM_0 0,

#define NUM_ 
#define JC_(a,...)  NUM_##a  __VA_ARGS__
#define JC(...)  FOREACHTAIL(JC_, __VA_ARGS__)

#define MAKEARG(a) JC(JC(JC(JC(JC(JC(JC(JC(a))))))))  0

#define STRFY1(...) STRFY2(__VA_ARGS__)
#define STRFY2(...) STRFY(__VA_ARGS__)
#define STRFY(...) #__VA_ARGS__

int *ARR[ARR_Y];
int ARR_IDX = -1;

#define MKARR(a) { MAKEARG(a) }
#define APPEND_ARR(a) ({ \
	int arr[] = MKARR(a); \
	int *arr_p = malloc(sizeof(int)*8); \
	for(int i=0;i<6;++i) arr_p[i] = arr[i]; \
	ARR[++ARR_IDX] = arr_p; \
})

#define FREE_ARR() for(int i=0;i<6;++i) free(ARR[i])

#define DEC_N1(...) int  _##__VA_ARGS__;(void)_##__VA_ARGS__;
#define DEC_N2(a,...) int a;(void)a; DEC_N1(__VA_ARGS__) 
#define DEC_N3(a,...) int a;(void)a; DEC_N2(__VA_ARGS__) 
#define DEC_N(_3,_2,_1,N,...) DEC_N##N
#define DECLAIR_VAN(...) DEC_N(__VA_ARGS__,3,2,1)(__VA_ARGS__)

#define MAP(a,f,...)  if(a>0) { f(__VA_ARGS__)
#define DECLAIR__(var) int _## var;(void)_##var;
#define DECLAIR_VA(...) int _## __VA_ARGS__;
#define DECLAIR1(var,val) int _##var = val; (void)_##var;
#define DECLAIR_(des,var,value) MAP(des,DECLAIR1,var,value) 
#define END_DECLAIR }

#ifdef __STATIC_DECLAIR
#define STATIC_DECLAIR_V(var,value) DECLAIR1(var,value)
#define STATIC_DECLAIR(...) DECLAIR_VA(__VA_ARGS__)
#else 
#define STATIC_DECLAIR_V(var,value) 
#define STATIC_DECLAIR(...) ;
#endif
