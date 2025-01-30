const obs_native = require("bindings")("mini-obs");

exports.test = () => "testing";
exports.getNAPIVersion = obs_native.getNAPIVersion;



