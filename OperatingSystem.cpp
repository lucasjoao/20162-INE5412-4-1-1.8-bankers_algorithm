/*
 * File:   System.cpp
 * Author: cancian
 *
 * Created on October 9, 2015, 9:04 PM
 */
#include <string>

#include "OperatingSystem.h"
#include "Application.h"
#include "HW_CPU.h"
#include "HW_Machine.h"
#include "Simulator.h"


void OperatingSystem::LoadApplication(Application* app, MMU::PhysicalAddress address) {
    Debug::cout(Debug::Level::trace, "OperatingSystem::LoadApplication(" + std::to_string(reinterpret_cast<unsigned long> (app)) + "," + std::to_string(address) + ")");
    std::list<Application::Information> code = app->getCode();
    HW_MMU::Information info;
    for(std::list<HW_MMU::Information>::iterator it = code.begin(); it != code.end(); it++) {
        info = (*it);
        HW_Machine::RAM()->write(address, info);
        address++;
    }
}

void OperatingSystem::SetBootApplication(Application* app) {
    Debug::cout(Debug::Level::trace, "OperatingSystem::SetBootApplication(" + std::to_string(reinterpret_cast<unsigned long> (app)) + ")");
    HW_CPU::Register address = HW_Machine::CPU()->readRegister(HW_CPU::pc);
    LoadApplication(app, address);
}


void OperatingSystem::Init() {
    Debug::cout(Debug::Level::trace, "OperatingSystem::init()");
    HW_Machine::Init();

    OperatingSystem::CPU_Mediator();
    OperatingSystem::DMA_Mediator();
    OperatingSystem::HardDisk_Mediator();
    OperatingSystem::MMU_Mediator();
    OperatingSystem::Timer_Mediator();

    SetBootApplication(Application::DefaultBootApplication());  // load boot application into RAMs
}


void OperatingSystem::ExecuteTestCode() {
  Debug::cout(Debug::Level::trace, "OperatingSystem::ExecuteTestCode()");
  Simulator* simulator = Simulator::getInstance();

  srand(time(NULL));
  // better use a number where memorySize % number = 0
  // if change here, need update Process::exec
  auto nro_process = 5;
  Debug::cout(Debug::Level::info,
    "Recurso utilizado na aplicação do algoritmo do banqueiro é a memória. " +
    std::to_string(nro_process) + " processos na simulação.");

  auto memorySize = Traits<MemoryManager>::physicalMemorySize;
  Debug::cout(Debug::Level::info,
    "Total de memória na simulação é " + std::to_string(memorySize));

  std::vector<Process*>* processes = new std::vector<Process*>();
  // this solution is not good, but i took a error if diff...
  std::vector<unsigned int>* ids = new std::vector<unsigned int>();
  std::vector<unsigned int>* hasMems = new std::vector<unsigned int>();
  std::vector<unsigned int>* needMems = new std::vector<unsigned int>();
  for (int i = 0; i < nro_process; i++) {
    processes->push_back(Process::exec());
    // not good again
    ids->push_back(processes->at(i)->getId());
    hasMems->push_back(processes->at(i)->getHasMem());
    needMems->push_back(processes->at(i)->getNeedMem());
  }

  MemoryManager *memoryManager = OperatingSystem::Memory_Manager();
  auto ask = 0;
  auto areSafe = 0;
  bool safe;
  clock_t start;
  double time;
  double allTime = 0;
  for (int j = 0; j < nro_process; j++) {
    ask = rand() % memorySize;

    Debug::cout(Debug::Level::info, "Processo " + std::to_string(ids->at(j)) +
      " solicita " + std::to_string(ask) + " de memória. Será que é possível "
      + "fornecer e gerar um estado seguro?");

    start = clock();

    // ideal way is pass processes, but i did this...
    safe = memoryManager->banker(ids, hasMems, needMems, j, ask);

    time = double(clock() - start)/ CLOCKS_PER_SEC;

    Debug::cout(Debug::Level::info,
      "Foi gasto " + std::to_string(time) + " segundos no algoritmo.");
    Debug::cout(Debug::Level::info, "---------------------------------------");

    if (safe)
      areSafe += 1;

    allTime += time;
  }

  auto areSafePercent = (double(areSafe)/double(nro_process)) * 100;
  auto notSafePercent = 100 - areSafePercent;
  auto averageTime = allTime / nro_process;
  Debug::cout(Debug::Level::info, "De todos os pedidos de alocação "
    + std::to_string(areSafePercent) + "\045 serão aprovados e "
    + std::to_string(notSafePercent) + "\045 serão reprovados.");
  Debug::cout(Debug::Level::info, "De todas os pedidos de alocação "
    + std::to_string(areSafePercent) + "\045 serão estados seguros e "
    + std::to_string(notSafePercent) + "\045 serão estados inseguros.");
  Debug::cout(Debug::Level::info,
    "O tempo médio de execução do algoritmo foi de " +
    std::to_string(averageTime) + " segundos.");

  simulator->stop();
}

 /*
  Not used
  */
 HW_MMU::Information OperatingSystem::asmm(std::string mnemonic) {
     HW_MMU::Information bincode;
     /* @TODO
      */
     if (mnemonic == "Process:exec()") {
         bincode = 0xFFFFFFFF; // - (HW_CPU::addi<<24) +
     }
     //@TODO
     bincode = (HW_CPU::addi<<26) + (HW_CPU::s0<<21) + (HW_CPU::s1<<16) + 1; // for tests purpose only
     return bincode;
 }