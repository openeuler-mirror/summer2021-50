<template>
  <div>
    <el-affix style="position: absolute; top: 5px; left: 5px" :offset="65">
      <el-button style="padding: 0; width: 80px" type="primary" @click="test"
        >点击测试</el-button
      >
    </el-affix>

    <Title class="top-title" msg="添加原始图片" :status="origin"></Title>

    <el-upload
      ref="upload"
      class="image-uploader"
      :auto-upload="false"
      :show-file-list="false"
      action="alert"
      accept="image/png,image/jpeg,image/jpg"
      :on-change="originAdd"
      multiple
      drag
    >
      <image-viewer :status="origin"></image-viewer>
      <template #tip>
        <div class="el-upload__tip">
          只能上传jpg/png文件，且需要大于500KB，不超过1MB
        </div>
      </template>
    </el-upload>

    <Title class="medium-title" msg="添加水印图片" :status="watermark"></Title>

    <el-upload
      ref="upload"
      :auto-upload="false"
      :show-file-list="false"
      action="alert"
      accept="image/png,image/jpeg,image/jpg"
      :on-change="watermarkAdd"
      multiple
      drag
    >
      <image-viewer :status="watermark"></image-viewer>
      <template #tip>
        <div class="el-upload__tip">只能上传jpg/png文件，且不超过10KB</div>
      </template>
    </el-upload>

    <el-dialog
      custom-class="enterPass_page"
      v-model="flags.enterPass"
      destroy-on-close
      :width="320"
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
      :width="300"
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
      @click="embed"
      >加水印吧
    </el-button>

    <el-button class="clean-btn" type="primary" @click="clearImageInput"
      >清空</el-button
    >

    <Title class="bottom-title" msg="结果" :status="flags.success"></Title>

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
      :disabled="!flags.success"
      :type="flags.success ? 'success' : 'primary'"
      @click="download"
      style="width: 420px; margin-top: 10px"
    >
      {{ flags.success ? "请下载" : "未完成" }}
    </el-button>
    <img id="origin" :src="origin" style="display: None" alt="" />
    <img id="watermark" :src="watermark" style="display: None" alt="" />
  </div>
</template>

<script>
import ImageViewer from "@/components/MainComponents/image-viewer";
import DragVerify from "@/components/MainComponents/dragVerfiy";
import Title from "@/components/MainComponents/title";

import test_watermark from "@/assets/cat_test.png";
import test_origin from "@/assets/map_test.png";

let passwordEncrypt = require("@/assets/js/encrypt/encrypt").passwordEncrypt;
let paramsDecrypt = require("@/assets/js/encrypt/encrypt").paramsDecrypt;
let toHPW = require("@/assets/js/encrypt/encrypt").toHPW;
let LSB = require("@/assets/js/watermark/lsb.js").LSB;
let pwlcm = require("@/assets/js/watermark/pwlcm.js");
let imageConversion = require("image-conversion");
let cv = require("opencv.js");

let showloading = require("@/assets/js/axios/loading.js").showLoading;
let hideLoading = require("@/assets/js/axios/loading.js").hideLoading;
let validate = require("@/assets/js/axios/api.js").validate;
let embed = require("@/assets/js/axios/api.js").embed;
let add = require("@/assets/js/axios/api.js").add;
//
// let pwlcm_decrypt = require("@/assets/js/watermark/pwlcm.js").pwlcm_decrypt;
// let pwlcm_encrypt = require("@/assets/js/watermark/pwlcm.js").pwlcm_encrypt;
// let LSB_extract = require("@/assets/js/watermark/lsb.js").LSB_extract;

export default {
  name: "Embed",
  components: { DragVerify, ImageViewer, Title },

  data() {
    return {
      test_origin: test_origin,
      test_watermark: test_watermark,
      origin: false,
      watermark: false,
      flags: {
        isPassing: false,
        checkPass: false,
        enterPass: false,
        success: false,
      },
      file_info: {
        watermark: {
          size: "",
          type: "",
          name: "",
          m: "",
          n: "",
        },
        origin: {
          size: "",
          type: "",
          name: "",
        },
      },
      result: false,
      forms: { password: "", hpw: "" },
      rules: {
        password: [
          { required: true, message: "请输入密码", trigger: "change" },
          { min: 6, max: 16, message: "密码长度为6-16位", trigger: "blur" },
        ],
      },
    };
  },

  methods: {
    test() {
      this.watermark = this.test_watermark;
      this.origin = this.test_origin;
      this.file_info.origin.type = "image/png";
      this.file_info.origin.name = "test_origin.png";
      this.file_info.origin.size = 10;
      this.file_info.watermark.type = "image/png";
      this.file_info.watermark.name = "test_watermark.png";
      this.file_info.watermark.size = 10;
    },
    clearPasswordDialog() {
      this.flags.isPassing = false;
      this.$refs.dragVerify.reset();
    },
    showPasswordDialog() {
      if (!this.origin && !this.watermark) {
        this.$message.error("还未上传原始图片和水印图片");
        return false;
      }
      if (!this.origin) {
        this.$message.error("还未上传原始图片");
        return false;
      }
      if (!this.watermark) {
        this.$message.error("还未上传水印图片");
        return false;
      }
      this.flags.enterPass = true;
    },
    validatePassword() {
      let hun = localStorage.getItem("hun");
      this.forms.hpw = toHPW(hun, this.forms.password);
      let params = {
        hun: hun,
        cpw: passwordEncrypt(this.forms.hpw),
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
    clearImageInput() {
      this.origin = false;
      this.watermark = false;
      this.flags.checkPass = false;
      this.flags.success = false;
    },
    originAdd(file) {
      let isImage =
        ["image/png", "image/jpg", "image/jpeg"].indexOf(file.raw.type) !== -1;
      if (!isImage) {
        this.$message.error("上传图片只能是 JPG/PNG 格式!");
        this.clearImageInput();
        return false;
      }
      if (file.width <= 500 || file.height <= 500) {
        showloading();
        return new Promise((resolve) => {
          imageConversion
            .compressAccurately(file.raw, {
              width: 512,
              accuracy: 1,
            })
            .then((res) => {
              resolve(res);
              this.origin = URL.createObjectURL(res);
              this.file_info.origin.size = res.size;
              hideLoading();
            });
        });
      }

      this.file_info.origin.type = file.raw.type;
      this.file_info.origin.name = file.raw.name;

      if (file.size / 1024 / 1024 > 10) {
        this.$confirm("图片不能超过10MB,将进行压缩，是否继续?", "提示", {
          confirmButtonText: "确定",
          cancelButtonText: "取消",
          type: "warning",
        })
          .then(() => {
            showloading();
            return new Promise((resolve) => {
              imageConversion
                .compressAccurately(file.raw, {
                  width: 700,
                  accuracy: 1,
                })
                .then((res) => {
                  resolve(res);
                  this.origin = URL.createObjectURL(res);
                  this.file_info.origin.size = res.size;
                  hideLoading();
                });
            });
          })
          .catch(() => {
            this.clearImageInput();
            hideLoading();
            return false;
          });
      } else {
        this.origin = URL.createObjectURL(file.raw);
        this.file_info.origin.size = file.raw.size;
        return true;
      }
    },
    watermarkAdd(file) {
      let isImage =
        ["image/png", "image/jpg", "image/jpeg"].indexOf(file.raw.type) !== -1;
      if (!isImage) {
        this.$message.error("上传图片只能是 JPG/PNG 格式!");
        return false;
      }
      this.file_info.watermark.type = file.raw.type;
      this.file_info.watermark.name = file.raw.name;
      console.log(file.size / 1024);
      if (file.size / 1024 > 10) {
        this.$confirm("图片不能超过10KB,将进行压缩，是否继续?", "提示", {
          confirmButtonText: "确定",
          cancelButtonText: "取消",
          type: "warning",
        })
          .then(() => {
            showloading();
            return new Promise((resolve) => {
              imageConversion
                .compressAccurately(file.raw, { width: 128 })
                .then((res) => {
                  resolve(res);
                  this.watermark = URL.createObjectURL(res);
                  console.log(this.watermark);
                  this.file_info.watermark.size = res.size;
                  hideLoading();
                });
            });
          })
          .catch(() => {
            this.clearImageInput();
            hideLoading();
            return false;
          });
      } else {
        this.watermark = URL.createObjectURL(file.raw);
        this.file_info.watermark.size = file.raw.size;
        return true;
      }
    },
    download() {
      if (this.result) {
        let that = this;
        let watermarked_image = imageConversion.dataURLtoFile(that.result);
        watermarked_image.then((res) => {
          imageConversion.downloadFile(res, that.file_info.origin.name);
          let formData = new FormData();
          formData.append("cwid", that.params.cwid);
          formData.append("result", res);
          formData.append("ts", new Date().getTime());
          add({
            formData,
          }).then(function (response) {
            if (response.code === 0) {
              that.$message.success("成功");
            } else {
              that.$message({
                message: "错了哦! " + response.msg,
                center: true,
                type: "error",
              });
            }
          });
        });
      }
    },
    embed() {
      showloading();
      let sm3 = require("@/assets/js/encrypt/sm3");
      let hwid = sm3(String(new Date().getTime()));
      let origin = cv.imread("origin");
      let watermark = cv.imread("watermark");
      this.file_info.watermark.m = watermark.rows;
      this.file_info.watermark.n = watermark.cols;
      this.params = {
        cwid: passwordEncrypt(hwid),
        watermark_name: this.file_info.watermark.name,
        watermark_type: this.file_info.watermark.type,
        watermark_size: this.file_info.watermark.size,
        watermark_m: this.file_info.watermark.m,
        watermark_n: this.file_info.watermark.n,
        origin_name: this.file_info.origin.name,
        origin_type: this.file_info.origin.type,
        origin_size: this.file_info.origin.size,
        ts: new Date().getTime(),
      };
      console.log(this.params);
      let that = this;
      embed(this.params).then(function (response) {
        if (response.code === 0) {
          let k = response.data.k;
          let mu = response.data.mu;
          let x = response.data.x;
          console.log(k);
          console.log(mu);
          console.log(x);
          console.log(paramsDecrypt(x, that.forms.hpw, hwid));
          console.log(paramsDecrypt(mu, that.forms.hpw, hwid));
          console.log(paramsDecrypt(k, that.forms.hpw, hwid));
          console.log(paramsDecrypt(x, that.forms.hpw, hwid).slice(0, 3));
          console.log(paramsDecrypt(mu, that.forms.hpw, hwid).slice(0, 3));
          console.log(paramsDecrypt(k, that.forms.hpw, hwid).slice(0));
          x =
            parseInt(paramsDecrypt(x, that.forms.hpw, hwid).slice(0, 3), 16) /
            1000;
          mu =
            parseInt(paramsDecrypt(mu, that.forms.hpw, hwid).slice(0, 3), 16) /
            1000;
          k = parseInt(paramsDecrypt(k, that.forms.hpw, hwid).slice(0, 1), 16);
          console.log(x);
          console.log(mu);
          console.log(k);
          origin = pwlcm.pwlcm_encrypt(origin, x, mu, k);
          watermark = pwlcm.pwlcm_encrypt(watermark, x, mu, k);
          origin = LSB(origin, watermark);
          origin = pwlcm.pwlcm_decrypt(origin, x, mu, k);
          let outputCanvas = document.createElement("canvas");
          outputCanvas.height = origin.rows;
          outputCanvas.width = origin.width;
          cv.imshow(outputCanvas, origin);
          let watermarked_image_url = imageConversion.canvastoDataURL(
            outputCanvas,
            1,
            that.file_info.origin.type
          );
          watermarked_image_url.then((res) => {
            let header = res.split(";base64,")[0] + ";base64,";
            let body = window.atob(res.split(";base64,")[1]) + hwid;
            that.result = header + window.btoa(body);
            that.flags.success = true;
            that.flags.checkPass = false;
            that.$message.success("成功");
            hideLoading();
            // let watermarked_image = imageConversion.dataURLtoFile(
            //   that.result,
            //   that.file_info.origin.type
            // );
            // watermarked_image.then((res) => {
            //   console.log(res);
            //   let watermarked_img = cv.imread(outputCanvas);
            //   watermarked_img = pwlcm_encrypt(watermarked_img, x, mu, k);
            //   let watermark = LSB_extract(
            //     watermarked_img,
            //     that.file_info.watermark.n,
            //     that.file_info.watermark.m
            //   );
            //   watermark = pwlcm_decrypt(watermark, x, mu, k);
            //   cv.imshow("tmp", watermark);
            // });
          });
        } else {
          that.$message({
            message: "错了哦! " + response.msg,
            center: true,
            type: "error",
          });
          hideLoading();
        }
      });
    },
  },
};
</script>

<style>
.top-title {
  margin-top: 10px;
  margin-bottom: 50px;
}
.bottom-title,
.medium-title {
  margin-top: 50px;
  margin-bottom: 50px;
}
.add-btn {
  margin-top: 20px;
  width: 300px;
}
.clean-btn {
  width: 100px;
}
.image-previewer {
  height: 178px;
}
.enterPass_page .el-dialog__header {
  text-align: center;
  border-radius: 4px;
  font-weight: bolder;
  color: white;
  background-image: url("../../../resource/background.jpeg");
}
.enterPass_page .el-dialog__body {
  text-align: center;
  border-radius: 4px;
}
</style>
