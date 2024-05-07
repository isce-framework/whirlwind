from . import _lib
from ._network import Network

__all__ = [
    "successive_shortest_paths",
]


def successive_shortest_paths(network: Network) -> None:
    _lib.successive_shortest_paths(network._impl)
