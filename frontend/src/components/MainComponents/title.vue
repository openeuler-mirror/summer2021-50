<template>
  <el-divider>
    <i
      v-if="status"
      style="background-color: yellow; color: red"
      class="el-icon-star-on"
    />
    <i v-if="!status" style="" class="el-icon-star-off" />
    <span style="color: black; font-weight: bolder">{{ msg }}</span>
  </el-divider>
</template>

<script>
export default {
  name: "title",
  props: {
    status: {
      type: Boolean,
      required: true,
    },
    msg: {},
  },
};
</script>

<style scoped>
.el-divider {
  width: 100%;
  background-image: url("../../../resource/background.jpeg");
  /*background: -webkit-linear-gradient(*/
  /*    left,*/
  /*    rgba(64, 159, 255, 1),*/
  /*    rgba(64, 159, 255, 0.5),*/
  /*    rgba(64, 159, 255, 0.6),*/
  /*    rgba(64, 159, 255, 0.8),*/
  /*    #409eff*/
  /*  )*/
  /*  no-repeat;*/
  height: 5px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}
</style>
