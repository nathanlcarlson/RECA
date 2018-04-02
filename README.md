### Info
This repository contains the development of a framework to support the creation and research of new Monte Carlo simulation algorithms. It also will contain these algorithms and their development.

### Batch Runs
A Python script is included to run batch jobs. A configuration file using JSON is used to specify parameters for the simulations

### Configuration Options
| Option        | Help         | Default | Type     |
| ------------- |:-------------|---------|----------|
| *beta* | Beta, inverse temperature | None | Number
| *width* | Simulation is square array, this is the width to use | None | Integer
| *percent_RECA* | This is tuned to test RECA and Metropolis mixes | None | Number, 0 - 100
| *n_states* | Number of States to use | None | Integer, greater than or equal to 2
| *seed* | Seed for Random Number Generator | Random Integer 0 - 2^20 | Integer
| *cpu_seconds* | Number of CPU seconds to evolve the system for | 30 | Number
| *samples* | Number of times to simulate each set of parameters | None | Integer
| *root* | The root level for data file storage | None | String

### Use
usage: run_job.py [-h] -f JSON

arguments:

  -h, --help            show this help message and exit

  -f JSON, --config JSON  JSON config file


### Additional Info
#### Algorithms
Several algorithms are implemented in a file. Note though that the idea is anyone using this structure can 'plug-in' their own algorithms.
#### State
Stores information about the system being simulated. It is attached closely to algorithm objects. Allows for the user to implement their own energy function (Hamiltonian).
#### Couplings
Simulations generally evolve based on the interaction between 'nodes'. This class provides a tool to build a 2D periodic or non-periodic array of nodes.
#### Utils
Useful functionalities.
