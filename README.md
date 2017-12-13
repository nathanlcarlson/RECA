### Info
This repository contains the development of a framework to support the creation and research of new Monte Carlo simulation algorithms. It also will contain these algorithms and their development.

### Batch Runs
A Python script is included to run batch jobs. A configuration file using YAML formatting is used to specify parameters for the simulations

### Configuration Options
| Option        | Help         | Default | Type     |
| ------------- |:-------------|---------|----------|
| *beta* | Values of Beta, inverse temperature | [1] | List of Numbers
| *width* | Simulation is square array, these are the widths to use | [10] | List of Integers
| *percent_RECA* | This is tuned to test RECA and Metropolis mixes | [5] | List of Numbers, 0 - 100
| *steps* | Number of Monte Carlo steps in a simulation | 2^15 | Integer
| *samples* | Number of times to simulate each set of parameters | 5 | Integer
| *root* | The root level for data file storage | 'data' | String

### Use
usage: run_job.py [-h] -f YAML

arguments:

  -h, --help            show this help message and exit

  -f YAML, --yaml YAML  YAML config file


### Additional Info
#### Algorithms
Several algorithms are implemented in a file. Note though that the idea is anyone using this structure can 'plug-in' their own algorithms.
#### State
Stores information about the system being simulated. It is attached closely to algorithm objects. Allows for the user to implement their own energy function (Hamiltonian).
#### Couplings
Simulations generally evolve based on the interaction between 'nodes'. This class provides a tool to build a 2D periodic or non-periodic array of nodes.
#### Utils
Useful functionalities.
