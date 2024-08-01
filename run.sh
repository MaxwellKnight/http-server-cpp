#!/bin/bash

for i in {1..50}
do
  echo "Run #$i"
  python3 client.py
done
