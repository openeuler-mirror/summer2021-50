<template>
  <div style="width: 80%;margin-left: 10%">
    <Title class="top-title" msg="认证流程" :status="true"></Title>
      <el-image :src="show" style="width: 100%">
      </el-image>
    <Title class="top-title" msg="猫映射" :status="true"></Title>

    <div>
      <el-row :gutter="20">
        <el-col :span="8">
          <el-card>
            <img :src="watermark" id="cat_input" class="image" />
          </el-card>
        </el-col>
        <el-col :span="8">
          <el-card>
            <canvas
              style="max-height: 512px; max-width: 512px"
              id="cat_output"
              class="image"
            />
          </el-card>
        </el-col>
        <el-col :span="8">
          <el-card>
            <canvas
              style="max-height: 512px; max-width: 512px"
              id="cat_return"
              class="image"
            />
          </el-card>
        </el-col>
      </el-row>
    </div>

    <el-button style="width: 80%; margin-bottom: 10px" @click="flags.showCatSetting=true" type="primary">
      设置
    </el-button>

    <el-dialog
      :width="400"
      v-model="flags.showCatSetting"
      @close="flags.showCatSetting = false">
      <el-row>
        <el-col :span="6">
          <el-slider
            vertical
            @change="showCatMap"
            v-model="cat_parameters.cat_value"
            input-size="small"
            height="200px"
            :max="cat_parameters.cat_max"
            :show-tooltip="false"
          ></el-slider>
        </el-col>
        <el-col :span="18">
          <el-form label-width="30px" :model="cat_parameters">
            <el-form-item label="a">
              <el-input-number
                v-model="cat_parameters.a"
                :precision="0"
                :min="1"
                :step="1"
                :max="10"
              ></el-input-number>
            </el-form-item>
            <el-form-item label="b">
              <el-input-number
                v-model="cat_parameters.b"
                :precision="0"
                :min="1"
                :step="1"
                :max="10"
              ></el-input-number>
            </el-form-item>
            <el-form-item label="miu">
              <el-input-number
                v-model="cat_parameters.miu"
                :precision="3"
                :step="0.01"
                :max="4"
                :min="3.6"
              ></el-input-number>
            </el-form-item>
          </el-form>
        </el-col>
      </el-row>
    </el-dialog>

    <Title class="top-title" msg="超混沌序列" :status="true"></Title>

    <div>
      <el-row :gutter="20">
        <el-col :span="8">
          <el-card>
            <img
              style="max-height: 512px; max-width: 512px"
              :src="watermark"
              id="pwlcm_input"
              class="image"
            />
          </el-card>
        </el-col>
        <el-col style="max-height: 512px" :span="8">
          <el-card style="max-height: 512px; text-align: center">
            <canvas
              style="max-height: 512px; max-width: 512px"
              id="pwlcm_output"
              class="image"
            />
          </el-card>
        </el-col>
        <el-col style="max-height: 512px" :span="8">
          <el-card style="max-height: 512px; text-align: center">
            <canvas
              style="max-height: 512px; max-width: 512px"
              id="pwlcm_return"
              class="image"
            />
          </el-card>
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script>
import watermark from "../../assets/cat_test.png";
import test from "../../assets/map_test.png";
import Title from "@/components/MainComponents/title";
import show from "../../assets/show.jpg";
let cv = require("opencv.js");
let pwlcm_decrypt = require("@/assets/js/watermark/pwlcm.js").pwlcm_decrypt;
let pwlcm_encrypt = require("@/assets/js/watermark/pwlcm.js").pwlcm_encrypt;
let cat_decrypt = require("@/assets/js/watermark/cat.js").cat_decrypt;
let cat_encrypt = require("@/assets/js/watermark/cat.js").cat_encrypt;

let cat = ""
export default {
  name: "Display",
  components: {
    Title,
  },
  data() {
    return {
      flags:{
        showCatSetting:false,
      },
      show: show,
      cat_parameters: {
        a: 0,
        b: 0,
        miu: 0,
        cat_value: 0,
        cat_max: 10,
      },

      watermark: watermark,
      test: test,

      load: false,
    };
  },
  methods: {
    showCatMap() {
      if (this.cat_parameters.cat_value === 0) {
        cv.imshow("cat_output", cat);
        cv.imshow("cat_return", cat);
        return;
      }
      if (cat) {
        cv.imshow("cat_return", cat);
        let dst = cat_encrypt(
          cat,
          this.cat_parameters.miu,
          this.cat_parameters.cat_value,
          this.cat_parameters.a,
          this.cat_parameters.b
        );
        cv.imshow("cat_output", dst);
        let dst_return = cat_decrypt(
          dst,
          this.cat_parameters.miu,
          this.cat_parameters.cat_value,
          this.cat_parameters.a,
          this.cat_parameters.b
        );
        cv.imshow("cat_return", dst_return);
      }
    },
  },
  mounted() {
    let cat_input = document.getElementById("cat_input");
    cat_input.onload = () => {
      let src = cv.imread("cat_input");
      cat = src.clone();

      cv.imshow("cat_output", cat);
      cv.imshow("cat_return", cat);
      src.delete();
    };

    let pwlcm_input = document.getElementById("pwlcm_input");
    pwlcm_input.onload = () => {
      let src = cv.imread("pwlcm_input");
      let dst = pwlcm_encrypt(src, 0.33, 0.2, 10);
      cv.imshow("pwlcm_output", dst);
      let dst_return = pwlcm_decrypt(dst, 0.33, 0.2, 10);
      cv.imshow("pwlcm_return", dst_return);
    };
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
.image {
  width: 100%;
  display: block;
}

.cat {
  max-height: 512px;
  padding: 0;
  margin: 0;
  text-align: center;
}
.cat img {
  max-height: 512px;
  max-width: 512px;
}
</style>
