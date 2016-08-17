#!/usr/bin/env bash

# Check that nvm is loaded
if [ -s "$NVM_DIR/nvm.sh" ]
then
    source $NVM_DIR/nvm.sh
fi

# Install dependencies, build, and test
npm install || exit 1
node-gyp rebuild --verbose || exit 1
npm test
