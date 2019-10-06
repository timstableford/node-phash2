/// <reference types="node" />

export const DEFAULT_MH_ALPHA: number;
export const DEFAULT_MH_LEVEL: number;
export const DEFAULT_VIDEO_DISTANCE_THRESHOLD: number;

export interface Info {
  about: string;
  video: boolean;
  audio: boolean;
  image: boolean;
}

export function info(): Info;

// Generate a DCT image hash.
export function getDctImageHash(path: string): Promise<Buffer>;
// Get the hamming distance (number of different bits in the hash) of two hashes.
export function getHammingDistance(hashA: Buffer, hashB: Buffer): number;

// Generate the MH image hash.
export function getMhImageHash(path: string, alpha?: number, level?: number): Promise<Buffer>;
// Get the distance between two MH hashes. Scale 0 to 1, where 0 is identical.
export function getHammingDistance2(hashA: Buffer, hashB: Buffer): number;

// Generate DCT hash of a video.
export function getDctVideoHash(path: string): Promise<Buffer>;
// Get the average hamming distance between two hashes. Scale 0 to 1 where 1 is identical.
export function getVideoDistance(hashA: Buffer, hashB: Buffer, threshold?: number): Promise<number>;
