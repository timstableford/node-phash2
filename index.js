const Util = require('util');
const pHash = require('./build/Release/phash.node');

const DEFAULT_MH_ALPHA = 2.0;
const DEFAULT_MH_LEVEL = 1.0;
const DEFAULT_VIDEO_DISTANCE_THRESHOLD = 21;

function info() {
    return {
        about: pHash.about(),
        video: pHash.isVideoSupported(),
        audio: pHash.isAudioSupported(),
        image: pHash.isImageSupported(),
    };
}

const imageExports = pHash.isImageSupported() ? {
    getDctImageHash: path => Util.promisify(pHash.getDctImageHash)(path),
    getMhImageHash: (path, alpha = DEFAULT_MH_ALPHA, level = DEFAULT_MH_LEVEL) => {
        return Util.promisify(pHash.getMhImageHash)(path, alpha, level);
    },
} : {
    getDctImageHash: () => Promise.reject(new Error('Images are not supported')),
    getMhImageHash: () => Promise.reject(new Error('Images are not supported')),
};

const videoExports = pHash.isVideoSupported() ? {
    getDctVideoHash: path => Util.promisify(pHash.getDctVideoHash)(path),
    // Scale 0 to 1, where 1 is the same.
    getVideoDistance: (hashA, hashB, threshold = DEFAULT_VIDEO_DISTANCE_THRESHOLD) => {
        return Util.promisify(pHash.getVideoDistance)(hashA, hashB, threshold);
    },
} : {
    getDctVideoHash: () => Promise.reject(new Error('Videos are not supported')),
    getVideoDistance: () => Promise.reject(new Error('Videos are not supported')),
};


module.exports = {
    // Constants
    DEFAULT_MH_ALPHA,
    DEFAULT_MH_LEVEL,
    DEFAULT_VIDEO_DISTANCE_THRESHOLD,
    // Functions
    info,
    // Images
    ...imageExports,
    // Just the number of bits different.
    getHammingDistance: pHash.getHammingDistance,
    // Scale 0 to 1 where 0 is the same.
    getHammingDistance2: pHash.getHammingDistance2,
    // Videos
    ...videoExports,
};
