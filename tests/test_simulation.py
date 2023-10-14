import os
import pytest
from setuptools import setup
import shutil
import sys

import cardstream as cs

LOGFNAME = "LOG2.csv"
MODULE_DIR = "../cardstream/"


@pytest.fixture(scope="session", autouse=True)
def setup_before_all_tests():
    make_clean_dir()
    setup_script_path = "../setup.py"

    # Load and execute the setup.py script
    setup_script_globals = {}
    with open(setup_script_path, "rb") as f:
        exec(compile(f.read(), setup_script_path, "exec"), setup_script_globals)

    # Run the setup function in the setup.py script
    assert "setup" in setup_script_globals
    setup(**setup_script_globals["setup"])


@pytest.fixture(scope="session", autouse=True)
def teardown_after_all_tests():
    make_clean_dir


def test_strat_input_utils():
    global strat_args
    strat_args = cs._strat_to_numpy_arrayfmt(
        strat_relpath="teststrat/BasicNoDeviations-4to8Decks-HitSoft17.csv",
        count_relpath="teststrat/HiLoCount.csv",
    )


def test_simulator_constructor():
    global test_simulator
    test_simulator = cs._wrappers._SimEngineWrapper(
        6, 0.5, True, 3, [strat_args for i in range(3)]
    )


def test_log_file_config():
    test_simulator._py_set_log_file(LOGFNAME)


def test_socket_config():
    test_simulator._py_set_socket_connection("127.0.0.1", 11111)


def test_simulation_run():
    test_simulator.run(100)
    current_directory = os.getcwd()
    assert os.path.isfile(
        os.path.join(current_directory, LOGFNAME)
    ), f"{LOGFNAME} does not exist in the current directory."


def make_clean_dir():
    # remove build folder
    try:
        shutil.rmtree(MODULE_DIR + "build")
        print("Removed build folder")
    except FileNotFoundError:
        pass

    # Cython generated C++ file
    try:
        os.remove(MODULE_DIR + "_wrappers.cpp")
    except FileNotFoundError:
        pass

    # remove cython generated files, and compiled extensions
    for file in os.listdir(MODULE_DIR):
        if file.endswith(".pyd"):
            os.remove(MODULE_DIR + file)
            print("Removed", file)

    # clean from a simulation run
    for file in os.listdir():
        if "LOG" in file or "ERROR" in file:
            os.remove(file)
            print("Removed", file)
