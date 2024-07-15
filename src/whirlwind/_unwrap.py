import numpy as np
from numpy.typing import ArrayLike

from ._cost import compute_carballo_costs
from ._lib import integrate_unwrapped_gradients
from ._lib import residue as get_residues
from .graph import RectangularGridGraph
from .network import Network, primal_dual

__all__ = [
    "unwrap",
]


def unwrap(
    igram: ArrayLike,
    corr: ArrayLike,
    nlooks: float,
    *,
    mask: ArrayLike | None = None,
) -> np.ndarray:
    """ """
    phase = np.angle(igram)

    residue = get_residues(phase)

    surplus = residue.flatten()
    cost = compute_carballo_costs(igram, corr, nlooks, mask)

    graph = RectangularGridGraph(*residue.shape)
    network = Network(graph, surplus, cost, capacity=1)

    primal_dual(network, maxiter=8)

    return integrate_unwrapped_gradients(phase, network._impl)
