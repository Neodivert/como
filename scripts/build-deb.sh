#!/bin/bash
DEB_SOFTWARE_NAME=como
DEB_ARCHITECTURE=amd64
DEB_MAJOR_VERSION=0
DEB_MINOR_VERSION=7
DEB_REVISION=1
DEB_PACKAGE_NAME="$DEB_SOFTWARE_NAME-$DEB_ARCHITECTURE-$DEB_MAJOR_VERSION.$DEB_MINOR_VERSION-$DEB_REVISION"

TOP_LEVEL_DIR=`pwd`

SUB_PROGRAMS[0]="server"
SUB_PROGRAMS[1]="client"

for subprogram in "${SUB_PROGRAMS[@]}"
do
	echo "Building subprogram [$subprogram]"
	BUILD_DIR="$TOP_LEVEL_DIR/../bin/$subprogram"
	BIN_DIR="$TOP_LEVEL_DIR/$DEB_PACKAGE_NAME/opt/como/$subprogram"

	# Create the binary dir.
	echo "Creating dir: [$BIN_DIR]"
	mkdir -p "$BIN_DIR"

	# Build subprogram and copy the binary to its final location.
	cd "$BUILD_DIR"
	echo "Building from [$BUILD_DIR]"
	qmake "$TOP_LEVEL_DIR/../project/$subprogram/$subprogram.pro"
	make -j 5
	cp -R "$BUILD_DIR/$subprogram" "$BIN_DIR/$subprogram"
	#sudo chmod 0755 "$BIN_DIR/$subprogram"
	strip "$BIN_DIR/$subprogram" # This removes lintian error "unstripped-binary-or-object"
	
	# Copy the subprogram's dir "data" from repository.
	cd "$BIN_DIR"
	svn checkout "https://github.com/moisesjbc/como/trunk/bin/$subprogram/data"
	sudo rm -r "$BIN_DIR/data/.svn"
	sudo chmod -R +w "$BIN_DIR/data/"
done

cd $TOP_LEVEL_DIR

# Create the DEB control file.
DEB_CONTROL_DIR="$TOP_LEVEL_DIR/$DEB_PACKAGE_NAME/DEBIAN"
mkdir -p $DEB_CONTROL_DIR
echo "Package: $DEB_SOFTWARE_NAME
Version: $DEB_MAJOR_VERSION.$DEB_MINOR_VERSION-$DEB_REVISION
Section: graphics
Priority: optional
Architecture: amd64
Depends: libc6 (>= 2.19), libsdl2-image-2.0-0 (>=2.0.0), libboost-system1.55.0 (>= 1.55.0), libboost-thread1.55.0 (>= 1.55.0), libboost-filesystem1.55.0 (>= 1.55.0), libqt5opengl5 (>= 5.2.1)
Maintainer: Moisés J. Bonilla Caraballo <moisesjbc@gmail.com>
Description: COoperative MOdeller (COMO) allows multiple users to work on the same scene across the network
 COoperative MOdeller (COMO) allows multiple users to work on the same scene across the network
" > "$DEB_CONTROL_DIR/control"

sudo chown -R root:root "$TOP_LEVEL_DIR/$DEB_PACKAGE_NAME"

# Build the DEB package.
dpkg-deb --build $DEB_PACKAGE_NAME
