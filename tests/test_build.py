import glob
import os
import pytest
import shutil
import subprocess
import sys
from typing import List

# pytest runs in the parent directory of this script (the root of the repository)
MODULE_DIR = "./cardstream/"
SETUP_DIR = "./"
TEST_DIR = "./tests/"

LOGFNAME = "LOG2.csv"


@pytest.fixture(scope="session", autouse=True)
def setup_before_all_tests():
    """
    A really hackish way to initiate a build.
    """

    make_clean_build_env()
    invoke_setup_script(["build_ext", "--inplace"])
    assert glob.glob(os.path.join(MODULE_DIR, "*.pyd")), "No extension module found"

    import cardstream

    global cs
    cs = cardstream

    yield

    # make_clean_dir()
    invoke_setup_script(["clean", "--all"])


@pytest.mark.order(1)
def test_strat_input_utils():
    global strat_args
    strat_args = cs._strat_to_numpy_arrayfmt(
        strat_relpath=TEST_DIR + "strategy/BasicNoDeviations-4to8Decks-HitSoft17.csv",
        count_relpath=TEST_DIR + "strategy/HiLoCount.csv",
    )


@pytest.mark.order(2)
def test_simulator_constructor():
    global test_simulator
    test_simulator = cs._wrappers._SimEngineWrapper(
        6, 0.5, True, 3, [strat_args for i in range(3)]
    )


@pytest.mark.order(3)
def test_log_file_config():
    test_simulator.py_set_log_file(LOGFNAME)


@pytest.mark.order(4)
def test_socket_config():
    test_simulator.py_set_socket_connection("127.0.0.1", 11111)


@pytest.mark.order(5)
def test_simulation_run():
    make_clean_dir()
    test_simulator.run(1000)
    assert os.path.isfile(
        os.path.join(os.getcwd(), LOGFNAME)
    ), f"{LOGFNAME} does not exist in the current directory."


def make_clean_build_env():
    # build folder
    try:
        shutil.rmtree(os.path.join(MODULE_DIR, "build"))
    except FileNotFoundError:
        pass

    # Cython generated C++ file
    try:
        os.remove(os.path.join(MODULE_DIR, "_wrappers.cpp"))
    except FileNotFoundError:
        pass

    # compiled extensions
    for file in os.listdir(MODULE_DIR):
        if file.endswith(".pyd") and file.startswith("_wrappers"):
            # do not place in try-except block
            os.remove(os.path.join(MODULE_DIR, file))


def make_clean_dir():
    # clean from a simulation run
    for file in os.listdir():
        if "LOG" in file or "ERROR" in file:
            os.remove(os.path.join(file))


def invoke_setup_script(args: List[str]):
    """
    Hacky way to invoke the setup.py file.
    """
    original_dir = os.getcwd()
    os.chdir(SETUP_DIR)
    assert (
        subprocess.run([sys.executable, "setup.py"] + args).returncode == 0
    ), f"Setup script with {args} arguments failed!"
    os.chdir(original_dir)
