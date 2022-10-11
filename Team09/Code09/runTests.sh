#!/bin/bash

# THE OUTPUT DIRECTORY
outputDir=../Tests09/output-system-tests

# AutoTester and test folder
autoTester=./Debug/AutoTester.exe
testFolder=../Tests09

# Week 6
$autoTester $testFolder/Sample_source.txt $testFolder/Sample_queries.txt $outputDir/Sample_output.xml > $outputDir/Sample_output.txt

# Invalid queries. Should terminate with error
$autoTester $testFolder/InvalidSimple1_source.txt $testFolder/Sample_queries.txt $outputDir/InvalidSimple1.xml > $outputDir/InvalidSimple1.txt
$autoTester $testFolder/InvalidSimple2_source.txt $testFolder/Sample_queries.txt $outputDir/InvalidSimple2.xml > $outputDir/InvalidSimple2.txt

# Week 8 Demo
$autoTester $testFolder/Week8Demo_source.txt $testFolder/Week8Demo_queries.txt $outputDir/Week8Demo_output.xml > $outputDir/Week8Demo_output.txt
