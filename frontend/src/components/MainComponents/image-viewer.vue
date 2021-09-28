<template>
  <img v-if="status" :src="status" class="image-previewer" alt="" />
  <i v-if="!status" class="el-icon-upload" />
  <div v-if="!status" class="el-upload__text">
    将图片拖到此处，或<em>点击上传</em>
  </div>
</template>

<script>
export default {
  name: "image-viewer",
  props: {
    status: {
      type: Boolean,
    },
  },
};
</script>

<style scoped>
.image-previewer {
  height: 178px;
}
.el-upload__text em {
  color: black;
  font-weight: bolder;
}
</style>
