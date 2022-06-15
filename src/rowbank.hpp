#ifndef ROWBANK_incl_
#define ROWBANK_incl_

#include <vector>
#include "serialize.hpp"
#include "cell.hpp"

class RowBank : public BlobNode {
  std::vector<Row> rows_;
  public:

  virtual Blob toBlob() const;
  static RowBank deserialize(const Blob& b);
};

#endif
