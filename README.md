# Reflection in C++26

Small proofs of concepts to try C++26's reflection and see what can be achieved using it.  

Needs a compiler which supports reflection.  
As of today, the recently released GCC 16.1.1 and an [experimental Clang](https://github.com/bloomberg/clang-p2996/tree/p2996) seem alright.  

**IMPORTANT:** set `CXX` and `AR` variables in `build.mk` to the filepath of your working compiler and archiver executables.

## Mini-projects
- `dumptype` : Dumps a C++ class type (`struct`, `class` or `union`\*)
- `argparse` : Parses command-line arguments according to a class type's members

\* As type-punning with unions is explicitly forbidden in C++, this code was invalid even before reflection :
```cpp
union S { int a; float b; };
S s{ .a = 4 }; // a is now the only active member, b shouldn't be read (but can be written)
std::println("{}", s.b); // Very likely to work as expected (reinterpret bytes of s.a as a float (type of s.b)), but technically is UB (undefined behavior)
```

However, with reflection, one can iterate over members of a union/struct/class and access them, but for unions, it seems to lack a function to know if the member is the active one, to avoid undefined access.
```cpp
#include <meta>
#include <print>

int main() {
    union S { int a; float b; };
    S s{ .a = 4 };

    template for (constexpr auto member : define_static_array(nonstatic_data_members_of(^^S, std::meta::access_context::current()))) {
        // s.[:member:] accesses every member of the union, the member being active or not --> technically UB (undefined behavior)
        std::println("{} = {}", identifier_of(member), s.[:member:]);
    }
    std::println("a = {}, b = {}", s.a, s.b);
}
```

See this proposal https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3587r0.html, at section §3.1.1 :  
> ... Naively, the algorithm presented in [P2996R8] ignores the presence of the union and proceeds to visit each non-static data member regardless. Not only does this result in undefined behaviour, ...

Maybe future changes in the language will provide a function trait, or alter the behavior.  

## Build

From the repository root : `make` to build, `make clean` to remove object and executable files, `make re` to rebuild from scratch.  
The same commands are available per project, just move into the project folder and run the command, e.g. `cd dumptype; make`.  

