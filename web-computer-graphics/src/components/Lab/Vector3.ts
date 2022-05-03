// собственный тип данных
class Vector3 {
  x: number = 0;
  y: number = 0;
  z: number = 0;

  constructor(x: number = 0, y: number = 0, z: number = 0) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  norm2(): number {
    let { x, y, z } = this;
    return Math.sqrt(x * x + y * y + z * z);
  }

  public static mul(vec: Vector3, scale: number): Vector3 {
    const { x, y, z } = vec;
    return new Vector3(x * scale, y * scale, z * scale);
  }

  public static dotProduct(v1: Vector3, v2: Vector3): number {
    let dotProduct = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    let v1_norm = v1.norm2();
    let v2_norm = v2.norm2();
    return dotProduct / (v1_norm * v2_norm);
  }
  public static crossProduct(v1: Vector3, v2: Vector3): Vector3 {
    const x: number = v1.y * v2.z - v1.z * v2.y;
    const y: number = v1.z * v2.x - v1.x * v2.z;
    const z: number = v1.x * v2.y - v1.y * v2.x;
    return new Vector3(x, y, z);
  }

  public static lepr(v1: Vector3, v2: Vector3, t: number) {
    const negV1 = Vector3.mul(v1, -1);
    let difference = Vector3.add(v2, negV1);
    difference = Vector3.mul(difference, t);
    const result = Vector3.add(v1, difference);

    return result;
  }

  public add(vec: Vector3) {
    this.x += vec.x;
    this.y += vec.y;
    this.z += vec.z;
  }

  public static add(v1: Vector3, v2: Vector3): Vector3 {
    return new Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
  }

  // перемножение матриц из трехмерного пространства
  public static matMul(matrix: Array<Vector3>, toProjVec: Vector3): Vector3 {
    let proj2D = new Vector3();
    // let resultVec = [];
    let x = 0;
    let y = 0;
    let z = 0;
    let toProjArr = [toProjVec.x, toProjVec.y, toProjVec.z];
    let matrixArr = [
      matrix[0].toArray(),
      matrix[1].toArray(),
      matrix[2].toArray(),
    ];
    for (let j = 0; j < 3; j++) {
      x += matrixArr[0][j] * toProjArr[j];
      y += matrixArr[1][j] * toProjArr[j];
      z += matrixArr[2][j] * toProjArr[j];
      //   resultVec.push(res);
    }
    proj2D.x = x;
    proj2D.y = y;
    proj2D.z = z;
    // console.log(proj2D);
    return proj2D;
  }

  toString(): string {
    const { x, y, z } = this;
    return `x=${x} y=${y} z=${z}`;
  }

  toArray(): Array<number> {
    return [this.x, this.y, this.z];
  }
}

export { Vector3 };
