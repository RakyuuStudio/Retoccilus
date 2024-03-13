### Naming Conventions
##### Ryan "NvKopres" Almond, and Rakyuu Studio.
##### Verified on Mar/11/2024, Language: en-US (English)

***
This document outlines the naming conventions to be followed for maintaining consistency and readability within our codebase. 

Adherence to these conventions is crucial for code contributions, including Pull Requests, to be considered for acceptance.
***

#### General Guidelines

- **Camel Case Preference**:
  For the majority of variables, functions, and methods, we recommend using Camel Case notation. This convention aids in reading and understanding the code more efficiently.
```c++
int exampleVariable = 0;

//Class follow Major Camel Case
class ExampleClass {
    ~ExampleClass();
    virtual void exampleVirtualFunction();
};
```
  The use of Camel Case makes the code more approachable and easier to navigate.

- **Avoidance of Snake Case**: While we recognize that Snake Case (`snake_case`) is a popular convention in some languages and projects, we have opted not to adopt it in this project. We find that Camel Case provides a cleaner and more consistent look across our codebase.
***

#### Macro Guidelines
While we generally prefer Camel Case, but Macro is the exception.  

For macros, we prefer the Snake Case, to clearly distinguish from other elements in the code base.   

Macro Name Convention: `[Major Section]_[Minor Section]_[Function]`  

For example:  
```c++
#ifndef RETOCCILUS_GLOBAL_UTILITY
#define RETOCCILUS_GLOBAL_UTILITY

//And your code here

#endif
```

This convention helps in identifying macros at a glance and underscores their global applicability or specific functionality within the project.
***
#### Rationale
The decision to prefer Camel Case over Snake Case is based on internal discussions and considerations of readability and aesthetic consistency across multiple languages used in our projects. We understand that preferences vary among developers, and we appreciate your flexibility in adhering to these conventions.  

By following these guidelines, we aim to create a cohesive and maintainable codebase that all contributors can navigate and understand easily. We thank you for your cooperation and contributions to the project.