#ifndef BANKER_H
#define BANKER_H

#include <vector>

class Banker {
private:
  std::vector<int>* _existenceResources;
  std::vector<int>* _availableResources;
  std::vector<std::vector<int>>* _currentAllocation;
  std::vector<std::vector<int>>* _processNeeds;

public:
  Banker();
  Banker(int numberOfResources, int numberOfProcesses);
  virtual ~Banker();
  void addExistenceResources(int resource, int amount);
  void addProcessNeeds(int process, int resource, int amount);
  bool request(int process, int resource, int amount);
  void free(int process, int resource);
};

#endif /* BANKER_H */