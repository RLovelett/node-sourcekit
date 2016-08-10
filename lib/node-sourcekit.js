'use strict'

const SourceKit = require('bindings')('node_sourcekit');

exports.NativeInterface = SourceKit;

exports.cursorinfo = function(options) {
    return new Promise((resolve, reject) => {
        try {
            SourceKit.cursorinfo(options, resolve);
        }
        catch (e) {
            reject(e);
        }
    });
};

exports.documentinfo = function(options) {
    return new Promise((resolve, reject) => {
        try {
            SourceKit.documentinfo(options, resolve);
        }
        catch (e) {
            reject(e);
        }
    });
};