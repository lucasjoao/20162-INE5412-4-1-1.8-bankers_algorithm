#ifndef BANKER_H
#define BANKER_H

class Banker {
private:
public:
  Banker();
  virtual ~Banker();
  void addEntry();
  void request();
  void free();
}

#endif /* BANKER_H */