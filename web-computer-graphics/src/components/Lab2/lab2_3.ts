import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";
import { factorial } from "../Lab/utils";

class Lab_2_3 {
  private canvas: Canvas;
  public coords_2_3: Array<Vector3> = [];
  private vueComponent: any = null;
  private partsIsOn = true;

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

      this.coords_2_3.push(new Vector3(x, y, 0));
      this.vueComponent.coords_2_3.push([x, y]);
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
    if (this.coords_2_3.length > 0) this.displayCoords();
  }

  private displayCoords() {
    this.clearScreen();
    this.setPoints();
    this.drawCurve();
  }

  private setPoints() {
    for (let coord of this.coords_2_3) {
      this.canvas.setPoint(coord.x, coord.y, 3);
    }
  }

  public changeCoordsPos(pos: number, coordType: string, value: number) {
    // @ts-ignore
    this.coords_2_3[pos][coordType] = Number(value);
  }

  private drawCurve() {
    if (this.coords_2_3.length >= 3) {
      let points: Array<Vector3> = [];
      // compute point coordinate
      let delta = 0.01;

      // // @ts-ignore
      let controlPoints: Array<Vector3> = this.coords_2_3.slice();

      if (this.partsIsOn) {
        let newControlPoints: Array<Vector3> = [];
        const controlLen = controlPoints.length;
        for (let i = 0; i < controlLen; i++) {
          newControlPoints.push(controlPoints[i]);
          if (i < controlLen - 1) {
            let point_1: Vector3 = controlPoints[i];
            let point_2: Vector3 = controlPoints[i + 1];
            let sum: Vector3 = Vector3.add(point_1, point_2);
            sum = Vector3.mul(sum, 0.5);
            newControlPoints.push(sum);
          }
        }
        // debugger;
        controlPoints = newControlPoints.slice();
      }

      for (let i = 0; i < controlPoints.length; i++) {
        this.canvas.setPoint(controlPoints[i].x, controlPoints[i].y, 4);
      }

      const degree = 3 - 1;
      const degreeFac = factorial(degree);
      for (let j = 1; j < controlPoints.length - degree; j += degree) {
        for (let t = delta; t < 1; t += delta) {
          let point: Vector3 = new Vector3(0, 0, 0);
          // debugger;

          for (let i = 0; i <= degree; i++) {
            let vec: Vector3 = new Vector3(
              controlPoints[j + i].x,
              controlPoints[j + i].y,
              0
            );

            let factor = degreeFac / (factorial(i) * factorial(degree - i));
            let power = Math.pow(1 - t, degree - i);
            let power_t = Math.pow(t, i);
            vec = Vector3.mul(vec, factor * power * power_t);
            point.add(vec);
          }

          points.push(point);
        }
      }
      // points.push(end);

      this.canvas.drawLine(
        this.coords_2_3[0].x,
        this.coords_2_3[0].y,
        points[0].x,
        points[0].y
      );
      for (let i = 0; i < points.length - 1; i++) {
        const v1: Vector3 = points[i];
        this.canvas.setPoint(v1.x, v1.y);
        const v2: Vector3 = points[i + 1];
        this.canvas.drawLine(v1.x, v1.y, v2.x, v2.y);
      }
      this.canvas.drawLine(
        points[points.length - 1].x,
        points[points.length - 1].y,
        this.coords_2_3[this.coords_2_3.length - 1].x,
        this.coords_2_3[this.coords_2_3.length - 1].y
      );

      // for (let i = 0; i < this.coords_2_3.length - 1; i++) {
      //   const v1: Vector3 = this.coords_2_3[i];
      //   const v2: Vector3 = this.coords_2_3[i + 1];
      //   this.canvas.drawLine(v1.x, v1.y, v2.x, v2.y);
      // }
    }
  }

  clearScreen() {
    this.canvas.clear();
    // console.log("clear");
  }
}

export { Lab_2_3 };
