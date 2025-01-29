#!/bin/bash

for i in {1..300}
do
  echo "Run #$i"
  python3 client.py
done

wait  # Waits for all background jobs to finish before the script exits
