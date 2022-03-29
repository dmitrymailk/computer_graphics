import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";

import { interpolate } from "./bspline.js";

class Lab_3_1 {
  private canvas: Canvas;
  public coords_3_1: Array<Vector3> = [];
  private vueComponent: any = null;
  private knots: Array<number> = [];
  private degree: number = 2;
  private precision: number = 1 / 100;

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
      this.vueComponent.pointsAmount += 1;
      // this.coords
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
    this.setPoints();
    if (this.coords_3_1.length > 2) this.drawB_Spline();
    // if (this.coords_3_1.length > 2) this.drawB_Spline_2();
  }
  private setPoints() {
    for (let coord of this.coords_3_1) {
      this.canvas.setPoint(coord.x, coord.y, 2);
    }
  }

  private debug_func() {
    this.drawB_Spline();
  }

  public setDegree(amount: number) {
    // console.log("setDegree", amount);
    if (amount <= 2) this.degree = 2;
    else if (amount < this.coords_3_1.length - 1) this.degree = amount;
    else this.degree = this.coords_3_1.length - 1;
    // debugger;
  }

  drawB_Spline() {
    const precision = this.precision;
    let points: Array<Array<number>> = [];
    let controlPoints = [];
    for (let i = 0; i < this.coords_3_1.length; i++) {
      controlPoints.push([this.coords_3_1[i].x, this.coords_3_1[i].y]);
    }
    const degree = this.degree;
    let knots = [];
    const n = this.coords_3_1.length;
    const len = n + degree + 1;
    for (let i = 0; i < len; i++) {
      if (i <= degree) knots.push(0);
      else if (i < len - degree - 1) {
        knots.push(i / len);
      } else {
        knots.push(1);
      }
    }
    // debugger;

    this.knots = knots;

    let pointsAmount = this.coords_3_1.length;
    for (let u = 0; u <= 1; u += precision) {
      let x = 0;
      let y = 0;
      for (let i = 0; i < pointsAmount; i++) {
        let res = this.N(i, degree, u);
        x += res * this.coords_3_1[i].x;
        y += res * this.coords_3_1[i].y;
      }
      points.push([x, y]);
      // console.log([x, y]);
    }

    for (let i = 0; i < points.length - 1; i++) {
      const v1 = points[i];
      const v2 = points[i + 1];
      this.canvas.drawLine(v1[0], v1[1], v2[0], v2[1]);
    }
    this.canvas.drawLine(
      points[points.length - 1][0],
      points[points.length - 1][1],
      this.coords_3_1[this.coords_3_1.length - 1].x,
      this.coords_3_1[this.coords_3_1.length - 1].y
    );
  }

  drawB_Spline_2() {
    const precision = 100;
    let points = [];
    let controlPoints = [];
    for (let i = 0; i < this.coords_3_1.length; i++) {
      controlPoints.push([this.coords_3_1[i].x, this.coords_3_1[i].y]);
    }
    const degree = 2;
    let knots = [];
    const len = this.coords_3_1.length + degree + 1;
    let knotNum = 0;
    for (let i = 0; i < len; i++) {
      if (i < degree - 1) knots.push(knotNum);
      else if (i < len - degree) {
        knots.push(knotNum / degree);
        knotNum = 1;
      } else {
        knots.push(knotNum / degree);
      }
    }
    // debugger;
    // console.log(knots);
    for (let i = 0; i < 1; i += 1 / precision) {
      // @ts-ignore
      let point = interpolate(i, degree, controlPoints, knots);
      console.log(point);
      points.push(point);
    }

    for (let i = 0; i < points.length - 1; i++) {
      const v1 = points[i];
      const v2 = points[i + 1];
      this.canvas.drawLine(v1[0], v1[1], v2[0], v2[1]);
    }
  }

  clearScreen() {
    this.canvas.clear();
    // console.log("clear");
  }

  N(i: number, m: number, u: number): number {
    if (m == 0) {
      if (this.knots[i] <= u && u <= this.knots[i + 1]) return 1;
      return 0;
    }
    let part_1: number;
    if (this.knots[i + m] == this.knots[i]) {
      if (u == this.knots[i]) part_1 = 0;
      else part_1 = 1;
    } else part_1 = (u - this.knots[i]) / (this.knots[i + m] - this.knots[i]);

    let part_2: number;
    if (this.knots[i + m + 1] == this.knots[i + 1]) {
      if (this.knots[i + m + 1] == u) part_2 = 0;
      else part_2 = 1;
    } else
      part_2 =
        (this.knots[i + m + 1] - u) /
        (this.knots[i + m + 1] - this.knots[i + 1]);

    return part_1 * this.N(i, m - 1, u) + part_2 * this.N(i + 1, m - 1, u);
  }
}

export { Lab_3_1 };
