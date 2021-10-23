# runtime_template
fairly simple way of specializing template functions at runtime.


# How to run:
just run `./run.sh` and it will compile to an executable `template_test`.

To test it, try the following examples:
```
./template_test size_t void
./template_test "std::pair<size_t, char*>" "std::char_traits<wchar_t>"
... or whatever you can think of.
```

Also you can tinker with the `header.inc` file to your needs (as long as you can make it compile).

Since it is source code gen so it can do anything you want. But the goal is that we provide a way to specialize template functions at runtime (for LLVM code generation).

There are many many ways to improve it, the best way to do is to follow the path of `clangJIT` ( https://arxiv.org/abs/1904.08555 ), but it is incremental improvement because it does not change the interface.
