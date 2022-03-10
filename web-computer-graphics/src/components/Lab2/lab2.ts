import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";

class Lab_2 {
  private canvas: Canvas;
  private coords: Array<Vector3> = [];
  private isDrawCurve: boolean = false;

  constructor(drawInstance: Canvas) {
    this.canvas = drawInstance;

    this.init();
  }

  private startUpdate() {
    window.requestAnimationFrame(this.update.bind(this));
  }

  init() {
    this.canvas.on("click", (e: PointerEvent) => {
      // console.log(e.offsetX, e.offsetY);
      const x = e.offsetX;
      const y = e.offsetY;

      this.coords.push(new Vector3(x, y, 0));
    });
    this.startUpdate();
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
    if (this.coords.length > 0) this.displayCoords();
  }

  private displayCoords() {
    // this.clearScreen();
    this.setPoints();
    this.drawCurve();
  }

  private setPoints() {
    for (let coord of this.coords) {
      this.canvas.setPoint(coord.x, coord.y, 3);
    }
  }

  private drawCurve() {
    if (this.isDrawCurve) {
      for (let t = 0; t < 1.0001; t += 0.001) {
        let queue = this.coords.slice();
        const power = 1;
        // debugger;
        while (queue.length != power) {
          queue = this.dublicate(queue);
          for (let i = 0; i < queue.length; i++) {
            // @ts-ignore
            const p1: Vector3 = queue.shift();
            // @ts-ignore
            const p2: Vector3 = queue.shift();
            // const p3: Vector3 = queue.pop();
            const p_1_2: Vector3 = Vector3.lepr(p1, p2, t);
            queue.push(p_1_2);
          }
        }

        for (let coord of queue) {
          this.canvas.setPoint(coord.x, coord.y, 1);
        }
      }

      this.isDrawCurve = false;
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
    if (this.coords.length >= 3) {
      this.isDrawCurve = true;
      // @ts-ignore
      const start: Vector3 = this.coords.shift();
      // @ts-ignore
      const end: Vector3 = this.coords.shift();
      this.coords.unshift(start);
      this.coords.push(end);
    }
  }

  clearScreen() {
    this.canvas.clear();
  }
}

export { Lab_2 };
