#pragma once
#include "Environment.h"

class EnvironmentManager {
public:
  EnvironmentManager(Environment*& currentEnvironment, Environment* newEnvironment)
    : currentEnvironment_(currentEnvironment), previousEnvironment_(currentEnvironment) {
    currentEnvironment_ = newEnvironment;
  }

  ~EnvironmentManager() {
    // Reset the environment to its previous state
    currentEnvironment_ = previousEnvironment_;

  }

private:
  Environment*& currentEnvironment_; // Reference to the current environment
  Environment* previousEnvironment_; // Store the previous environment
};
