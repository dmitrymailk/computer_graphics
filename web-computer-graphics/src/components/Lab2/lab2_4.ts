import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";
import { factorial } from "../Lab/utils";

class Lab_2_4 {
  private canvas: Canvas;
  public coords_2_4: Array<Vector3> = [];
  private vueComponent: any = null;
  private partsIsOn = true;
  private weights: Array<number> = [];

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

      this.coords_2_4.push(new Vector3(x, y, 0));
      this.vueComponent.coords_2_4.push([x, y]);
      this.weights.push(1);
    });
    this.startUpdate();
  }

  setPartsIsOn() {
    this.partsIsOn = !this.partsIsOn;
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
    if (this.coords_2_4.length > 0) this.displayCoords();
  }

  private displayCoords() {
    this.clearScreen();
    this.setPoints();
    this.drawCurve();
  }

  private setPoints() {
    for (let coord of this.coords_2_4) {
      this.canvas.setPoint(coord.x, coord.y, 3);
    }
  }

  B(t: number, i: number, degreeFac: number, degree: number) {
    let factor = degreeFac / (factorial(i) * factorial(degree - i));
    let power = Math.pow(1 - t, degree - i);
    let power_t = Math.pow(t, i);
    return factor * power * power_t;
  }

  Sigma(t: number, degreeFac: number, degree: number) {
    let totalSum: number = 0;
    for (let j = 0; j < this.coords_2_4.length; j++) {
      totalSum += this.B(t, j, degreeFac, degree) * this.weights[j];
    }
    return totalSum;
  }

  R(t: number, i: number, degreeFac: number, degree: number) {
    const B = this.B(t, i, degreeFac, degree) * this.weights[i];
    const Sigma = this.Sigma(t, degreeFac, degree);
    return B / Sigma;
  }

  private drawCurve() {
    if (this.coords_2_4.length >= 3) {
      let points: Array<Vector3> = [];
      // compute point coordinate
      let delta = 0.01;
      const degree = this.coords_2_4.length - 1;
      const degreeFac = factorial(degree);

      // @ts-ignore
      let queue = this.coords_2_4.slice();
      console.log(this.weights);
      for (let t = 0; t <= 1 + delta; t += delta) {
        let point: Vector3 = new Vector3(0, 0, 0);
        // debugger;
        for (let i = 0; i <= degree; i++) {
          let vec: Vector3 = new Vector3(queue[i].x, queue[i].y, 0);
          const R = this.R(t, i, degreeFac, degree);
          vec = Vector3.mul(vec, R);
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

      // for (let i = 0; i < queue.length - 1; i++) {
      //   const v1: Vector3 = queue[i];
      //   const v2: Vector3 = queue[i + 1];
      //   this.canvas.drawLine(v1.x, v1.y, v2.x, v2.y);
      // }
    }
  }
  changeWeights(pos: number, value: number) {
    this.weights[pos] = 1 + 10 * (Number(value) / 100);
  }

  clearScreen() {
    this.canvas.clear();
    // console.log("clear");
  }
}

export { Lab_2_4 };
