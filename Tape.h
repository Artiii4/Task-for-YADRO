#include "TapeHelper.h"
#include "Delays.h"

#include <string>
#include <vector>

#ifndef TAPE_H
#define TAPE_H

class Tape : public TapeHelper {
public:
  Tape(const std::string& name, const Delays& delays, bool tapeToSave);
  int read() override;
  void write(int value) override;
  void moveLeft() override;
  void moveRight() override;
  int getID() override;
  int getLength() override;
  int getPosition();
  void rewind();
  void save();
  void load();

private:
  std::string name;
  std::vector<int> tapeData;
  size_t position = 0;
  Delays delays;
  bool tapeToSave = false;

  void delaySimulation(int time) const;
};

#endif
