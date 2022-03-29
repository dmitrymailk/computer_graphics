import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";

import { interpolate } from "./bspline.js";

class Lab_3_1 {
  private canvas: Canvas;
  public coords_3_1: Array<Vector3> = [];
  private vueComponent: any = null;
  private grid_points_x: Array<number> = [];
  private knots: Array<number> = [];

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
    // this.drawB_Spline_old();
    // if (this.coords_3_1.length > 2) this.drawB_Spline_3();
    // if (this.coords_3_1.length > 2) this.drawB_Spline_4();
    if (this.coords_3_1.length > 2) this.drawB_Spline_2();
  }
  private setPoints() {
    for (let coord of this.coords_3_1) {
      this.canvas.setPoint(coord.x, coord.y, 2);
    }
  }

  private debug_func() {
    const degree = 2;
    const len = 3 + degree + 1;
    this.knots = [];
    let knotNum = 0;
    for (let i = 0; i < len; i++) {
      // if (i < degree) this.knots.push(knotNum / len);
      // else if (i < len - degree - 1) {
      //   this.knots.push(knotNum / len);
      //   knotNum += 1;
      // } else {
      //   this.knots.push(knotNum / len);
      // }
      this.knots.push(i / degree);
    }
    console.log(this.knots);
    for (let u = 0.1; u < 1; u += 0.1) {
      let sum = 0;
      console.log("----------------------");
      for (let i = 0; i < 3; i++) {
        // const u = 0;
        const res = this.N(i, degree, u);
        console.log(`i=${i} deg=${degree} u=${u} N(i,deg,n) ${res}`);
        sum += res;
      }
      console.log(`SUM=${sum}`);
    }
  }

  drawB_Spline_4() {
    const degree = 2;
    const len = this.coords_3_1.length + degree + 1;
    this.knots = [];
    let knotNum = 0;
    for (let i = 0; i < len; i++) {
      // if (i < degree) this.knots.push(knotNum / len);
      // else if (i < len - degree - 1) {
      //   this.knots.push(knotNum / len);
      //   knotNum += 1;
      // } else {
      //   this.knots.push(knotNum / len);
      // }
      this.knots.push(i / degree);
    }
    console.log(this.knots);
    let points = [];
    for (let u = 0.1; u < 1; u += 0.1) {
      // let sum = 0;
      let x = 0;
      let y = 0;
      console.log("----------------------");
      for (let i = 0; i < 3; i++) {
        // const u = 0;
        const res = this.N(i, degree, u);
        // console.log(`i=${i} deg=${degree} u=${u} N(i,deg,n) ${res}`);
        // sum += res;
        x = this.coords_3_1[i].x + (1 - res) * this.coords_3_1[i].x;
        y = this.coords_3_1[i].y + (1 - res) * this.coords_3_1[i].y;
        points.push([x, y]);
      }
      // console.log(`SUM=${sum}`);
    }

    for (let i = 0; i < points.length - 1; i++) {
      const v1 = points[i];
      const v2 = points[i + 1];
      this.canvas.drawLine(v1[0], v1[1], v2[0], v2[1]);
    }
  }

  drawB_Spline_3() {
    const precision = 1 / 100;
    let points: Array<Array<number>> = [];
    let controlPoints = [];
    for (let i = 0; i < this.coords_3_1.length; i++) {
      controlPoints.push([this.coords_3_1[i].x, this.coords_3_1[i].y]);
    }
    const degree = 2;
    let knots = [];
    const len = this.coords_3_1.length + degree + 1;
    let knotNum = 0;
    for (let i = 0; i < len; i++) {
      // if (i < degree) knots.push(knotNum / degree);
      // else if (i < len - degree - 1) {
      //   knots.push(knotNum / degree);
      //   knotNum += 1;
      // } else {
      //   knots.push(knotNum / degree);
      // }
      knots.push(i / (len - 1));
    }

    this.knots = knots;

    // console.log(knots);
    let pointsAmount = this.coords_3_1.length;
    for (let u = precision; u < 1 - precision; u += precision) {
      let x = 0;
      let y = 0;
      for (let i = 0; i < pointsAmount; i++) {
        // console.log(this.N(i, pointsAmount, u) * this.coords_3_1[i].x);
        x += this.N(i, pointsAmount, u) * this.coords_3_1[i].x;
        y += this.N(i, pointsAmount, u) * this.coords_3_1[i].y;
      }

      points.push([x, y]);

      console.log([x, y]);
    }
    // console.log(points);

    for (let i = 0; i < points.length - 1; i++) {
      const v1 = points[i];
      const v2 = points[i + 1];
      this.canvas.drawLine(v1[0], v1[1], v2[0], v2[1]);
    }
  }

  debour(
    i: number,
    u: number,
    knots: Array<number>,
    controlPoints: Array<Array<number>>,
    degree: number,
    pos: number
  ) {
    `
    def deBoor(k: int, x: int, t, c, p: int):
    """Evaluates S(x).

    Arguments
    ---------
    k: Index of knot interval that contains x.
    x: Position.
    t: Array of knot positions, needs to be padded as described above.
    c: Array of control points.
    p: Degree of B-spline.
    """
    d = [c[j + k - p] for j in range(0, p + 1)]

    for r in range(1, p + 1):
        for j in range(p, r - 1, -1):
            alpha = (x - t[j + k - p]) / (t[j + 1 + k - r] - t[j + k - p])
            d[j] = (1.0 - alpha) * d[j - 1] + alpha * d[j]

    return d[p]
    `;
    let d: Array<number> = [];
    for (let j = 0; j < controlPoints.length; j++) {
      // debugger;
      d.push(controlPoints[j + i - degree][pos]);
    }
    for (let r = 1; r < degree + 1; r++) {
      for (let j = r - 1; j > degree; j--) {
        let alpha =
          (u - knots[j + i - degree]) /
          (knots[j + 1 + i - r] - knots[j + i - degree]);
        d[j] = (1.0 - alpha) * d[j - 1] + alpha * d[j];
      }
    }

    return d[degree];
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
      if (i < degree) knots.push(knotNum);
      else if (i < len - degree - 1) {
        knots.push(knotNum / degree);
        knotNum += 1;
      } else {
        knots.push(knotNum / degree);
      }
    }
    // console.log(knots);
    for (let i = 0; i < 1; i += 1 / precision) {
      // @ts-ignore
      let point = interpolate(i, degree, controlPoints, knots);
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

  drawB_Spline_old() {
    let points_x: Array<number> = [];
    let points_y: Array<number> = [];
    let grid_N = 100;

    for (let item of this.coords_3_1) {
      points_x.push(item.x);
      points_y.push(item.y);
    }

    let min_x = Math.min(...points_x);
    let max_x = Math.max(...points_x);
    let min_y = Math.min(...points_y);
    let max_y = Math.max(...points_y);

    let delta_grid_x: number = (max_x - min_x) / grid_N;
    let delta_grid_y: number = (max_x - min_x) / grid_N;
    let subgrid_N = 5;
    let delta_x = delta_grid_x / subgrid_N;
    let delta_y = delta_grid_y / subgrid_N;
    this.grid_points_x = [];

    for (let delta = min_x; delta < max_x; delta += delta_grid_x) {
      this.grid_points_x.push(delta);
    }
    for (let delta = min_y; delta < max_y; delta += delta_grid_x) {
      this.grid_points_x.push(delta);
    }

    for (let i = 0; i < this.grid_points_x.length - 1; i++) {
      for (
        let delta = this.grid_points_x[i];
        delta < this.grid_points_x[i + 1];
        delta += delta_x
      ) {}
    }
    // for (let i = 0; i < this.grid_points_y.length - 1; i++) {
    //   for (
    //     let delta = this.grid_points_x[i];
    //     delta < this.grid_points_x[i + 1];
    //     delta += delta_x
    //   ) {

    //   }
    // }
  }

  N(i: number, m: number, u: number) {
    // debugger;
    if (m == 0) {
      if (this.knots[i] <= u && u <= this.knots[i + 1]) return 1.0;
      return 0.0;
    } else {
      let part_1: number =
        this.div(u - this.knots[i], this.knots[i + m] - this.knots[i]) *
        this.N(i, m - 1, u);

      let part_2: number =
        this.div(
          this.knots[i + m + 1] - u,
          this.knots[i + m + 1] - this.knots[i + 1]
        ) * this.N(i + 1, m - 1, u);

      return part_1 + part_2;
    }
  }
  div(num_1: number, num_2: number) {
    // if (num_1 == 0 || num_2 == 0) return 0;
    return num_1 / num_2;
  }
}

export { Lab_3_1 };
