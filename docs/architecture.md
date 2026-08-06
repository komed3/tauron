# Tauron Architecture

## Overview

Tauron is a **lightweight, modular cryptographic engine** designed around deterministic state evolution.

The project consists of a single executable that exposes a command-line interface (CLI). No graphical user interface is planned.

Internally, Tauron is built around a **processing pipeline**. Data is passed through a series of **independent modules** that operate on a **shared execution context**. Each module has a single responsibility and can transform the internal state before passing control to the next stage.

The c**ryptographic algorithm** itself is intentionally **separated from the engine**. The engine provides the infrastructure, while cryptographic behavior is implemented through modules.

Instead of transferring cryptographic state between participants, Tauron derives states independently on every participating node.

The system separates:

- Communication state management
- Cryptographic processing
- Pipeline execution
- Data transformation

A Tauron instance **does not distinguish between sender and receiver**. Every participant is a node capable of both encryption and decryption.

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
  User["Application / CLI"]
  Node["Tauron Node"]
  Context["Execution Context"]
  Pipeline["Processing Pipeline"]
  Modules["Cryptographic Modules"]
  State["Local State"]
  Output["Encrypted / Decrypted Data"]

  User --> Node
  Node --> Context
  Context --> State
  Context --> Pipeline
  Pipeline --> Modules
  Modules --> State
  Modules --> Output
```

## Node-Based Communication Model

Tauron uses a node-based model instead of a sender/receiver model.

Every participant owns an independent Tauron instance.

```mermaid
flowchart LR
  NodeA["Node A"]
  NodeB["Node B"]
  NodeC["Node C"]

  NodeA <-->|"Independent state evolution"| NodeB
  NodeA <-->|"Independent state evolution"| NodeC
  NodeB <-->|"Independent state evolution"| NodeC
```

Nodes never exchange internal states.

Each node derives required states independently.

## State Hierarchy

Tauron uses multiple layers of state.

Each layer is derived from the previous layer.

**Lower layers must never be able to reconstruct higher layers.**

```mermaid
flowchart TD
  Root["Root Secret"]
  Session["Session State"]
  Direction["Direction State"]
  NodeState["Node State"]
  Epoch["Epoch State"]
  Packet["Packet State"]

  Root -->|"derive"| Session
  Session -->|"derive"| Direction
  Direction -->|"derive"| NodeState
  NodeState -->|"derive"| Epoch
  Epoch -->|"derive"| Packet
```

## Information Flow Rules

Allowed:

```mermaid
flowchart LR
  Root["Root Secret"]
  Session["Session State"]
  Epoch["Epoch State"]
  Packet["Packet State"]

  Root --> Session
  Session --> Epoch
  Epoch --> Packet
```

Forbidden:

```mermaid
flowchart LR
  Session["Session State"]
  Epoch["Epoch State"]
  Packet["Packet State"]

  Packet --> Epoch
  Epoch --> Epoch
  Epoch --> Session
```

A compromised lower-level state must not expose higher-level secrets.

## Direction Separation

Communication directions use independent states.

Example:

```mermaid
flowchart LR
  Session["Session State"]
  A["Node A → Node B"]
  B["Node B → Node A"]

  Session --> A
  Session --> B

  A --> AState["Independent State"]
  B --> BState["Independent State"]
```

The state used for outgoing communication is never reused for incoming communication.

## Participant-Specific Derivation

Multiple nodes can participate in the same communication environment.

Each node receives a unique derived state.

```mermaid
flowchart TD
  Shared["Shared Session Material"]
  A["Node Identity A"]
  B["Node Identity B"]
  C["Node Identity C"]

  Shared --> Derive["State Derivation"]

  Derive --> SA["Node A State"]
  Derive --> SB["Node B State"]
  Derive --> SC["Node C State"]
```

Compromise of one node does not automatically compromise other nodes.

## State Evolution

States are not transferred.

They evolve locally and deterministically.

```mermaid
sequenceDiagram
  participant A as Node A
  participant B as Node B

  A->>A: Derive Packet State
  A->>B: Encrypted Data + Public Metadata

  B->>B: Derive Same Packet State

  B->>B: Decrypt Data

  A->>A: Mutate State
  B->>B: Mutate State
```

## Packet Structure

Packets contain only information required for synchronization and processing.

No secret state material is transmitted.

|      Packet         |
|:-------------------:|
|    Public Header    |
|  Encrypted Payload  |
| Authentication Data |

Possible public metadata:

- Version
- Counter
- Epoch identifier
- Algorithm profile
- Flags

## Core Engine Flow

```mermaid
flowchart TD
  Input["Input Data"]
  Context["Execution Context"]
  Pipeline["Pipeline"]
  Modules["Modules"]
  State["Current State"]
  Output["Output Data"]

  Input --> Context
  State --> Context
  Context --> Pipeline
  Pipeline --> Modules
  Modules --> State
  Modules --> Output
```
