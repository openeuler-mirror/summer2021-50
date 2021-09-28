<template>
  <div class="homepage-hero-module">
    <div class="video-container">
      <div :style="fixStyle" class="filter"></div>
      <video :style="fixStyle" autoplay loop v-on:canplay="canplay">
        <source src="http://www.secgear.fishworld.site/secgear/background.mp4" type="video/mp4" />
        浏览器不支持 video 标签，建议升级浏览器。
        <!--        <source src="PATH_TO_WEBM" type="video/webm"/>-->
        浏览器不支持 video 标签，建议升级浏览器。
      </video>
    </div>
  </div>
</template>

<script>
export default {
  name: "Background",
  data() {
    return {
      vedioCanPlay: false,
      fixStyle: "",
    };
  },
  methods: {
    canplay() {
      this.vedioCanPlay = true;
    },
  },
  mounted: function () {
    var mo = function (e) {
      e.preventDefault();
    };
    document.body.style.overflow = "hidden";
    document.addEventListener("touchmove", mo, false);
    window.onresize = () => {
      const windowWidth = document.body.clientWidth;
      const windowHeight = document.body.clientHeight;
      const windowAspectRatio = windowHeight / windowWidth;
      let videoWidth;
      let videoHeight;
      if (windowAspectRatio < 0.5625) {
        videoWidth = windowWidth;
        videoHeight = videoWidth * 0.5625;
        this.fixStyle = {
          height: windowWidth * 0.5625 + "px",
          width: windowWidth + "px",
          "margin-bottom": (windowHeight - videoHeight) / 2 + "px",
          "margin-left": 0,
        };
      } else {
        videoHeight = windowHeight;
        videoWidth = videoHeight / 0.5625;
        this.fixStyle = {
          height: windowHeight + "px",
          width: windowHeight / 0.5625 + "px",
          "margin-left": (windowWidth - videoWidth) / 2 + "px",
          "margin-bottom": "initial",
        };
      }
    };
    window.onresize();
  },
};
</script>

<style scoped>
.homepage-hero-module,
.video-container .poster img,
.video-container video {
  z-index: 0;
  height: 100%;
  width: 100%;
  position: absolute;
}

.video-container .filter {
  height: 100%;
  width: 100%;
  z-index: 1;
  position: absolute;
  background: rgba(0, 0, 0, 0.4);
}
</style>
