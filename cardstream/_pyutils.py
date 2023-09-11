from typing import Tuple

import numpy as np
from numpy import typing as npt
import pandas as pd


def _strat_to_numpy_arrayfmt(
    *,
    strat_relpath: str,
    count_relpath: str,
) -> Tuple[np.ndarray]:
    """ 
    Convert strategy and count configurations into C-order numpy arrays. 
    NOTE: These must be in an identical format to the preconfigured templates.
    """
    strat = pd.read_csv(strat_relpath, header=None)

    hard = strat.iloc[0:18, :].copy().reset_index(drop=True)
    hard = hard.values.astype(str)
    hrd = hard[1:, 1:].astype("S1", order="C")

    soft = strat.iloc[18:27, :].copy().reset_index(drop=True)
    soft = soft.values.astype(str)
    sft = soft[1:, 1:].astype("S1", order="C")

    splits = strat.iloc[27:38, :].copy().reset_index(drop=True)
    splits = splits.values.astype(str)
    splts = splits[1:, 1:].astype("S1", order="C")

    count = pd.read_csv(count_relpath, header=None)
    cnt = cnt = count.iloc[:, 1].values.astype(float, order="C")

    return hrd, sft, splts, cnt
