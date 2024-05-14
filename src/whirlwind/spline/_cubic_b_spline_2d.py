from typing import TypeVar

import numpy as np
from numpy.typing import ArrayLike

from . import _lib
from ._cubic_b_spline import CubicBSpline
from ._cubic_b_spline_basis import CubicBSplineBasis

__all__ = [
    "BiCubicBSpline",
]


BiCubicBSplineT = TypeVar("BiCubicBSplineT", bound="BiCubicBSpline")


def _make_bi_cubic_b_spline_impl(basis0, basis1, control_points):  # type: ignore[no-untyped-def]
    control_points = np.ascontiguousarray(control_points)
    if control_points.dtype == np.float32:
        return _lib.BiCubicBSpline__f32(basis0._impl, basis1._impl, control_points)
    else:
        return _lib.BiCubicBSpline__f64(basis0._impl, basis1._impl, control_points)


class BiCubicBSpline:
    def __init__(
        self,
        bases: tuple[CubicBSplineBasis, CubicBSplineBasis],
        control_points: ArrayLike,
    ):
        self._impl = _make_bi_cubic_b_spline_impl(bases[0], bases[1], control_points)  # type: ignore[no-untyped-call]

    @classmethod
    def interpolate(
        cls: type[BiCubicBSplineT],
        knots: tuple[ArrayLike, ArrayLike],
        values: ArrayLike,
    ) -> BiCubicBSplineT:
        """ """
        basis0 = CubicBSplineBasis(knots[0])
        basis1 = CubicBSplineBasis(knots[1])

        knots0 = basis0.knots
        knots1 = basis1.knots
        values = np.asanyarray(values)

        m = len(knots0)
        n = len(knots1)

        dtype = np.common_type(values)

        tmp = np.zeros_like(values, dtype=dtype, shape=(m, n + 2))
        for i in range(m):
            spline = CubicBSpline.interpolate(knots1, values[i, :])
            tmp[i, :] = spline.control_points

        control_points = np.zeros_like(tmp, shape=(m + 2, n + 2))
        for j in range(n + 2):
            spline = CubicBSpline.interpolate(knots0, tmp[:, j])
            control_points[:, j] = spline.control_points

        return cls((basis0, basis1), control_points)

    @property
    def knots(self) -> tuple[np.ndarray, np.ndarray]:
        """ """
        return self._impl.knots

    @property
    def control_points(self) -> np.ndarray:
        """ """
        return self._impl.control_points

    def __call__(self, x0: float, x1: float) -> float:
        """ """
        return self._impl(x0, x1)
