echo "Please place all sources into the sources directory. "
root_dir=$(pwd)
noCores=$(nproc)

## opencv extract and build

cd $root_dir/sources/opencv || exit
echo $(pwd)

OPENCV_CMAKE_OPTIONS=(-DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=OFF -DBUILD_DOCS:BOOL=OFF  -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=OFF -DINSTALL_CREATE_DISTRIB=ON);
cmake ../opencv-4.5.5/ "${OPENCV_CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$root_dir"/builds/opencv
make -j $noCores
make install