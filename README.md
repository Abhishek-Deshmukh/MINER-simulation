# MINER-sim

## How to run

After installing dependencies.
Go the folder you wanna keep this repo in

```
$ git clone https://github.com/Abhishek-Deshmukh/MINER-sim
$ mkdir MINER-sim-build
$ cd MINER-sim-build
$ cmake ../MINER-sim
$ make
$ ./MINERsim  # this will run the visualisation
```

## Dependencies

This has been tested on arch linux and ubuntu, so all arch based and
debian/ubuntu based distrubutions should work with it.

Change `mercury` with the name of your user on your machine.

### Preparing directories

```
$ cd ~/Downloads
$ mkdir CLHEP CLHEP/CLHEP-build ~/.local/CLHEP \
  ROOT ROOT/root-build \
  GEANT4 ~/.local/GEANT4 \
  xerces-c CRY
```

### Installing dependencies

Dependencies which can be installed from packages managers

Arch based distros(Arch, Manjaro, Garuda etc.):

```
$ sudo pacman -S doxygen cmake git boost
```

Debian based distros(Debian, Ubuntu etc.):

```
$ sudo apt install doxygen cmake git boost
```

To build dependencies

#### Installing CLHEP

```
$ cd CLHEP
$ git clone https://gitlab.cern.ch/CLHEP/CLHEP.git
$ cd CLHEP
$ git checkout master
$ cd ../CLHEP-build
$ cmake \
  -DCMAKE_INSTALL_PREFIX=/home/mercury/.local/CLHEP \
  -DCLHEP_BUILD_DOCS=ON \
  ../CLHEP
$ cmake --build . --config RelWithDebInfo
$ ctest
$ cmake --build . --target install
```

#### Installing xerces-c

Download the latest version `xerces-c-x.y.z.tar.gz` file from `http://xerces.apache.org/xerces-c/download.cgi` into `~/Downloads/xerces-c`.

`x.y.z` will be replaced by the ongoing version.

```
$ cd ~/Downloads/xerces-c
$ tar -xvzf xerces-c-3.2.3
$ mkdir build ~/.local/xerces-c-3.2.3
$ cd build
$ cmake \
  -DCMAKE_INSTALL_PREFIX=/home/mercury/.local/xerces-c-3.2.3 \
  -DCMAKE_BUILD_TYPE=Debug \
  -Dmessage-loader=icu \
  ../xerces-c-3.2.3
$ make
$ make test
$ make install
```

#### Installing Geant4

Download the latest version of `GEANT4` in `~/Downloads/GEANT4`

```
$ cd ~/Downloads/GEANT4
$ tar -xvzf geant4.10.07.tar.gz
$ mkdir geant4.10.07-build
$ cd geant4.10.07-build
$ cmake \
  -DCMAKE_INSTALL_PREFIX=~/.local/GEANT4 \
  -DGEANT4_USE_GDML=ON \
  -DGEANT4_INSTALL_DATA=ON \
  -DGEANT4_USE_PYTHON=ON \
  -DGEANT4_USE_QT=ON \
  -DXERCESC_ROOT_DIR=~/.local/xerces-c-3.2.3 \
  -DGEANT4_USE_SYSTEM_CLHEP=ON \
  -DCLHEP_ROOT_DIR=~/.local/CLHEP/ \
  -DGEANT4_INSTALL_EXAMPLES=ON \
  -DGEANT4_USE_OPENGL_X11=ON \
  -DGEANT4_USE_RAYTRACER_X11=ON \
  ../geant4.10.07
$ make
$ make install
$ echo "source ~/.local/GEANT4/bin/geant4.sh" >> ~/.bashrc
$ echo "source ~/.local/GEANT4/share/Geant4-10.7.1/geant4make/geant4make.sh" >> ~/.bashrc
```

#### Installing ROOT

```
$ cd ~/Downloads/ROOT
$ git clone https://github.com/root-project/root.git
$ cd ./root-build
$ cmake ../root
$ sudo cmake --build . --target install
$ echo "source /usr/local/bin/thisroot.sh" >> ~/.bashrc
```
