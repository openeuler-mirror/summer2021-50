import { createApp } from "vue";
import App from "./App.vue";
import router from "./router";
import ElementPlus from "element-plus";

import {
  ElAlert,
  ElBacktop,
  ElBadge,
  ElButton,
  ElButtonGroup,
  ElCard,
  ElCol,
  ElContainer,
  ElDialog,
  ElDivider,
  ElFooter,
  ElForm,
  ElFormItem,
  ElHeader,
  ElIcon,
  ElImage,
  ElInput,
  ElMain,
  ElMenu,
  ElMenuItem,
  ElMenuItemGroup,
  ElPageHeader,
  ElRow,
  ElTabPane,
  ElTable,
  ElTableColumn,
  ElTabs,
  ElTag,
  ElTooltip,
  ElLoading,
  ElMessage,
  ElMessageBox,
  ElNotification,
} from "element-plus";

const components = [
  ElAlert,
  ElBacktop,
  ElBadge,
  ElButton,
  ElButtonGroup,
  ElCard,
  ElCol,
  ElContainer,
  ElDialog,
  ElDivider,
  ElFooter,
  ElForm,
  ElFormItem,
  ElHeader,
  ElIcon,
  ElImage,
  ElInput,
  ElMain,
  ElMenu,
  ElMenuItem,
  ElMenuItemGroup,
  ElPageHeader,
  ElRow,
  ElTabPane,
  ElTable,
  ElTableColumn,
  ElTabs,
  ElTag,
  ElTooltip,
];

const plugins = [ElLoading, ElMessage, ElMessageBox, ElNotification];

// register globally
const app = createApp(App);
app.use(ElementPlus);
app.use(router);
app.mount("#app");
export default app;
components.forEach((component) => {
  app.component(component.name, component);
});

plugins.forEach((plugin) => {
  app.use(plugin);
});
