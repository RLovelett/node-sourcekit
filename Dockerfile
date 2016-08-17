FROM norionomura/sourcekit:30p5
MAINTAINER Ryan Lovelett <ryan@lovelett.me>

# Install wget
RUN apt-get update \
 && apt-get install -y wget build-essential \
 && rm -rf /var/lib/apt/lists/*

# Install nvm, Node.js and npm
ENV NVM_DIR /usr/local/nvm
ENV NODE_VERSION 4.5.0
RUN wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.31.4/install.sh | bash \
 && . $NVM_DIR/nvm.sh \
 && nvm install $NODE_VERSION \
 && nvm alias default $NODE_VERSION \
 && nvm use default \
 && npm install -g node-gyp
ENV NODE_PATH $NVM_DIR/v$NODE_VERSION/lib/node_modules
ENV PATH      $NVM_DIR/v$NODE_VERSION/bin:$PATH

# Install Vim
#RUN apt-get update \
# && apt-get install -y vim git \
# && rm -rf /var/lib/apt/lists/*

# Copy over the source
VOLUME ["/node-sourcekit"]
WORKDIR /node-sourcekit

ENTRYPOINT ["/node-sourcekit/test.sh"]