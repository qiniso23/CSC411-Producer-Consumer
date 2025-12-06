# CSC411-Producer-Consumer
CSC411 Mini Project - Producer-Consumer Problem

## Project Information

**Course**: CSC411 - Integrative Programming Technologies
**Institution**: University of Eswatini
**Department**: Computer Science & Engineering
**Programme**: BSc Information Technology - IDE
**Due Date**: December 7, 2025

### Team Members
- **Student 1**: Qiniso Motsa - 147496 - https://github.com/qiniso23
- **Student 2**: Simanga Msibi - 143816 - https://github.com/MzwaneleM

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Prerequisites](#prerequisites)
3. [Project Structure](#project-structure)
4. [Design Documentation](#design-documentation)
5. [Demo Video](#demo-video)
6. [Testing Results](#testing-results)
7. [References](#references)

---

## Project Overview

This project implements the classic **Producer-Consumer Problem** using C++ with the following components:

- **Part 1**: Multi-threaded producer-consumer with shared buffer and synchronization
- **Part 2**: Version control and collaboration using GitHub
- **Part 3**: Socket programming implementation using client-server architecture
- **Part 4**: Video demonstration of the working system

The system simulates student data management where:
- The **Producer** generates random student information and stores it as XML files
- The **Buffer** acts as a shared queue with mutual exclusion (max 10 elements)
- The **Consumer** reads XML files, calculates student averages, and determines pass/fail status

---

## Prerequisites

### Software Requirements
- **Operating System**: Windows 10/11
- **IDE**: Microsoft Visual Studio Community 2019 or later
- **Compiler**: MSVC with C++17 support
- **Git**: Version 2.30 or later

### Libraries Used
- Standard C++ Library (STL)
- `<thread>` - Multi-threading support
- `<mutex>` and `<condition_variable>` - Synchronization
- `<filesystem>` - File operations (C++17)
- `<winsock2.h>` - Windows socket programming

---

## Project Structure

```
CSC411-Producer-Consumer/
|
|- README.md                           # This file
|- demo_video.mkv                      # Presentation video
|
|- LocalImplementation/                # Part 1: Local Threading implementation
|   |- main.cpp                        # Main program entry point
|   |- shared_files/                   # Directory for XML files (created at runtime)
|
|- SocketImplementation/               # Part 3: Socket programming implementation
|   |- SocketConsumer/
|   |   |- main.cpp                    # Main consumer thread entry point
|   |
|   |- SocketProducer/
|       |- main.cpp                    # Main consumer thread entry point
|       |- network_server.cpp          # Producer socket server implementation
|       |- network_server.h            # Producer socket server header
|
|- source/
    |- buffer/
    |   |- buffer.cpp                  # Buffer class implementation
    |   |- buffer.h                    # Buffer class header
    |   |- network_buffer.cpp          # Socket buffer class implementation
    |   |- network_buffer.h            # Socket buffer class header
    |
    |- consumer/
    |   |- consumer.cpp                # Consumer class implementation
    |   |- consumer.h                  # Consumer class header
    |   |- socket_consumer.cpp         # Socket consumer class implementation
    |   |- socket_consumer.h           # Socket consumer class header
    |
    |- producer/
    |   |- networt_producer.cpp        # Socket producer class implementation
    |   |- networt_producer.h          # Socket producer class header
    |   |- producer.cpp                # Producer class implementation
    |   |- producer.h                  # Producer class header
    |
    |- student/
        |- course_mark.h               # Struct with student's course code and mark
        |- ITstudent.cpp               # IT student class implementation
        |- ITstudent.h                 # IT student class hearder

```

---

### Key Design Decisions

1. **Synchronization Mechanism**: 
   - Used `std::mutex` for mutual exclusion
   - Used `std::condition_variable` for producer/consumer signaling
   - Ensures thread-safe access to shared buffer

2. **Data Format**:
   - XML chosen for structured data representation
   - Simple custom parser (educational purposes)
   - Each student in separate file for clear producer-consumer separation

3. **Random Data Generation**:
   - `std::mt19937` for high-quality random numbers
   - Predefined lists for realistic names and courses
   - Random marks between 0-100

4. **Socket Communication**:
   - TCP sockets for reliable data transfer
   - Server waits for client connections
   - Each connection transfers one student's data

### Class Descriptions

#### ITstudent Class
- **Purpose**: Represents a student with all their information
- **Key Methods**:
  - `to_XML()`: Converts student data to XML format
  - `from_XML()`: Parses XML and creates ITStudent object
  - `calculate_average()`: Computes average mark from all courses
  - `print_student_info()`: Displays formatted student information

#### buffer Class
- **Purpose**: Thread-safe queue for producer-consumer communication
- **Key Features**:
  - Fixed size (10 elements maximum)
  - Mutex-protected operations
  - Condition variables for blocking when full/empty
- **Key Methods**:
  - `produce(int)`: Adds item to buffer (blocks if full)
  - `consume()`: Removes item from buffer (blocks if empty)

#### producer
- **Purpose**: Generates student data and creates XML files
- **Key Features**:
  - Random data generation for realistic students
  - Creates XML files in shared directory
  - Adds file numbers to buffer
  - Configurable production delay

#### consumer
- **Purpose**: Processes student data from XML files
- **Key Features**:
  - Reads XML files from shared directory
  - Parses and processes student information
  - Calculates averages and determines pass/fail
  - Deletes processed files
  - Removes entries from buffer

---

## Demo Video

**Video Link**: https://github.com/qiniso23/CSC411-Producer-Consumer/blob/main/demo_video.mkv

**Video Contents:**
- Introduction and team members
- Code structure explanation
- Part 1 local demonstration
- Part 3 socket demonstration
- GitHub repository tour

---

## Testing Results

### Test Case 1: Basic Functionality
- **Test**: Run with 10 students
- **Result**: PASSED
- **Details**: All students processed correctly, no deadlocks

### Test Case 2: Buffer Size Limit
- **Test**: Verify buffer never exceeds 10 elements
- **Result**: PASSED
- **Details**: Producer blocks when buffer is full

### Test Case 3: Empty Buffer Handling
- **Test**: Consumer behavior with empty buffer
- **Result**: PASSED
- **Details**: Consumer waits when buffer is empty

### Test Case 4: XML Integrity
- **Test**: Verify XML parsing accuracy
- **Result**: PASSED
- **Details**: All student data correctly parsed and displayed

### Test Case 5: Socket Communication
- **Test**: Network data transfer between server and client
- **Result**: PASSED
- **Details**: Data transmitted without corruption

### Test Case 6: Stress Test
- **Test**: Run with 50 students
- **Result**: PASSED
- **Details**: System stable, no memory leaks

---

## References

1. Operating System Concepts (10th Edition) - Silberschatz, Galvin, Gagne
2. C++ Concurrency in Action (2nd Edition) - Anthony Williams
3. Beej's Guide to Network Programming - Brian "Beej" Hall
4. Microsoft Visual Studio Documentation: https://docs.microsoft.com/en-us/visualstudio/
5. C++ Threading Documentation: https://en.cppreference.com/w/cpp/thread
6. Git Documentation: https://git-scm.com/doc

---

**Last Updated**: December 2025
