import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";

class Lab_3_1 {
  private canvas: Canvas;
  public coords_3_1: Array<Vector3> = [];
  private vueComponent: any = null;

  constructor(drawInstance: Canvas, vueComponent: any) {
    this.canvas = drawInstance;
    this.vueComponent = vueComponent;
    this.init();
  }

  private startUpdate() {
    window.requestAnimationFrame(this.update.bind(this));
  }

  init() {
    // console.log("vueComponent", this.vueComponent?.$data);
    this.canvas.on("click", (e: PointerEvent) => {
      // console.log(e.offsetX, e.offsetY);
      const x = e.offsetX;
      const y = e.offsetY;

      this.coords_3_1.push(new Vector3(x, y, 0));
    });
    this.startUpdate();
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
    if (this.coords_3_1.length > 0) this.displayCoords();
  }

  private displayCoords() {
    this.clearScreen();
  }

  clearScreen() {
    this.canvas.clear();
    // console.log("clear");
  }
}

export { Lab_3_1 };
