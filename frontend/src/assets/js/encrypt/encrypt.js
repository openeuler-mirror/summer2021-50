let Base64 = require("js-base64").Base64;
let bigInt = require("big-integer");
let sm3 = require("./sm3");

export function paramsDecrypt(x, hpw, hwid) {
  let bigInt = require("big-integer");
  let sm3 = require("@/assets/js/encrypt/sm3");
  let y = sm3(hpw + hwid, true);
  return bigInt(x, 16).xor(bigInt(y, 16)).toString(16);
}

export function toHPW(hun, passwd) {
  let hpw = sm3(passwd);
  return sm3(hun + hpw);
}

export function passwordEncrypt(s_sha256) {
  let pw = localStorage.getItem("PubKey");
  let text = bigInt(s_sha256, 16);
  let pw0, pw1, pwlist, n, e, cpw;
  pw0 = Base64.decode(pw);
  pw1 = decodeURIComponent(pw0);
  pwlist = pw1.split("&");
  n = bigInt(pwlist[0].slice(2), 16); // mod
  e = bigInt(pwlist[1].slice(2), 16); // exponent
  console.log("hpw:" + s_sha256);
  console.log("hpw_10: ", text);
  console.log("n:", n);
  console.log("e: ", e);
  cpw = text.modPow(e, n);
  console.log("cpw:" + cpw.toString(16));
  return cpw.toString(16);
}
