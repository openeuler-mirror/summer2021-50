let cv = require("opencv.js");

export function resize(img) {
  let row = img.rows,
    col = img.cols;
  let N = row > col ? row : col;
  let resize_dst = new cv.Mat();
  let s = new cv.Scalar(0, 0, 0, 255);
  cv.copyMakeBorder(
    img,
    resize_dst,
    N - row,
    0,
    N - col,
    0,
    cv.BORDER_CONSTANT,
    s
  );
  return resize_dst;
}

export function resize_return(img, row, col) {
  let roi_dst;
  let N = img.rows;
  let rect = new cv.Rect(N - col, N - row, col, row);
  roi_dst = img.roi(rect);
  return roi_dst;
}

export function arrayKeys(arr) {
  let i = 0,
    len = arr.length,
    keys = [];
  while (i < len) {
    keys.push(i++);
  }
  return keys;
}

Array.prototype.mergeSort = function (key, order) {
  key = {}.toString.call(key) === "[object Array]" ? key : [];
  function mergeGroups(arr, len, gap) {
    let i;
    for (i = 0; i + 2 * gap - 1 < len; i += gap * 2) {
      merge(arr, i, i + gap - 1, i + 2 * gap - 1);
    }
    if (i + gap - 1 < len - 1) {
      merge(arr, i, i + gap - 1, len - 1);
    }
  }
  // 核心排序过程
  function merge(arr, start, mid, end) {
    var i = start;
    var j = mid + 1;
    var aTmp = [];
    var kTmp = [];
    var isAsc = (order + "").toLowerCase() !== "desc";
    /* 排序过程开始 */
    while (i <= mid && j <= end) {
      if ((isAsc && arr[i] <= arr[j]) || (!isAsc && arr[i] >= arr[j])) {
        aTmp.push(arr[i]);
        kTmp.push(key[i++]);
      } else {
        aTmp.push(arr[j]);
        kTmp.push(key[j++]);
      }
    }
    while (i <= mid) {
      aTmp.push(arr[i]);
      kTmp.push(key[i++]);
    }
    while (j <= end) {
      aTmp.push(arr[j]);
      kTmp.push(key[j++]);
    } /*排序过程结束*/
    var len = aTmp.length,
      k;
    for (k = 0; k < len; k++) {
      arr[start + k] = aTmp[k];
      key[start + k] = kTmp[k];
    }
  }
  // 归并排序(从下往上)
  return (function (arr) {
    var len = arr.length;
    if (len <= 0) return arr;
    for (var i = 1; i < len; i *= 2) {
      mergeGroups(arr, len, i);
    }
    return arr;
  })(this);
};

export function split(img) {
  let rgbaPlanes = new cv.MatVector();
  cv.split(img, rgbaPlanes);
  img.delete();
  return rgbaPlanes;
}

export function merge(array) {
  let mat = new cv.Mat();
  cv.merge(array, mat);
  return mat;
}
