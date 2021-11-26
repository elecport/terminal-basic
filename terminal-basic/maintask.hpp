#include "basic_task.hpp"

class MainTask : public ::Task
{
public:

  MainTask() = default;

  ~MainTask() override = default;
  // Task interface
public:

  void init() override;

  void step() override;
};
