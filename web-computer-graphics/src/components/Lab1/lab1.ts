class Canvas {
  canvas: HTMLCanvasElement;
  ctx!: CanvasRenderingContext2D;
  width: number = 600;
  height: number = 600;

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
    this.sandbox();
  }

  setupCanvasElement() {
    this.canvas.width = this.width;
    this.canvas.height = this.height;
  }

  setupCtx() {
    const context = this.canvas.getContext("2d");
    if (context) this.ctx = context;
    this.ctx.imageSmoothingEnabled = false;
  }

  setPoint(x: number, y: number, size: number = 1) {
    this.ctx.fillRect(x, y, size, size);
  }

  drawLine(x_start: number, y_start: number, x_end: number, y_end: number) {
    const ctx = this.ctx;

    ctx.beginPath();
    ctx.moveTo(x_start, y_start);
    ctx.lineTo(x_end, y_end);
    ctx.stroke();
  }

  sandbox() {
    this.setPoint(0, 0);
    this.drawLine(0, 0, this.width, this.height);
  }
}

class Lab_1 {
  canvas: Canvas;

  constructor(drawInstance: Canvas) {
    this.canvas = drawInstance;
  }

  // тут переписать 6 лабу с с#
}

export { Canvas, Lab_1 };
