<template>
  <div class="init-page">
    <Background />
    <div class="init-main">
      <div>
        <el-tabs stretch v-model="activeName">
          <div class="init-logo">
            <el-image :src="logo" style="width: 300px" />
          </div>
          <el-tab-pane name="login">
            <template #label>
              <span class="logo-label"> 登录 </span>
            </template>
            <Login />
          </el-tab-pane>
          <el-tab-pane name="register">
            <template #label>
              <span class="logo-label"> 注册 </span>
            </template>
            <Register />
          </el-tab-pane>
        </el-tabs>
      </div>
    </div>
  </div>
</template>

<script>
import Register from "../components/LoginComponents/Register";
import Login from "../components/LoginComponents/Login";
import Background from "../components/Background";
import logo from "../assets/logo.png";

export default {
  name: "Init",
  components: {
    Login,
    Register,
    Background,
  },
  data() {
    return {
      activeName: "login",
      logo: logo,
    };
  },
  created() {
    if (localStorage.getItem("Authorization")) {
      this.$router.push({ path: "/" });
    }
  },
};
</script>

<style scoped>
.init-page {
  width: 100%;
  height: 100%;
  display: flex;
  justify-content: center;
  align-items: center;
}

.init-main {
  text-align: center;
  z-index: 99;
  padding: 0;
}

.init-logo {
  margin: 5px auto 30px auto;
}

.logo-label {
  /*color: white;*/
  color: black;
  font-family: "Hiragino Sans GB", serif;
  line-height: 1.7;
  font-size: 18px;
  margin-bottom: 25px;
  font-weight: bolder;
}

.el-tabs {
  z-index: 99;
  /*background-color: rgba(0, 0, 0, 1.0);*/
  background-color: white;
  width: 100%;
  min-width: 320px;
  height: 100%;
  margin: 0;
  padding: 10px;
  box-shadow: 0 2px 4px #000000, 0 0 10px #dcdfe6;
  border-radius: 4px;
}
</style>
