#include "Sorter.h"
#include "Tape.h"

#include <filesystem>
#include <limits>
#include <algorithm>
#include <cmath>
#include <iostream>

Sorter::Sorter(int numBuckets, const std::string& directory)
  : numBuckets(numBuckets), directory(directory)
{
  std::filesystem::create_directories(directory);
}

std::vector<Sorter::BucketTape> Sorter::createBucket() const {
  std::vector<BucketTape> buckets(numBuckets);

  for (int i = 0; i < numBuckets; ++i) {
    buckets[i].name = directory + "/bucket" + std::to_string(i) + ".tmp";
    buckets[i].tape = std::make_unique<Tape>(buckets[i].name, Delays{ 1, 1, 1 }, true);
  }
  return buckets;
}

void Sorter::findMinMax(TapeHelper& tape) {
  min = std::numeric_limits<int>::max();
  max = std::numeric_limits<int>::min();

  tape.rewind();
  while (tape.getPosition() < tape.getLength()) {
    int value = tape.read();
    min = std::min(min, value);
    max = std::max(max, value);
    tape.moveRight();
  }
}

void Sorter::distributeToBuckets(TapeHelper& inputTape, std::vector<BucketTape>& buckets) {
  int actualBuckets = std::min(numBuckets, inputTape.getLength());

  if (actualBuckets != numBuckets) {
    numBuckets = actualBuckets;
    buckets.resize(numBuckets);
  }

  inputTape.rewind();

  std::vector<std::vector<int>> bucketValues(numBuckets);

  double step = (max - min) / static_cast<double>(numBuckets);
  if (step == 0) step = 1;

  while (inputTape.getPosition() < inputTape.getLength()) {
    int value = inputTape.read();
    int bucketIndex = static_cast<int>(std::floor((value - min) / step));

    if (bucketIndex >= numBuckets) {
      bucketIndex = numBuckets - 1;
    }
    bucketValues[bucketIndex].push_back(value);
    inputTape.moveRight();
  }

  for (int i = 0; i < numBuckets; ++i) {
    auto& values = bucketValues[i];
    auto& bucket = buckets[i];

    std::sort(values.begin(), values.end());

    for (int value : values) {
      bucket.tape->write(value);
      bucket.tape->moveRight();
      bucket.size++;
    }
    dynamic_cast<Tape*>(bucket.tape.get())->save();
    bucket.tape->rewind();
  }
}

void Sorter::mergeBuckets(std::vector<BucketTape>& buckets, TapeHelper& outputTape) {
  outputTape.rewind();
  int totalSize = 0;

  for (const auto& bucket : buckets) {
    totalSize += bucket.size;
  }
  
  int currentPos = 0;

  for (auto& bucket : buckets) {
    bucket.tape->rewind();
    for (int i = 0; i < bucket.size; ++i) {
      int value = bucket.tape->read();
      outputTape.write(value);
      if (currentPos < totalSize - 1) {
        outputTape.moveRight();
      }
      if (i < bucket.size - 1) {
        bucket.tape->moveRight();
      }
      currentPos++;
    }
  }
  dynamic_cast<Tape*>(&outputTape)->save();
}

void Sorter::cleanup(const std::vector<BucketTape>& buckets) const {
  for (const auto& bucket : buckets) {
    std::filesystem::remove(bucket.name);
  }
  std::filesystem::remove_all(directory);
}

void Sorter::sort(TapeHelper& input, TapeHelper& output) {
  findMinMax(input);
  if (min == max) {
    return;
  }

  auto buckets = createBucket();
  distributeToBuckets(input, buckets);
  mergeBuckets(buckets, output);
  cleanup(buckets);
}
