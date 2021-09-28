<template>
  <el-form :model="forms" :rules="rules" ref="ruleForm" class="register-main">
    <el-form-item prop="username">
      <el-input
        type="text"
        placeholder="请输入用户名"
        :maxlength="20"
        v-model="forms.username"
        prefix-icon="el-icon-user-solid"
      ></el-input>
    </el-form-item>
    <el-form-item prop="phone">
      <el-input
        placeholder="请输入手机号"
        v-model="forms.phone"
        prefix-icon="el-icon-mobile-phone"
      >
      </el-input>
    </el-form-item>

    <el-button-group>
      <el-row>
        <el-col :span="12">
          <el-form-item prop="code">
            <el-input
              v-model="forms.code"
              placeholder="验证码"
              :minlength="6"
              :maxlength="6"
            >
            </el-input>
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-button
            class="phone-code-btn"
            @click="getCode"
            type="primary"
            :disabled="isPhoneCodeDisabled"
          >
            {{
              isPhoneCodeDisabled
                ? countText.count + "s后获取"
                : countText.click
            }}
          </el-button>
        </el-col>
      </el-row>
    </el-button-group>

    <el-form-item prop="password">
      <el-tooltip effect="light" trigger="focus" placement="left">
        <template class="password-tips" #content>
          <status :status="password_warns[0]" /> 包含数字<br />
          <status :status="password_warns[1]" /> 包含至少一位小写字母<br />
          <status :status="password_warns[2]" /> 包含至少一位大写字母<br />
          <status :status="password_warns[3]" /> 包含至少一位特殊符号<br />
          <status :status="password_warns[4]" /> 长度在6-16之间 <br />
        </template>
        <el-input
          type="password"
          placeholder="请输入密码"
          v-model="forms.password"
          prefix-icon="el-icon-lock"
          :maxlength="20"
        >
          <template #suffix>
            {{ score.text }}
          </template>
        </el-input>
      </el-tooltip>
    </el-form-item>
    <el-form-item prop="checkPass">
      <el-input
        type="password"
        placeholder="请再次输入密码"
        v-model="forms.checkPass"
        prefix-icon="el-icon-lock"
        :maxlength="20"
      >
      </el-input>
    </el-form-item>
    <div class="register-btn">
      <el-button @click="register('ruleForm')" type="primary">注册</el-button>
    </div>
  </el-form>
</template>

<script>
import status from "@/components/status";
let toHPW = require("@/assets/js/encrypt/encrypt.js").toHPW;
let passwordEncrypt = require("@/assets/js/encrypt/encrypt.js").passwordEncrypt;
let validate = require("@/assets/js/tools/validate.js");
let sm3 = require("@/assets/js/encrypt/sm3.js");
let register = require("@/assets/js/axios/api.js").register;

export default {
  name: "Register",
  components: { status },
  data() {
    const validateConfirmPas = (rule, value, callback) => {
      if (value !== this.forms.password) {
        callback(new Error("两次输入的密码不一致"));
      }
      callback();
    };

    const validatePass = (_, value, callback) => {
      let exam_result = validate.exam_pwd(value);
      this.password_warns = exam_result;
      for (let i = 0, len = exam_result.length; i < len; i++) {
        this.score.score += exam_result[i];
      }
      if (this.score.score !== 5) {
        switch (this.score.score) {
          case 0:
            this.score.text = "😱";
            callback(new Error("快输入密码！"));
            break;
          case 1:
            this.score.text = "😱";
            callback(new Error("密码太弱啦！"));
            break;
          case 2:
            this.score.text = "😊";
            callback(new Error("密码不太行！"));
            break;
          case 3:
            this.score.text = "🤨";
            callback(new Error("密码还要再强一些！"));
            break;
          case 4:
            this.score.text = "🙂";
            callback(new Error("还能再强一些！"));
            break;
        }
      } else {
        this.score.text = "😁";
        callback();
      }
      this.score.score = 0;
    };

    return {
      password_warns: [false, false, false, false, false],
      isPhoneCodeDisabled: false,
      countText: {
        count: "59",
        click: "获取短信",
      },
      score: {
        score: 0,
        text: "",
      },
      forms: {
        username: "",
        phone: "",
        code: "",
        password: "",
        checkPass: "",
      },
      rules: {
        username: [
          { required: true, message: "请输入用户名", trigger: "blur" },
          { validator: validate.ValidateUsername },
        ],
        phone: [
          { required: true, message: "请输入手机号", trigger: "blur" },
          { validator: validate.Phone },
        ],
        code: [
          { required: true, message: "请输入验证码", trigger: "blur" },
          { min: 6, max: 6, message: "验证码长度为6", trigger: "blur" },
        ],
        password: [
          { required: true, message: "请输入密码", trigger: "blur" },
          { validator: validatePass },
        ],
        checkPass: [
          { required: true, message: "请再一次输入密码", trigger: "blur" },
          { validator: validateConfirmPas },
        ],
      },
    };
  },
  methods: {
    countTime() {
      const TIME_COUNT = 60;
      if (!this.timer) {
        this.countText.count = TIME_COUNT;
        this.isPhoneCodeDisabled = true;
        console.log(this.isPhoneCodeDisabled);
        this.timer = setInterval(() => {
          if (this.countText.count > 0 && this.countText.count <= TIME_COUNT) {
            this.countText.count--;
          } else {
            this.isPhoneCodeDisabled = false;
            clearInterval(this.timer);
            this.timer = null;
          }
        }, 1000);
      }
    },

    getCode() {
      let that = this;
      let captcha = new window.TencentCaptcha(
        localStorage.getItem("AppID"),
        function (res) {
          if (res.ret === 0) {
            that.countTime();
          }
        }
      );
      captcha.show();
    },

    register(formName) {
      let that = this;
      let hun = sm3(this.forms.username);
      let params = {
        hun: hun,
        cpw: passwordEncrypt(toHPW(hun, this.forms.password)),
        phone: this.forms.phone,
        code: sm3(this.forms.code),
        ts: new Date().getTime(),
      };
      this.$refs[formName].validate((valid) => {
        if (valid) {
          let captcha = new window.TencentCaptcha(
            localStorage.getItem("AppID"),
            function (res) {
              if (res.ret === 0) {
                register(params).then(function (response) {
                  if (response.code === 0) {
                    console.log(
                      new Date().getTime() - parseFloat(response.data.ts)
                    );
                    console.log(response.token);
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

<style lang="scss" scoped>
.register-main {
  text-align: center;
  width: 70%;
  margin: 10px 15% 30px 15%;
  border-radius: 5px;
}

.password-tips {
  margin: 10px;
  line-height: 1.4;
  font-size: 6px;
  text-align: left;
}

.phone-code-btn {
  width: 100%;
  margin: 0;
  margin-down: 30px;
}

.register-btn button {
  width: 100%;
  height: 36px;
  margin-top: 10px;
  margin-down: 30px;
  border-radius: 10px;
}
</style>
