from __future__ import annotations

from collections.abc import Iterable
from typing import Generic, TypeVar

import numpy as np
from numpy.typing import ArrayLike

from whirlwind.graph import RectangularGridGraph

from . import _lib

__all__ = [
    "Network",
]


# FIXME
Graph = TypeVar("Graph")
Node = TypeVar("Node")
Arc = TypeVar("Arc")
ResidualGraph = TypeVar("ResidualGraph")
Cost = TypeVar("Cost")
Flow = TypeVar("Flow")


def _make_network_impl(graph, surplus, cost, capacity):  # type: ignore[no-untyped-def]
    if type(graph) != RectangularGridGraph:
        raise NotImplementedError

    # FIXME
    surplus = np.asanyarray(surplus)
    cost = np.asanyarray(cost)

    if not np.issubdtype(surplus.dtype, np.integer):
        raise TypeError

    if capacity is None:
        if np.issubdtype(cost.dtype, np.floating):
            cls = _lib.Network__RectangularGridGraph_f32_i32_vector_Uncapacitated
        elif np.issubdtype(cost.dtype, np.integer):
            cls = _lib.Network__RectangularGridGraph_i32_i32_vector_Uncapacitated
        else:
            raise TypeError
    elif capacity == 1:
        if np.issubdtype(cost.dtype, np.floating):
            cls = _lib.Network__RectangularGridGraph_f32_i32_vector_UnitCapacity
        elif np.issubdtype(cost.dtype, np.integer):
            cls = _lib.Network__RectangularGridGraph_i32_i32_vector_UnitCapacity
        else:
            raise TypeError
    else:
        raise NotImplementedError

    return cls(graph=graph._impl, surplus=surplus, cost=cost)


class Network(Generic[ResidualGraph, Cost, Flow]):

    def __init__(
        self,
        graph: Graph,
        surplus: ArrayLike,
        cost: ArrayLike,
        capacity: int | ArrayLike | None = None,
    ):
        self._impl = _make_network_impl(graph, surplus, cost, capacity)  # type: ignore[no-untyped-call]

    @property
    def residual_graph(self) -> ResidualGraph:
        """The residual graph of the network."""
        return self._impl.residual_graph

    @property
    def num_nodes(self) -> int:
        """The total number of nodes in the network."""
        return self._impl.num_nodes

    @property
    def num_arcs(self) -> int:
        """
        The total number of arcs in the network's residual graph (including arcs whose
        residual capacity is zero).
        """
        return self._impl.num_arcs

    @property
    def num_forward_arcs(self) -> int:
        return self._impl.num_forward_arcs

    def contains_node(self, node: Node) -> bool:
        """Check whether the network contains the specified node."""
        return self._impl.contains_node(node)

    def contains_arc(self, arc: Arc) -> bool:
        """
        Check whether the network's residual graph contains the specified arc.

        Arcs with zero residual capacity are considered to be contained in the residual
        graph.
        """
        return self._impl.contains_arc(arc)

    def get_node_id(self, node: Node) -> int:
        """
        Get the unique array index of a node.

        Given a node in the network, get the associated node index in the range [0, N),
        where N is the total number of nodes.

        Parameters
        ----------
        node : Node
            The input node. Must be a valid node in the network.

        Returns
        -------
        int
            The node index.
        """
        return self._impl.get_node_id(node)

    def get_arc_id(self, arc: Arc) -> int:
        """
        Get the unique array index of an arc.

        Given an arc in the network's residual graph, get the associated arc index in
        the range [0, A), where A is the total number of arcs.

        Parameters
        ----------
        arc : Arc
            The input arc. Must be a valid arc in the network's residual graph (though
            its residual capacity may be zero).

        Returns
        -------
        int
            The arc index.
        """
        return self._impl.get_arc_id(arc)

    def nodes(self) -> Iterable[Node]:
        """
        Iterate over nodes in the network.

        Returns a view of all nodes in the network in order from smallest node index to
        largest.
        """
        return self._impl.nodes()

    def arcs(self) -> Iterable[Arc]:
        """
        Iterate over arcs in the network's residual graph.

        Returns a view of all arcs in the residual graph (including arcs whose residual
        capacity is zero) in order from smallest arc index to largest.
        """
        return self._impl.arcs()

    def outgoing_arcs(self, node: Node) -> Iterable[tuple[Arc, Node]]:
        """
        Iterate over outgoing arcs (and corresponding head nodes) of a node.

        Returns a view of ordered (arc,head) pairs over all arcs emanating from the
        specified node in the network's residual graph.

        Parameters
        ----------
        node : Node
            The input node. Must be a valid node in the network.

        Returns
        -------
        iterable of (Arc, Node)
            A view of the node's outgoing incident arcs and successor nodes in the
            residual graph.
        """
        return self._impl.outgoing_arcs(node)

    def is_forward_arc(self, arc: Arc) -> bool:
        """
        Check whether an arc in the network's residual graph is a forward arc.

        Forward arcs are those found in the original graph, whereas reverse arcs are
        found only in the residual graph.

        Parameters
        ----------
        arc : Arc
            The input arc. Must be a valid arc in the network's residual graph (though
            its residual capacity may be zero).

        Returns
        -------
            True if `arc` is a forward arc; otherwise false.
        """
        return self._impl.is_forward_arc(arc)

    def get_residual_graph_arc_id(self, edge_id: int) -> int:
        """
        Given the edge index of an edge in the original graph, get the arc index of the
        corresponding arc in the residual graph.

        Parameters
        ----------
        edge_id : int
            The input edge index. Must be the index of a valid edge in the original
            graph.

        Returns
        -------
        int
            The arc index of the corresponding arc in the residual graph.
        """
        return self._impl.get_residual_graph_arc_id(edge_id)

    def get_transpose_arc_id(self, arc: Arc) -> int:
        """
        Given a forward or reverse arc in the network's residual graph, get the index of
        its transpose arc.

        If the input arc was a forward arc from `tail` to `head`, its transpose arc is a
        reverse arc from `head` to `tail`. Likewise, if the input arc was a reverse arc
        from `head` to `tail`, its transpose arc is a forward arc from `tail` to `head`.

        Parameters
        ----------
        arc : Arc
            The input arc. Must be a valid arc in the network's residual graph (though
            its residual capacity may be zero).

        Returns
        -------
        int
            The arc index of the transpose arc.
        """
        return self._impl.get_transpose_arc_id(arc)

    def arc_capacity(self, arc: Arc) -> Flow:
        """
        Get the upper capacity of an arc in the network.

        Parameters
        ----------
        arc : Arc
            The input arc. Must be a valid arc in the network's residual graph (though
            its residual capacity may be zero).

        Returns
        -------
        Flow
            The upper capacity of the arc.
        """
        return self._impl.arc_capacity(arc)

    def is_arc_saturated(self, arc: Arc) -> bool:
        """
        Check whether an arc is saturated.

        Parameters
        ----------
        arc : Arc
            The input arc. Must be a valid arc in the network's residual graph (though
            its residual capacity may be zero).

        Returns
        -------
        bool
            True if the arc is saturated (i.e. its residual capacity is zero); otherwise
            false.
        """
        return self._impl.is_arc_saturated(arc)

    def arc_residual_capacity(self, arc: Arc) -> Flow:
        """
        Get the residual capacity of an arc.

        Parameters
        ----------
        arc : Arc
            The input arc. Must be a valid arc in the network's residual graph (though
            its residual capacity may be zero).

        Returns
        -------
        Flow
            The residual capacity of the arc.
        """
        return self._impl.arc_residual_capacity(arc)

    def arc_flow(self, arc: Arc) -> Flow:
        """
        Get the amount of flow in an arc.

        Parameters
        ----------
        arc : Arc
            The input arc. Must be a valid arc in the network's residual graph (though
            its residual capacity may be zero).

        Returns
        -------
        Flow
            The amount of flow in the arc.
        """
        return self._impl.arc_flow(arc)

    def increase_arc_flow(self, arc: Arc, delta: Flow) -> None:
        """
        Increase flow in an arc.

        Adds `delta` units of flow to `arc` and removes `delta` units of flow from its
        corresponding transpose arc in the residual graph. Does not modify the
        excess/deficit of the arc's head or tail nodes.

        Parameters
        ----------
        arc : Arc
            The input arc. Must be a valid, unsaturated arc in the network.
        delta : Flow
            The amount of additional flow to add to the arc. Must be > 0 and <= the
            arc's residual capacity.
        """
        self._impl.increase_arc_flow(arc, delta)

    def node_excess(self, node: Node) -> Flow:
        return self._impl.node_excess(node)

    def increase_node_excess(self, node: Node, delta: Flow) -> None:
        self._impl.increase_node_excess(node, delta)

    def decrease_node_excess(self, node: Node, delta: Flow) -> None:
        self._impl.decrease_node_excess(node, delta)

    def is_excess_node(self, node: Node) -> bool:
        return self._impl.is_excess_node(node)

    def is_deficit_node(self, node: Node) -> bool:
        return self._impl.is_deficit_node(node)

    def excess_nodes(self) -> Iterable[Node]:
        return self._impl.excess_nodes()

    def deficit_nodes(self) -> Iterable[Node]:
        return self._impl.deficit_nodes()

    def total_excess(self) -> int:
        return self._impl.total_excess()

    def total_deficit(self) -> int:
        return self._impl.total_deficit()

    def is_balanced(self) -> bool:
        return self._impl.is_balanced()

    def node_potential(self, node: Node) -> Cost:
        return self._impl.node_potential(node)

    def increase_node_potential(self, node: Node, delta: Cost) -> None:
        self._impl.increase_node_potential(node, delta)

    def decrease_node_potential(self, node: Node, delta: Cost) -> None:
        self._impl.decrease_node_potential(node, delta)

    def arc_cost(self, arc: Arc) -> Cost:
        return self._impl.arc_cost(arc)

    def arc_reduced_cost(self, arc: Arc, tail: Node, head: Node) -> Cost:
        return self._impl.arc_reduced_cost(arc=arc, tail=tail, head=head)

    def total_cost(self) -> Cost:
        return self._impl.total_cost()
