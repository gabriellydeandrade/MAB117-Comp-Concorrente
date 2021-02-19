import argparse
from random import random, SystemRandom

random = SystemRandom()

parser = argparse.ArgumentParser(description='Generate quicksort test files')
parser.add_argument('--threads', metavar='t', type=int, nargs='+', help='number of threads')
parser.add_argument('--power', metavar='e', type=int, help='10 ^ number of elements')

args = parser.parse_args()

elements = [random.randint(-1000000 , 1000000) for _ in range(10**args.power)]

for n_threads in args.threads:
    filename = f'10e{args.power}_elements-{n_threads}_threads.txt'

    with open(filename, 'w') as file:
        file.write(f'{10**args.power} {n_threads}\n')
        file.write(' '.join(map(str, elements)))

