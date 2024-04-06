# Algorithmic Model Checking : Petri Net Symbolic Model Checker

> **!! Make sure you do NOT create a public fork of this repository with your solution !!**

Instead, you can import this repository as a private repository as follows:
1. Go to [https://github.com/new/import](https://github.com/new/import) (log in with your personal/student account)
2. Click "Private" and select `https://github.com/jacopol/amc-petri-nets` for import
3. Before submitting, add me `jacopol` to your private project
under _settings/collaborators_.

## Dependencies

> **WINDOWS USERS:** To not cause any frustrations, we recommend you use [*Windows Subsystem for
> Linux*](https://learn.microsoft.com/en-us/windows/wsl/install) or a virtual machine and then
> proceeed as if you were using Ubuntu.

### Compiler and Build System

To compile the project, you need a C and C++ compiler together with the CMake build system. These
can be installed with your package manager with the respective command below.

| Operating System | Shell command                      |
|------------------|------------------------------------|
| Ubuntu 22+       | `apt install        cmake g++    ` |
| Fedora 36+       | `dnf install        cmake gcc-c++` |
| Arch Linux       | `pacman -S --needed cmake gcc    ` |
| MacOS            | `homebrew install   cmake gcc    ` |

### Sylvan

For the "symbolic" part of the model checker, we use the
[Sylvan](https://github.com/trolando/sylvan) BDD package. For your convenience, it is included as a
submodule.
```bash
git submodule update --init --recursive
```

For Sylvan, you also need the *GNU Multiple Precision Arithmetic* and *Portable Hardware Locality*
libraries. These can be installed with your package manager with the respective command below.

| Operating System | Shell command                                |
|------------------|----------------------------------------------|
| Ubuntu 22+       | `apt install        libgmp-dev libhwloc-dev` |
| Fedora 36+       | `dnf install        gmp-devel  hwloc-devel ` |
| Arch Linux       | `pacman -S --needed gmp        hwloc       ` |
| MacOS            | `homebrew install   gmp        hwloc       ` |

### PugiXML

To parse *.pnml* inputs, we depend on [pugixml](https://github.com/zeux/pugixml). For your
convenience, it is included as a submodule.

```bash
git submodule update --init --recursive
```

### Graphviz (dot) (optional dependency for debugging)

You can visually inspect the BDDs by exporting them to *.dot* files with `bdd_printdot(f, ...)`,
and displaying the dot file in an online dot-viewer, like [GraphvizOnline](https://dreampuf.github.io/GraphvizOnline).
Alternatively, dot-files can be converted to svg/pdf/... with *dot*, included as part of *graphviz*, which you can obtain with the respective command below.

| Operating System | Shell command                 |
|------------------|-------------------------------|
| Ubuntu 22+       | `apt install        graphviz` |
| Fedora 36+       | `dnf install        graphviz` |
| Arch Linux       | `pacman -S --needed graphviz` |
| MacOS            | `homebrew install   graphviz` |

## Compiling and Running

After installing the dependencies, you can build your project as follows:

```bash
make build
```

Other make targets are:

| target  | effect                                |
|---------|---------------------------------------|
| `build` | Build *main.cpp* and its dependencies |
| `clean` | Remove all build files                |
| `run`   | Run the *main.cpp* executable         |

You can parse the input file with the parameter *i*. The parameter *M* is the amount of memory (in
*MiB*) to use for the BDD package. For example, to run the program with *i* set to its default of
*pnml/cycle_4.pnml* and *M* its default of *1024* (1 GiB) write:

```bash
make run i=pnml/cycle_4.pnml M=1024
```

## Getting Started with C++ development

Before getting started on the assignment below, you should at least
acquaint yourself with the functions declared in *src/bdd.h*. This is the relevant subset of a BDD
interface you need for this assignment.

All of these functions are prefixed with `bdd_`. This makes it easier for your *intellisense* to
discover the relevant functions.

### Visual Studio Code

You can use any editor of your liking. 
If you use Visual Studio Code as your editor, install these *extensions* to get intellisense.

| Name                         | ID                        |
|------------------------------|---------------------------|
| **CMake Tools**              | ms-vscode.cmake-tools     |
| **C/C++**                    | ms-vscode.cpptools        |

## Assignment

For the assignment, solve the following task (in this order):

### Compulsory tasks:

1. *src/closure.cpp*: Implement the function `closure(init_states, transitions)` to compute the set
   of all reachable states from *init_states* by repeated use of the given *transitions*.

2. *src/ctl.cpp*: Implement the remaining *CTL operators*. The CTL test cases can be modified in *src/ctl_test.h*

### Optional tasks:

3. detect deadlocks

4. implement CTL with Fairness

5. generate counter-examples

## Authors

Steffan Sølvsten, Jaco van de Pol, Aarhus University, April 2024