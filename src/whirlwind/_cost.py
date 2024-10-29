import importlib.resources
import pickle

import numpy as np
import scipy.ndimage

__all__ = [
    "calc_smooth_phase_gradients",
    "compute_carballo_costs",
    "load_carballo_pdf_splines",
]


def calc_smooth_phase_gradients(igram):
    """ """
    dy_igram = igram[1:, :] * igram[:-1, :].conj()
    dx_igram = igram[:, 1:] * igram[:, :-1].conj()

    phase_dy = np.angle(dy_igram)
    phase_dx = np.angle(dx_igram)

    phase_dy_smooth = scipy.ndimage.uniform_filter(
        phase_dy, size=(7, 7), mode="nearest"
    )
    phase_dx_smooth = scipy.ndimage.uniform_filter(
        phase_dx, size=(7, 7), mode="nearest"
    )

    return phase_dy_smooth, phase_dx_smooth


def load_carballo_pdf_splines():
    """ """
    files = importlib.resources.files(__package__)

    with files.joinpath("carballo-pdf-0-spline.pkl").open("rb") as f:
        spline_pdf0 = pickle.load(f)
    with files.joinpath("carballo-pdf-1-spline.pkl").open("rb") as f:
        spline_pdf1 = pickle.load(f)

    return spline_pdf0, spline_pdf1


def compute_carballo_costs(igram, corr, nlooks, mask, batch_size: int = 1000):
    """Compute phase gradient costs for unwrapping grid."""
    phase_dy_smooth, phase_dx_smooth = calc_smooth_phase_gradients(igram)

    corr = np.asanyarray(corr)
    corr_dy = np.minimum(corr[1:, :], corr[:-1, :])
    corr_dx = np.minimum(corr[:, 1:], corr[:, :-1])

    spline_pdf0, spline_pdf1 = load_carballo_pdf_splines()

    def compute_cost(phase_diff, min_corr, batch_size=batch_size):
        total_size = phase_diff.size
        costs = np.empty_like(phase_diff)

        for start_idx in range(0, total_size, batch_size):
            end_idx = min(start_idx + batch_size, total_size)
            # Flatten the input arrays for the batch
            phase_batch = phase_diff.ravel()[start_idx:end_idx]
            corr_batch = min_corr.ravel()[start_idx:end_idx]

            # Compute probabilities for the batch
            p1_batch = spline_pdf1((phase_batch, corr_batch, nlooks))
            p0_batch = spline_pdf0((phase_batch, corr_batch, nlooks))

            # Store results back in original shape
            costs.ravel()[start_idx:end_idx] = -np.log(p1_batch / p0_batch)

        return costs

    # Calculate costs with batched processing
    cost_up = compute_cost(-phase_dx_smooth, corr_dx)
    cost_lt = compute_cost(phase_dy_smooth, corr_dy)
    cost_dn = compute_cost(phase_dx_smooth, corr_dx)
    cost_rt = compute_cost(-phase_dy_smooth, corr_dy)

    if mask is not None:
        mask = np.asanyarray(mask)
        mask_dy = np.logical_and(mask[1:, :], mask[:-1, :])
        mask_dx = np.logical_and(mask[:, 1:], mask[:, :-1])

        cost_dn[mask_dx] = np.nan
        cost_up[mask_dx] = np.nan
        cost_rt[mask_dy] = np.nan
        cost_lt[mask_dy] = np.nan

    # Original concatenation logic since that wasn't the memory issue
    cost = np.ascontiguousarray(
        np.concatenate(
            [
                np.pad(cost_up, pad_width=[(0, 0), (1, 1)]).flatten(),
                np.pad(cost_lt, pad_width=[(1, 1), (0, 0)]).flatten(),
                np.pad(cost_dn, pad_width=[(0, 0), (1, 1)]).flatten(),
                np.pad(cost_rt, pad_width=[(1, 1), (0, 0)]).flatten(),
            ]
        )
    )
    cost[np.isnan(cost)] = 0.0
    return (100.0 * cost).astype(np.int32)
