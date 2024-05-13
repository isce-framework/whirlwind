# from typing import Self

import numpy as np
from numpy.typing import ArrayLike

from . import _lib
from ._cubic_b_spline_basis import CubicBSplineBasis

__all__ = [
    "TriCubicBSpline",
]


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

    # @classmethod
    # def interpolate(
    #     cls: type[Self],
    #     knots: tuple[ArrayLike, ArrayLike, ArrayLike],
    #     values: ArrayLike,
    # ) -> Self:
    #     """ """
    #     return

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
