import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";

class Lab_2 {
  private canvas: Canvas;

  constructor(drawInstance: Canvas) {
    this.canvas = drawInstance;

    this.init();
  }

  private startUpdate() {
    window.requestAnimationFrame(this.update.bind(this));
  }

  init() {
    this.startUpdate();
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
  }

  clearScreen() {
    this.canvas.clear();
  }
}

export { Lab_2 };
