FROM rafaelsoares/archlinux:2016-06-17
MAINTAINER Ryan Lovelett <ryan@lovelett.me>

RUN mkdir -p /opt/swift
WORKDIR /opt/swift

# Install Swift dependencies
RUN pacman --sync --noconfirm \
  autoconf \
  automake \
  cmake \
  clang \
  git \
  icu \
  libbsd \
  libedit \
  libtool \
  libutil-linux \
  libxml2 \
  make \
  ncurses \
  ninja \
  pkg-config \
  python2 \
  python2-pexpect \
  rsync \
  sqlite \
  swig
RUN ln -sf /usr/bin/python2 /usr/bin/python
ENV PATH $PATH:/usr/bin/core_perl

# Setup Git (I'd really like this to go away)
RUN git config --global user.email "ryan@lovelett.me"
RUN git config --global user.name "Ryan Lovelett"

# Get the Swift source checkedout and working
WORKDIR /opt/swift
RUN git clone https://github.com/apple/swift.git
WORKDIR /opt/swift/swift
RUN ./utils/update-checkout --clone

# Get the BlocksRuntime source checkedout
WORKDIR /opt/swift
RUN git clone https://github.com/mackyle/blocksruntime.git
WORKDIR /opt/swift/blocksruntime
RUN CFLAGS='-fPIC' ./buildlib
RUN ./installlib

# First pass of a Swift build (with libdispatch)
WORKDIR /opt/swift/swift
RUN ./utils/build-script \
  --build-subdir=buildbot_linux \
  --assertions \
  --no-swift-stdlib-assertions \
  --release \
  --libdispatch \
  -- \
  --swift-enable-ast-verifier=0

# Ensure a bunch of things are gone before starting the next step
RUN rm ../build/buildbot_linux/swift-linux-x86_64/CMakeCache.txt
RUN rm ../build/buildbot_linux/libdispatch-linux-x86_64/Makefile
RUN rm -rf ../build/buildbot_linux/libdispatch-linux-x86_64/config*

# Second pass of a Swift build (with SourceKit)
WORKDIR /opt/swift/swift
RUN ./utils/build-script \
  --build-subdir=buildbot_linux \
  --assertions \
  --no-swift-stdlib-assertions \
  --release \
  --libdispatch \
  --foundation \
  --llbuild \
  --swiftpm \
  --xctest \
  --lldb \
  --extra-cmake-options="-DSWIFT_BUILD_SOURCEKIT:BOOL=TRUE" \
  -- \
  --swift-enable-ast-verifier=0 \
  --build-swift-static-stdlib \
  --build-swift-static-sdk-overlay \
  --build-swift-stdlib-unittest-extra \
  --install-swift \
  --install-libdispatch \
  --install-foundation \
  --install-llbuild \
  --install-swiftpm \
  --install-xctest \
  --install-lldb \
  --install-prefix=/usr \
  '--swift-install-components=autolink-driver;compiler;clang-builtin-headers;stdlib;swift-remote-mirror;sdk-overlay;license;sourcekit-inproc;tools' \
  --install-destdir=/ \
  --installable-package=/tmp/swift.tar.gz \
  --reconfigure

# Install nvm, Node.js and npm
RUN pacman --sync --noconfirm wget
ENV NVM_DIR /usr/local/nvm
ENV NODE_VERSION 4.4.6
RUN wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.31.4/install.sh | bash \
 && . $NVM_DIR/nvm.sh \
 && nvm install $NODE_VERSION \
 && nvm alias default $NODE_VERSION \
 && nvm use default
ENV NODE_PATH $NVM_DIR/v$NODE_VERSION/lib/node_modules
ENV PATH      $NVM_DIR/v$NODE_VERSION/bin:$PATH

# Copy over the source
VOLUME ["/node-sourcekit"]
WORKDIR /node-sourcekit