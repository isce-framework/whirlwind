# from typing import Self

import numpy as np
from numpy.typing import ArrayLike

from ._cubic_b_spline_basis import CubicBSplineBasis
from . import _lib

__all__ = [
    "CubicBSpline",
]


def _make_cubic_b_spline_impl(basis, control_points):  # type: ignore[no-untyped-def]
    control_points = np.ascontiguousarray(control_points)
    if control_points.dtype == np.float32:
        return _lib.CubicBSpline__f32(basis._impl, control_points)
    else:
        return _lib.CubicBSpline__f64(basis._impl, control_points)


class CubicBSpline:
    def __init__(self, basis: CubicBSplineBasis, control_points: ArrayLike):
        self._impl = _make_cubic_b_spline_impl(basis, control_points)
        ...

    # @classmethod
    # def interpolate(cls, knots: ArrayLike, values: ArrayLike) -> Self:
    #     """ """
    #     return

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
