from collections.abc import Iterator

from . import _lib

__all__ = [
    "EdgeList",
]


class EdgeList:
    """A sequence of (tail,head) vertex pairs."""

    Vertex = int

    def __init__(self):  # type: ignore[no-untyped-def]
        self._impl = _lib.EdgeList()

    @property
    def size(self) -> int:
        """The total number of edges."""
        return self._impl.size

    def add_edge(self, tail: Vertex, head: Vertex) -> None:
        """
        Add a new edge.

        Parameters
        ----------
        tail : Vertex
            The tail vertex of the edge.
        head : Vertex
            The head vertex of the edge.
        """
        self._impl.add_edge(tail, head)

    def clear(self) -> None:
        """Erase all edges from the sequence."""
        self._impl.clear()

    def empty(self) -> bool:
        """Check if the container is empty."""
        return self._impl.empty()

    def __len__(self) -> int:
        return len(self._impl)

    def __getitem__(self, /, key: int) -> tuple[Vertex, Vertex]:
        return self._impl[key]

    def __iter__(self) -> Iterator[tuple[Vertex, Vertex]]:
        return iter(self._impl)
