import Init from "../views/Init.vue";

let showLoading = require("@/assets/js/axios/loading.js").showLoading;
let hideLoading = require("@/assets/js/axios/loading.js").hideLoading;
let createRouter = require("vue-router").createRouter;
let createWebHashHistory = require("vue-router").createWebHashHistory;
const Main = () => import("../views/Main.vue");
const routes = [
  {
    path: "/",
    component: Main,
  },
  {
    path: "/login",
    name: "Login",
    component: Init,
  },
];

const router = createRouter({
  history: createWebHashHistory(),
  routes,
});
router.beforeEach((to, from, next) => {
  if (to.path === "/login") {
    showLoading();
    next();
  } else {
    let token = localStorage.getItem("Authorization");
    console.log(token);
    if (token === null || token === "") {
      next("/login");
    } else {
      showLoading();
      next();
    }
  }
});
// eslint-disable-next-line no-unused-vars
router.afterEach((to, from, next) => {
  hideLoading();
});
export default router;

export function _isMobile() {
  return navigator.userAgent.match(
    /(phone|pad|pod|iPhone|iPod|ios|iPad|Android|Mobile|BlackBerry|IEMobile|MQQBrowser|JUC|Fennec|wOSBrowser|BrowserNG|WebOS|Symbian|Windows Phone)/i
  );
}
