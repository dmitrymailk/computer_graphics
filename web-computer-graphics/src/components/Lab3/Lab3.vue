<template>
  <div class="container">
    <div class="p-2">
      <h3>Лабораторная №3</h3>
      3. Рисование сплайновых кривых
      <ul>
        <li>сплайновая кривая 2-го порядка (4)</li>
        <li>сплайновая кривая 3-го порядка (4)</li>
        <li>сплайновая кривая произвольного порядка (6)</li>
      </ul>
    </div>
    <div class="container">
      <h3>2.1 обычный алгоритм (4)</h3>
      <canvas class="lab_3__canvas" ref="canvas_elem_3_1"></canvas>
    </div>
  </div>
</template>

<script lang="ts">
import { ref } from "vue";

import { Canvas } from "../Lab/Canvas";
import { Lab_3_1 } from "./lab3_1";
import VueMarkdown from "vue-markdown-render";
import { defineComponent } from "vue";

export default defineComponent({
  data() {
    return {
      lab_3_1: null,
    };
  },
  components: {
    VueMarkdown,
  },
  methods: {},
  mounted() {
    // @ts-ignore
    const canvas_elem_3_1: HTMLCanvasElement = this.$refs.canvas_elem_3_1;
    const canvas_3_1 = new Canvas(canvas_elem_3_1);
    const lab_3_1 = new Lab_3_1(canvas_3_1, this);
    this.lab_3_1 = lab_3_1;
  },
});
</script>

<style lang="sass">
.lab_3
	&__canvas
		width: 600px
		height: 600px
		border: 1px solid #222
</style>
