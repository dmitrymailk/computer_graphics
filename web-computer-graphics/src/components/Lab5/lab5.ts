import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";
// @ts-ignore
import Papa from "papaparse";

import { generateRandomFloatInRange, download, copyCoords } from "../Lab/utils";

class Lab_5 {
  private canvas: Canvas;

  private originalCoords: Array<Vector3> = [];
  private projectedCoords: Array<Vector3> = [];

  // параметры поворота для всех осей
  private angleZ: number = 0;
  private angleX: number = 0;
  private angleY: number = 0;

  // параметры скейла для всех осей
  private scaleX: number = 200;
  private scaleY: number = 200;
  private scaleZ: number = 200;
  private maxScale: number = 600;

  // матрица проекции
  private projection: Array<Vector3> = [];

  // матрицы поворота
  private rotationZ: Array<Vector3> = [];
  private rotationX: Array<Vector3> = [];
  private rotationY: Array<Vector3> = [];

  // вектор переноса
  private translation: Vector3 = new Vector3();

  // параметр переноса
  private translationX: number = 0;
  private translationY: number = 0;
  private translationZ: number = 0;

  // параметры поверхности
  // private surfaceWidth: number = 5;
  // private surfaceHeight: number = 5;
  private surfaceWidth: number = 2 * Math.PI;
  private surfaceHeight: number = 2 * Math.PI;
  private surfaceStepX = 0.2;
  private surfaceStepY = 0.2;
  private surfaceInitialShiftX = 0;
  private surfaceInitialShiftY = 0;

  private gridCoords: Array<Array<Vector3>> = [];

  constructor(drawInstance: Canvas) {
    this.canvas = drawInstance;
    this.init();
  }

  private addCoords(data: Array<Array<string>>) {
    for (let i = 1; i < data.length; i++) {
      this.originalCoords.push(
        new Vector3(Number(data[i][0]), Number(data[i][1]), Number(data[i][2]))
      );
    }
  }

  private startUpdate() {
    window.requestAnimationFrame(this.update.bind(this));
  }

  init(data?: Array<Array<string>>) {
    const color: string = "#000000";
    this.canvas.color = color;

    this.originalCoords = [];
    this.projectedCoords = [];
    if (data) {
      // задаем координаты объекта в трехмерном пространстве
      this.addCoords(data);
      // add events

      this.startUpdate();
    } else {
      this.addCoordsManually();
    }
    window.addEventListener("keydown", (e: KeyboardEvent) => {
      this.keyboardEvent(e);
    });

    this.startUpdate();
  }

  private addCoordsManually() {
    // добавляем координаты в originalCoords
    console.log("add");
    for (
      let i = 0;
      i < Math.ceil(this.surfaceHeight / this.surfaceStepY);
      i++
    ) {
      this.gridCoords.push([]);
    }

    for (
      let x = this.surfaceInitialShiftX, i = 0;
      x < this.surfaceWidth;
      x += this.surfaceStepX, i += 1
    ) {
      for (
        let y = this.surfaceInitialShiftY, j = 0;
        y < this.surfaceHeight;
        y += this.surfaceStepY, j += 1
      ) {
        const _x = this.fx(x, y);
        const _y = this.fy(x, y);
        const _z = this.fz(x, y);
        const surfacePoint = new Vector3(_x, _y, _z);
        this.originalCoords.push(surfacePoint);

        this.gridCoords[j].push(surfacePoint);
      }
    }
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
    if (this.originalCoords.length > 0) this.displayCoords2();
  }

  private surfaceFunction(x: number, y: number): number {
    return Math.sin(2 * y) + Math.sin(2 * x);
    // return Math.pow(x / (x + y + 1) + 1, 2) + Math.pow(y / (x + y + 1) + 1, 2);
  }

  // RENDER TORUS OBJECT https://en.wikipedia.org/wiki/Torus
  private fx(u: number, v: number): number {
    const R = 0.5;
    const r = 0.4;
    return (R + r * Math.cos(v)) * Math.cos(u);
  }
  private fy(u: number, v: number): number {
    const R = 0.5;
    const r = 0.4;
    return (R + r * Math.cos(v)) * Math.sin(u);
  }
  private fz(u: number, v: number) {
    const R = 0.5;
    const r = 0.4;
    return r * Math.sin(v);
  }

  // RENDER  another object https://docs.exponenta.ru/symbolic/ezsurf.html
  // private fx(u: number, v: number): number {
  //   const r = 2 + Math.sin(7 * u + 5 * v);
  //   return r * Math.cos(u) * Math.sin(v);
  // }
  // private fy(u: number, v: number): number {
  //   const r = 2 + Math.sin(7 * u + 5 * v);
  //   return r * Math.sin(u) * Math.sin(v);
  // }
  // private fz(u: number, v: number) {
  //   const r = 2 + Math.sin(7 * u + 5 * v);
  //   return r * Math.cos(v);
  // }

  // RENDER cos sin surface
  // private fx(u: number, v: number): number {
  //   return u;
  // }
  // private fy(u: number, v: number): number {
  //   return v;
  // }
  // private fz(u: number, v: number) {
  //   return Math.cos(2 * u) + Math.sin(2 * v);
  // }

  displayCoords() {
    this.clearScreen();
    this.updateRotation();
    this.updateProjection();

    this.projectedCoords = [];
    let centerVec = new Vector3(0, 0, 0);

    for (let i = 0; i < this.originalCoords.length; i++) {
      let vec: Vector3 = this.originalCoords[i];
      centerVec.add(vec);
    }
    // центр по всем проекциям
    centerVec = Vector3.mul(centerVec, 1 / this.originalCoords.length);

    // трансформация всех координат куба
    let i = 0;
    for (let coord of this.originalCoords) {
      let proj2D = new Vector3(coord.x, coord.y, coord.z);

      //   перенос пространства
      let trans = new Vector3(
        this.translationX,
        this.translationY,
        this.translationZ
      );

      //   перенос объекта
      let transObject = new Vector3(
        this.translationX,
        this.translationY,
        this.translationZ
      );

      // центрируем координаты фигуры
      proj2D = Vector3.add(proj2D, Vector3.mul(centerVec, -1));

      proj2D = Vector3.matMul(this.rotationX, proj2D);
      proj2D = Vector3.matMul(this.rotationZ, proj2D);
      proj2D = Vector3.matMul(this.rotationY, proj2D);

      proj2D = Vector3.add(proj2D, centerVec);

      // переводим кординаты в размерность экрана, потому что раньше они были от -1 до 1
      proj2D = Vector3.matMul(this.projection, proj2D);
      // перенос объекта
      proj2D = Vector3.add(proj2D, transObject);

      this.projectedCoords.push(proj2D);

      let convertedCoords: Vector3 = this.convertCoords(proj2D.x, proj2D.y);
      // this.canvas.ctx.fillText(`${i}`, convertedCoords.x, convertedCoords.y);
      // console.log(convertedCoords);
      this.canvas.setPoint(convertedCoords.x, convertedCoords.y);
      i += 1;
    }

    const height = Math.floor(this.surfaceHeight / this.surfaceStepY);
    const width = Math.floor(this.surfaceWidth / this.surfaceStepX) - 1;
    for (let xi = 0; xi < width; xi++) {
      for (let yi = 0; yi < height; yi++) {
        // this.connectProjectedDots(
        //   height * xi + xi + yi,
        //   height * xi + yi + (yi + 1)
        // );
        // this.connectProjectedDots(yi, yi + 1);
      }
    }
  }

  displayCoords2() {
    this.clearScreen();
    this.updateRotation();
    this.updateProjection();

    let centerVec = new Vector3(0, 0, 0);

    for (let i = 0; i < this.originalCoords.length; i++) {
      let vec: Vector3 = this.originalCoords[i];
      centerVec.add(vec);
    }
    // центр по всем проекциям
    centerVec = Vector3.mul(centerVec, 1 / this.originalCoords.length);

    // трансформация всех координат куба
    let i = 0;
    let projectedGridCoords: Array<Array<Vector3>> = [];
    for (let i = 0; i < this.gridCoords.length; i++)
      projectedGridCoords.push([]);

    for (let i = 0; i < this.gridCoords.length; i++)
      for (let j = 0; j < this.gridCoords[0].length; j++) {
        let coord = JSON.parse(JSON.stringify(this.gridCoords[i][j]));
        let proj2D = new Vector3(coord.x, coord.y, coord.z);

        //   перенос пространства
        let trans = new Vector3(
          this.translationX,
          this.translationY,
          this.translationZ
        );

        //   перенос объекта
        let transObject = new Vector3(
          this.translationX,
          this.translationY,
          this.translationZ
        );

        // центрируем координаты фигуры
        proj2D = Vector3.add(proj2D, Vector3.mul(centerVec, -1));

        proj2D = Vector3.matMul(this.rotationX, proj2D);
        proj2D = Vector3.matMul(this.rotationZ, proj2D);
        proj2D = Vector3.matMul(this.rotationY, proj2D);

        proj2D = Vector3.add(proj2D, centerVec);

        // переводим кординаты в размерность экрана, потому что раньше они были от -1 до 1
        proj2D = Vector3.matMul(this.projection, proj2D);
        // перенос объекта
        proj2D = Vector3.add(proj2D, transObject);

        this.projectedCoords.push(proj2D);

        let convertedCoords: Vector3 = this.convertCoords(proj2D.x, proj2D.y);

        this.canvas.setPoint(convertedCoords.x, convertedCoords.y);
        projectedGridCoords[i].push(convertedCoords);
      }

    const relu = (x: number, max: number): number => (x >= max ? 0 : x);

    for (let i = 0; i < this.gridCoords.length; i++)
      for (let j = 0; j < this.gridCoords[0].length; j++) {
        // @ts-ignore
        let start: Vector3 = projectedGridCoords[i][j];
        let end: Vector3 =
          projectedGridCoords[relu(i + 1, this.gridCoords.length)][j];

        // start = this.convertCoords(start.x, start.y);
        // end = this.convertCoords(end.x, end.y);

        this.canvas.drawLine(start.x, start.y, end.x, end.y);

        start = projectedGridCoords[i][j];
        end = projectedGridCoords[i][relu(j + 1, this.gridCoords[0].length)];
        this.canvas.drawLine(start.x, start.y, end.x, end.y);
      }

    // @ts-ignore
    projectedGridCoords = null;
  }

  convertCoords(x: number, y: number): Vector3 {
    x += this.canvas.width / 2; //- (this.surfaceWidth * this.scaleX) / 2;
    y += this.canvas.height / 2; //- (this.surfaceHeight * this.scaleY) / 2;
    return new Vector3(x, y, 0);
  }

  // поворачиваем наш куб по всем трем направлениям
  private updateRotation() {
    const angleX = this.angleX;
    const angleY = this.angleY;
    const angleZ = this.angleZ;

    this.rotationZ = [];
    this.rotationZ.push(new Vector3(Math.cos(angleZ), -Math.sin(angleZ), 0));
    this.rotationZ.push(new Vector3(Math.sin(angleZ), Math.cos(angleZ), 0));
    this.rotationZ.push(new Vector3(0, 0, 1));

    this.rotationX = [];
    this.rotationX.push(new Vector3(1, 0, 0));
    this.rotationX.push(new Vector3(0, Math.cos(angleX), -Math.sin(angleX)));
    this.rotationX.push(new Vector3(0, Math.sin(angleX), Math.cos(angleX)));

    this.rotationY = [];
    this.rotationY.push(new Vector3(Math.cos(angleY), 0, Math.sin(angleY)));
    this.rotationY.push(new Vector3(0, 1, 0));
    this.rotationY.push(new Vector3(-Math.sin(angleY), 0, Math.cos(angleY)));
  }

  private updateProjection() {
    // обновляем матрицу проекции
    const scaleX = this.scaleX;
    const scaleY = this.scaleY;
    const scaleZ = this.scaleZ;

    this.projection = [];

    this.projection.push(Vector3.mul(new Vector3(1.0, 0.0, 0.0), scaleX));
    this.projection.push(Vector3.mul(new Vector3(0.0, 1.0, 0.0), scaleY));
    this.projection.push(Vector3.mul(new Vector3(0.0, 0.0, 1.0), scaleZ));
  }

  private keyboardEvent(e: KeyboardEvent) {
    const key = e.code;

    switch (key) {
      case "KeyZ": {
        this.angleZ += 0.1;
        console.log("rotate z");
        break;
      }
      case "KeyX": {
        this.angleX += 0.1;
        console.log("rotate x");
        break;
      }
      case "KeyY": {
        this.angleY += 0.1;
        break;
      }
      case "KeyQ": {
        this.translationX += 10;
        break;
      }
      case "KeyW": {
        this.translationY += 10;
        break;
      }
      case "KeyE": {
        this.translationZ += 10;
        break;
      }
      case "KeyA": {
        this.scaleX += 1;
        break;
      }
      case "KeyS": {
        this.scaleY += 1;
        break;
      }
      case "KeyD": {
        this.scaleZ += 1;
        break;
      }
    }
  }

  clearScreen() {
    this.canvas.clear();
  }

  clearCoords() {
    this.originalCoords = [];
    this.angleX = 0;
    this.angleY = 0;
    this.angleZ = 0;
    this.translationX = 0;
    this.translationY = 0;
    this.translationZ = 0;
    this.scaleX = this.canvas.width;
    this.scaleY = this.canvas.width;
    this.scaleZ = this.canvas.width;

    this.clearScreen();
  }

  private connectProjectedDots(startNum: number, endNum: number) {
    let start: Vector3 = this.projectedCoords[startNum];
    let end: Vector3 = this.projectedCoords[endNum];

    start = this.convertCoords(start.x, start.y);
    end = this.convertCoords(end.x, end.y);

    this.canvas.drawLine(start.x, start.y, end.x, end.y);
  }

  changeScale(type: string, value: number) {
    switch (type) {
      case "X": {
        this.scaleX = this.maxScale * (value / 100);
        break;
      }
      case "Y": {
        this.scaleY = this.maxScale * (value / 100);
        break;
      }
      case "Z": {
        this.scaleZ = this.maxScale * (value / 100);
        break;
      }
      case "XYZ": {
        this.scaleX = this.maxScale * (value / 100);
        this.scaleY = this.maxScale * (value / 100);
        this.scaleZ = this.maxScale * (value / 100);
      }
    }
  }

  saveFigure() {
    let csv = Papa.unparse(this.originalCoords);
    download(csv, "figure.csv", "text/csv");
  }
  loadFigure(e: Event) {
    // @ts-ignore
    const file = e.target?.files[0];
    const reader = new FileReader();
    reader.addEventListener("load", (e) => {
      // @ts-ignore
      const result = e.currentTarget.result;

      let parsedCSV = Papa.parse(result);
      parsedCSV = parsedCSV.data;
      this.addCoords(parsedCSV);
    });

    reader.readAsText(file);
  }
}

export { Lab_5 };
