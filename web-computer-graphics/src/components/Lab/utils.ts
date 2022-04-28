import type { Vector3 } from "./Vector3";

function generateRandomFloatInRange(min: number, max: number) {
  return Math.random() * (max - min) + min;
}

function download(text: string, name: string, type: string) {
  let myBlob = new Blob([text], { type: type });

  let url = window.URL.createObjectURL(myBlob);
  let anchor = document.createElement("a");
  anchor.href = url;
  anchor.download = name;

  anchor.click();
  window.URL.revokeObjectURL(url);
  anchor.remove();
}

function copyCoords(source: Array<Vector3>, dest: Array<Vector3>) {
  for (let item of source) {
    dest.push(item);
  }
}

function factorial(n: number) {
  let answer = 1;
  if (n == 0 || n == 1) {
    return answer;
  } else {
    for (var i = n; i >= 1; i--) {
      answer = answer * i;
    }
    return answer;
  }
}

export { generateRandomFloatInRange, download, factorial, copyCoords };
