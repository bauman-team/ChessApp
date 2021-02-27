set -eo pipefail

export SFML_VERSION=2.5.0
export SFML_ROOT=$HOME/SFML-${SFML_VERSION}_LINUX
export BUILD_FOLDER=build_$CXX

if [[ ! -d "$SFML_ROOT/lib" ]]; then
  echo "$(tput setaf 3)Rebuilding SFML: no cache available$(tput sgr 0)"

  wget -O SFML.tar.gz https://github.com/SFML/SFML/archive/${SFML_VERSION}.tar.gz
  tar -xzf SFML.tar.gz
  cd SFML-${SFML_VERSION}

  cmake -DCMAKE_INSTALL_PREFIX=$SFML_ROOT -DSFML_BUILD_AUDIO=FALSE -DSFML_BUILD_NETWORK=FALSE .
  make -j2
  make install
  cd ..
else
  echo "$(tput setaf 2)Using cached SFML directory$(tput sgr 0)"
fi

mkdir $BUILD_FOLDER
cd $BUILD_FOLDER
CXXFLAGS="-fprofile-arcs -ftest-coverage -DTGUI_SKIP_DRAW_TESTS -DTGUI_REMOVE_DEPRECATED_CODE" cmake -DCMAKE_BUILD_TYPE=Debug -DTGUI_BUILD_GUI_BUILDER=FALSE -DTGUI_BUILD_TESTS=TRUE -DTGUI_OPTIMIZE_TESTS_SINGLE_BUILD=TRUE -DTGUI_OPTIMIZE_SINGLE_BUILD_THREADS=2 ..
make -j2
cd tests/
./tests
cd ../..
