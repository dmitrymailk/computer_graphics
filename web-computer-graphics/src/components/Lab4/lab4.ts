import type { Canvas } from "../Lab/Canvas";
// import type { Vector3 } from "./Vector3";
import { Vector3 } from "../Lab/Vector3";
// @ts-ignore
import Papa from "papaparse";

import { generateRandomFloatInRange, download, copyCoords } from "../Lab/utils";

class Lab_4 {
  private canvas: Canvas;

  private originalCoords: Array<Vector3> = [];
  private transformedCoords: Array<Vector3> = [];
  private projectedCoords: Array<Vector3> = [];
  private traversalIndexes: Array<Array<number>> = [];

  // параметры поворота для всех осей
  private angleZ: number = 0;
  private angleX: number = 0;
  private angleY: number = 0;

  // параметры скейла для всех осей
  private scaleX: number = 150;
  private scaleY: number = 150;
  private scaleZ: number = 150;
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

  constructor(drawInstance: Canvas) {
    this.canvas = drawInstance;
    // this.scaleX = this.canvas.width;
    // this.scaleY = this.canvas.height;
    // this.scaleZ = this.canvas.width;
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
    let coords = [
      [-0.5, -0.5, -0.5],
      [0.5, -0.5, -0.5],
      [0.5, 0.5, -0.5],
      [-0.5, 0.5, -0.5],

      [-0.5, -0.5, 0.5],
      [0.5, -0.5, 0.5],
      [0.5, 0.5, 0.5],
      [-0.5, 0.5, 0.5],

      // new coord
      [-0.25, -0.25, -0.25],
    ];

    for (const item of coords) {
      this.originalCoords.push(new Vector3(item[0], item[1], item[2]));
    }

    const traversalIndexes = [
      // первая сторона
      // [0, 1],
      [1, 2],
      [2, 3],
      [3, 0],

      // вторая сторона
      [4, 5],
      [5, 6],
      [6, 7],
      [7, 4],

      // боковые стороны
      [0, 4],
      [1, 5],
      [2, 6],
      [3, 7],

      // создание невыпуклости
      [0, 8],
      [8, 1],
    ];

    this.traversalIndexes = traversalIndexes;
  }

  private update(ts: number) {
    this.updated(ts);
    window.requestAnimationFrame(this.update.bind(this));
  }

  private updated(ts: number) {
    // console.log(ts);
    if (this.originalCoords.length > 0) this.displayCoords();
  }

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
      this.canvas.ctx.fillText(`${i}`, convertedCoords.x, convertedCoords.y);
      this.canvas.setPoint(convertedCoords.x, convertedCoords.y);
      i += 1;
    }

    // for (let i = 0; i < this.projectedCoords.length - 1; i += 2) {
    //   this.connectProjectedDots(i, i + 1);
    // }
    for (const traversal of this.traversalIndexes) {
      this.connectProjectedDots(traversal[0], traversal[1]);
    }
  }

  convertCoords(x: number, y: number): Vector3 {
    x += this.canvas.width / 2;
    y += this.canvas.height / 2;
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
        break;
      }
      case "KeyX": {
        this.angleX += 0.1;
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

export { Lab_4 };
