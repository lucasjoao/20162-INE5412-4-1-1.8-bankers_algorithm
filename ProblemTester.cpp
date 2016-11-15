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
  int resource, resourceID, process, processID, amount;
  Banker* banker; // i do new in every case to 'clean' the banker

  switch (numTest) {
    case 1: // test 1
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 2;
      amount = 4;
      resourceID = resource - 1;

      testInfo.SetTestName(
        "Verificar Banker::addExistenceResources(" + std::to_string(resource) +
        ", " + std::to_string(amount) + ") no vetor _existenceResources");

      // call the tested function
      banker->addExistenceResources(resource, amount);

      // check if was added the amount
      if (banker->getExistenceResources()->at(resourceID) == amount)
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      break;
    case 2: // test 2
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 3;
      amount = 1;
      resourceID = resource - 1;

      testInfo.SetTestName(
        "Verificar Banker::addExistenceResources(" + std::to_string(resource) +
        ", " + std::to_string(amount) + ") no vetor _availableResources");

      // call the tested function
      banker->addExistenceResources(resource, amount);

      // check if was added the amount
      if (banker->getAvailableResources()->at(resourceID) != amount)
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      else
        testInfo.SetFailed(false);
      break;
    case 3: // test 3
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 1;
      process = 2;
      amount = 3;
      resourceID = resource - 1;
      processID = process - 1;

      testInfo.SetTestName(
        "Verificar Banker::addProcessNeeds(" + std::to_string(process) + ", " +
         std::to_string(resource) + ", " + std::to_string(amount) + ") na" +
         " matriz _processNeeds");

      // call the tested function
      banker->addProcessNeeds(process, resource, amount);

      // check if was added the amount
      if (banker->getProcessNeeds()->at(processID).at(resourceID) == amount)
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      break;
    case 4: // test 4
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 3;
      process = 3;
      amount = 1;
      resourceID = resource - 1;
      processID = process - 1;

      testInfo.SetTestName(
        "Verificar Banker::addProcessNeeds(" + std::to_string(process) + ", " +
         std::to_string(resource) + ", " + std::to_string(amount) + ") na" +
         " matriz _processNeeds");

      // call the tested function
      banker->addProcessNeeds(process, resource, amount);

      // check if was added the amount
      if (banker->getProcessNeeds()->at(processID).at(resourceID) != amount)
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      else
        testInfo.SetFailed(false);
      break;
    case 5: // test 5
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 1;
      process = 1;
      amount = 1;

      testInfo.SetTestName(
        "Verificar que Banker::request(" + std::to_string(process) + ", " +
         std::to_string(resource) + ", " + std::to_string(amount) + ")" +
         " retorna FALSE se o processo pede mais do que precisa");

      // call the tested function, checking if the state will be safe or not
      if (!banker->request(process, resource, amount))
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      break;
    case 6: // test 6
      banker = new Banker(numberOfResources, numberOfProcesses);
      amount = 1;

      testInfo.SetTestName(
        "Verificar que Banker::request(" + std::to_string(process) + ", " +
         std::to_string(resource) + ", " + std::to_string(amount) + ")" +
         " retorna TRUE se um estado seguro for gerado");

      // make a little db to run the test
      for (auto i = 1; i < 3; i++) {
        banker->addExistenceResources(i, amount);
        for (auto j = 1; j < 3; j++)
          banker->addProcessNeeds(i, j, amount);
      }

      // call the tested function, checking if the state will be safe or not
      if (banker->request(1, 1, amount))
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      break;
      break;
    case 7: // test 7
      banker = new Banker(numberOfResources, numberOfProcesses);
      amount = 1;

      testInfo.SetTestName(
        "Verificar que Banker::request(" + std::to_string(process) + ", " +
         std::to_string(resource) + ", " + std::to_string(amount) + ")" +
         " retorna FALSE se um estado inseguro for gerado");

      // make a little db to run the test
      for (auto i = 1; i < 3; i++) {
        banker->addExistenceResources(i, amount);
        for (auto j = 1; j < 3; j++)
          banker->addProcessNeeds(i, j, amount);
      }
      banker->request(1, 1, amount);

      // call the tested function, checking if the state will be safe or not
      if (!banker->request(2, 2, amount))
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      break;
    case 8: // test 8
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 2;
      process = 1;
      amount = 3;

      testInfo.SetTestName(
        "Verificar que Banker::algorithm(" + std::to_string(process) + ", " +
         std::to_string(resource) + ", " + std::to_string(amount) + ")" +
         " retorna TRUE se é solicitado uma quantidade que faz o processo " +
         "terminar ao mesmo tempo que a solicitação gera um estado seguro");

      // make a little db to run the test
      banker->addExistenceResources(resource, amount);
      banker->addProcessNeeds(process, resource, amount);

      // call the tested function, checking if the state will be safe or not
      if (banker->algorithm(process, resource, amount))
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      break;
    case 9: // test 9
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 3;
      process = 3;
      amount = 3;

      testInfo.SetTestName(
        "Verificar que Banker::algorithm(" + std::to_string(process) + ", " +
         std::to_string(resource) + ", " + std::to_string(amount) + ")" +
         " retorna TRUE se a solicitação gera um estado seguro");

      // make a little db to run the test
      banker->addExistenceResources(resource, amount*3);
      banker->addProcessNeeds(process, resource, amount*2);

      // call the tested function, checking if the state will be safe or not
      if (banker->algorithm(process, resource, amount))
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      break;
    case 10: // test 10
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 1;
      process = 2;
      resourceID = resource - 1;
      processID = process - 1;

      testInfo.SetTestName(
        "Verificar que Banker::free(" + std::to_string(process) + ", " +
         std::to_string(resource) + ") não trabalha se não houver recurso");

      // call the tested function
      banker->free(process, resource);

      // check if the function dont change any value
      if (banker->getProcessNeeds()->at(processID).at(resourceID) == 0 &&
          banker->getAvailableResources()->at(resourceID) == 0 &&
          banker->getCurrentAllocation()->at(processID).at(resourceID) == 0)
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
      break;
    case 11: // test 11
      banker = new Banker(numberOfResources, numberOfProcesses);
      resource = 2;
      process = 2;
      amount = 1;
      resourceID = resource - 1;
      processID = process - 1;

      testInfo.SetTestName(
        "Verificar que Banker::free(" + std::to_string(process) + ", " +
         std::to_string(resource) + ") atualiza três atributos do Banker");

      // make a little db to run the test
      banker->addExistenceResources(resource, amount);
      banker->addProcessNeeds(process, resource, amount);
      banker->request(process, resource, amount);
      // call the tested function
      banker->free(process, resource);

      // check if the function changed the values in the right way
      if (banker->getProcessNeeds()->at(processID).at(resourceID) == 1 &&
          banker->getAvailableResources()->at(resourceID) == 1 &&
          banker->getCurrentAllocation()->at(processID).at(resourceID) == 0)
        testInfo.SetFailed(false);
      else
        testInfo.SetMessage("Aconteceu algum problema, verifique o código.");
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

