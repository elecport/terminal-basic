#include "maintask.hpp"

  void MainTask::init()
  {
    HAL_gfx_setmode(2);
    m_halproxyStream.println("Select application:");
    m_halproxyStream.println("1. Terminal-BASIC");
  }
  
  void MainTask::step()
  {
    delay(10);
    if (m_halproxyStream.available()) {
      char c = m_halproxyStream.read();
      if (c == '1')
        newTask = new BASIC::Task;
    }
  }
