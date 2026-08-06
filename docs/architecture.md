# Tauron Architecture

## Overview

Tauron is designed as a **lightweight, modular cryptographic framework** focused on performance, streaming support and extensibility.

The project consists of a single executable that exposes a command-line interface (CLI). No graphical user interface is planned.

Internally, Tauron is built around a **processing pipeline**. Data is passed through a series of **independent modules** that operate on a **shared execution context**. Each module has a single responsibility and can **transform the internal state** before passing control to the next stage.

The **cryptographic algorithm** itself is intentionally **separated from the engine**. The engine provides the infrastructure, while cryptographic behavior is implemented through modules.

## Design Goals

- Cross-platform
- Single executable
- Command-line interface
- High performance
- Low memory overhead
- Streaming support
- Modular architecture
- Strong separation of responsibilities
- Configurable cryptographic pipeline
- Future-proof and easy to extend

## High-Level Architecture

```mermaid
flowchart TD
  CLI["CLI"]
  Engine["Engine"]
  Pipeline["Pipeline"]
  Context["Execution Context"]

  ModuleA["Module"]
  ModuleB["Module"]
  ModuleC["Module"]

  Output["Result"]

  CLI --> Engine
  Engine --> Pipeline

  Pipeline --> ModuleA
  ModuleA --> ModuleB
  ModuleB --> ModuleC

  Context -. shared .-> ModuleA
  Context -. shared .-> ModuleB
  Context -. shared .-> ModuleC

  ModuleC --> Output
```

## Components

### CLI

The command-line interface is the only public entry point.

Responsibilities:

- Parse command-line arguments
- Load configuration
- Open files or streams
- Initialize the engine
- Execute the requested operation
- Report progress and errors

The CLI should remain thin and contain no cryptographic logic.

### Engine

The engine is responsible for orchestrating the entire execution.

Responsibilities:

- Create the execution context
- Load configuration
- Build the processing pipeline
- Register modules
- Execute the pipeline
- Handle errors and lifecycle events

The engine does not implement cryptographic primitives.

### Pipeline

The pipeline defines the execution order of all registered modules.

Responsibilities:

- Maintain module order
- Execute modules sequentially
- Provide deterministic execution
- Allow future extension through additional modules

The pipeline itself should contain no cryptographic logic.

### Modules

Modules perform the actual work.

Each module should have exactly one responsibility.

Examples include:

- Key scheduling
- State initialization
- Mixing
- Mutation
- Permutation
- Random generation
- Integrity verification

Modules communicate exclusively through the shared execution context.

### Execution Context

The execution context is shared by every module.

It stores all information required during execution.

Examples:

- Configuration
- Keys
- Internal state
- Input buffers
- Output buffers
- Metadata
- Runtime statistics

Modules should never communicate directly with each other.

All shared information flows through the context.
