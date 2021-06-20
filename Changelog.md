<!--
Copyright 2021 Felix Bertoni

SPDX-License-Identifier: MIT
-->

# Changelog {#Changelog}

@page changelog changelog

This page lists changes happening at each release. 

# 0.0.1 - 2021.20.06

## Contributors 

Felix Bertoni (feloxyde), felix.bertoni987@gmail.com

## Features added
- fatigue 
    - Basic options, as test filtering and help display
    - method chaining

- Test class
    - checks calling basic comparison operator
    - checks for any boolean value
    - checks for exceptions
    - custom output through messages and ```ftg::to_string```
    - loading/unloading for resource management

- Suite class
    - grouping tests
    - recursive containment

- DefaultRunner (internal class)
    - runs tests sequentially
    - supports selection filters, 
    - supports value, name and type display.

- Checker (internal class) 
    - Enforces verifications mechanisms through privateness and friendship

## Docs additions
- Main page
- Tutorial 
    - install 
    - basic example
    - checks 
    - extending fatigue
    - corresponding automated tests
- Architecture 
- Develoment guide

## Other additions
- Added Code Of Conduct
- CI configuration
- Install as sources through CMake
