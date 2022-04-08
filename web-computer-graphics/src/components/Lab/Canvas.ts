import { Vector3 } from "./Vector3";

class Canvas {
  /** html dom canvas element */
  canvas: HTMLCanvasElement;
  ctx!: CanvasRenderingContext2D;
  width: number = 600;
  height: number = 600;
  private _color: string = "#000000";

  constructor(
    canvas: HTMLCanvasElement,
    width: number = 600,
    height: number = 600
  ) {
    this.canvas = canvas;
    this.width = width;
    this.height = height;

    this.setupCanvasElement();
    this.setupCtx();
  }

  private setupCanvasElement() {
    this.canvas.width = this.width;
    this.canvas.height = this.height;
  }

  private setupCtx() {
    const context = this.canvas.getContext("2d");
    if (context) {
      this.ctx = context;
      this.ctx.font = "18px serif";
    }
    this.ctx.imageSmoothingEnabled = false;
  }

  setPoint(x: number, y: number, size: number = 1) {
    this.ctx.fillRect(x, y, size, size);
  }

  drawLine(
    x_start: number,
    y_start: number,
    x_end: number,
    y_end: number
  ): void {
    const ctx = this.ctx;

    ctx.beginPath();
    ctx.moveTo(x_start, y_start);
    ctx.lineTo(x_end, y_end);
    ctx.stroke();
  }

  set color(color: string) {
    this._color = color;
    this.ctx.fillStyle = color;
  }

  get color(): string {
    return this._color;
  }

  clear() {
    this.ctx.clearRect(0, 0, this.width, this.height);
  }

  on(eventType: keyof HTMLElementEventMap, callback: any) {
    this.canvas.addEventListener(eventType, callback);
  }
}

export { Canvas };
