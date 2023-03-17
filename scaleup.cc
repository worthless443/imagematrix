#include<stdio.h>
#include<stdint.h>
#include<stdarg.h>
#include<stdlib.h>

#include<vector>
#include<iostream>
#include<cassert>

#include<multiutils.h>

struct Tuple {int x,y,z,a,b,c,d,e;}; // supports up to 3d

static void print_mat2d_plain(auto vec) {
	for(auto v : vec) {
		for(int _v : v)
			printf("%d ",  _v);
		printf("\n");
	}
}

static std::ostream &print_mat2d_plain_ostream(std::ostream &cout, std::vector<std::vector<int>> vec) {
	for(auto v : vec) {
		for(int _v : v)
			cout <<  _v << " ";
		cout << "\n";
	}
	return cout;
}
#ifdef __OVERLOAD_OSTREAM
std::ostream &operator<<(std::ostream &o, auto v) {
	o << print_mat2d_plain_ostream(o,v);
	return o;
}
std::ostream &operator<<(std::ostream &o, const char *chr) {
	o << chr;
	return o;
}
#endif

static void setup_scaling(struct Tuple *t_arr, int size) {
	for(int i=0;i<2;++i) {
		struct Tuple scale = *(t_arr + i);
		if(!i) {
			*((int*)&scale + 0) = size;
			*((int*)&scale + 1 + 1) = 1;
		}
		else {
			*((int*)&scale + 0) = 1;
			*((int*)&scale + 0 + 1) = size;
		}
		*(t_arr + i) = scale;
	}
}
				   
std::vector<std::vector<int>> VA;
void append_arr_sized(const int c,...) {
	std::vector<int> v;
	va_list list;
	va_start(list, c);
	for(int i=0;i<c;++i) v.push_back(va_arg(list, int));
	va_end(list);
	VA.push_back(v);
}

#define APPEND_ARR_(a) append_arr_sized(6, MAKEARG(a))

int getdim(struct Tuple *t) {
	int size;
	for(size=0;*((int*)t + size);++size)  
		if(*((unsigned int*)t + size)>(unsigned)0xfff) break; // only works for small arrays
	
	return size;
}
struct Tuple create_mat2d() {
	APPEND_ARR_(0 1 0 0 1 0 1);
	APPEND_ARR_(1 1 0 1 1 0 1);
	APPEND_ARR_(1 0 1 0 1 0 0);
	APPEND_ARR_(1 1 1 1 1 1 1);
	APPEND_ARR_(0 1 1 1 1 1 1);
	APPEND_ARR_(0 1 0 1 1 1 1);
	APPEND_ARR_(0 1 0 1 1 0 1);
	APPEND_ARR_(0 1 0 1 1 0 1);
	APPEND_ARR_(0 1 0 1 1 0 1);
	struct Tuple t = {6,9};
	return t;
}
auto factor_size_struct = [](struct Tuple size, struct Tuple factor) {
	struct Tuple result;
	for(int i=0;i<getdim(&factor);++i) *((int*)(&result) + i) = *(((int*)&size) + i) * *((int*)&factor + i);
	return result;
};

void print_mat2d(auto v, struct Tuple size2d) {
#if defined(DEBUG_MAT)
	int x,y;
	// bad idea
	if(getdim(&size2d)==2) x = size2d.x, y = size2d.y; // depricate this soon
	else if(getdim(&size2d)==1) x = size2d.x, y = 4; 
	else x = 2,y = 2;
	// bad idea
	for(int i=0;i<y;++i) {
		printf("\n");
		for(int j=0;j<x;++j)
			printf("%d ", v[i][j]);
		if(i==y-1) printf("\n");
	}
#else
	printf("not compiled with DEBUG_MAT\n");
#endif
}

void print_mat2d(struct Tuple size2d) {
#if defined(DEBUG_MAT)
	if(getdim(&size2d)==2) {
		for(int i=0;i<size2d.y;++i) {
			for(int j=0;j<size2d.x;++j)
				printf("%d ", VA[i][j]);
		printf("\n");
		}
	}
#else
	printf("not compiled with DEBUG_MAT\n");
#endif
}


int **allocate_arr(struct Tuple size) {
	int **tmp = (int**)malloc(sizeof(int)*size.y);
	for(int j=0;j<size.x;++j) tmp[j] = (int*)malloc(sizeof(int)*size.x);
	return tmp;
}
void free_arr(int **arr, struct Tuple size) {
	for(int i=0;i<size.y;++i) free(arr[i]);
	free(arr);
}

std::vector<std::vector<int>> upscale_mat2d_x(auto v, struct Tuple size, struct Tuple factor) {
	std::vector<std::vector<int>> v_;
	DECLAIR_VAN(x,y,_);
	x = size.x * factor.x; 
	y = size.y * factor.y;	
	int arr[x+1];
	for(int i=0;i<size.y;++i) {
		std::vector<int> v1;
		for(int start=0,a=0;start<size.x;++start) 
			for(int j=0;j<factor.x;++j,++a) 
				arr[a] = v[i][start];
		for(int i=0;i<size.x * factor.x;++i) v1.push_back(arr[i]);
		v_.push_back(v1);
	}
	return v_;
}

std::vector<std::vector<int>> upscale_mat2d_y(auto v, struct Tuple size, struct Tuple factor) {
	std::vector<std::vector<int>> v2d;
	for(int i=0;i<size.y;++i) 
		for(int _=0;_<factor.y;++_) 
			v2d.push_back(v[i]);
	return v2d;

}
template<class Container=std::vector<int>>
int dotprod(Container a, std::vector<Container> b, int b_x) {
		int dot = 1;	
		if(a.size() < b.size()) {
			for(unsigned int i=0;i<a.size();++i) {
				dot += a[i] * b[i][b_x];
			}
			for(unsigned int i=a.size();i<b.size();++i)
				dot += b[i][b_x];
		}
		return dot;
}

// FIXME actually do the multification right
template<class Container=std::vector<int>>
std::vector<Container> mltyply_matrix_even(auto a, auto b) {
	std::vector<Container> vec2d;
	for(unsigned int i=0;i<a.size();++i) {
		Container vec;
		for(unsigned int j=0;j<b[0].size();++j)
			vec.push_back(dotprod(a[i],b,j));
		vec2d.push_back(vec);
	}
	return vec2d;
}


// just combine two matrix without context preserved wihtin the new matrix
// just for resizing 
std::vector<std::vector<int>> combine_mat_2d(auto a, auto b) {
	std::vector<std::vector<int>> v2d;
	 int a_x = a[0].size(), a_y = a.size();
	 int b_x = b[0].size(), b_y = b.size();
	  int tmp_a_y = 0;
	 if(b_y > a_y) {
		 for(int i=0;i<b_y;++i) {
			if(tmp_a_y>=a_y) tmp_a_y = 0;
			std::vector<int> v_tmp;
			if(a_x > b_x) {
				if(i < a_y) {
					for(int j=0;j<b_x;++j) 
						v_tmp.push_back(b[i][j]);
					for(int j=b_x;j<a_x;++j)
						v_tmp.push_back(a[i][j]);
				}
				else {
					for(int j=0;j<b_x;++j)	
						v_tmp.push_back(b[i][j]);
					for(int j=b_x;j<a_x;++j)	
						v_tmp.push_back(a[tmp_a_y][j]);
					tmp_a_y+=1;
				}
			}
			v2d.push_back(v_tmp);
		 }
	 }
	 return v2d;
}

int main() {
	struct Tuple size = create_mat2d(), factor_xy[2];
	setup_scaling(factor_xy, 5);
	auto vx = upscale_mat2d_x(VA, size, factor_xy[0]);
	//print_mat2d(vx, factor_size_struct(size,factor_x));
	//print_mat2d(vx);
	auto vy = upscale_mat2d_y(VA, size, factor_xy[1]);
	auto mxy = mltyply_matrix_even(vx,vy);
	auto yx = combine_mat_2d(vx,vy);
	//std::cout << "\n";
	print_mat2d_plain(mxy);
	std::cout << "\n\n";
	print_mat2d_plain(yx);
	return 0;
	//print_mat2d(vy, factor_size_struct(size,factor_y));
	// jus a test
	//int **arr = allocate_arr(size);
	//free_arr(arr,size);
	//for(int i=0;i<5;++i) printf("%d\n", VA[1][i]);
}

