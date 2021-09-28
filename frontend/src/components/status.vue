<template>
  <i class="el-icon-success" v-if="status" style="color: green"></i>
  <i class="el-icon-error" v-if="!status" style="color: red"></i>
</template>

<script>
export default {
  name: "status",
  props: {
    status: {
      type: Boolean,
      required: true,
    },
  },
};
</script>

<style scoped></style>
