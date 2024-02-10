IN PROGRESS
-----------
cardstream is a C++ BlackJack simulation engine, exposed to Python through some
clunky Cython wrappers. 


It aims to do the following:
----------------------------
    1. Implement an intuitive object-oriented abstraction of BlackJack in C++

    2. Allow users to configure different card playing/counting strategies, and 
    game variations easily, and interact with the simulation engine in Python

    3. Run simulations, and log the granular game data

    4. 
        (Simple users) Write the data into csv format

        (Advanced users) Leveraging concurrency and functional-programming in 
        Python, enable users to specify analytical operations to process the 
        logging data on-the-fly, and avoid (the slow process of) dumping it all 
        to file 

Most of the core stuff is here, but there's a bit more programming (on the 
Python side), testing, benchmarking, and refining, before it can become a useful 
tool for analysing the game of BlackJack. 


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
