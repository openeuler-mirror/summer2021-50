import axios from "axios";
import { showLoading, hideLoading } from "./loading";
import { ElMessageBox, ElMessage } from "element-plus";
import router from "../../../router";

const service = axios.create({
  baseURL: "/api", // api的base_url
  timeout: 10000, // 请求超时时间
  withCredentials: true,
});

const error_message = function (msg) {
  ElMessageBox.alert(msg, "错误提示", {
    confirmButtonText: "确定",
    type: "error",
  });
};

service.interceptors.request.use(
  (config) => {
    showLoading();

    if (localStorage.getItem("Authorization"))
      config.headers.Authorization = localStorage.getItem("Authorization");
    if (config.url === "/add"){
      // config.headers["Content-Type"] = 'multipart/form-data;boundary = ' + new Date().getTime();
      config.data = config.data.formData;
      return config
    }
    config.method === "post"
      ? (config.data = { ...config.data })
      : (config.params = { ...config.params });
    return config;
  },
  //请求错误处理
  (error) => {
    let text = JSON.parse(JSON.stringify(error)).response;
    error_message(text);
    Promise.reject(error);
  }
);

service.interceptors.response.use(
  //成功请求到数据
  (response) => {
    hideLoading();
    if (response.status === 200) {
      if (response.data.code === 0) {
        return response.data;
      } else if (response.data.code === 4101) {
        localStorage.removeItem("Authorization");
        localStorage.removeItem("hun");
        router.push("/login").then(r => location.reload());
        ElMessage.warning({
          title: "警告",
          message: "请重新登陆",
        });
      } else {
        return response.data;
      }
    } else {
      error_message(response.statusText);
    }
  },
  //响应错误处理
  (error) => {
    console.log("error");
    console.log(error);
    console.log(JSON.stringify(error));
    hideLoading();
    let text = JSON.parse(JSON.stringify(error)).response;
    error_message(text);
    return Promise.reject(error);
  }
);

export default service;
