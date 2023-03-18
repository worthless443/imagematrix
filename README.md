### Meta
Scale a matrix by a factor to x-axis and y-axis respectively, Multiply the two scaled up matrices and the original matrix, the scale up works kind of, but the multipication approach doesn't work for scaling down. Use `--mltipd 1` to get the multiplied appraoch or by default (or `--mltipd 0`), we receieve successive scaling by x and the y axis. 

### Running 
simply compile, 
```sh
./compile
```
simply compile and run,
```sh
./compile -r
# compile with your custom compiler, set the CXX
CXX=clang++ ./compile 
```
#### running with flags
```sh
./scaleup <options> <up-factor> <down-factor>
./scaleup --help # for more info 
```
**example running**
```sh
./scaleup --infile <file> 6 2 --color-out 4 --crop 2 
```
### TODO

* fix the down scale multipication 
* sperate file for upscaling, downscaling and main wrapping functions
