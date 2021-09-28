<template>
  <div class="main_page">
    <el-container direction="vertical">
      <el-header>
        <el-menu
          background-color="rgb(0,0,0,0.6)"
          :default-active="activeIndex"
          mode="horizontal"
          active-text-color="#ffd04b"
          text-color="white"
          @select="handleSelect"
        >
          <el-menu-item disabled
            ><el-image style="" :src="logo"
          /></el-menu-item>
          <el-menu-item index="1">加水印</el-menu-item>
          <el-menu-item index="2">解水印</el-menu-item>
          <el-menu-item index="3">项目介绍</el-menu-item>
          <el-menu-item index="4">个人中心</el-menu-item>
          <div class="logout-container">
            <el-popconfirm
              class="confirmLogout"
              title="确定要离开吗？"
              confirmButtonText="不要"
              cancelButtonText="好的"
              icon="el-icon-info"
              iconColor="red"
              @cancel="logout"
            >
              <template #reference>
                <el-button
                  size="small"
                  type="warning"
                  icon="el-icon-caret-right"
                  >登出</el-button
                >
              </template>
            </el-popconfirm>
          </div>
        </el-menu>
      </el-header>
      <el-main>
        <el-row type="flex" class="main_component" justify="center">
          <el-card>
            <Embed v-if="activeIndex === '1'" />
            <Extract v-if="activeIndex === '2'" />
            <Display v-if="activeIndex === '3'" />
            <Center v-if="activeIndex === '4'" />
          </el-card>
          <div class="footer">
            Copyright © 2021 secDRM-数字版权保护云平台. V1.0 <br />
            secDRM团队版权所有<br />
          </div>
        </el-row>
      </el-main>
    </el-container>
  </div>
</template>

<script>
import Embed from "../components/MainComponents/Embed";
import Extract from "../components/MainComponents/Extract";
import Display from "../components/MainComponents/Display";
import logo from "../assets/logo-black.png";
import Center from "@/components/MainComponents/Center";

export default {
  name: "Main",
  data() {
    return {
      height: window.innerHeight - 136 + "px",
      activeIndex: "1",
      logo: logo,
    };
  },
  components: {
    Center,
    Extract,
    Embed: Embed,
    Display: Display,
  },
  methods: {
    handleSelect(key) {
      this.activeIndex = key;
    },
    logout() {
      this.$router.push("/login");
      localStorage.removeItem("Authorization");
      localStorage.removeItem("hun");
    },
  },
};
</script>

<style scoped>
.main_page {
  width: 100%;
  min-width: 600px;
  height: 100%;
  padding: 0;
}
.main_page .el-header {
  padding: 0;
  min-width: 600px;
  background-image: url("../../resource/background.jpeg");
}
.main_page .el-main {
  display: flex;
  padding: 0;
  text-align: center;
  height: 100%;
  width: 100%;
  min-width: 600px;
  background-color: white;
  z-index: 99;
  margin-bottom: 70px;
}
.main_page .footer {
  color: white;
  text-align: center;
  /*margin-top: 240px;*/
  position: fixed;
  bottom: 0;
  padding: 10px 0 5px 0;
  width: 100%;
  height: 60px;
  font-family: serif;
  font-weight: bolder;
  line-height: 1.4;
  background-image: url("../../resource/background.jpeg");
}

.main_component {
  width: 100%;
  min-width: 600px;
  display: flex;
  text-align: center;
}
.el-container {
  width: 100%;
  height: 100%;
}
.logout-container {
  background-color: rgba(0, 0, 0, 0);
  text-align: right;
  padding: 0;
}
.el-menu-item.is-disabled {
  opacity: 1;
  cursor: default;
}
.el-menu-item.is-disabled .el-image {
  text-align: center;
  width: 150px;
  margin-top: 10px;
}

.el-button--warning:hover {
  background-color: rgba(255, 255, 255, 0.2);
  padding: 10px 15px 10px 10px;
}
.el-button--warning {
  background-color: rgba(255, 255, 255, 0.1);
  border: black;
  font-weight: bolder;
  padding: 10px 10px 10px 5px;
  font-size: 16px;
  color: white;
  height: 61px;
  margin-top: 0;
}
</style>
