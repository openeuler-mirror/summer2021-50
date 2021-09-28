const email = /^([a-z0-9_.-]+)@([\da-z.-]+)\.([a-z.]{2,6})$/;

const phone = /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/;

function isLetter(pwd) {
  for (var i in pwd) {
    var asc = pwd.charCodeAt(i);
    if (asc >= 97 && asc <= 122) {
      return true;
    }
  }
  return false;
}

function is_C_Letter(pwd) {
  for (var i in pwd) {
    var asc = pwd.charCodeAt(i);
    if (asc >= 65 && asc <= 90) {
      return true;
    }
  }
  return false;
}

export function exam_username(username) {
  let re = /^[0-9a-zA-Z]+$/;
  let result = [];
  if (re.test(username)) {
    result.push(true);
  } else {
    result.push(false);
  }
  if (username.length >= 6 && username.length <= 20) {
    result.push(true);
  } else {
    result.push(false);
  }
  return result;
}

export function exam_pwd(pwd) {
  let result = [];
  // 检查是否包含数字
  let re_num = /\d/;
  if (re_num.test(pwd)) {
    result.push(true);
  } else {
    result.push(false);
  }

  // 检查是否包含字母
  result.push(isLetter(pwd));

  // 检查是否包含大写字母
  result.push(is_C_Letter(pwd));

  // 检查是否包含特殊符号
  let re_char = new RegExp(
    "[`~!@#$^&*()=|{}':;',\\[\\].<>《》/?~！@#￥……&*（）——|{}【】‘；：”“'。，、？]"
  );
  if (re_char.test(pwd)) {
    result.push(true);
  } else {
    result.push(false);
  }
  // 检查是否长度为6-16
  if (6 <= pwd.length && pwd.length <= 16) {
    result.push(true);
  } else {
    result.push(false);
  }

  return result;
}

export const ValidateUsername = function (rule, value, callback) {
  let exam_result = exam_username(value);
  if (!exam_result[0]) {
    callback(new Error("不要出现除了字母和数字以外的字符！"));
  }
  if (!exam_result[1]) {
    callback(new Error("输入长度在6-20之间的字母和数字"));
  }
  callback();
};

export const Email = (rule, value, callback) => {
  // eslint-disable-next-line no-useless-escape
  if (email.test(value)) {
    callback();
  } else {
    callback(new Error("请输入正确的邮箱"));
  }
};

export const Phone = (rule, value, callback) => {
  // eslint-disable-next-line no-useless-escape
  if (phone.test(value)) {
    callback();
  } else {
    callback(new Error("请输入正确的手机号"));
  }
};
