###Info
This repository contains the development of a framework to support the creation and research of new Monte Carlo simulation algorithms. Naturally, it also will contain these algorithms and their development.
##Algorithms
Several algorithms are implemented in a file. Note though that the idea is anyone using this structure can 'plug-in' their own algorithms.
##State
Stores information about the system being simulated. It is attached closely to algorithm objects. Allows for the user to implement their own energy function (Hamiltonian).
##Couplings
Simulations generally evolve based on the interaction between 'nodes'. This class provides a tool to build a 2D periodic or non-periodic array of nodes.
##Utils
Useful functionalities.
