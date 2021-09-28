<template>
  <div>
    <Title
      class="top-title"
      msg="添加需要提取水印的图像"
      :status="watermarked_img"
    ></Title>

    <el-upload
      ref="upload"
      :auto-upload="false"
      :show-file-list="false"
      action="alert"
      accept="image/png,image/jpeg,image/jpg"
      :on-change="imageAdd"
      multiple
      drag
    >
      <image-viewer :status="watermarked_img"></image-viewer>
      <template #tip>
        <div class="el-upload__tip">只能上传jpg/png文件，且不超过50kb</div>
      </template>
    </el-upload>

    <el-dialog
      custom-class="enterPass_page"
      v-model="flags.enterPass"
      :width="320"
      destroy-on-close
      append-to-body
      :show-close="false"
      @close="clearPasswordDialog"
    >
      <template #title> 请输入密码吧</template>
      <el-form ref="checkPass" :model="forms" :rules="rules">
        <el-form-item prop="password">
          <el-input
            class="check-pass"
            placeholder="请输入密码"
            type="password"
            prefix-icon="el-icon-lock"
            v-model="forms.password"
          />
        </el-form-item>
      </el-form>
      <drag-verify
        :width="290"
        ref="dragVerify"
        handlerBg="black"
        v-model:isPassing="flags.isPassing"
        text="滑动滑块验证密码吧"
        successText="恭喜你，通过了！"
        handlerIcon="el-icon-d-arrow-right"
        successIcon="el-icon-circle-check"
        @passcallback="validatePassword"
      ></drag-verify>
    </el-dialog>

    <el-button
      ref="add"
      v-if="!flags.checkPass"
      class="add-btn"
      type="primary"
      @click="showPasswordDialog"
      >请先验证哟
    </el-button>
    <el-button
      v-if="flags.checkPass"
      ref="add"
      class="add-btn"
      type="success"
      @click="extract"
      >提取水印吧
    </el-button>
    <el-button
      size="big"
      style="width: 100px"
      type="primary"
      @click="clearImageInput"
      >清空</el-button
    >

    <Title
      class="medium-title"
      msg="提取水印结果"
      :status="flags.success"
    ></Title>

    <el-card style="margin: 0; padding: 0">
      <img v-if="flags.success" :src="result" class="image-previewer" alt="" />
      <el-empty
        class="image-previewer"
        :image-size="128"
        description="无结果"
        v-if="!flags.success"
      ></el-empty>
    </el-card>

    <el-button
      ref="download"
      :disabled="!flags.success"
      :type="flags.success ? 'success' : 'primary'"
      @click="download_click"
      style="width: 420px; margin-top: 10px"
    >
      {{ flags.success ? "请下载" : "未完成" }}
    </el-button>

    <img id="watermarked_img" :src="watermarked_img" style="overflow: hidden;display: None" />
  </div>
</template>

<script>
import Title from "@/components/MainComponents/title";
import ImageViewer from "@/components/MainComponents/image-viewer";
import DragVerify from "@/components/MainComponents/dragVerfiy";
let pwlcm_decrypt = require("@/assets/js/watermark/pwlcm.js").pwlcm_decrypt;
let pwlcm_encrypt = require("@/assets/js/watermark/pwlcm.js").pwlcm_encrypt;
let LSB_extract = require("@/assets/js/watermark/lsb.js").LSB_extract;
let cv = require("opencv.js");
let validate = require("@/assets/js/axios/api.js").validate;
let extract = require("@/assets/js/axios/api.js").extract;
let passwordEncrypt = require("@/assets/js/encrypt/encrypt").passwordEncrypt;
let toHPW = require("@/assets/js/encrypt/encrypt").toHPW;
let paramsDecrypt = require("@/assets/js/encrypt/encrypt").paramsDecrypt;
export default {
  name: "Extract",
  components: { Title, ImageViewer, DragVerify },
  data() {
    return {
      watermarked_img: false,
      cwid: "",
      flags: {
        isPassing: false,
        checkPass: false,
        enterPass: false,
        success: false,
      },
      file_info: {
        size: "",
        type: "",
        name: "",
        head: "",
      },
      watermark: false,
      forms: { password: "" },
      rules: {
        password: [
          { required: true, message: "请输入密码", trigger: "change" },
          { min: 6, max: 16, message: "密码长度为6-16位", trigger: "blur" },
        ],
      },
    };
  },
  methods: {
    clearImageInput() {
      this.watermarked_img = false;
      this.result = false;
      this.flags.checkPass = false;
      this.flags.success = false;
    },
    clearPasswordDialog() {
      this.flags.isPassing = false;
      this.$refs.dragVerify.reset();
    },
    showPasswordDialog() {
      if (!this.watermarked_img) {
        this.$message.error("还未上传水印图片");
        return false;
      }
      this.flags.enterPass = true;
    },
    validatePassword() {
      let hun = localStorage.getItem("hun");
      this.hpw = toHPW(hun, this.forms.password);
      let params = {
        hun: hun,
        cpw: passwordEncrypt(this.hpw),
        ts: new Date().getTime(),
      };
      let that = this;
      validate(params).then(function (response) {
        if (response.code === 0) {
          that.flags.checkPass = true;
          that.flags.enterPass = false;
        } else {
          that.flags.checkPass = false;
          that.$message({
            message: "错了哦! " + response.msg,
            center: true,
            type: "error",
          });
        }
        that.clearPasswordDialog();
        that.forms.password = "";
        that.$refs.checkPass.resetFields();
      });
    },
    imageAdd(file) {
      const isImage =
        ["image/png", "image/jpg", "image/jpeg"].indexOf(file.raw.type) !== -1;
      const isLt2M = true;
      if (!isImage) {
        this.$message.error("上传图片只能是 JPG/PNG 格式!");
        return false;
      }
      function readBlobAsDataURL(blob, callback) {
        let a = new FileReader();
        a.onload = function (e) {
          callback(e.target.result);
        };
        a.readAsDataURL(blob);
      }
      this.watermarked_img = URL.createObjectURL(file.raw);
      this.file_info.size = file.raw.size;
      this.file_info.type = file.raw.type;
      this.file_info.name = file.raw.name;
      readBlobAsDataURL(file.raw, this.extract_wid);
      return isImage && isLt2M;
    },
    extract_wid(dataUrl) {
      let payload = dataUrl.split(";base64,")[1];
      this.file_info.head = dataUrl.split(";base64,")[0] + ";base64,";
      this.cwid = window.atob(payload).slice(window.atob(payload).length - 64);
      console.log(this.cwid);
    },

    extract() {
      let params = {
        cwid: passwordEncrypt(this.cwid),
        ts: new Date().getTime(),
      };
      let that = this;
      extract(params).then(function (response) {
        if (response.code === 0) {
          let k = response.data.k;
          let mu = response.data.mu;
          let x = response.data.x;
          console.log(k);
          console.log(mu);
          console.log(x);
          console.log(paramsDecrypt(x, that.hpw, that.cwid));
          console.log(paramsDecrypt(mu, that.hpw, that.cwid));
          console.log(paramsDecrypt(k, that.hpw, that.cwid));
          console.log(paramsDecrypt(x, that.hpw, that.cwid).slice(0, 3));
          console.log(paramsDecrypt(mu, that.hpw, that.cwid).slice(0, 3));
          console.log(paramsDecrypt(k, that.hpw, that.cwid).slice(0, 1));
          x =
            parseInt(paramsDecrypt(x, that.hpw, that.cwid).slice(0, 3), 16) /
            1000;
          mu =
            parseInt(paramsDecrypt(mu, that.hpw, that.cwid).slice(0, 3), 16) /
            1000;
          k = parseInt(paramsDecrypt(k, that.hpw, that.cwid).slice(0, 1), 16);
          console.log(x);
          console.log(mu);
          console.log(k);
          let watermarked_img = cv.imread("watermarked_img");
          let outputCanvas = document.createElement("canvas");
          watermarked_img = pwlcm_encrypt(watermarked_img, x, mu, k);
          let watermark = LSB_extract(
            watermarked_img,
            response.data.n,
            response.data.m
          );
          watermark = pwlcm_decrypt(watermark, x, mu, k);
          cv.imshow(outputCanvas, watermark);
          that.result = outputCanvas.toDataURL(response.data.type, 1);
          that.flags.success = true;
          that.flags.checkPass = false;
          that.$message.success("成功");
        } else {
          that.flags.checkPass = false;
          that.$message({
            message: "错了哦! " + response.msg,
            center: true,
            type: "error",
          });
        }
      });
    },

    download_click() {
      if (this.result) {
        let creatDom = document.createElement("a");
        document.body.appendChild(creatDom);
        creatDom.href = this.result;
        creatDom.download = this.file_info.origin.name;
        creatDom.click();
      }
    },
  },
};
</script>

<style scoped>
.bottom-title,
.medium-title {
  margin-top: 50px;
  margin-bottom: 50px;
}

.top-title {
  width: 100%;
  margin-top: 10px;
  margin-bottom: 50px;
}
</style>
