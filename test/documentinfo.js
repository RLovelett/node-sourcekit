const chai = require('chai');
const expect = chai.expect;
const chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);

const path = require('path');

// Load the Node module to test
const SourceKit = require('../lib/node-sourcekit');

describe('SourceKit', function () {
    it('should have a method "documentinfo"', function () {
        expect(SourceKit).to.respondTo('documentinfo');
    });
    describe('[source.request.docinfo] SourceKit.documentinfo', function () {
        describe('sending a malformed options parameter', function() {
            it('should not resolve without a "sourcefile" property');
            it('should not resolve without a "compilerargs" property');
            it('should not resolve without a "sourcetext" property');
        });
        describe('getting compiler diagnostics for a malformed Swift source', function() {
            it('should return an Object');
            describe('the structure of the returned Object', function() {
                it('should have a "key.diagnostics" property');
            });
        });
    });
});