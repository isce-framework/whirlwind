from . import _lib
from ._network import Network

__all__ = [
    "primal_dual",
]


def primal_dual(network: Network, maxiter: int = 0) -> None:
    _lib.primal_dual(network._impl, maxiter)
