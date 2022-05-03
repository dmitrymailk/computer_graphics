<template>
  <div class="container">
    <div class="p-2">
      <h3>Лабораторная №6</h3>
      <h4>6. Удаление невидимых граней и линий.</h4>
      <ul>
        <li>а) отсечение нелицевых граней многогранника (5)</li>
      </ul>
    </div>
    <h1>а) отсечение нелицевых граней многогранника (5)</h1>
    <div class="lab_6__field">
      <canvas class="lab_6__canvas" ref="canvas_elem"></canvas>
    </div>
  </div>
</template>

<script lang="ts">
import { ref } from "vue";

import { Canvas } from "../Lab/Canvas";
import { Lab_6 } from "./lab6";

export default {
  data() {
    return {
      lab6: null,
    };
  },
  components: {},

  mounted() {
    // @ts-ignore
    const canvas_elem: HTMLCanvasElement = this.$refs.canvas_elem;
    const canvas = new Canvas(canvas_elem, 600, 600);
    const lab_6 = new Lab_6(canvas);
    this.lab6 = lab_6;
  },
};
</script>

<style lang="sass">
.lab_6
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
