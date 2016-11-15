/*
 * File:   Tester.cpp
 * Author: cancian
 *
 * Created on November 7, 2015, 3:44 PM
 */



#include <iostream>

#include "OperatingSystem.h"
#include "Traits.h"

#include "Simulator.h"
#include "SourceModule.h"
#include "Simul_Entity.h"
#include "ModuleCreate.h"
#include "ModuleDispose.h"
#include "ModelBuilder.h"

#include "ProblemTester.h"

#include "Banker.h"


ProblemTester::ProblemTester() {
}

ProblemTester::ProblemTester(const ProblemTester& orig) {
}

ProblemTester::~ProblemTester() {
}

void ProblemTester::_clearSystem() {
    /*
    Process::getProcessesList()->clear();
    Thread::getThreadsList()->clear();
    Thread* t;
    while ((t = System::scheduler()->choose()) != nullptr) {
        System::scheduler()->remove(t);
    }
    Thread::_running = nullptr;
     */
}


// test here while there are no subclasses

// VIRTUAL!!
TestInfo ProblemTester::do_real_test(int numTest){
  TestInfo testInfo = TestInfo();
  // Traits<Model>::ProblemSolving problem = Traits<Model>::problemChoosen;
  int numberOfResources = 3;
  int numberOfProcesses = 3;
  int resource, resourceID, amount;
  Banker* banker = new Banker(numberOfResources, numberOfProcesses);

  switch (numTest) {
    case 1:
      resource = 2;
      amount = 4;
      resourceID = resource - 1;

      testInfo.SetTestName(
        "Verificar Banker::addExistenceResources(" + std::to_string(resource) +
        ", " + std::to_string(amount) + ") no vetor _existenceResources");

      banker->addExistenceResources(resource, amount);

      if (banker->getExistenceResources()->at(resourceID) == amount) {
        testInfo.SetFailed(false);
      } else {
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      }
      break;
    case 2:
      resource = 3;
      amount = 1;
      resourceID = resource - 1;

      testInfo.SetTestName(
        "Verificar Banker::addExistenceResources(" + std::to_string(resource) +
        ", " + std::to_string(amount) + ") no vetor _availableResources");

      banker->addExistenceResources(resource, amount);

      if (banker->getAvailableResources()->at(resourceID) == 0) {
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      } else {
        testInfo.SetFailed(false);
      }
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      break;
    case 12:
      break;
    case 13:
      break;
    default:
      break;
  }

  return testInfo;
}


void ProblemTester::test(int numTest) {
  Model* model = _modelBuilder->buildModel();

  Simulator* simulator = Simulator::getInstance();
  simulator->setModel(model);
  //simulator->step();

  TestInfo testInfo = TestInfo();
  try {
    testInfo = do_real_test(numTest);
  } catch (int ex) {
    testInfo.SetFailed(true);
    testInfo.SetMessage("Exception raised when executing the test. Tester has failed.");
  }

  std::string message = testInfo.GetMessage();
  if (testInfo.IsFailed()) {
    std::cout << "[[TEST_FAILED]]";
    message = "ERROR: " + message;
  } else {
    std::cout << "[[TEST_SUCCESS]]";
    message = "Woohoo.";
  }
  std::cout << " (" << numTest << ") testName='" << testInfo.GetTestName() << "' message='" << message << "'" << std::endl;
}

