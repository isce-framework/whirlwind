from typing import TypeVar

import numpy as np
from numpy.typing import ArrayLike
from scipy.linalg.lapack import dgtsv

from . import _lib
from ._cubic_b_spline_basis import CubicBSplineBasis

__all__ = [
    "CubicBSpline",
]


CubicBSplineT = TypeVar("CubicBSplineT", bound="CubicBSpline")


def _make_cubic_b_spline_impl(basis, control_points):  # type: ignore[no-untyped-def]
    control_points = np.ascontiguousarray(control_points)
    if control_points.dtype == np.float32:
        return _lib.CubicBSpline__f32(basis._impl, control_points)
    else:
        return _lib.CubicBSpline__f64(basis._impl, control_points)


class CubicBSpline:
    def __init__(self, basis: CubicBSplineBasis, control_points: ArrayLike):
        self._impl = _make_cubic_b_spline_impl(basis, control_points)  # type: ignore[no-untyped-call]

    @classmethod
    def interpolate(
        cls: type[CubicBSplineT], knots: ArrayLike, values: ArrayLike
    ) -> CubicBSplineT:
        """ """

        basis = CubicBSplineBasis(knots)

        knots = basis.knots
        values = np.asanyarray(values)

        du = []
        d = []
        dl = []
        b = []

        d0, du0, c0, _ = basis.eval_second_derivative_in_interval(knots[0], 0)

        dl1, d1, du1, _ = basis.eval_in_interval(knots[0], 0)

        # Row reduction (first row)
        d0 -= c0 * dl1 / du1
        du0 -= c0 * d1 / du1
        b0 = -c0 * values[0] / du1

        du.append(du0)
        d.append(d0)
        b.append(b0)

        dl.append(dl1)
        d.append(d1)
        du.append(du1)
        b.append(values[0])

        for i in range(1, len(knots) - 1):
            dl2, d2, du2, _ = basis.eval_in_interval(knots[i], i)
            dl.append(dl2)
            d.append(d2)
            du.append(du2)
            b.append(values[i])

        _, dl3, d3, du3 = basis.eval_in_interval(knots[-1], len(knots) - 2)
        dl.append(dl3)
        d.append(d3)
        du.append(du3)
        b.append(values[-1])

        _, c1, dl4, d4 = basis.eval_second_derivative_in_interval(
            knots[-1], len(knots) - 2
        )

        # Row reduction (last row)
        dl4 -= c1 * d3 / dl3
        d4 -= c1 * du3 / dl3
        b1 = -c1 * values[-1] / dl3

        dl.append(dl4)
        d.append(d4)
        b.append(b1)

        du = np.asarray(du)
        d = np.asarray(d)
        dl = np.asarray(dl)
        b = np.asarray(b)

        _, _, _, control_points, _ = dgtsv(dl, d, du, b)

        return cls(basis, control_points)

    @property
    def knots(self) -> np.ndarray:
        """ """
        return self._impl.knots

    @property
    def control_points(self) -> np.ndarray:
        """ """
        return self._impl.control_points

    def __call__(self, x: float) -> float:
        """ """
        return self._impl(x)
