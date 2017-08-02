# disruptor-cpp

A rewrite of the open source project LMAX-Exchange/Disruptor in C++.

Disruptor is an inter-thread communication queue that handles classic producer and
consumer paradigm. It comes with a few minor changes, of which the most
prominent one is the ability to multi-cast the same message to all consumers. It also
allows consumer to have order (dependence) as it is often the case in real
world applications. The project was originally developed by LMAX-Exchange to handle
low latency high throughput scenarios. It aimed to increase sharing and avoid locking
as much as possible. Disruptor's java version was then published as an open source
project for general inter-thread message distribution.

The disruptor system is evolving. Albeit some similar efforts, this C++ version is
based on the latest disruptor version (v3.3), providing up-to-date native support to the
C++ community. In addition, it includes a comprehensive set of perf tests; as they often
say, the number one rule in performance optimization is "Don't just guess, test!"
The tests also serve as examples for one2one, one2many, and many2many use cases.

To build the examples:

```
cd ./build
cmake ..
make
```

The individual binaries generated are perf tests (including benchmark) for the project.
