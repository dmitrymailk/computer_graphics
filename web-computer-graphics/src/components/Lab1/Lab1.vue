<template>
  <div class="lab_1">
    <div class="lab_1__title">Lab 1</div>
    <div class="lab_1__field">
      <canvas class="lab_1__canvas" ref="canvas_elem"></canvas>
    </div>
    <div class="lab1__controls">
      <button @click="clearScreen">Clear Screen</button>
    </div>
  </div>
</template>

<script lang="ts">
import { ref } from "vue";

import { Canvas } from "./Canvas";
import { Lab_1 } from "./lab1";

export default {
  data() {
    return {
      test: 123,
      lab1: null,
    };
  },

  methods: {
    clearScreen() {
      this.lab1.clearScreen();
    },
  },
  mounted() {
    // @ts-ignore
    const canvas_elem: HTMLCanvasElement = this.$refs.canvas_elem;
    const canvas = new Canvas(canvas_elem);
    const lab_1 = new Lab_1(canvas);
    this.lab1 = lab_1;
  },
};
</script>

<style lang="sass">
.lab_1
	// width: 100%
	margin: 0 auto
	max-width: 860px
	&__field
		width: 100%
		display: flex
	&__canvas
		margin: 0 auto
		width: 600px
		height: 600px
		border: 1px solid #222
</style>
