#!/bin/bash

# CREATE THE OUTPUT DIRECTORY
outputDir=../Tests09/output-system-tests
mkdir -p $outputDir

# AutoTester and test folder
autoTester=./Debug/AutoTester.exe
testFolder=../Tests09

# Week 6
$autoTester $testFolder/Sample_source.txt $testFolder/Sample_queries.txt $outputDir/Sample_output.xml > $outputDir/Sample_output.txt
