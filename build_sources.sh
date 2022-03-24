echo "Please place all sources into the sources directory. "
root_dir=$(pwd)/deps
noCores=$(nproc)

## opencv extract and build
mkdir -p $root_dir/builds/opencv;
mkdir -p $root_dir/sources/opencv;


## go to downloaded opencv install
echo $(pwd)
echo $root_dir

OPENCV_CMAKE_OPTIONS=(-DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=OFF -DBUILD_DOCS:BOOL=OFF  -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=OFF -DINSTALL_CREATE_DISTRIB=ON);
cmake -S $root_dir/downloads/opencv -B $root_dir/sources/opencv "${OPENCV_CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$root_dir"/builds/opencv;
## goto source dir
cd $root_dir/sources/opencv || exit
## build
make -j $noCores
make install