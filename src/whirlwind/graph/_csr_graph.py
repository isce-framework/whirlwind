from collections.abc import Iterable

from . import _lib
from ._edge_list import EdgeList

__all__ = [
    "CSRGraph",
]


class CSRGraph:
    """
    A compressed sparse row (CSR) digraph.

    Vertices and edges are represented by unsigned integers. The graph topology cannot
    be modified after construction. Parallel edges and self-loops are supported.
    """

    Vertex = int
    Edge = int

    def __init__(self, edge_list: EdgeList):
        """Create a new `CSRGraph` from a sequence of (tail,head) pairs."""
        self._impl = _lib.CSRGraph(edge_list._impl)

    @property
    def num_vertices(self) -> int:
        """int : The total number of vertices in the graph."""  # noqa: D403
        return self._impl.num_vertices

    @property
    def num_edges(self) -> int:
        """int : The total number of edges in the graph."""  # noqa: D403
        return self._impl.num_edges

    def get_vertex_id(self, vertex: Vertex) -> int:
        """
        Get the unique array index of a vertex.

        Given a vertex in the graph, get the associated vertex index in the range
        [0, V), where V is the total number of vertices.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid vertex in the graph.

        Returns
        -------
            The vertex index.
        """
        return self._impl.get_vertex_id(vertex)

    def get_edge_id(self, edge: Edge) -> int:
        """
        Get the unique array index of an edge.

        Given an edge in the graph, get the associated edge index in the range [0, E),
        where E is the total number of edges.

        Parameters
        ----------
        edge : Edge
            The input edge. Must be a valid edge in the graph.

        Returns
        -------
            The edge index.
        """
        return self._impl.get_edge_id(edge)

    def vertices(self) -> Iterable[Vertex]:
        """
        Iterate over vertices in the graph.

        Returns a view of all vertices in the graph in order from smallest index to
        largest.
        """
        return self._impl.vertices()

    def edges(self) -> Iterable[Edge]:
        """
        Iterate over edges in the graph.

        Returns a view of all edges in the graph in order from smallest index to
        largest.
        """
        return self._impl.edges()

    def contains_vertex(self, vertex: Vertex) -> bool:
        """Check whether the graph contains the specified vertex."""
        return self._impl.contains_vertex(vertex)

    def contains_edge(self, edge: Edge) -> bool:
        """Check whether the graph contains the specified edge."""
        return self._impl.contains_edge(edge)

    def outdegree(self, vertex: Vertex) -> int:
        """
        Get the number of outgoing edges of a vertex.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid vertex in the graph.

        Returns
        -------
        int
            The outdegree of the vertex.
        """
        return self._impl.outdegree(vertex)

    def outgoing_edges(self, vertex: Vertex) -> Iterable[tuple[Edge, Vertex]]:
        """
        Iterate over outgoing edges (and corresponding head vertices) of a vertex.

        Returns a view of ordered (edge,head) pairs over all edges emanating from the
        specified vertex in the graph.

        Parameters
        ----------
        vertex : Vertex
            The input vertex. Must be a valid vertex in the graph.

        Returns
        -------
        iterable of (Edge, Vertex)
            A view of the vertex's outgoing incident edges and successor vertices.
        """
        return self._impl.outgoing_edges(vertex)
