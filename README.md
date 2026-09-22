# Philosophers

My implementation of the **42 / 1337 Philosophers** project.

The project is based on the dining philosophers problem and focuses on synchronization, threads, mutexes, processes and semaphores.

## Mandatory

The mandatory version is inside `philo/`.

It uses POSIX threads and mutexes. Each philosopher repeatedly:

- thinks
- takes forks
- eats
- sleeps

The simulation also monitors the time since each philosopher's last meal and stops when a philosopher dies or, when provided, when every philosopher has eaten the required number of times.

## Bonus

The bonus version uses processes and semaphores instead of threads and individual fork mutexes.

## Build

Mandatory version:

```bash
cd philo
make
```

Run it with:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep
```

An optional fifth argument sets how many times each philosopher must eat:

```bash
./philo 5 800 200 200 7
```

Bonus version:

```bash
cd " philo_bonus"
make
```

Then run:

```bash
./philo_bonus 5 800 200 200
```

## What I practiced

- POSIX threads
- mutex synchronization
- avoiding race conditions
- timing and monitoring
- process creation
- semaphores
- shared simulation state
- resource cleanup

The project was written in C as part of the 42 / 1337 curriculum.

