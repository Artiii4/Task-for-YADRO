#include <vector>
#include <memory>
#include <string>
#include "Tape.h"
#include "Delays.h"

#ifndef SORTER_H
#define SORTER_H

class Sorter {
public:
  Sorter (int numBuckets, const std::string& directory);
  void sort(TapeHelper& input, TapeHelper& output);

private:
  struct BucketTape {
    std::unique_ptr<TapeHelper> tape;
    int size = 0;
    std::string name;
  };

  int numBuckets;
  std::string directory;
  int min = 0;
  int max = 0;

  std::vector<BucketTape> createBucket() const;
  void findMinMax(TapeHelper& tape);
  void distributeToBuckets(TapeHelper& inputTape, std::vector<BucketTape>& buckets);
  void mergeBuckets(std::vector<BucketTape>& buckets, TapeHelper& outputTape);
  void cleanup(const std::vector<BucketTape>& buckets) const;
};

#endif
