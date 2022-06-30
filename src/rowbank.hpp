#ifndef ROWBANK_incl_
#define ROWBANK_incl_

<<<<<<< HEAD
#include <vector>
=======
>>>>>>> 5ee66cd (Inroads to tabular append.)
#include "serialize.hpp"
#include "cell.hpp"

class RowBank : public BlobNode {
<<<<<<< HEAD
  std::vector<Row> rows_;
  public:

  virtual Blob toBlob() const;
  static RowBank deserialize(const Blob& b);
=======
    std::vector<Row> rows_;
    size_t max_;

    public:

    RowBank(size_t max = 512)
    : max_(max)
    { }

    size_t size() const;
    size_t max() const;
    bool isFull() const;
    virtual Blob toBlob() const;
    static RowBank deserialize(const Blob& b);

    // Weird API: returns nullable new RB in case full,
    // updates in place otherwise
    RowBank* append(const Row& r);
>>>>>>> 5ee66cd (Inroads to tabular append.)
};

#endif
