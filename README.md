# ⚙️ Task Scheduling with PSO Algorithm

An implementation of **Task Scheduling using Particle Swarm Optimization (PSO)** in C++. This project simulates the assignment of independent tasks to virtual machines (VMs) in a cloud computing environment to **minimize the makespan** and **improve load balancing**.

---

## 🐦 Particle Swarm Optimization (PSO)

- Each **particle** represents a candidate solution: a vector where each element indicates the VM assigned to a task.
- The **fitness function** is based on:
  - Makespan: time taken for the slowest VM to finish its assigned tasks.
  - Load Balancing: standard deviation of execution time across VMs.

> Fitness = 0.5 × (1 / Makespan) + 0.5 × (1 / Load Balancing Ratio)

---

## 🛠️ How to Compile & Run

```bash
g++ -o scheduler pso.cpp
./scheduler
