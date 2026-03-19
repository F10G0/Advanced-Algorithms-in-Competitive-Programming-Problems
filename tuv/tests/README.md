# Tests

This directory contains sample test cases and scripts for validating the implementation.

## Structure

- `sample_inputs/`: input files
- `sample_outputs/`: expected outputs
- `run_tests.sh`: test script for Linux / Ubuntu
- `run_tests.bat`: test script for Windows

## Run Tests (Linux)

```bash
cd tests
./run_tests.sh
```

## Run Tests (Windows)

```bat
cd tests
run_tests.bat
```

## Notes

- Each test runs the program on an input file and compares the output with the corresponding expected output.
- A test passes if the outputs match exactly.
