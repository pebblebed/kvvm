# kvvm-cpp

Experimenting with a few ideas for a bizarre, post-modern database.

All low-level storage is as immutable, content-addressable blobs. Garbage collection, caching, and so on are deferred to other, hopefully smarter layers.

Absolutely all data transformations are framed as UDFs. Since UDFs are pure functions of their inputs, and the UDF bodies themselves have a hashable content, the intermediate results of queries are memoizable (and cacheable) in the same way as all low-level row data is.

