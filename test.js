const Path = require('path');
const SourceKit = require('./lib/node-sourcekit');

let source = Path.join(__dirname, 'test', 'fixtures', 'example.swift');
let options = {
    sourcefile: source,
    offset: 487,
    compilerargs: [
        source
    ]
};
SourceKit.cursorinfo(options).then((result) => {
    console.log("From Node:");
    console.log(result);
});
