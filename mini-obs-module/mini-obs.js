const obs_native = require("./build_root/build/Release/mini-obs");

exports.startup = obs_native.startup;
exports.resetVideo = obs_native.resetVideo;
exports.shutdown = obs_native.shutdown;
exports.getNAPIVersion = obs_native.getNAPIVersion;

