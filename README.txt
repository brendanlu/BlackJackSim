IN PROGRESS
-----------
cardstream is yet another BlackJack simulator in Python, with core game logic 
written in C++.


It aims to:
-----------
    1. Implement an intuitive object-oriented abstraction of BlackJack in C++, 
    which allows for nonstandard game mechanisms (push on 22, side bets on 
    special hands etc.) to be added fairly easily.

    2. Allow users to configure:
        - playing strategy
        - (TODO) betting strategy 
        - card counting strategy
        - number of decks
        - deck penetration (before reshuffle)
        - dealer action on soft-17s 
        - (TODO) BlackJack payout multiplier
    at runtime using Python arguments and csv configuration files.

    3. Log data from simulation runs using one of two main approaches:
        - (1) write all the data to a csv file.
        - (2) concurrently send the data through a socket to a different 
        process, which extracts key statistics from the data based on 
        user-defined functions, and avoids large, slow writes of redundant 
        information to file.

    4. Ultimately, ship as a Python package, and ideally avoid any external C++
    libraries (Boost, Asio C++) that add headaches for those who want to build 
    and develop themselves (using Cython).

Most of the core stuff is here, but there's a bit more Python programming, 
testing, benchmarking, and refining, before it can become a useful tool for 
analysing the game of BlackJack. 


To get developing:
------------------
    1. (Recommended) Create a new Python virtual environment

    2. Run:
        pip install -r requirements.txt

    3. Ensure that you have installed the same C compiler which was used to 
    build Python on your system 

    4. To check things are working, run:
        pytest . 

    5. Write some code

    6. Autoformat the code by running: 
        black . 

    7. If needed, add some new tests, and check things work with: 
        pytest . 

    8. Clean up your directory before the next git commit with: 
        python clean.py
