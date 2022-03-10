<template>
  <div class="lab_2">
    <div class="lab_2__title">
      <vue-markdown :source="src" />
    </div>
    <div class="lab_2__field">
      <canvas class="lab_2__canvas" ref="canvas_elem"></canvas>
    </div>
    <div class="lab_2__controls"></div>
  </div>
</template>

<script lang="ts">
import { ref } from "vue";

// import { Canvas } from "../Lab/Canvas";
import { Lab_2 } from "./lab2";
import VueMarkdown from "vue-markdown-render";

export default {
  data() {
    return {
      lab1: null,
    };
  },
  components: {
    VueMarkdown,
  },
  methods: {},
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
    this.lab1 = lab_2;
  },
};
</script>

<style lang="sass">
.lab_2
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
	&__range
		width: 300px
</style>
