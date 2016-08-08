# node-sourcekit

![Downloads](https://img.shields.io/github/downloads/RLovelett/node-sourcekit/total.svg?style=flat-square)
![NPM](https://img.shields.io/github/npm/dm/node-sourcekit.svg?style=flat-square)
![NPM](https://img.shields.io/github/npm/dt/node-sourcekit.svg?style=flat-square)
![License](https://img.shields.io/github/license/RLovelett/node-sourcekit.svg?style=flat-square)

node-sourcekit is a native C++ wrapper for the [SourceKit framework](https://github.com/apple/swift/tree/48bfec49fb4e97584caaa409a99fac48d47de6b0/tools/SourceKit)
that exposes the SourceKit protocol to Node.js. SourceKit is a framework in the
[Swift](https://swift.org) language that provides IDE type features for the Swift
language. Features like indexing, syntax-coloring, code-completion. SourceKit currently only
supports the Swift language.

This project was created to support [vscode-swift](https://github.com/RLovelett/vscode-swift) and [nuclide-swift](https://github.com/facebook/nuclide/tree/master/pkg/nuclide-swift).

## TODO

The final goal of the package is to provide 100% support for all the known SourceKit
requests. The documentented requests can be found in the SourceKit docs in
[Protocol.md](https://github.com/apple/swift/blob/master/tools/SourceKit/docs/Protocol.md).

The list of _known_ SourceKit requests _yet_ to to be documented can be found in
[Swift Bug #2117](https://bugs.swift.org/browse/SR-2117).

Additionally I would like to provide [Typescript](https://www.typescriptlang.org) type definitions for this project.

## Usage

| node-sourcekit API | SourceKit Protocol |
| -------------:|:------------|
| [#cursorinfo](#cursor-info) | [source.request.cursorinfo](https://github.com/apple/swift/blob/master/tools/SourceKit/docs/Protocol.md#cursor-info) |

### Cursor Info

SourceKit is capable of providing information about a specific symbol at a specific
cursor position, or byte offset, in a document.

To gather documentation, SourceKit must be given the fully qualified path to a Swift
source document.

#### Example

```swift
// sourcefile.swift
struct Foo {
    let bar: Int
}
```

```javascript
const SourceKit = require('node-sourcekit');
let source = '/full/path/to/sourcefile.swift';
let options = {
    sourcefile: source,
    offset: 27,
    compilerargs: [
        source
    ]
};
// `cursorinfo` returns an ES6 `Promise`
SourceKit.cursorinfo(options).then(console.log);
// The output should look similar to:
// { 'key.kind': 'source.lang.swift.decl.struct',
//   'key.name': 'Foo',
//   'key.usr': 's:V7example3Foo',
//   'key.filepath': '/full/path/to/sourcefile.swift',
//   'key.offset': 27,
//   'key.length': 3,
//   'key.typename': 'Foo.Type',
//   'key.annotated_decl': '<Declaration>struct Foo</Declaration>',
//   'key.fully_annotated_decl': '<decl.struct><syntaxtype.keyword>struct</syntaxtype.keyword> <decl.name>Foo</decl.name></decl.struct>',
//   'key.typeusr': '_Tt' }
```

## Development/Contributing

There are a set of tests that are expected to be passing before an contribution will
be accepted. To run these checkout the code and then run the following:

```
npm install
npm test
```

### Xcode

I found an invaluable guide [How to debug node.js addons in xcode?](https://github.com/anvaka/how-to-debug-node-js-addons).

If you want to be able to debug the C++ code in this project that is the best resource
I have found to date.

### Getting Started

After I had struggled for awhile I found this series,
[Getting your C++ to the Web with Node.js by Scott Frees](https://blog.scottfrees.com/getting-your-c-to-the-web-with-node-js),
and I really wish I had found it to start with. It is thorough and wonderful.

The most illuminating in the series, with respect to node-sourcekit, is
[Part 4: Building an Asynchronous C++ Addon for Node.js using Nan](https://blog.scottfrees.com/building-an-asynchronous-c-addon-for-node-js-using-nan).

Additionally a few other resources I've found can be found
[here](https://gist.github.com/dmh2000/9519489)
and
[here](https://github.com/paulhauner/example-async-node-addon).

Hope that helps!

## License

![MIT License](https://img.shields.io/github/license/RLovelett/node-sourcekit.svg?style=flat-square)
