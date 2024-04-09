import enum
from typing import Generic, TypeVar
from collections.abc import Iterable

from . import _lib
from ._forest import Forest

__all__ = [
    "Dijkstra",
    "DistanceType",
]


# FIXME
Graph = TypeVar("Graph")
Vertex = TypeVar("Vertex")
Edge = TypeVar("Edge")
Distance = TypeVar("Distance")


class DistanceType(enum.Enum):
    INT = enum.auto()
    REAL = enum.auto()


def _make_dijkstra_impl(graph, distance_type):
    if distance_type == DistanceType.REAL:
        return _lib.Dijkstra(graph._impl, float())
    if distance_type == DistanceType.INT:
        return _lib.Dijkstra(graph._impl, int())
    raise ValueError


class Dijkstra(Forest, Generic[Graph, Distance]):

    def __init__(self, graph: Graph, distance_type: DistanceType = DistanceType.REAL):
        self._impl = _make_dijkstra_impl(graph, distance_type)
        self._distance_type = distance_type

    @property
    def distance_type(self) -> DistanceType:
        return self._distance_type

    def has_reached_vertex(self, vertex: Vertex) -> bool:
        return self._impl.has_reached_vertex(vertex)

    def has_visited_vertex(self, vertex: Vertex) -> bool:
        return self._impl.has_visited_vertex(vertex)

    def label_vertex_reached(self, vertex: Vertex) -> None:
        self._impl.label_vertex_reached(vertex)

    def label_vertex_visited(self, vertex: Vertex) -> None:
        self._impl.label_vertex_visited(vertex)

    def reached_vertices(self) -> Iterable[Vertex]:
        return self._impl.reached_vertices()

    def visited_vertices(self) -> Iterable[Vertex]:
        return self._impl.visited_vertices()

    def distance_to_vertex(self, vertex: Vertex) -> Distance:
        return self._impl.distance_to_vertex(vertex)

    def set_distance_to_vertex(self, vertex: Vertex, distance: Distance) -> None:
        self._impl.set_distance_to_vertex(vertex, distance)

    def add_source(self, source: Vertex) -> None:
        self._impl.add_source(source)

    def pop_next_unvisited_vertex(self) -> tuple[Vertex, Distance]:
        return self._impl.pop_next_unvisited_vertex()

    def visit_vertex(self, vertex: Vertex, distance: Distance) -> None:
        self._impl.visit_vertex(vertex, distance)

    def relax_edge(self, edge: Edge, tail: Vertex, head: Vertex, distance: Distance) -> None:
        self._impl.relax_edge(edge=edge, tail=tail, head=head, distance=distance)
