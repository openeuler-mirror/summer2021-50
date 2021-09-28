<template>
  <div class="login_main">
    <el-form :model="forms" :rules="rules" ref="ruleForm" @submit.prevent>
      <el-form-item prop="username">
        <el-input
          placeholder="请输入用户名"
          type="text"
          required="required"
          v-model="forms.username"
          prefix-icon="el-icon-user-solid"
        ></el-input>
      </el-form-item>
      <el-form-item prop="password">
        <el-input
          placeholder="请输入密码"
          type="password"
          prefix-icon="el-icon-lock"
          v-model="forms.password"
          @keyup.enter="login('ruleForm')"
        />
      </el-form-item>
      <div class="login-btn">
        <el-button @click="login('ruleForm')" type="primary">登录 </el-button>
      </div>
    </el-form>
  </div>
</template>

<script>
let passwordEncrypt = require("@/assets/js/encrypt/encrypt.js").passwordEncrypt;
let toHPW = require("@/assets/js/encrypt/encrypt.js").toHPW;
const validate = require("@/assets/js/tools/validate.js");
let login = require("@/assets/js/axios/api").login;
let sm3 = require("@/assets/js/encrypt/sm3.js");

export default {
  name: "Login",
  data() {
    return {
      forms: {
        password: "",
        username: "",
      },
      rules: {
        username: [
          { required: true, message: "请输入用户名吧", trigger: "blur" },
          { validator: validate.ValidateUsername },
        ],
        password: [
          { required: true, message: "请输入密码吧", trigger: "change" },
          { min: 6, max: 16, message: "密码长度为6-16位", trigger: "blur" },
        ],
      },
    };
  },
  methods: {
    login(formName) {
      let that = this;
      let hun = sm3(this.forms.username);
      let hpw = toHPW(hun, this.forms.password);
      let cpw = passwordEncrypt(hpw);
      this.$refs[formName].validate((valid) => {
        if (valid) {
          let captcha = new window.TencentCaptcha(
            localStorage.getItem("AppID"),
            function (res) {
              if (res.ret === 0) {
                let params = {
                  hun: hun,
                  cpw: cpw,
                  ts: new Date().getTime(),
                };
                login(params).then(function (response) {
                  if (response.code === 0) {
                    console.log(
                      new Date().getTime() - parseFloat(response.data.ts)
                    );
                    localStorage.setItem("Authorization", response.token);
                    localStorage.setItem("hun", hun);
                    that.$router.push({ path: "/" });
                  } else {
                    that.$message({
                      message: "错了哦! " + response.msg,
                      center: true,
                      type: "error",
                    });
                  }
                });
              }
            }
          );
          captcha.show();
        } else {
          return false;
        }
      });
    },
  },
};
</script>

<style scoped>
.login_main {
  text-align: center;
  width: 70%;
  margin: 10px 15% 30px 15%;
  border-radius: 5px;
}

.login-btn {
  text-align: center;
}

.login-btn button {
  width: 100%;
  height: 36px;
  margin-top: 10px;
}
</style>
