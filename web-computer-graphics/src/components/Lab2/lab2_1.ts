import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";
import { factorial } from "../Lab/utils";

class Lab_2_1 {
  private canvas: Canvas;
  public coords_2_1: Array<Vector3> = [];
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

      this.coords_2_1.push(new Vector3(x, y, 0));
      this.vueComponent.coords_2_1.push([x, y]);
    });
    this.startUpdate();
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
    if (this.coords_2_1.length > 0) this.displayCoords();
  }

  private displayCoords() {
    this.clearScreen();
    this.setPoints();
    this.drawCurve();
  }

  private setPoints() {
    for (let coord of this.coords_2_1) {
      this.canvas.setPoint(coord.x, coord.y, 3);
    }
  }

  public changeCoordsPos(pos: number, coordType: string, value: number) {
    // @ts-ignore
    this.coords_2_1[pos][coordType] = Number(value);
  }

  private drawCurve() {
    if (this.coords_2_1.length >= 3) {
      let points: Array<Vector3> = [];
      // compute point coordinate
      let delta = 0.01;
      const degree = this.coords_2_1.length - 1;
      const degreeFac = factorial(degree);

      // @ts-ignore
      let queue = this.coords_2_1.slice();
      // @ts-ignore
      const start: Vector3 = queue.shift();
      // @ts-ignore
      const end: Vector3 = queue.shift();
      queue.unshift(start);
      queue.push(end);

      for (let t = delta; t < 1; t += delta) {
        let point: Vector3 = new Vector3(0, 0, 0);
        // debugger;
        for (let i = 0; i <= degree; i++) {
          let vec: Vector3 = new Vector3(queue[i].x, queue[i].y, 0);
          let factor = degreeFac / (factorial(i) * factorial(degree - i));
          let power = Math.pow(1 - t, degree - i);
          let power_t = Math.pow(t, i);
          vec = Vector3.mul(vec, factor * power * power_t);
          point.add(vec);
        }

        points.push(point);
      }
      // points.push(end);

      for (let i = 0; i < points.length - 1; i++) {
        const v1: Vector3 = points[i];
        const v2: Vector3 = points[i + 1];
        this.canvas.drawLine(v1.x, v1.y, v2.x, v2.y);
      }

      for (let i = 0; i < queue.length - 1; i++) {
        const v1: Vector3 = queue[i];
        const v2: Vector3 = queue[i + 1];
        this.canvas.drawLine(v1.x, v1.y, v2.x, v2.y);
      }
    }
  }

  clearScreen() {
    this.canvas.clear();
    // console.log("clear");
  }
}

export { Lab_2_1 };
