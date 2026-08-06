# Tauron Architecture

## Overview

Tauron is a **lightweight, modular cryptographic engine** designed around deterministic state evolution.

The project consists of a single executable that exposes a command-line interface (CLI). No graphical user interface is planned.

Internally, Tauron is built around a **processing pipeline**. Data is passed through a series of **independent modules** that operate on a **shared execution context**. Each module has a single responsibility and can transform the internal state before passing control to the next stage.

The **cryptographic algorithm** itself is intentionally **separated from the engine**. The engine provides the infrastructure, while cryptographic behavior is implemented through modules.

Instead of transferring cryptographic state between participants, Tauron derives states independently on every participating node.

The system separates:

- Communication state management
- Cryptographic processing
- Pipeline execution
- Data transformation

A Tauron instance **does not distinguish between sender and receiver**. Every participant is a node capable of both encryption and decryption.

- Design Goals
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
