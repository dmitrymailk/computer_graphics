import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";

class Lab_2_2 {
  private canvas: Canvas;
  public coords_2_2: Array<Vector3> = [];
  private isDrawCurve: boolean = false;
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

      this.coords_2_2.push(new Vector3(x, y, 0));
      this.vueComponent.coords_2_2.push([x, y]);
    });
    this.startUpdate();
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
    if (this.coords_2_2.length > 0) this.displayCoords();
  }

  private displayCoords() {
    this.clearScreen();
    this.setPoints();
    this.drawCurve();
  }

  private setPoints() {
    for (let coord of this.coords_2_2) {
      this.canvas.setPoint(coord.x, coord.y, 1);
    }
  }

  public changeCoordsPos(pos: number, coordType: string, value: number) {
    // @ts-ignore
    this.coords_2_2[pos][coordType] = Number(value);
  }

  private drawCurve() {
    if (this.coords_2_2.length >= 3) {
      const points: Array<Vector3> = [];
      const delta = 0.01;
      for (let t = 0; t < 1.0001; t += delta) {
        let queue = this.coords_2_2.slice();
        // @ts-ignore
        const start: Vector3 = queue.shift();
        // @ts-ignore
        const end: Vector3 = queue.shift();
        queue.unshift(start);
        queue.push(end);

        while (queue.length != 1) {
          queue = this.dublicate(queue);
          for (let i = 0; i < queue.length; i++) {
            // @ts-ignore
            const p1: Vector3 = queue.shift();
            // @ts-ignore
            const p2: Vector3 = queue.shift();
            const p: Vector3 = Vector3.lepr(p1, p2, t);
            queue.push(p);
          }
        }
        points.push(queue[0]);
      }

      for (let i = 0; i < points.length - 1; i++) {
        const v1: Vector3 = points[i];
        const v2: Vector3 = points[i + 1];
        this.canvas.drawLine(v1.x, v1.y, v2.x, v2.y);
      }

      let queue = this.coords_2_2.slice();
      // @ts-ignore
      const start: Vector3 = queue.shift();
      // @ts-ignore
      const end: Vector3 = queue.shift();
      queue.unshift(start);
      queue.push(end);

      for (let i = 0; i < queue.length - 1; i++) {
        const v1: Vector3 = queue[i];
        const v2: Vector3 = queue[i + 1];
        this.canvas.drawLine(v1.x, v1.y, v2.x, v2.y);
      }
    }
  }

  private dublicate(coordsArray: Array<Vector3>) {
    let coords: Array<Vector3> = coordsArray.slice();
    let queue: Array<Vector3> = [];
    queue.push(coords.slice(0, 1)[0]);
    for (let i = 1; i < coords.length - 1; i++) {
      queue.push(coords.slice(i, i + 1)[0]);
      queue.push(coords.slice(i, i + 1)[0]);
    }
    queue.push(coords.slice(coords.length - 1)[0]);
    return queue;
  }

  public setDrawCurve() {
    console.log("draw curve");
    if (this.coords_2_2.length >= 3) {
      this.isDrawCurve = true;
      // @ts-ignore
      // const start: Vector3 = this.coords.shift();
      // // @ts-ignore
      // const end: Vector3 = this.coords.shift();
      // this.coords.unshift(start);
      // this.coords.push(end);
    }
  }

  clearScreen() {
    this.canvas.clear();
    // console.log("clear");
  }
}

export { Lab_2_2 };
