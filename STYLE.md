# Mamba C++ Style
This document is used to describe the style conventions of the C++
programming language within the project.    

As it often happens in the industry, C++ is not a single tool to
solve a problem, but rather a toolset with different paradigms,
emphasises and schools of thought how programming must be done. 
Every project approaches C++ in a way that reflects their needs 
and expectations, hence this style guide was born as the vision  
of how to use C++ effectively, and how to avoid it. 

# Table of contents
1. [Formatting](#formatting)
2. [Naming conventions](#naming-conventions)


# Formatting
Formatting should be entirely handled by clang-format.

# Naming conventions
1. Use **PascalCase** (all words beginning with uppercase) for: 
   class/struct names, template argument types/concepts, constants,
   global variables, accessors for global variables.
2. Use **camelCase** (the first word starting lowercase, then all words
   uppercase) for: function/method names, variable/parameter/property
   names.
3. Use the `.cc` file extension for C++ implementation files and `.hh` for 
   C++ header files. 

# C++ Standard
4. Use the C++17 standard thoroughly, and embrace C++20 additions that have
   been completely and reliably implemented. Consult compiler support before
   adopting a C++20 feature.
5. When making a decision about the standard support, always orientate on the 
   three great compilers: GCC, Clang and MVS.
6. Use `#pragma once` over include guards.

# Typing
7. Never use `auto` to obfuscate return type or run away from the template errors.
8. Never use `auto` in function signatures.
9. Use `auto` to deduce the type of:
   10. constants;
   11. cast expressions;
   12. constructor expressions.
13. Always prefix `auto` with ampersand `&` to indicate a reference and asterisk `*` 
    to hint a pointer.
# Object-oriented programming
14. Use **classes** when you intend to use object-oriented patterns, such
   as abstraction, encapsulation, inheritance, and polymorphism. Use 
   **structs** when you need to wrap multiple values around a single 
   named type or need to give a C struct a destructor.
15. All class properties must be either private or protected, and provide
   getters and setters if applicable. Unless a class is meant to be inherited
   from, a property should always be private.
15. Order class declaration in the following fashion:
    17. Always start with the public members, followed by protected, and then private. 
    18. 8Always declare properties before methods. 
    19. The first methods to declare are constructors. 
    20. Constructors are followed by operator overloads. 
    21. Operator overloads are followed by public method in preferred order. 
    22. Destructor, if any, is always the final method.
23. Always declare default constructor the first one. Depending on the number of
    custom constructors, put copy and move constructors in the order of their 
    significance: if there are many ways to construct an object, put copy and 
    move constructors above to leave the space for documenting custom constructors. 
24. A class should always answer the following questions:
    * Can it be default constructed?
    * Can it be copied?
    * Can it be moved? 
    
    As a consequence, if any of the constructors have a custom logic, declare them.
    If there are no such constructors, `delete` them. If the logic of these constructors
    is trivial, declare them as `default`. 
25. Structs must have all of their members public and can only have constructors, 
    operator overloads and destructor, but no named methods.