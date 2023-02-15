echo "Please place all sources into the sources directory. "
old_dir=$(pwd)
root_dir=$(pwd)/deps
noCores=$(nproc)

## opencv extract and build


## go to downloaded opencv install
echo $(pwd)
echo $root_dir


echo "Doing OPENCV"
#### OPENCV
if [ ! -d $root_dir/builds/opencv ]
then
  echo "OPENCV Build disabld"

#mkdir -p $root_dir/builds/opencv;
#mkdir -p $root_dir/sources/opencv;

OPENCV_CMAKE_OPTIONS=(-DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=OFF -DBUILD_DOCS:BOOL=OFF  -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=OFF -DINSTALL_CREATE_DISTRIB=ON

);
#cmake -S $root_dir/downloads/opencv -B $root_dir/sources/opencv "${OPENCV_CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$root_dir"/builds/opencv;
## goto source dir
#cd $root_dir/sources/opencv || exit
## build
#make -j $noCores
#make install


fi



#### CEREAL
echo "Doing Cereal - Header only"
if [ ! -d $root_dir/builds/cereal ]
then
#CEREAL_CMAKE_OPTIONS=(-DSKIP_PERFORMANCE_COMPARISON=ON -DBUILD_SANDBOX=OFF )
#cmake -S $root_dir/downloads/cereal -B $root_dir/sources/cereal "${CEREAL_CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$root_dir"/builds/cereal
#cd $root_dir/sources/cereal || exit
#make -j $noCores
#make install
mkdir -p $root_dir/builds/cereal
cp -r $root_dir/downloads/cereal/include $root_dir/builds/cereal/include

fi
#### GPIOLIB
echo "Doing libgpiod - Full Build"
if [ ! -d $root_dir/builds/libgpiod ]
then

cd "$root_dir"/downloads/libgpiod || exit
"$root_dir"/downloads/libgpiod/autogen.sh  --enable-bindings-cxx  --prefix="$root_dir"/builds/libgpiod
make -j $noCores
make install

fi

#### SPIDEV Wrapper
echo "Doing SPIDEV"

if [ ! -d $root_dir/builds/spidev ]
then

mkdir -p $root_dir/builds/spidev;
mkdir -p $root_dir/sources/spidev;
OPENCV_CMAKE_OPTIONS=();
cmake -S $root_dir/downloads/spidev-lib-master -B $root_dir/sources/spidev "${OPENCV_CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$root_dir"/builds/spidev;
## goto source dir
cd $root_dir/sources/spidev || exit
## build
make -j $noCores
make install

fi

#### LIBEVENT
echo "Doing libevent - Full Build"
if [ ! -d $root_dir/builds/libevent ]
then

mkdir -p $root_dir/builds/libevent


OPENCV_CMAKE_OPTIONS=();
cmake -S $root_dir/downloads/libevent -B $root_dir/sources/libevent "${OPENCV_CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$root_dir"/builds/libevent;
cd $root_dir/sources/libevent || exit
## builds
make -j $noCores
make install

fi

##### PIGPIO
#echo "Doing PIGPIO - Full Build"
#if [ ! -d $root/builds/pigpio ]
#then
#
#mkdir -p $root_dir/builds/pigpio
#
#OPENCV_CMAKE_OPTIONS=();
#cmake -S $root_dir/downloads/pigpio-master -B $root_dir/sources/pigpio-master "${OPENCV_CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$root_dir"/builds/pigpio
#cd $root_dir/sources/pigpio-master
### builds
#make -j $noCores
#make install
#
#fi

#### RapidJSON
#echo "Doing RapidJSON - Full Build"
#if [ ! -d $root/builds/rapidjson ]
#then
#
#mkdir -p $root_dir/builds/rapidjson
#
#OPENCV_CMAKE_OPTIONS=();
#cmake -S $root_dir/downloads/rapidjson-master -B $root_dir/sources/rapidjson "${OPENCV_CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$root_dir"/builds/rapidjson
#cd $root_dir/sources/rapidjson
### builds
#make -j $noCores
#make install
#
#fi

#### nemalib
echo "Doing NEMALIB - Full Buiild"
if [ ! -d $root/builds/nmea ]
then
  mkdir -p $root_dir/builds/nmea
  cd $root_dir/downloads/nmealib || exit
  make
  cp -r $root_dir/downloads/nmealib/lib $root_dir/builds/nmea/lib
  cp -r $root_dir/downloads/nmealib/include $root_dir/builds/nmea/include
  cd "$old_dir" || exit
fi

#### enet
echo "Doing NEMALIB - Full Buiild"
if [ ! -d $root/builds/enet ]
then
  mkdir -p $root_dir/builds/enet
  cd $root_dir/downloads/enet || exit
  ./configure
  make -j 4
  make install
  cd "$old_dir" || exit
fi
