#include "Tape.h"
#include "TapeHelper.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>

Tape::Tape(const std::string& name, const Delays& delays, bool tapeToSave):
  name(name), delays(delays), tapeToSave(tapeToSave), position(0)
{
tapeData.clear();

  if (tapeToSave) {
    std::ofstream out(name);
    if (!out) {
      throw std::runtime_error("Cant create file: " + name);
    }
    out.close();
  }
  else {
    load();
  }
}

int Tape::read()
{
  delaySimulation(delays.readDelay);
  if (position >= tapeData.size()) {
    throw std::out_of_range("Out of range in file " + name);
  }
  return tapeData[position];
}

void Tape::write(int value)
{
  delaySimulation(delays.writeDelay);
  if (position >= tapeData.size()) {
    tapeData.resize(position + 1);
  }
  tapeData[position] = value;
}

void Tape::moveLeft()
{
  delaySimulation(delays.moveDelay);
  if (position == 0) {
    throw std::out_of_range("moveLeft out of range in file " + name);
  }
  position--;
}

void Tape::moveRight()
{
  delaySimulation(delays.moveDelay);
  if (position > tapeData.size()) {
    throw std::out_of_range("moveRight out of range in file " + name);
  }
  position++;
}

int Tape::getID()
{
  return 0;
}

int Tape::getLength()
{
  return tapeData.size();
}

void Tape::rewind()
{
  for (int i = 0; i < position; i++) {
    delaySimulation(delays.moveDelay);
  }
  position = 0;
}

int Tape::getPosition()
{
  return position;
}

void Tape::delaySimulation(int time) const
{
  std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void Tape::load()
{
  std::ifstream in(name);
  if (!in) {
    throw std::runtime_error("Cant open tape file for reading: " + name);
  }

  tapeData.clear();
  int value;

  while (in >> value) {
    tapeData.push_back(value);
  }
  position = 0;
}

void Tape::save()
{
  std::ofstream out(name);
  if (!out) {
    throw std::runtime_error("Cant open tape file for writing: " + name);
  }

  for (size_t i = 0; i < tapeData.size(); ++i) {
    out << tapeData[i];
    if (i < tapeData.size() - 1) {
      out << " ";
    }
  }
}
