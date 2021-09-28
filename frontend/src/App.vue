<template>
  <div v-show="!ismobile" style="height: 100%; width: 100%">
    <router-view />
  </div>
  <div
    v-show="ismobile"
    style="height: 100%; width: 100%; background-color: black"
  ></div>
</template>

<script>
let isMobile = require("./router/index.js")._isMobile();
export default {
  name: "App",
  data() {
    return {
      ismobile: false,
    };
  },
  created() {
    this.ismobile = isMobile;
    if (this.ismobile) {
      this.$alert("该系统暂不支持手机端，请移步电脑端", "", {
        distinguishCancelAndClose: true,
        closeOnClickModal: false,
        closeOnPressEscape: false,
        closeOnHashChange: false,
        showClose: false,
        center: true,
        showConfirmButton: false,
      });
    } else {
      let init;
      init = require("./assets/js/axios/api.js").init;
      init().then(function (response) {
        localStorage.setItem("AppID", String(response.data.AppId));
        localStorage.setItem("PubKey", String(response.data.PubKey));
      });
    }
  },
};
</script>

<style lang="scss">
html,
body,
#app {
  width: 100%;
  margin: 0;
  height: 100%;
}

.el-input.is-active .el-input__inner,
.el-input__inner:focus {
  border-color: black;
}

.el-input__prefix,
.el-input__suffix {
  color: black;
}

.el-tabs__active-bar {
  background-image: url("../resource/background.jpeg");
}

.el-button--primary:focus,
.el-button--primary {
  background-image: url("../resource/background.jpeg");
  background-color: black;

  color: white;
  border: white;
}

.el-button.is-disabled,
.el-button.is-disabled:hover,
.el-button--primary.is-disabled:focus,
.el-button--primary.is-disabled:hover,
.el-button--primary:hover {
  background-image: url("../resource/background.jpeg");
  opacity: 0.5;
  background-color: black;
  border: white;
}

.el-upload-dragger .el-icon-upload {
  color: black;
}

.el-upload-dragger .el-upload:hover {
  border: 1px dashed black;
}

.el-button--text {
  color: black;
  text-align: center;
}
.el-popconfirm__main {
  margin: 5px;
}
.el-loading-spinner .el-loading-text {
  color: black;
}
</style>
