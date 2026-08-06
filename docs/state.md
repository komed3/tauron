# Tauron Internal State

## Overview

The internal state is the **core working memory** of the Tauron engine.

Unlike cryptographic keys, which remain largely constant throughout an operation, the state is designed to **evolve continuously while data is processed**.

Every **module may observe or modify the state** according to its responsibility.

The state is expected to become the **primary source of diffusion, mutation and unpredictability** throughout the cryptographic pipeline.

## Design Goals

The state should be:

- Dynamic
- Deterministic
- Efficient
- Mutation-friendly
- Suitable for both block and streaming operations
- Independent of specific algorithms

## High-Level Structure

```mermaid
flowchart TD
  State["Internal State"]

  State --> Core["Core Memory"]
  State --> Counters["Counters"]
  State --> Entropy["Entropy Pool"]
  State --> Mutation["Mutation Data"]
  State --> Runtime["Runtime Values"]
```

## Components

### Core Memory

The core memory represents the primary cryptographic working area.

It contains the mutable data manipulated by cryptographic modules.

Its exact size is intentionally configurable and may depend on:

- Security profile
- Key size
- Selected algorithm
- User configuration

Modules should treat the core memory as the primary source for transformations.

### Counters

Counters provide deterministic progression throughout execution.

Possible examples include:

- Block counter
- Stream position
- Round counter
- Mutation counter
- Internal iteration counter

Counters allow identical input and keys to produce deterministic execution while ensuring the state evolves continuously.

### Entropy Pool

The entropy pool stores values used to increase internal diversity.

Possible future sources include:

- Derived key material
- Previous state values
- Internal mixing operations
- Timing-independent deterministic generators

The entropy pool is never intended to rely on external randomness during normal execution.

### Mutation Data

Mutation data controls how the state evolves over time.

Rather than following a completely static execution path, future modules may use these values to influence:

- Transformation order
- Rotation values
- Mixing patterns
- Internal scheduling
- Module behavior

Mutation must always remain deterministic.

Identical input and configuration must produce identical output.

### Runtime Values

Runtime values contain temporary execution data.

Examples include:

- Active module index
- Current processing phase
- Temporary intermediate values
- Synchronization markers

These values support execution but are not considered long-term cryptographic state.

## State Evolution

```mermaid
flowchart LR
  Initial["Initial State"]
  M1["Module"]
  M2["Module"]
  M3["Module"]
  Final["Updated State"]

  Initial --> M1
  M1 --> M2
  M2 --> M3
  M3 --> Final
```

Every module contributes to the evolution of the state.

No module should assume that the state remains unchanged after another module has executed.
