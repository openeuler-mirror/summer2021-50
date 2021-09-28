let cv = require("opencv.js");

import { merge, resize, split } from "../tools/image";

// function generate_logsit(x, miu, N) {
//   let map = [x];
//   for (let i = 1; i < N * N; i++) {
//     map[i] = miu * map[i - 1] * (1 - map[i - 1]);
//   }
//   return map;
// }

function cat(data, n, a, b) {
  let N = data.rows;
  let result = cv.Mat.zeros(N, N, cv.CV_8UC1);
  for (let i = 0; i < n; i++) {
    for (let y = 0; y < N; y++) {
      for (let x = 0; x < N; x++) {
        let x_encrypt = (x + b * y) % N;
        let y_encrypt = (a * x + (a * b + 1) * y) % N;
        result.ucharPtr(y_encrypt, x_encrypt)[0] = data.ucharPtr(y, x)[0];
      }
    }
    data = result.clone();
  }
  return result;
}

function cat_inverse(result, n, a, b) {
  let N = result.rows;
  let data = cv.Mat.zeros(N, N, cv.CV_8UC1);
  for (let i = 0; i < n; i++) {
    for (let y_encrypt = 0; y_encrypt <= N - 1; y_encrypt++) {
      for (let x_encrypt = 0; x_encrypt <= N - 1; x_encrypt++) {
        let x = ((a * b + 1) * x_encrypt - b * y_encrypt) % N;
        let y = (-a * x_encrypt + y_encrypt) % N;
        x = x < 0 ? x + N : x;
        y = y < 0 ? y + N : y;
        data.ucharPtr(y, x)[0] = result.ucharPtr(y_encrypt, x_encrypt)[0];
      }
    }
    result = data.clone();
  }
  return data;
}

export function cat_encrypt(watermark, miu, n, a, b) {
  watermark = resize(watermark);
  let rgbaPlanes = split(watermark);
  let array = new cv.MatVector();
  for (let i = 0; i <= 3; i++) {
    let channel = rgbaPlanes.get(i);
    array.push_back(cat(channel, n, a, b));
  }
  return merge(array);
}

export function cat_decrypt(watermark, miu, n, a, b) {
  let rgbaPlanes = split(watermark);
  let array = new cv.MatVector();
  for (let i = 0; i <= 3; i++) {
    let channel = rgbaPlanes.get(i);
    array.push_back(cat_inverse(channel, n, a, b));
  }
  return merge(array);
}
