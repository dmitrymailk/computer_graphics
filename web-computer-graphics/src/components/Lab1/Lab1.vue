<template>
  <div class="lab_1">
    <div class="lab_1__title">Lab 1</div>
    <div class="lab_1__field">
      <canvas class="lab_1__canvas" ref="canvas_elem"></canvas>
    </div>
    <div class="lab1__controls">
      <button @click="clearScreen">Clear Screen</button>
      <button @click="saveFigure">Save Figure</button>
      <div>
        <input type="file" name="file" @change="loadFigure" />
        <label for="file">Load figure</label>
      </div>
      <div>
        <input
          type="range"
          name="scaleXYZ"
          min="1"
          max="100"
          @input="(e) => changeScale(e, 'X')"
        />
        <label for="scaleXYZ">scaleXYZ</label>
      </div>
      <div>
        <input
          type="range"
          name="scaleX"
          min="1"
          max="100"
          @input="(e) => changeScale(e, 'X')"
        />
        <label for="scaleX">scaleX</label>
      </div>
      <div>
        <input
          type="range"
          name="scaleY"
          min="1"
          max="100"
          @change="(e) => changeScale(e, 'Y')"
        />
        <label for="scaleY">scaleY</label>
      </div>
      <div>
        <input
          type="range"
          id="scaleZ"
          name="scaleZ"
          min="1"
          max="100"
          @change="(e) => changeScale(e, 'Z')"
        />
        <label for="scaleZ">scaleZ</label>
      </div>
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
      this.lab1.clearCoords();
    },
    changeScale(e, type) {
      console.log(e);
      const value = e.srcElement.value;
      this.lab1.changeScale(type, value);
    },
    saveFigure() {
      this.lab1.saveFigure();
    },
    loadFigure(e) {
      this.lab1.loadFigure(e);
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
