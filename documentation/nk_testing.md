# Testing Neon-Kit Event Driven System implementation

Along Event Driven System sources unit-tests are bundled in `tests` directory.

Unit-testing can be run in the following ways:
- natively on Ubuntu machine
- using Docker Ubuntu virtual machine

## Testing on Ubuntu machine

In order to execute unit-tests do the following:

    sudo apt-get install git make gcc
    git submodule update --init --force --recursive
    cd build
    make test

## Testing using Docker

The main docker file is `Dockerfile` in top-level project directory. Start the
testing using this dockerfile. On a linux machine do:

    make test

