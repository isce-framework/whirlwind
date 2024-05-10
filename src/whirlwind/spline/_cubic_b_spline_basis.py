import numpy as np
from numpy.typing import ArrayLike

from . import _lib

__all__ = [
    "CubicBSplineBasis",
]


def _make_cubic_b_spline_basis_impl(knots):  # type: ignore[no-untyped-def]
    knots = np.ascontiguousarray(knots)
    if knots.dtype == np.float32:
        return _lib.CubicBSplineBasis__f32(knots)
    else:
        return _lib.CubicBSplineBasis__f64(knots)


class CubicBSplineBasis:
    def __init__(self, knots: ArrayLike):
        self._impl = _make_cubic_b_spline_basis_impl(knots)  # type: ignore[no-untyped-call]

    @property
    def knots(self) -> np.ndarray:
        """ """
        return self._impl.knots

    @property
    def num_knot_intervals(self) -> int:
        """ """
        return self._impl.num_knot_intervals

    @property
    def num_basis_funcs(self) -> int:
        """ """
        return self._impl.num_basis_funcs

    def get_knot_interval(self, x: float) -> int:
        """ """
        return self._impl.get_knot_interval(x)

    def eval_in_interval(self, x: float, i: int) -> float:
        """ """
        return self._impl.eval_in_interval(x, i)

    def eval_derivative_in_interval(self, x: float, i: int) -> float:
        """ """
        return self._impl.eval_derivative_in_interval(x, i)

    def eval_second_derivative_in_interval(self, x: float, i: int) -> float:
        """ """
        return self._impl.eval_second_derivative_in_interval(x, i)
