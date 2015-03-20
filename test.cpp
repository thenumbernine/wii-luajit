#include <stdio.h>

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define LT <
#define GT >

#define FOR_EACH_1(what, x, ...) what LT x GT(L, n-i++)
#define FOR_EACH_2(what, x, ...) what LT x GT(L, n-i++), FOR_EACH_1(what,  __VA_ARGS__)
#define FOR_EACH_3(what, x, ...) what LT x GT(L, n-i++), FOR_EACH_2(what, __VA_ARGS__)
#define FOR_EACH_4(what, x, ...) what LT x GT(L, n-i++), FOR_EACH_3(what,  __VA_ARGS__)
#define FOR_EACH_5(what, x, ...) what LT x GT(L, n-i++), FOR_EACH_4(what,  __VA_ARGS__)
#define FOR_EACH_6(what, x, ...) what LT x GT(L, n-i++), FOR_EACH_5(what,  __VA_ARGS__)
#define FOR_EACH_7(what, x, ...) what LT x GT(L, n-i++), FOR_EACH_6(what,  __VA_ARGS__)
#define FOR_EACH_8(what, x, ...) what LT x GT(L, n-i++), FOR_EACH_7(what,  __VA_ARGS__)

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__) 
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N 
#define FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, ...) CONCATENATE(FOR_EACH_, N)(what, __VA_ARGS__)
#define FOR_EACH(what, ...) FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, __VA_ARGS__)


////'n' is always lhs=N, rhs=1, (backwards)
//
//template<typename T> T getT(void *, int);
//template<> char getT(void *, int i) { return i; }
//template<> int getT(void *, int i) { return i; }
//template<> size_t getT(void *, int i) { return i; }
//template<> double getT(void *, int i) { return i; }
//
//#define NARGS(...)	#__VA_ARGS__
//
//void testing() {};
//
//int testing2(void) {};
//
//int main() {
//	void *L;
//	int n = 4;
//	int i = 0;
//	i = 0; n = 1;
//	printf("%d\n", FOR_EACH(getT, char));
//	i = 0; n = 2;
//	printf("%d %d\n", FOR_EACH(getT, char, char));
//	i = 0; n = 4;
//	printf("%d %d %d %d\n", FOR_EACH(getT, char, char, char, char));
//	
//	printf("%d\n", testing2(testing()));
//}


#define PARAM_FOR_EACH_0()	
#define PARAM_FOR_EACH_1(x) x p1
#define PARAM_FOR_EACH_2(x, ...) x p2, PARAM_FOR_EACH_1( __VA_ARGS__)
#define PARAM_FOR_EACH_3(x, ...) x p3, PARAM_FOR_EACH_2(__VA_ARGS__)
#define PARAM_FOR_EACH_4(x, ...) x p4, PARAM_FOR_EACH_3( __VA_ARGS__)
#define PARAM_FOR_EACH_5(x, ...) x p5, PARAM_FOR_EACH_4( __VA_ARGS__)
#define PARAM_FOR_EACH_6(x, ...) x p6, PARAM_FOR_EACH_5( __VA_ARGS__)
#define PARAM_FOR_EACH_7(x, ...) x p7, PARAM_FOR_EACH_6( __VA_ARGS__)
#define PARAM_FOR_EACH_8(x, ...) x p8, PARAM_FOR_EACH_7( __VA_ARGS__)

#define PARAM_FOR_EACH_(N, ...) CONCATENATE(PARAM_FOR_EACH_, N)(__VA_ARGS__)
#define PARAM_FOR_EACH(...) PARAM_FOR_EACH_(VARARG_NARGS(__VA_ARGS__), __VA_ARGS__)



#define INVOKE_FOR_EACH_0()	
#define INVOKE_FOR_EACH_1(x) p1
#define INVOKE_FOR_EACH_2(x, ...) p2, INVOKE_FOR_EACH_1( __VA_ARGS__)
#define INVOKE_FOR_EACH_3(x, ...) p3, INVOKE_FOR_EACH_2(__VA_ARGS__)
#define INVOKE_FOR_EACH_4(x, ...) p4, INVOKE_FOR_EACH_3( __VA_ARGS__)
#define INVOKE_FOR_EACH_5(x, ...) p5, INVOKE_FOR_EACH_4( __VA_ARGS__)
#define INVOKE_FOR_EACH_6(x, ...) p6, INVOKE_FOR_EACH_5( __VA_ARGS__)
#define INVOKE_FOR_EACH_7(x, ...) p7, INVOKE_FOR_EACH_6( __VA_ARGS__)
#define INVOKE_FOR_EACH_8(x, ...) p8, INVOKE_FOR_EACH_7( __VA_ARGS__)

#define INVOKE_FOR_EACH_(N, ...) CONCATENATE(INVOKE_FOR_EACH_, N)(__VA_ARGS__)
#define INVOKE_FOR_EACH(...) INVOKE_FOR_EACH_(VARARG_NARGS(__VA_ARGS__), ##__VA_ARGS__)


#define VARARG_NARGS(...) VARARG_NARGS_II((VARARG_NARGS_PREFIX_ ## __VA_ARGS__ ## _VARARG_NARGS_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))
#define VARARG_NARGS_II(__args) VARARG_NARGS_I __args
#define VARARG_NARGS_PREFIX__VARARG_NARGS_POSTFIX ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0
#define VARARG_NARGS_I(__p0,__p1,__p2,__p3,__p4,__p5,__p6,__p7,__p8,__p9,__p10,__p11,__p12,__p13,__p14,__p15,__p16,__p17,__p18,__p19,__p20,__p21,__p22,__p23,__p24,__p25,__p26,__p27,__p28,__p29,__p30,__p31,__n,...) __n


void f(PARAM_FOR_EACH()) {
//	printf("%d %d %d\n", INVOKE_FOR_EACH());
}

int main() {
	f();
}