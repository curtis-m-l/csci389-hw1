# csci389-hw1
Problem #1:
  While most of the specifics are explained in the body of measure_memory_latency.cc,
  the main ideas behind it are as follows:

    The program creates a buffer of a given size (ranging from 2^10-2^28) and
    fills it with random character values from 'a' to 'z'. The particular
    characters aren't important; they just have to be random enough that the
    compiler can't detect a pattern and optimize it.

    Once the buffer has been initialized and filled, random bytes are read out
    of it an arbitrary number of times to ensure that the data is being stored
    in the highest cache that the CPU can fit it into. Then, the program measures
    how long it takes to read a fixed number of bytes from the buffer and averages
    the result.

    In order to ensure the compiler doesn't throw away the whole buffer, the
    ascii values of each random character are summed and printed. The time taken
    to add each ascii value to the sum is also included in the time measurement,
    which may increase the result slightly.

Problem #2:

  ![](screenshot.png)

    Bytes Read:   Time Taken Per Byte:
    2^10          0.6695
    2^11          0.6311
    2^12          0.7217
    2^13          0.7257
    2^14          0.6872
    2^15          0.7094
    2^16          0.8118
    2^17          0.9356
    2^18          2.6877
    2^19          2.1956
    2^20          3.6867
    2^21          4.9607
    2^22          6.0017
    2^23          7.0809
    2^24          7.5461
    2^25          8.1559
    2^26          8.7941
    2^27          12.2806
    2^28          18.8876

Problem #3:

(1) Based on the above numbers, the L1 cache in my computer should be able to
    hold at least 2^17 bytes of data. (Assuming that an average read from L1
    would be around 1ns) Beyond that point, there are two distinct categories
    that help narrow down the remaining times. 2^28 is consistently higher than
    2^27, and it stands to reason that this is the jump between the L3 cache
    and DRAM. In between those two sections is a fairly linear progression,
    which makes it hard to distinguish between the L2 and L3 caches. The largest
    jump in time normally occurs between 2^20 and 2^21, which would mean that my
    computer has the following cache sizes:

    L1: 2^17 bytes (131 KB)
    L2: 2^20 bytes (1048 KB)
    L3: 2^27 bytes (~134 MB)
    (Or more accurately, this is the largest size that the caches could hold
    when measuring in powers of 2)

(2) Assuming the previous guesses are true, my results aren't totally consistent
    with the numbers provided. My L1 cache runs closer to 1ns than 0.5ns, and my
    L2 cache runs significantly faster than 7ns. No measurement for L3 is given,
    but my DRAM read time is significantly faster than 100ns. This could be due
    to the prefetcher succeeding much more often than expected, or an
    inconsistency between the read times of my current DRAM and the average
    read time when those results were reported.

(3) Actual Processor Specs:
          CPU:    Intel Core i5-8300H
    L1d cache:    32K
    L1i cache:    32K
     L2 cache:    256K
     L3 cache:    8192K

    If the code were functioning properly, then the results should be as follows:
    L1d cache:    2^14 bytes
     L2 cache:    2^18 bytes
     L3 cache:    2^22 bytes

    The cache sizes reported by the test program are consistently higher than
    the actual sizes of my caches, which implies that either the optimizer was
    able to determine some pattern and streamline the results, or that the
    prefetcher was able to predict values more often than expected at runtime.
