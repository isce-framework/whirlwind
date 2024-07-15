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


def compute_carballo_costs(igram, corr, nlooks, mask):
    """ """
    phase_dy_smooth, phase_dx_smooth = calc_smooth_phase_gradients(igram)

    corr = np.asanyarray(corr)
    corr_dy = np.minimum(corr[1:, :], corr[:-1, :])
    corr_dx = np.minimum(corr[:, 1:], corr[:, :-1])

    spline_pdf0, spline_pdf1 = load_carballo_pdf_splines()

    def compute_cost(phase_diff, min_corr):
        p1 = spline_pdf1((phase_diff, min_corr, nlooks))
        p0 = spline_pdf0((phase_diff, min_corr, nlooks))
        return -np.log(p1 / p0)

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
    cost = (100.0 * cost).astype(np.int32)

    return cost
