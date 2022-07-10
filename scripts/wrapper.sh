#!/bin/bash

cat $2 | bin/compiler 2> out/log.txt 1> $4
