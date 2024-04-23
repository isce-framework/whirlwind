from collections.abc import Iterable
from typing import TypeVar

from . import _lib

__all__ = [
    "Forest",
]


# FIXME
Graph = TypeVar("Graph")
Vertex = TypeVar("Vertex")
Edge = TypeVar("Edge")


class Forest:
    """
    A forest of rooted trees in a graph.

    Each vertex belongs to a single tree in the forest and stores its predecessor (or
    parent) vertex and edge in the tree, enabling traversal up to the tree's root. A
    root vertex's predecessor is itself.
    """

    def __init__(self, graph: Graph):
        """
        Create a new `Forest`.

        The forest is initialized such that each vertex in the graph is the root of its
        own singleton tree (by setting its predecessor vertex to itself).

        Parameters
        ----------
        graph : Graph
            The forest's underlying graph.
        """
        self._impl = _lib.Forest(graph._impl)

    @property
    def graph(self) -> Graph:
        """The underlying graph."""
        return self._impl.graph  # FIXME

    def depth(self, vertex: Vertex) -> int:
        """
        Get the depth of a vertex.

        Returns the total number of ancestors of the specified vertex. Root vertices
        have a depth of zero.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid vertex in the graph.

        Returns
        -------
        int
            The depth of the vertex.
        """
        return self._impl.depth(vertex)

    def predecessor_vertex(self, vertex: Vertex) -> Vertex:
        """
        Get the predecessor (parent) vertex of a vertex.

        If `vertex` is a root vertex, its predecessor is itself.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid vertex in the graph.

        Returns
        -------
        Vertex
            The vertex's predecessor vertex.
        """
        return self._impl.predecessor_vertex(vertex)

    def predecessor_edge(self, vertex: Vertex) -> Edge:
        """
        Get the predecessor edge of a vertex.

        The predecessor edge is only valid if `vertex` is not a root vertex of a tree in
        the forest.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid, non-root vertex in the graph.

        Returns
        -------
        Edge
            The vertex's predecessor edge.
        """
        return self._impl.predecessor_edge(vertex)

    def predecessor(self, vertex: Vertex) -> tuple[Vertex, Edge]:
        """
        Get the predecessor vertex and edge of a vertex.

        The predecessor edge is only valid if `vertex` is not a root vertex of a tree in
        the forest.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid, non-root vertex in the graph.

        Returns
        -------
        pred_vertex : Vertex
            The vertex's predecessor vertex.
        pred_edge : Edge
            The vertex's predecessor edge.
        """
        return self._impl.predecessor(vertex)

    def predecessors(self, vertex: Vertex) -> Iterable[tuple[Vertex, Edge]]:
        """
        Iterate over a vertex's predecessor vertices and edges.

        Returns a view of the input vertex's predecessors. Iterating over the result
        traverses a path in the graph from `vertex` to the root of the tree on which it
        resides, while yielding (tail,edge) pairs of the predecessors of each visited
        vertex (excluding the root vertex).

        The view is empty if `vertex` is a root vertex.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid vertex in the graph.

        Returns
        -------
        iterable of (Vertex, Edge)
            A view of the vertex's predecessors.
        """
        return self._impl.predecessors(vertex)

    def set_predecessor(
        self, vertex: Vertex, pred_vertex: Vertex, pred_edge: Edge
    ) -> None:
        """
        Assign a vertex's predecessor.

        Parameters
        ----------
        vertex : Vertex
            The successor (child) vertex. Must be a valid vertex in the graph.
        pred_vertex : Vertex
            The predecessor (parent) vertex. Must be a valid vertex in the graph.
        pred_edge : Edge
            The predecessor edge of `vertex`. If `vertex` is not a root vertex, it must
            be a valid edge in the graph whose head is `vertex` and whose tail is
            `pred_vertex`.
        """
        self._impl.set_predecessor(vertex, pred_vertex, pred_edge)

    def make_root_vertex(self, vertex: Vertex) -> None:
        """
        Make the specified vertex into a root vertex.

        Make the specified vertex the root of a new tree by setting its predecessor
        vertex to itself.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid vertex in the graph.
        """
        self._impl.make_root_vertex(vertex)

    def is_root_vertex(self, vertex: Vertex) -> bool:
        """
        Check if a vertex is the root of a tree in the forest.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid vertex in the graph.

        Returns
        -------
        bool
            True if the vertex is a root vertex; otherwise false.
        """
        return self._impl.is_root_vertex(vertex)

    def reset(self) -> None:
        """
        Reset the forest to its initial state.

        Re-initializes the forest such that each vertex in the graph is the root of its
        own singleton tree (by setting its predecessor vertex to itself). Each
        predecessor edge is set to the value of `edge_fill_value`.
        """
        self._impl.reset()
