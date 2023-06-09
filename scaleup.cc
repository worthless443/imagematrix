#include<stdio.h>
#include<stdint.h>
#include<stdarg.h>
#include<stdlib.h>

#include<vector>
#include<iostream>
#include<cassert>
#include<cstring>

#include<multiutils.h>

#ifndef __cplusplus
#error "this is C++ code"
#endif

int color_opt_n = 4;
int __errno;
struct Tuple {int x,y,z,a,b,c,d,e;}; // supports up to 3d

static void print_mat2d_plain(auto vec) {
	for(auto v : vec) {
		for(int _v : v)
			printf("%d ",  _v);
		printf("\n");
	}
}

static void print_mat2d_plain_color(auto vec) {
	const char *format = "\033[1;3%dm";
	char color_str[strlen(format)];
	sprintf(color_str, format, color_opt_n);
	for(auto v : vec) {
		for(int _v : v)
			printf("%s%d\033[00m ",  _v == 0 ? "\033[2;38m" : color_str, _v);
		printf("\n");
	}
}

#ifdef __OVERLOAD_OSTREAM
std::ostream &operator<<(std::ostream &o, std::vector<std::vector<int>> v) {
	print_mat2d_plain_color(v);
	return o;
}
std::ostream &operator<<(std::ostream &cout, char *&nigger) {
	cout << nigger;
	return cout;
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
struct Tuple create_mat2d_() {
	APPEND_ARR_(0 0 0 0 0 0 0 0);
	APPEND_ARR_(0 1 1 0 1 1 0 0);
	APPEND_ARR_(0 0 1 0 1 0 0 0);
	APPEND_ARR_(0 0 0 1 0 0 0 0);
	APPEND_ARR_(0 0 0 1 0 0 0 0);
	APPEND_ARR_(0 0 0 1 0 0 0 0);
	APPEND_ARR_(0 0 1 1 1 0 0 0);
	APPEND_ARR_(0 1 0 0 0 1 0 0);
	APPEND_ARR_(1 0 0 1 0 0 1 0);
	struct Tuple t = {6,9};
	return t;
}
struct Tuple create_mat2d() {
	APPEND_ARR_(1 1 1 1 1 0 0 0);
	APPEND_ARR_(0 0 0 1 1 0 0 0);
	APPEND_ARR_(0 0 0 1 1 0 0 0);
	APPEND_ARR_(0 0 0 1 1 1 1 1);
	APPEND_ARR_(0 0 0 1 1 0 0 0);
	APPEND_ARR_(0 0 0 1 1 0 0 0);
	APPEND_ARR_(0 0 0 1 1 0 0 0);
	APPEND_ARR_(0 0 0 1 1 0 0 0);
	APPEND_ARR_(1 1 1 1 1 0 0 0);
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

std::vector<std::vector<int>> downscale_mat2d_x(auto v, struct Tuple size, struct Tuple factor) {
	std::vector<std::vector<int>> v_;
	DECLAIR_VAN(x,y,_);
	x = size.x/factor.x; 
	y = size.y/factor.y;	
	for(int i=0;i<size.y;++i) {
		std::vector<int> v1;
		for(int start=0,a=0;start<size.x;start+=factor.x,++a) 
			v1.push_back(v[i][start]);
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

std::vector<std::vector<int>> downscale_mat2d_y(auto v, struct Tuple size, struct Tuple factor) {
	std::vector<std::vector<int>> v2d;
	for(int i=0;i<size.y;i+=factor.y)
			v2d.push_back(v[i]);
	return v2d;
}

template<class Container=std::vector<int>>
Container dotprod(Container a, std::vector<Container> b, int b_x) {
		Container v;
		if(a.size() < b.size()) {
				for(unsigned int i=0;i<b[0].size();++i)  {
					if((unsigned)b_x<b[0].size()) 
						v.push_back(a[i] | b[i][b_x]);
					else 
						v.push_back(a[i] | b[b_x][i]);
				}
				//for(unsigned int i=b[0].size();i<a.size();++i)
					//v.push_back(b[i][b_x]);
			
		}
		return v;
}
template<class Container=std::vector<int>>
Container cmpprod(Container a, Container b) {
	Container cmprow;
	if(a.size() < b.size()) {
		for(unsigned int i=0;i<a.size();++i) {
			cmprow.push_back(a[i] | b[i]);
		}
		for(unsigned int i=a.size();i<b.size();++i)
			cmprow.push_back(b[i]);
	}
	else {
		for(unsigned int i=0;i<b.size();++i) {
			cmprow.push_back(a[i] | b[i]);
		}
		for(unsigned int i=b.size();i<a.size();++i)
			cmprow.push_back(a[i]);
	}
	return cmprow;
}

template<class Container=std::vector<int>>
Container  dot_prod(Container a, Container b) {
	Container v;
	if(a.size() > b.size()) 
		for(unsigned int i=0;i<b.size();++i)
			v.push_back(b[i] | a[i]);
	else 
		for(unsigned int i=0;i<a.size();++i)
			v.push_back(b[i] | a[i]);
	return v;
}


template<class Container=std::vector<int>>
Container cmpprod_down(Container a, Container b) {
	Container cmprow;
	if(a.size() < b.size()) 
		for(unsigned int i=0;i<a.size();++i) 
			cmprow.push_back(a[i] | b[i]);
	else 
		for(unsigned int i=0;i<b.size();++i) 
			cmprow.push_back(a[i] | b[i]);
	
	return cmprow;
}

// FIXME actually do the multification right
template<class Container=std::vector<int>>
std::vector<Container> mltyply_matrix_even(auto a, auto b) {
	std::vector<Container> vec2d;
	if(a.size() > b.size()) 
		for(unsigned int i=0;i<a[0].size();++i) {
				//vec2d.push_back(dotprod(a[i],b,i));
				if(i<a.size()) vec2d.push_back(dotprod(a[i],b,i));
				else break;
		}
	else 
		for(unsigned int i=0;i<b.size();++i) 
				vec2d.push_back(cmpprod_down(a[i],b[i]));

	return vec2d;
}

template<class Container=std::vector<int>>
std::vector<Container> mltyply_matrix(auto a, auto b) {
	std::vector<Container> vec2d;
	if(a.size() < b.size()) 
		for(unsigned int i=0;i<a.size();++i) 
			vec2d.push_back(dotprod(a[i],b,i));
	return vec2d;
}

template<class Container=std::vector<int>>
std::vector<Container> cmpprod_mat_2d(auto a, auto b) {
	std::vector<Container> vec2d;
	if(a.size() < b.size()) {
		int j = 0;
		unsigned int factor = b.size()/a.size();
		for(unsigned int i=0;i<a.size();++i) {
			for(unsigned int ii=0;ii<factor;++ii,++j)
				vec2d.push_back(cmpprod(a[i],b[j]));
		}
	}
	else {
		int j = 0;
		unsigned int factor = a.size()/b.size();
		for(unsigned int i=0;i<b.size();++i) {
			for(unsigned int ii=0;ii<factor;++ii,++j)
				vec2d.push_back(cmpprod(b[i],a[j]));
		}
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
			v2d.push_back(v_tmp); }
	 }
	 return v2d;
}
template<class Ret> 
Ret scale_matrix(Ret mat, int factor, struct Tuple size) {
	struct Tuple factor_xy[2];
	setup_scaling(factor_xy, factor);
	auto vx = upscale_mat2d_x(mat, size, factor_xy[0]);
	auto vy = upscale_mat2d_y(mat, size, factor_xy[1]);
	auto cxy = cmpprod_mat_2d(vx,vy);
	return cxy;
}

template<class Ret> 
Ret downscale_matrix(Ret mat, int factor, int retmat=0) {
	struct Tuple size_xy, d_factor_xy[2];
	size_xy.y = mat.size();
	size_xy.x = mat[0].size();
	setup_scaling(d_factor_xy, factor);
	auto dx = downscale_mat2d_x(mat,size_xy,d_factor_xy[0]);
	auto dy = downscale_mat2d_y(mat,size_xy,d_factor_xy[1]);
	auto dyy = downscale_mat2d_y(dx,size_xy,d_factor_xy[1]);
	if(retmat) {
		auto dxy =  mltyply_matrix_even(dy,mltyply_matrix(dy,dx));
		return dxy;
	}
	return dyy;
}

static const char *parse_args(const char *input) {
	int i;
	for(i=0;*(input + i)=='-';++i);
	if(!i) return (char*)-1;
	if(i<=2) return input + i;
	else if(i>2) return (char*)-2;
	return (char*)0;
}

static auto parse_file_mat(const char *fname) {
	int i = -1, arr[11], y;
	FILE *f = NULL;
	std::vector<std::vector<int>> v2d;
	char tmpbuf[1];
	if((f = fopen(fname, "r"))==NULL) {
		perror(fname);
		__errno = -3;
		return v2d;
	}
	while(fread(tmpbuf,1,1,f)) {
		if(*tmpbuf >= '0' && *tmpbuf <= '9') {
			arr[++i] = atoi(tmpbuf);
		}
		else if(*tmpbuf == ' ' || *tmpbuf == '\n');
		else __errno = -1;
		if(*tmpbuf == '\n') {
			std::vector<int> v;
			for(int j=0;j<5;++j) v.push_back(arr[j]);
			v2d.push_back(v);
			memset(arr, '\0', 7);
			i=-1;
		}
	}
	y = v2d.size();
	if(y < 9) __errno = -2;
	fclose(f);
	return v2d;
}

#define HELP_MSG "scaleup <options = --color-out | --crop | --infile > <up> <down>\n./scaleup --help to use this message\n"

int main(int argc, char **argv) {
	int factor=5,downfactor=2,argn=0,mltipld_output=0, cropf=0,flg=0;
	int n = 0;
	char *infile = NULL;
	std::vector<std::vector<int>> inmat;
	if(argc>1) {
		for(int i=1;i<argc;++i) {
			if(parse_args(argv[i])==(char*)-1 && atoi(argv[i])>0 && argn == 0) {
				factor = atoi(*(argv + i));
				argn = 1;
				++n;
			}
			else if(0 == strcmp(parse_args(argv[i]), "help")) {
				fprintf(stderr, HELP_MSG);
				return 0;
			}

			else if(parse_args(argv[i])==(char*)-1 && atoi(argv[i])>0 && argn == 1) {
				downfactor = atoi(*(argv + i));
				argn = 2;
				++n;
			}
			else if(parse_args(argv[i])==(char*)-1 && atoi(argv[i])==0) {
				fprintf(stderr, "expecting a number, not \'%s\'\n", argv[i]);
				return 1;
			}
			
			else if(parse_args(argv[i])==(char*)-2) {
				fprintf(stderr, "invalid flag: \'%s\'\n", argv[i]);
				return 1;
			}
			
			else if(0 == strcmp(parse_args(argv[i]), "mltipd")) {
				if(flg) n+=2;
				else ++n;
				if(argc - n - 1==0) {
					fprintf(stderr, "expecting arg to \'mltipd\'\n");
					return 1;
				}
				mltipld_output = atoi(argv[++i]);
				flg = 1;
			}
			else  if(0 == strcmp(parse_args(argv[i]), "crop")) {
				if(flg) n+=2;
				else ++n;
				if(argc - n - 1==0) {
					fprintf(stderr, "expecting arg to \'crop\'\n");
					return 1;
				}
				cropf = atoi(argv[++i]);
				flg = 1;
			}
			else if(0 == strcmp(parse_args(argv[i]), "color-out")) {
				if(flg) n+=2;
				else ++n;
				if(argc - n - 1==0) {
					fprintf(stderr, "expecting arg to \'color-out\'\n");
					return 1;
				}
				color_opt_n = atoi(argv[++i]);
				flg = 1;
			}
			else if(0 == strcmp(parse_args(argv[i]), "infile")) {
				if(flg) n+=2;
				else ++n;
				if(argc - n - 1==0) {
					fprintf(stderr, "expecting arg to \'infile\'\n");
					return 1;
				}
				infile = (char*)malloc(sizeof(char)*100);
				memcpy(infile,argv[++i],strlen(argv[i]));
				flg = 1;
			}
			else {
				fprintf(stderr, "invalid given arg:\'%s\'\n", argv[i]);
				return 1;
			}
		}
	}

	struct Tuple size = create_mat2d_();
	if(infile!=NULL) {
		inmat = parse_file_mat(infile);
		if(__errno == -1) {
			fprintf(stderr,"%s:corroupted file\n", infile);
			free(infile);
			return 1;
		}
		else if(__errno == -2) {
			fprintf(stderr,"%s:matrix size assertion required - x < y\n", infile);
			free(infile);
			return 1;
		}
		else if(__errno == -3) {
			free(infile);
			return 1;
		}
		else free(infile);
	}
	else inmat = VA;

	auto cxy = scale_matrix(inmat,factor,size);
	auto dxy = downscale_matrix(cxy,downfactor,mltipld_output);
	std::cout << cxy << "\n";
	std::cout << dxy << "\n";
	if(cropf) 
		std::cout << downscale_matrix(cxy,cropf,1) << "\n";
	return 0;
}

