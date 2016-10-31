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
  for(int i = 0; i < nro_process; i++) {
    processes->push_back(Process::exec());
    // not good again
    ids->push_back(processes->at(i)->getId());
    hasMems->push_back(processes->at(i)->getHasMem());
    needMems->push_back(processes->at(i)->getNeedMem());
  }

  MemoryManager *memoryManager = OperatingSystem::Memory_Manager();
  // ideal way is pass processes, but i did this...
  memoryManager->banker(ids, hasMems, needMems, 0, 1);
  // colocar cout de pedidos ao jogar no for

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