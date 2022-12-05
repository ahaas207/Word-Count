# Word-Count
An exploration of forking off child processes and threads.

Timing Word Count for ten lengthy files to determine which was fastest, 
best of ten runs for 4693223 total words:

The Traditional Processing (no forking/threading)
real 0m0.191s
user 0m0.087s
sys 0m0.012s

Forking Child Processes
real 0m0.040s
user 0m0.001s
sys 0m0.003s

Threading
real 0m0.052s
user 0m0.092s
sys 0m0.032s



