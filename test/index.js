'use strict';

const chai = require('chai');
const expect = chai.expect;
const chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);

const path = require('path');

// Load the Node module to test
const SourceKit = require('../lib/node-sourcekit');

describe('SourceKit', function () {
    it('should have a method "cursorinfo"', function () {
        expect(SourceKit).to.respondTo('cursorinfo');
    });
    describe('[source.request.cursorinfo] SourceKit.cursorinfo', function () {
        describe('sending a malformed options object', function() {
            let options;
            beforeEach(function() {
                const sourcefile = path.join(__dirname, 'fixtures', 'example.swift');
                options = {
                    sourcefile: sourcefile,
                    offset: 487,
                    compilerargs: [ sourcefile ]
                };
            });
            it('should not resolve without a "sourcefile" property', function() {
                delete options.sourcefile;
                return expect(SourceKit.cursorinfo(options)).to.eventually.be.rejected;
            });
            it('should not resolve without a "offset" property', function() {
                delete options.offset;
                return expect(SourceKit.cursorinfo(options)).to.eventually.be.rejected;
            });
            it('should not resolve without a "compilerargs" property', function() {
                delete options.compilerargs;
                return expect(SourceKit.cursorinfo(options)).to.eventually.be.rejected;
            });
        });
        describe('getting the type of the symbol in a struct declaration', function () {
            let options;
            beforeEach(function () {
                const sourcefile = path.join(__dirname, 'fixtures', 'example.swift');
                options = {
                    sourcefile: sourcefile,
                    offset: 487,
                    compilerargs: [ sourcefile ]
                };
            });
            it('should return an Object', function () {
                return expect(SourceKit.cursorinfo(options)).to.eventually.be.an('object');
            });
            describe('the structure of the returned Object', function () {
                it('should have 10 properties', function () {
                    return expect(SourceKit.cursorinfo(options)).to.eventually.contain.keys(
                        'key.kind',
                        'key.name',
                        'key.filepath',
                        'key.offset',
                        'key.length',
                        'key.typename',
                        'key.annotated_decl',
                        'key.fully_annotated_decl',
                        'key.typeusr'
                    );
                });
                it('should have a "key.kind" property', function () {
                    return expect(SourceKit.cursorinfo(options)).to.eventually.have.property('key.kind', 'source.lang.swift.decl.struct');
                });
                it('should have a "key.filepath" property', function () {
                    return expect(SourceKit.cursorinfo(options)).to.eventually.have.property('key.filepath', options.sourcefile);
                });
                it('should have a "key.offset" property', function () {
                    return expect(SourceKit.cursorinfo(options)).to.eventually.have.property('key.offset', options.offset);
                });
                it('should have a "key.name" property', function () {
                    return expect(SourceKit.cursorinfo(options)).to.eventually.have.property('key.name', 'Foo');
                });
            });
        });
    });
});