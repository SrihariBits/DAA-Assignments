<br />
<p align="center">
  <a href="https://github.com/SrihariBits/DAA_assignment1"></a>
  <h2 align="center">DAA Assignment</h2>
  </p>
</p>


## Table of Contents

- [Table of Contents](#table-of-contents)
- [About The Project](#about-the-project)
  - [Dependencies](#dependencies)
  - [Run An Algorithm](#run-an-algorithm)
- [Project Layout](#project-layout)
- [Project Contributors](#project-contributors)


## About The Project

This repo has both [Assignment 1](./Assign1src) and [Assignment 2](./Assign2src)  in 
Assign1src and Assign2src directories respectly. The docuemntation for both can be found
in the `docs` directory in their respective folders

### Dependencies

Dependencies required to compile the code
 - g++ from the GNU compiler suite to compile the source code
 - Doxygen to generate documentation from the source files

### Run An Algorithm
 
1. Clone the DAA_assignment1
```sh
git clone https://github.com/SrihariBits/DAA_assignment1.git
cd DAA_assignment1
```
2. Go to the required directory (assignment 1 or 2)
```sh
cd Assign1src # or Assign2src
```
3. Compile a source file
```sh
g++ kosaraju.cpp
```
4. Run the program
```sh
./a.out ../datasets/CollegeMsg.txt # kosaraju algo requires the vertices and edges as input.
```

## Project Layout

```sh
Root Directory
├── Assign1src
│   ├── docs # Documentation
│   ├── dcsc.cpp
│   └── kosaraju.cpp
├── Assing2src
│   ├── docs # Documentation
│   ├── bipartiteMatching.cpp
│   ├── DAA_FordFulkerson.cpp
│   ├── FordFulkerson.h
│   └── minCut.cpp
├── datasets # All the datasets used in the assignment
├── gephi # gephi tools working direcotory
└── README.md # this README file
```

## Project Contributors

 - L Srihari - 2017A7PS1670H
 - Raj Kashyap Mallala - 2017A7PS0025H
 - T Naga Sai Bharath - 2017A7PS0209H
 - Rikil Gajarla - 2017A7PS0202H

Repo Link: [https://github.com/SrihariBits/DAA_assignment1](https://github.com/SrihariBits/DAA_assignment1)