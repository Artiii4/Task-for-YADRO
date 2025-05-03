#ifndef TAPEHELPER_H
#define TAPEHELPER_H

class TapeHelper {
public:
  virtual int read() = 0;
  virtual void write(int value) = 0;
  virtual void moveLeft() = 0;
  virtual void moveRight() = 0;
  virtual int getID() = 0;
  virtual int getLength() = 0;
  virtual void rewind() = 0;
  virtual int getPosition() = 0; 
  virtual ~TapeHelper() = default; 
};

#endif
