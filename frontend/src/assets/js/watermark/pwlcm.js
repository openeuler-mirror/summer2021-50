import { arrayKeys, merge, split } from "../tools/image";
let cv = require("opencv.js");

export function pwlcm_generate(x, miu, k, m, n) {
  for (let i = 1; i < k; i++) {
    x = pwlcm_function(x, miu);
  }
  let map = [x];
  for (let i = 1; i < m * n; i++) {
    map[i] = pwlcm_function(map[i - 1], miu);
  }
  let key = arrayKeys(map);
  map.mergeSort(key);
  return key;
}

function pwlcm_function(xn, miu) {
  if (0 < xn && xn < miu) {
    return xn / miu;
  } else if (miu <= xn && xn < 0.5) {
    return (xn - miu) / (0.5 - miu);
  } else if (0.5 <= xn && xn < 1 - miu) {
    return (1 - xn - miu) / (0.5 - miu);
  } else if (1 - miu <= xn && xn <= 1) {
    return (1 - xn) / miu;
  }
}

function pwlcm_change(img, key) {
  let m = img.rows,
    n = img.cols;
  let result = new Uint8Array(m * n);
  for (let i = 0; i <= m * n - 1; i++) {
    result[i] = img.data[key[i]];
  }
  return cv.matFromArray(m, n, cv.CV_8UC1, result);
}

function pwlcm_return(result, key) {
  let m = result.rows,
    n = result.cols;
  let img = new Uint8Array(m * n);
  for (let i = 0; i <= m * n - 1; i++) {
    img[key[i]] = result.data[i];
  }
  return cv.matFromArray(m, n, cv.CV_8UC1, img);
}

export function pwlcm_encrypt(img, x, miu, k) {
  let m = img.rows,
    n = img.cols;
  let rgbaPlanes = split(img);
  let array = new cv.MatVector();
  let key = pwlcm_generate(x, miu, k, m, n);
  for (let i = 0; i <= 3; i++) {
    let channel = rgbaPlanes.get(i);
    array.push_back(pwlcm_change(channel, key));
  }
  return merge(array);
}

export function pwlcm_decrypt(img, x, miu, k) {
  let m = img.rows,
    n = img.cols;
  let rgbaPlanes = split(img);
  let array = new cv.MatVector();
  let key = pwlcm_generate(x, miu, k, m, n);
  for (let i = 0; i <= 3; i++) {
    let channel = rgbaPlanes.get(i);
    array.push_back(pwlcm_return(channel, key));
  }
  return merge(array);
}
