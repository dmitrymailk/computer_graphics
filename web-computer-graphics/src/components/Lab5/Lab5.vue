<template>
  <div class="container">
    <div class="p-2">
      <h3>Лабораторная №5</h3>
      <h4>Рисование поверхности в пространстве.</h4>
      <ul>
        <li>а) поверхность, заданная формулой. (6)</li>
        <li>б) поверхность Безье (8)</li>
        <li>в) сплайновая поверхность (8)</li>
      </ul>
    </div>
    <h1>а) поверхность, заданная формулой. (6)</h1>
    <div class="lab_5__field">
      <canvas class="lab_5__canvas" ref="canvas_elem"></canvas>
    </div>
    <h1>б) поверхность Безье (8)</h1>
    <div class="lab_5__field">
      <canvas class="lab_5__canvas" ref="canvas_elem_2"></canvas>
    </div>
    <h1>б) сплайновая поверхность (8)</h1>
    <div class="lab_5__field">
      <canvas class="lab_5__canvas" ref="canvas_elem_3"></canvas>
    </div>
  </div>
</template>

<script lang="ts">
import { ref } from "vue";

import { Canvas } from "../Lab/Canvas";
import { Lab_5 } from "./lab5";
import { Lab_5_2 } from "./lab5_2";
import { Lab_5_3 } from "./lab5_3";

export default {
  data() {
    return {
      lab5: null,
      lab5_2: null,
      lab5_3: null,
    };
  },
  components: {},
  methods: {
    clearScreen() {
      this.lab5.clearCoords();
    },
    changeScale(e, type) {
      console.log(e);
      const value = e.srcElement.value;
      this.lab5.changeScale(type, value);
    },
    saveFigure() {
      this.lab5.saveFigure();
    },
    loadFigure(e) {
      this.lab5.loadFigure(e);
    },
  },

  mounted() {
    // @ts-ignore
    const canvas_elem: HTMLCanvasElement = this.$refs.canvas_elem;
    const canvas = new Canvas(canvas_elem, 600, 600);
    const lab_5 = new Lab_5(canvas);
    this.lab5 = lab_5;

    const canvas_elem_2: HTMLCanvasElement = this.$refs.canvas_elem_2;
    const canvas_2 = new Canvas(canvas_elem_2, 600, 600);
    const lab_5_2 = new Lab_5_2(canvas_2);
    this.lab5_2 = lab_5_2;

    const canvas_elem_3: HTMLCanvasElement = this.$refs.canvas_elem_3;
    const canvas_3 = new Canvas(canvas_elem_3, 600, 600);
    const lab_5_3 = new Lab_5_3(canvas_3);
    this.lab5_3 = lab_5_3;
  },
};
</script>

<style lang="sass">
.lab_5
	// width: 100%
	margin: 0 auto
	max-width: 860px
	&__field
		width: 100%
		display: flex
	&__canvas
		margin: 0 auto
		width: 800px
		height: 800px
		border: 1px solid #222
	&__range
		width: 300px
</style>
