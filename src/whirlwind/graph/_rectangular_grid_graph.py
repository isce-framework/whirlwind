from collections.abc import Iterable

from . import _lib

__all__ = [
    "RectangularGridGraph",
]


def _make_rectangular_grid_graph_impl(num_rows, num_cols, num_parallel_edges):  # type: ignore[no-untyped-def]
    if num_parallel_edges == 1:
        return _lib.RectangularGridGraph__1(num_rows=num_rows, num_cols=num_cols)
    if num_parallel_edges == 2:
        return _lib.RectangularGridGraph__2(num_rows=num_rows, num_cols=num_cols)
    raise ValueError


class RectangularGridGraph:
    """
    A 2-dimensional rectangular grid graph.

    A graph consisting of an M x N Cartesian grid of vertices. Each vertex has an
    outgoing edge to each of its four neighboring vertices (except at the boundaries).
    Vertices are represented by (row,col) index pairs. Edges are represented by unsigned
    integers.
    """

    Vertex = tuple[int, int]
    Edge = int

    def __init__(self, num_rows: int, num_cols: int, num_parallel_edges: int = 1):
        """
        Create a new `RectangularGridGraph`.

        Parameters
        ----------
        num_rows : int
            The number of rows in the 2-D array of vertices.
        num_cols : int
            The number of columns in the 2-D array of vertices.
        num_parallel_edges : int, optional
            The number of parallel edges between adjacent vertices in the graph. Must be
            1 or 2. Defaults to 1.
        """
        self._impl = _make_rectangular_grid_graph_impl(
            num_rows, num_cols, num_parallel_edges
        )  # type: ignore[no-untyped-call]

    @property
    def num_parallel_edges(self) -> int:
        """int : The number of parallel edges between adjacent vertices in the graph."""  # noqa: D403
        return self._impl.num_parallel_edges

    @property
    def num_rows(self) -> int:
        """int : The number of rows of vertices in the graph."""  # noqa: D403
        return self._impl.num_rows

    @property
    def num_cols(self) -> int:
        """int : The number of columns of vertices in the graph."""  # noqa: D403
        return self._impl.num_cols

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

    def __repr__(self) -> str:
        return repr(self._impl)
