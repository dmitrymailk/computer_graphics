<template>
  <div class="container">
    <div class="p-2">
      <h3>Лабораторная №1</h3>
      2. Рисование кривой Безье
      <ul>
        <li>
          <a
            target="_blank"
            href="https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Explicit_definition"
            >2.1 обычный алгоритм (4)</a
          >
        </li>
        <li>
          <a
            href="https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Quadratic_B%C3%A9zier_curves"
            target="_blank"
            >2.2 алгоритм де Кастельо (4)</a
          >
        </li>
        <li>2.3 составная кривая, рациональная кривая Безье (6)</li>
      </ul>
    </div>
    <div class="container">
      <h3>2.1 обычный алгоритм (4)</h3>
      <canvas class="lab_2__canvas" ref="canvas_elem_2_1"></canvas>
      <br />
      <div class="card mt-2" v-for="(item, index) in coords">
        <div class="card-body">
          <h5 class="card-title">Point {{ index }}</h5>
          <label for="x" class="form-label">x</label>
          <input
            type="range"
            name="x"
            min="1"
            max="600"
            @input="(e) => changeCoordsPos_2_1(e, 'x', index)"
            class="form-range"
          />
          <label for="y" class="form-label">y</label>
          <input
            type="range"
            name="y"
            min="1"
            max="600"
            @input="(e) => changeCoordsPos_2_1(e, 'y', index)"
            class="form-range"
          />
        </div>
      </div>
      <hr />
    </div>
    <div class="container">
      <h3>2.2 алгоритм де Кастельо (4)</h3>
      <canvas class="lab_2__canvas" ref="canvas_elem_2_2"></canvas>
      <br />
      <div class="card mt-2" v-for="(item, index) in coords">
        <div class="card-body">
          <h5 class="card-title">Point {{ index }}</h5>
          <label for="x" class="form-label">x</label>
          <input
            type="range"
            name="x"
            min="1"
            max="600"
            @input="(e) => changeCoordsPos_2_2(e, 'x', index)"
            class="form-range"
          />
          <label for="y" class="form-label">y</label>
          <input
            type="range"
            name="y"
            min="1"
            max="600"
            @input="(e) => changeCoordsPos_2_2(e, 'y', index)"
            class="form-range"
          />
        </div>
      </div>
      <hr />
    </div>
  </div>
</template>

<script lang="ts">
import { ref } from "vue";

import { Canvas } from "../Lab/Canvas";
import { Lab_2_2 } from "./lab2_2";
import { Lab_2_1 } from "./lab2_1";
import VueMarkdown from "vue-markdown-render";
import { defineComponent } from "vue";

export default defineComponent({
  data() {
    return {
      lab_2_1: null,
      coords_2_1: [],
      lab_2_2: null,
      coords_2_2: [],
    };
  },
  components: {
    VueMarkdown,
  },
  methods: {
    setDrawCurve() {
      this.lab.setDrawCurve();
    },
    changeCoordsPos_2_2(e, typePos, pos) {
      let value = e.srcElement.value;
      this.lab_2_2.changeCoordsPos(pos, typePos, value);
    },
    changeCoordsPos_2_1(e, typePos, pos) {
      let value = e.srcElement.value;
      this.lab_2_1.changeCoordsPos(pos, typePos, value);
    },
  },
  mounted() {
    // @ts-ignore
    const canvas_elem_2_2: HTMLCanvasElement = this.$refs.canvas_elem_2_2;
    const canvas_2_2 = new Canvas(canvas_elem_2_2);
    const lab_2_2 = new Lab_2_2(canvas_2_2, this);
    this.lab_2_2 = lab_2_2;

    const canvas_elem_2_1: HTMLCanvasElement = this.$refs.canvas_elem_2_1;
    const canvas_2_1 = new Canvas(canvas_elem_2_1);
    const lab_2_1 = new Lab_2_1(canvas_2_1, this);
    this.lab_2_1 = lab_2_1;
  },
});
</script>

<style lang="sass">
.lab_2
	&__canvas
		width: 600px
		height: 600px
		border: 1px solid #222
</style>
