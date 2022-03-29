function interpolate(
  t: number,
  degree: number,
  controlPoints: Array<Array<number>>,
  knots: Array<number>,
  weights: Array<number>
) {
  let i, j, startKnot, level; // function-scoped iteration variables
  let n = controlPoints.length; // points count
  let vectorDimension = controlPoints[0].length; // point dimensionality

  if (degree < 1) throw new Error("degree must be at least 1 (linear)");
  if (degree > n - 1)
    throw new Error("degree must be less than or equal to point count - 1");

  if (!weights) {
    // build weight vector of length [n]
    weights = [];
    for (i = 0; i < n; i++) {
      weights[i] = 1;
    }
  }

  if (!knots) {
    // build knot vector of length [n + degree + 1]
    let knots = [];
    for (i = 0; i < n + degree + 1; i++) {
      knots[i] = i;
    }
  } else {
    if (knots.length !== n + degree + 1)
      throw new Error("bad knot vector length");
  }

  let domain = [degree, knots.length - 1 - degree];

  // remap t to the domain where the spline is defined
  let low = knots[domain[0]];
  let high = knots[domain[1]];
  t = t * (high - low) + low;

  if (t < low || t > high) throw new Error("out of bounds");

  // find s (the spline segment) for the [t] value provided
  for (startKnot = domain[0]; startKnot < domain[1]; startKnot++) {
    if (t >= knots[startKnot] && t <= knots[startKnot + 1]) {
      break;
    }
  }
  console.log(startKnot);

  // convert points to homogeneous coordinates
  // If we have NURBS https://en.wikipedia.org/wiki/Non-uniform_rational_B-spline
  let v: Array<Array<number>> = [];
  for (i = 0; i < n; i++) {
    v[i] = [];
    for (j = 0; j < vectorDimension; j++) {
      v[i][j] = controlPoints[i][j] * weights[i];
    }
    v[i][vectorDimension] = weights[i];
  }

  // l (level) goes from 1 to the curve degree + 1
  let alpha;
  for (level = 1; level <= degree + 1; level++) {
    // строим пирамиду из N коэфициентов
    for (i = startKnot; i > startKnot - degree - 1 + level; i--) {
      alpha = (t - knots[i]) / (knots[i + degree + 1 - level] - knots[i]);

      // interpolate each component
      for (j = 0; j < vectorDimension + 1; j++) {
        v[i][j] = (1 - alpha) * v[i - 1][j] + alpha * v[i][j];
      }
    }
  }
  // debugger;

  // convert back to cartesian and return
  let result = [];
  for (i = 0; i < vectorDimension; i++) {
    result[i] = v[startKnot][i] / v[startKnot][vectorDimension];
  }

  return result;
}

export { interpolate };
