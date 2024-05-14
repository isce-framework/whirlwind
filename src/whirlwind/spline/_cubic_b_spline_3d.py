from typing import TypeVar

import numpy as np
from numpy.typing import ArrayLike

from . import _lib
from ._cubic_b_spline import CubicBSpline
from ._cubic_b_spline_basis import CubicBSplineBasis

__all__ = [
    "TriCubicBSpline",
]


TriCubicBSplineT = TypeVar("TriCubicBSplineT", bound="TriCubicBSpline")


def _make_tri_cubic_b_spline_impl(basis0, basis1, basis2, control_points):  # type: ignore[no-untyped-def]
    control_points = np.ascontiguousarray(control_points)
    if control_points.dtype == np.float32:
        return _lib.TriCubicBSpline__f32(
            basis0._impl, basis1._impl, basis2._impl, control_points
        )
    else:
        return _lib.TriCubicBSpline__f64(
            basis0._impl, basis1._impl, basis2._impl, control_points
        )


class TriCubicBSpline:
    def __init__(
        self,
        bases: tuple[CubicBSplineBasis, CubicBSplineBasis, CubicBSplineBasis],
        control_points: ArrayLike,
    ):
        self._impl = _make_tri_cubic_b_spline_impl(  # type: ignore[no-untyped-call]
            bases[0], bases[1], bases[2], control_points
        )

    @classmethod
    def interpolate(
        cls: type[TriCubicBSplineT],
        knots: tuple[ArrayLike, ArrayLike, ArrayLike],
        values: ArrayLike,
    ) -> TriCubicBSplineT:
        """ """
        basis0 = CubicBSplineBasis(knots[0])
        basis1 = CubicBSplineBasis(knots[1])
        basis2 = CubicBSplineBasis(knots[2])

        knots0 = basis0.knots
        knots1 = basis1.knots
        knots2 = basis2.knots
        values = np.asanyarray(values)

        m = len(knots0)
        n = len(knots1)
        p = len(knots2)

        dtype = np.common_type(values)

        tmp1 = np.zeros_like(values, dtype=dtype, shape=(m, n, p + 2))
        for i in range(m):
            for j in range(n):
                spline = CubicBSpline.interpolate(knots2, values[i, j, :])
                tmp1[i, j, :] = spline.control_points

        tmp2 = np.zeros_like(tmp1, shape=(m, n + 2, p + 2))
        for i in range(m):
            for k in range(p + 2):
                spline = CubicBSpline.interpolate(knots1, tmp1[i, :, k])
                tmp2[i, :, k] = spline.control_points

        control_points = np.zeros_like(tmp2, shape=(m + 2, n + 2, p + 2))
        for j in range(n + 2):
            for k in range(p + 2):
                spline = CubicBSpline.interpolate(knots0, tmp2[:, j, k])
                control_points[:, j, k] = spline.control_points

        return cls((basis0, basis1, basis2), control_points)

    @property
    def knots(self) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        """ """
        return self._impl.knots

    @property
    def control_points(self) -> np.ndarray:
        """ """
        return self._impl.control_points

    def __call__(self, x0: float, x1: float, x2: float) -> float:
        """ """
        return self._impl(x0, x1, x2)
