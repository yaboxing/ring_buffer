#!/bin/bash

echo "checkout all submodule"

pwd

git status

git submodule status

git submodule update --init --recursive

