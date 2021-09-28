import { merge, split } from "../tools/image";
let cv = require("opencv.js");

export function LSB(origin, watermark) {
  let watermark_rgba = split(watermark);
  let total = 0;
  for (let i = 0; i <= 3; i++) {
    let watermark_channel = watermark_rgba.get(i);
    for (
      let j = 0;
      j <= watermark_channel.rows * watermark_channel.cols - 1;
      j++
    ) {
      let b = to_binary(watermark_channel.data[j]);
      for (let k = 0; k <= 7; k = k + 4) {
        let bit = b.slice(k, k + 4);
        let replace = to_binary(origin.data[origin.data.length - total - 1]);
        origin.data[origin.data.length - total - 1] = parseInt(
          replace.slice(0, 4) + bit,
          2
        );
        total += 3;
      }
    }
  }
  return origin;
}

export function to_binary(num) {
  let BinaryStr = num.toString(2);
  while (BinaryStr.length < 8) {
    BinaryStr = "0" + BinaryStr;
  }
  return BinaryStr;
}

export function LSB_extract(origin, m, n) {
  let total = 0;
  let watermark = new cv.MatVector();
  for (let i = 0; i <= 3; i++) {
    let watermark_channel = [];
    for (let j = 0; j <= m * n - 1; j++) {
      let re = "";
      for (let k = 0; k <= 7; k = k + 4) {
        let hide = to_binary(origin.data[origin.data.length - total - 1]);
        re += hide.slice(4, 8);
        total += 3;
      }
      watermark_channel.push(parseInt(re, 2));
    }
    watermark.push_back(cv.matFromArray(n, m, cv.CV_8UC1, watermark_channel));
  }
  return merge(watermark);
}
