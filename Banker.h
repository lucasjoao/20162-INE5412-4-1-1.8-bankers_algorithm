#ifndef BANKER_H
#define BANKER_H

#include <vector>

class Banker {
private:
  int _numberOfResources;
  int _numberOfProcesses;

  std::vector<int>* _existenceResources;
  std::vector<int>* _availableResources;
  std::vector<std::vector<int>>* _currentAllocation;
  std::vector<std::vector<int>>* _processNeeds;

public:
  Banker();
  Banker(int numberOfResources, int numberOfProcesses);
  virtual ~Banker();
  std::vector<int>* getExistenceResources();
  void addExistenceResources(int resource, int amount);
  void addProcessNeeds(int process, int resource, int amount);
  bool request(int process, int resource, int amount);
  bool algorithm(int process, int resource, int amount);
  void free(int process, int resource);
  void printHelperDebug();
};

#endif /* BANKER_H */