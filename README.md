This repo is effectively an abandoned experiment. After doing some significant research into performance on JS, I'm no longer in favor of a shared/xplat implementation for RX written in OCaml. If targeting bucklescript, you should instead write bindings to RxJS or MostJS and use those.

This repo could serve as the basis for a quality Native OCaml implementation, though in doing so I would re-evaluate some of the assumptions in the initial design.


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
