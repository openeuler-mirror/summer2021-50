<template>
  <div>
    <el-table :data="imageInfos" style="width: 100%; height: 100%">
      <el-table-column prop="time" label="添加水印时间" width="180">
      </el-table-column>
      <el-table-column prop="watermark_name" label="水印图像名称" width="180">
      </el-table-column>
      <el-table-column prop="watermark_type" label="水印图像类别" width="180">
      </el-table-column>
      <el-table-column prop="watermark_size" label="水印图像大小" width="180">
      </el-table-column>
      <el-table-column prop="origin_name" label="原始图像名称" width="180">
      </el-table-column>
      <el-table-column prop="origin_type" label="原始图像类别" width="180">
      </el-table-column>
      <el-table-column prop="watermark_size" label="原始图像大小" width="180">
      </el-table-column>
      <el-table-column align="images" width="80" prop="url" label="图片">
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
let getPersonInfo = require("@/assets/js/axios/api.js").getPersonInfo;
export default {
  name: "Center",
  data() {
    return {
      imageInfos: [],
    };
  },
  mounted() {
    let that = this;
    getPersonInfo().then(function (response) {
      if (response.code === 0) {
        that.imageInfos = response.data.datas;
      } else {
        this.$message({
          message: "错了哦! " + response.msg,
          center: true,
          type: "error",
        });
      }
    });
  },
};
</script>

<style scoped></style>
