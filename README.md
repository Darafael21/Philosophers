*This project has been created as part of the 42 curriculum by \<login\>.*

## Description

This project is an implementation of the classic **Dining Philosophers** synchronization problem.

One or more philosophers sit at a round table with a large bowl of spaghetti in the middle. There are as many forks as philosophers. Each philosopher must pick up both adjacent forks to eat. Philosophers cycle through eating, sleeping, and thinking. The simulation ends when a philosopher dies of starvation, or when all philosophers have eaten a required number of times.

The mandatory part implements each philosopher as a **POSIX thread**, with each fork protected by a **mutex** to prevent data races.

**Key rules:**
- A philosopher dies if they have not started eating within `time_to_die` ms of their last meal (or simulation start).
- Philosophers do not communicate with each other.
- Death must be reported within 10 ms of occurrence.
- No global variables; no data races.

## Instructions

### Compilation

```bash
cd philo
make
```

### Execution

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) at the table |
| `time_to_die` (ms) | Max time between start of last meal and start of next meal before death |
| `time_to_eat` (ms) | Time it takes to eat (holds two forks during this time) |
| `time_to_sleep` (ms) | Time spent sleeping |
| `number_of_times_each_philosopher_must_eat` | Optional: stop when all have eaten this many times |

### Examples

```bash
# 5 philosophers, should not die
./philo 5 800 200 200

# 4 philosophers, must each eat at least 7 times then stop
./philo 4 410 200 200 7

# 1 philosopher, will die (only one fork available)
./philo 1 800 200 200
```

### Cleanup

```bash
make fclean
```

## Resources

- [Dining Philosophers Problem – Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming – Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [pthread_mutex_lock(3) – Linux man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [gettimeofday(2) – Linux man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)

**AI usage:** GitHub Copilot was used to assist with structuring synchronization logic, verifying deadlock-prevention reasoning (odd/even fork ordering), and reviewing line counts against the 42 Norm constraints. All code was reviewed, understood, and validated manually.
