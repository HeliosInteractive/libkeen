var exports = {};
var lib = require('bindings')('libkeen');

exports.core 	= new lib.Core();
exports.client 	= lib.Client;
exports.version	= new lib.Version();

module.exports = exports;
