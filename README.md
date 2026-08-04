*This project has been created as part of the 42 curriculum by darafael.*

## Description

This project is an implementation of the classic **Dining Philosophers** synchronization problem.

One or more philosophers sit at a round table with a large bowl of spaghetti in the middle. There are as many forks as philosophers. Each philosopher must pick up both adjacent forks to eat. Philosophers cycle through eating, sleeping, and thinking. The simulation ends when a philosopher dies of starvation, or when all philosophers have eaten a required number of times.

The mandatory part implements each philosopher as a **POSIX thread**, with each fork protected by a **mutex** to prevent data races.

**Key rules:**
- A philosopher dies if they have not started eating within `time_to_die` ms of their last meal (or simulation start).
- Philosophers do not communicate with each other.
- Death must be reported within 10 ms of occurrence.
- No global variables; no data races.

## Features

- Full thread-based simulation using `pthread_create` / `pthread_join`.
- One mutex per fork, preventing two neighbors from using the same fork at once.
- A dedicated **monitor thread** that continuously checks each philosopher's last meal time to detect starvation.
- Per-philosopher `meal_mutex` protecting the `last_meal` timestamp against concurrent read/write.
- A single `print_mutex` guaranteeing log lines are never interleaved.
- Correct handling of edge cases:
  - A single philosopher (only one fork exists, so they starve without ever eating).
  - The optional `number_of_times_each_philosopher_must_eat` argument, stopping the simulation once satisfied.
  - Strict input validation (only positive integers, overflow checks).

## Project Structure

```
philo/
├── Makefile        # NAME, all, clean, fclean, re
├── philo.h         # Structs (t_data, t_philo, t_fork) and prototypes
├── main.c          # Entry point, argument parsing, start_simulation
├── init.c          # Data/fork/philosopher initialization, cleanup
├── routine.c       # Philosopher lifecycle (eat, sleep, think, take forks)
├── monitor.c       # Monitor thread: death detection, meal-count check
└── utils.c         # Time helpers, logging, ft_atoi, argument validation
```

## Technical Choices

- **Deadlock prevention:** forks are always picked up in a consistent order relative to fork index (lowest index first), so philosophers never all wait on the same fork simultaneously in a circular chain.
- **Timing:** `gettimeofday` combined with a custom `get_time()` helper gives millisecond-precision timestamps; `ft_usleep` performs sleeping in short loops to stay responsive to the simulation-stop flag instead of one long blocking `usleep`.
- **42 Norm compliance:** every function respects the constraints below, which were verified while writing and reviewing the code:
  - Max 4 parameters per function
  - Max 5 functions per file
  - Max 25 lines per function
  - No `for` loops (`while` only) and no ternary operators
  - No global variables

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

### Testing

Useful checks while developing or reviewing this project:

```bash
# Check for data races (requires a build with -fsanitize=thread, or use valgrind's tsan-like tools)
valgrind --tool=helgrind ./philo 4 410 200 200

# Check for memory leaks
valgrind --leak-check=full ./philo 5 800 200 200

# Stress test: many philosophers, tight timing
./philo 200 60 60 60

# Edge case: single philosopher (expected to die, only one fork)
./philo 1 800 200 200
```

## Resources

- [Dining Philosophers Problem – Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming – Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [pthread_mutex_lock(3) – Linux man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [pthread_create(3) – Linux man page](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
- [gettimeofday(2) – Linux man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)
- [Helgrind: a thread error detector – Valgrind documentation](https://valgrind.org/docs/manual/hg-manual.html)

**AI usage:** GitHub Copilot was used to assist with structuring synchronization logic, verifying deadlock-prevention reasoning (odd/even fork ordering), and reviewing line counts against the 42 Norm constraints. All code was reviewed, understood, and validated manually.
