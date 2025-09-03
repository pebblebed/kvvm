# kvvm-cpp

Experimenting with a few ideas for a bizarre, post-modern database.

All low-level storage is as immutable, content-addressable blobs. Garbage collection, caching, and so on are deferred to other, hopefully smarter layers.

Absolutely all data transformations are framed as UDFs. Since UDFs are pure functions of their inputs, and the UDF bodies themselves have a hashable content, the intermediate results of queries are memoizable (and cacheable) in the same way as all low-level row data is.

# Rust and WASM

This part of the world is still insanely immature, and gets a lot of excuses made for it for some reason. I
am not able to figure out how to build a C++ project that utilizes statically compiled WASM UDFs, for instance,
and perhaps attempting to do so is missing the point? So some host setup, and manual work, is required to get
the UDF tests passing.

```rustup target add wasm32-wasip1
cd src/udfs/rust-hello-world
cargo build --target wasm32-wasip1
file target/wasm32-wasip1/debug/hello_world.wasm 
```