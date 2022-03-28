import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";

import { interpolate } from "./bspline.js";

class Lab_3_1 {
  private canvas: Canvas;
  public coords_3_1: Array<Vector3> = [];
  private vueComponent: any = null;
  private grid_points_x: Array<number> = [];

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
    if (this.coords_3_1.length > 2) this.drawB_Spline_2();
  }
  private setPoints() {
    for (let coord of this.coords_3_1) {
      this.canvas.setPoint(coord.x, coord.y, 2);
    }
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
        knots.push(knotNum);
        knotNum += 1;
      } else {
        knots.push(knotNum);
      }
    }
    // console.log(knots);
    for (let i = 0; i < 1; i += 0.01) {
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

  N(i: number, m: number, x: number) {
    if (m == 0) {
      if (this.grid_points_x[i] < x && x < this.grid_points_x[i + 1]) return 1;
      return 0;
    } else {
      let part_1: number =
        ((x - this.grid_points_x[i]) /
          (this.grid_points_x[i + m] - this.grid_points_x[i])) *
        this.N(i, m - 1, x);

      let part_2: number =
        ((this.grid_points_x[i + m + 1] - x) /
          (this.grid_points_x[i + m + 1] - this.grid_points_x[i + 1])) *
        this.N(i + 1, m - 1, x);

      return part_1 + part_2;
    }
  }
}

export { Lab_3_1 };
