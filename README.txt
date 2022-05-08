# Project ICP
## Interactive UML editor

### Authors:
- Vítezslav Kríz <xkrizv03@stud.fit.vutbr.cz>
- Martin Kocich <xkocic02@stud.fit.vutbr.cz>

### Requirements:
- [QT    5.51+](http://qt-project.org/)
- [CMAKE 3.16+](https://cmake.org/)
- [C++   23+  ](https://en.wikipedia.org/wiki/C%2B%2B20)
- one of the following:
  - [GCC](https://gcc.gnu.org/)
  - [MSVC](https://www.microsoft.com/en-us/download/details.aspx?id=44982)

### Description:
- This is a simple interactive UML editor.
- It is based on Qt 5.5.1 and CMake 3.16.
- It is written in [MSVS](https://en.wikipedia.org/wiki/Microsoft_Visual_Studio) 2022 and [GCC](https://gcc.gnu.org/) 7.3.0.
- It is tested on Windows 10 and Ubuntu 18.04.
- It is licensed under [BSD 3](https://opensource.org/licenses/BSD-3-Clause) under [QT](https://doc.qt.io/qt-5/examples-license.html).
- Supports (class/sequence diagrams):
  - saving and loading of UML diagrams
  - live preview of diagrams
  - syntax highlighting
  - semantic analysis
  - live incosistency detection
  - support for:
    - multiple sequence diagrams
    - access modifiers
    - classes, interfaces
    - relations (association, inheritance, composition, generalization - only one level deep)
    - (de)activations of sequence diagram members
    - syntax highlighting
    - semantic highlighting and error reporting
    - automated error resolution through GUI in some error cases
  - interactive diagram editing
    - undo fuction
    - keyboard shortcuts
    - coordinates updating persistently
    - context menu on right click (default actions)
      - Class diagram (on class):
        - modify - shows class definition in text editor
        - delete - deletes class
        - add relation (+ type of relation) - adds relation between classes in interactive mode
      - Sequence diagram (column):
        - modify    - shows sequence diagram in text editor
        - view clas - shows class definition in text editor
      - Sequence diagram (method):
        - modify    - shows sequence method in text editor
        - delete    - deletes sequence method
        - duplicate - duplicates sequence method
    - context menu on right click (advanced actions)
      - Sequence diagram (column):
        - add class       - adds class declaration on non existing entity
        - add interface   - adds interface declaration on non existing entity
      - Sequence diagram (method):
        - add declaration - adds declaration to class
        - make public     - makes method of class public
