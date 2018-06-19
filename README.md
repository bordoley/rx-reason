# Reactive Extensions for Reason/OCaml 

An implementation of Rx for Reason/OCaml that is designed with the goal of providing a minimum base line feature set to support asynchronous dataflow patterns. Currently development is focused on bucklescript, but support for native ocaml is intended and desirable.

# Basic Concepts
* Observables, Singles and Maybes
* Disposables
* Subscribers and Operators
* Subjects
* Schedulers

# Build
```
cd ./Reactive.re
npm run build
```

# Testing

First build reunit. You only should have to do this once.
```
cd ./reUnit
npm run build
```

Then run the tests:
```
cd ./Reactive.re
npm run test
```

You can also test and generate code coverage reports:
```
cd ./Reactive.re
npm run test-coverage
```

Finally there is support for interactive debugging:
```
cd ./Reactive.re
npm run test-debug
```
