<template>
  <div class="container">
    <div class="p-2">
      <vue-markdown :source="src" />
    </div>
    <div class="container">
      <h3>2.1 обычный алгоритм (4)</h3>
      <canvas class="lab_2__canvas" ref="canvas_elem"></canvas>
      <br />
      <button class="btn btn-primary" type="submit" @click="setDrawCurve">
        Draw Curve
      </button>
      <hr />
    </div>
  </div>
</template>

<script lang="ts">
import { ref } from "vue";

import { Canvas } from "../Lab/Canvas";
import { Lab_2 } from "./lab2";
import VueMarkdown from "vue-markdown-render";

export default {
  data() {
    return {
      lab: null,
    };
  },
  components: {
    VueMarkdown,
  },
  methods: {
    setDrawCurve() {
      this.lab.setDrawCurve();
    },
  },
  setup(props, ctx) {
    const src = ref(`## Лабораторная №1
    2. Рисование кривой Безье
    2.1 обычный алгоритм (4)
    2.2 алгоритм де Кастельо (4)
    2.3 составная кривая, рациональная кривая Безье (6)`);

    return {
      src,
    };
  },
  mounted() {
    // @ts-ignore
    const canvas_elem: HTMLCanvasElement = this.$refs.canvas_elem;
    const canvas = new Canvas(canvas_elem);
    const lab_2 = new Lab_2(canvas);
    this.lab = lab_2;
  },
};
</script>

<style lang="sass">
.lab_2
	&__canvas
		width: 600px
		height: 600px
		border: 1px solid #222
</style>
