import service from "./requests";

export const init = (data) => {
  return service({
    url: "/tools/init",
    method: "get",
    data,
  });
};

export const login = (data) => {
  return service({
    url: "/login",
    method: "post",
    data,
  });
};

export const register = (data) => {
  return service({
    url: "/register",
    method: "post",
    data,
  });
};

export const validate = (data) => {
  return service({
    url: "/validate",
    method: "post",
    data,
  });
};

export const embed = (data) => {
  return service({
    url: "/embed",
    method: "post",
    data,
  });
};

export const add = (data) => {
  return service({
    url: "/add",
    method: "post",
    data,
  });
};

export const extract = (data) => {
  return service({
    url: "/extract",
    method: "post",
    data,
  });
};

export const getPersonInfo = (data) => {
  return service({
    url: "/personInfo",
    method: "get",
    data,
  });
};
