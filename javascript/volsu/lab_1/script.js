class Canvas {
	constructor(canvas_id) {
		this.canvas_id = canvas_id;
		this.canvas = document.querySelector(`#${this.canvas_id}`)
		this.ctx = this.canvas.getContext("2d"); 

		this.setupCtx()
		this.sandbox()
	}

    setupCanvas() {
        this.canvas.wid
    }

	setupCtx() {
		this.ctx.imageSmoothingEnabled = false;
	}

	setPoint(x, y, size = 1) {
		this.ctx.fillRect(x, y, size, size)
	}

	drawLine(x_start, y_start, x_end, y_end)
	{
		const ctx = this.ctx;

		ctx.beginPath();
		ctx.moveTo(x_start, y_start);
		ctx.lineTo(x_end, y_end);
		ctx.stroke();
	}

	sandbox() {
		this.setPoint(20, 20)
		this.drawLine(24, 24, 50, 56)
	}
}

class Lab_1 {
	constructor(drawInstance) {
		this.canvas = drawInstance;
	}

	// тут переписать 6 лабу с с#

}

window.onload = () => {
  const drawInstance = new Canvas("canvas_id")
	const lab_1 = new Lab_1(drawInstance)
};
