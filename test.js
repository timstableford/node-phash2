const pHash = require('./index');

async function doIt(name, hashFunc, hammingFunc) {
    console.time(name);
    const res = await Promise.all([
        hashFunc('./6kXnI2J.jpg'),
        hashFunc('./6kXnI2J_scales.jpg'),
        hashFunc('./6kXnI2J_scaled_cropped.jpg'),
        hashFunc('./IMG_20170916_134322.jpg'),
    ]);
    console.timeEnd(name);
    console.log(`${name} (scaled)`, hammingFunc(res[0], res[1]));
    console.log(`${name} (cropped)`, hammingFunc(res[0], res[2]));
    console.log(`${name} (completely different)`, hammingFunc(res[0], res[3]));
}

async function main() {
    console.log(pHash.info());

    await doIt('DCT', pHash.getDctImageHash, pHash.getHammingDistance);
    await doIt('DCT (H2)', pHash.getDctImageHash, pHash.getHammingDistance2);
    await doIt('MH', pHash.getMhImageHash, pHash.getHammingDistance2);
}

main().catch(err => {
    console.error(err);
    process.exit(1);
});
