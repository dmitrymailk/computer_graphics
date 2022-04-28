import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";
// @ts-ignore
import Papa from "papaparse";

import {
  generateRandomFloatInRange,
  download,
  copyCoords,
  factorial,
} from "../Lab/utils";

class Lab_5_2 {
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
  private surfaceWidth: number = 1.6;
  private surfaceHeight: number = 1.6;
  private surfaceStepX: number = 0.4;
  private surfaceStepY: number = 0.4;
  private surfaceInitialShiftX = 0;
  private surfaceInitialShiftY = 0;

  private gridCoords: Array<Array<Vector3>> = [];
  private gridSurfaceCoords: Array<Array<Vector3>> = [];

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
    // GENERATE INITIAL CONTROL POINTS
    for (
      let i = 0;
      i < Math.ceil(this.surfaceHeight / this.surfaceStepY);
      i++
    ) {
      this.gridCoords.push([]);
    }
    // console.log(this.gridCoords);
    const precision = 0.0001;

    for (
      let x = this.surfaceInitialShiftX, i = 0;
      x < this.surfaceWidth - precision;
      x += this.surfaceStepX, i += 1
    ) {
      for (
        let y = this.surfaceInitialShiftY, j = 0;
        y < this.surfaceHeight - precision;
        y += this.surfaceStepY, j += 1
      ) {
        const _x = x;
        const _y = y;
        const _z = generateRandomFloatInRange(0, 1);
        const surfacePoint = new Vector3(_x, _y, _z);
        this.originalCoords.push(surfacePoint);
        // console.log(x, y, _z);
        this.gridCoords[i].push(surfacePoint);
        // debugger;
      }
    }

    const parts = 30;
    const step = 1 / parts;

    for (let i = 0; i < parts; i++) this.gridSurfaceCoords.push([]);
    // debugger;

    const M = this.gridCoords.length;
    const N = this.gridCoords[0].length;
    // const M = 3;
    // const N = 3;

    let u = 0;
    for (let i = 0; i < parts; i += 1) {
      let v = 0;
      for (let j = 0; j < parts; j += 1) {
        const surfacePoint = this.bezierPoint(u, v, M, N);
        this.gridSurfaceCoords[i].push(surfacePoint);

        v += step;
      }
      u += step;
    }
  }

  // https://en.wikipedia.org/wiki/B%C3%A9zier_surface
  bezierPoint(u: number, v: number, m: number, n: number) {
    let surfacePoint = new Vector3(0, 0, 0);

    for (let i = 0; i < n; i++) {
      const B_i = this.B(n - 1, i, u);

      for (let j = 0; j < m; j++) {
        const B_j = this.B(m - 1, j, v);
        let controlPoint = this.gridCoords[i][j];
        controlPoint = new Vector3(
          controlPoint.x,
          controlPoint.y,
          controlPoint.z
        );
        controlPoint = Vector3.mul(controlPoint, B_i * B_j);
        surfacePoint.add(controlPoint);
      }
    }

    return surfacePoint;
  }

  B(n: number, i: number, t: number) {
    const factor = factorial(n) / (factorial(i) * factorial(n - i));
    const power = Math.pow(1 - t, n - i);
    const power_t = Math.pow(t, i);
    const B = factor * power * power_t;
    return B;
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
    if (this.originalCoords.length > 0) this.displayCoords2();
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
    // let projectedGridCoords: Array<Array<Vector3>> = [];
    // for (let i = 0; i < this.gridCoords.length; i++)
    //   projectedGridCoords.push([]);

    // CONTROL POINTS
    for (let i = 0; i < this.gridCoords.length; i++)
      for (let j = 0; j < this.gridCoords[0].length; j++) {
        let coord = this.gridCoords[i][j];

        let proj2D = new Vector3(coord.x, coord.y, coord.z);

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
        this.canvas.setPoint(convertedCoords.x, convertedCoords.y, 3);
      }
    // SURFACE POINTS
    // debugger;
    for (let i = 0; i < this.gridSurfaceCoords.length; i++)
      for (let j = 0; j < this.gridSurfaceCoords[0].length; j++) {
        let coord = this.gridSurfaceCoords[i][j];

        let proj2D = new Vector3(coord.x, coord.y, coord.z);

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
        // console.log(convertedCoords);
        this.canvas.setPoint(convertedCoords.x, convertedCoords.y, 1);
      }

    // @ts-ignore
    // projectedGridCoords = null;
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

export { Lab_5_2 };
