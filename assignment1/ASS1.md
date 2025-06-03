# Question 1

### a)

**processor** field contains the assigned identifier number to each logical CPU core. A logical CPU core is something that OS can treat as an independent CPU available for independent instructions. By use of hyperthreading a core can have multiple threads, i.e. multiple independent instructions that can be executed on them simultaneously. Each thread thus corresponds to a logical CPU core (not necessarily the physical cores present). This identifier is how OS can assign a process to the processor.

**cpu cores** field contains the total number of physical cores present in the processor.

## b)

File used: `/proc/cpuinfo`  ; command used `lscpu`
My virtual machine has 6 cores.

### c)

File used: `/proc/cpuinfo`  ; command used `lscpu`
6 processors (atleast what VM says it has)

### d)

File used: `/proc/cpuinfo` ; command used `lscpu`
Each processor in my VM seems to show $2096.060$ MHz. Although the actual machine shows variable results for each processor.

`lscpu` also shows the same result.

On the actual machine it shows the real time frequency of each processor, which is different for each one of them.  `lscpu` gives a CPU max and CPU min frequencies which are the limits in which processor is allowed to operate at.

## e)

File used: `/proc/cpuinfo`
Architecture is x86_64

## f)

File used: `/proc/meminfo`
Total physical memory which refers tho the amount of physical ram that system has is $4039356$ kB (approx 3.85 gB)

## g)

File used: `/proc/meminfo`
Of this the total free memory is $278436$ kB.

## h)

File used: `/proc/stat`

````
ctxt 6406047
processes 79492
````

ctxt tells the total number of context switches since boot.
processes tells the total number of forks since boot.

# Question 2

The top output:

|PID|USER|PR|NI|VIRT|RES|SHR|S|%CPU|%MEM|TIME+|COMMAND|
|---|----|--|--|----|---|---|-|----|----|-----|-------|
|14896|mentee|20|0|4384|716|652|R|100.0|0.0|0:38.78|cpu|

## a)

PID is 14896

## b)

It consumes 100% CPU and 0% memory

## c)

Current state is R. i.e. Running

# Question 3

Program : 'memory_1'

---

````
Virtual memory size (VSZ): 8304
Physical resident in memory (RSS): 756
````

Program : 'memory_2'

---

````
Virtual memory size (VSZ): 8300
Physical resident in memory (RSS): 3316
````

Program uses significantly less Physical memory than virtual memory. i.e. it thinks it has a lot of address space available when actually loaded in RAM is significantly less.

# Question 5
