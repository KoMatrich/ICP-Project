#pragma once

#include <QPair>
#include "Syntax/Analyzer.h"
#include "Services/Debug.h"
#include "Services/Code.h"

/// @brief Class that stores UML relation data
class UMLRelation
{
public:
    /// @brief public code position for highlighting
    size_t pos = 0;
    /// @brief default constructor
    UMLRelation() {}
    /// @brief constructor
    /// @param entity name of entity which is being related to
    /// @param type type of relation
    UMLRelation(QString entity, RuleID type)
    {
        this->entity = entity;
        this->type = type;
    }
    /// @brief destructor
    ~UMLRelation() {}
    /// @brief String representation of UML relation
    /// @return String representation of UML relation
    QString toString();
    /// @brief Getter for entity name
    /// @return entity name
    QString getEntity();
    /// @brief Getter for entity id
    /// @return entity id
    size_t getID() { return id; }
    /// @brief Getter for relation type
    /// @return relation type
    RuleID getType() { return type; }
    /// @brief Set relation as invalid
    inline void setInvalid() { isValid = false; }
    /// @brief Getter for validity
    /// @return validity
    inline bool getValid() { return isValid; }
    /// @brief Updates realtion parameters and returns true if relation has been updated
    /// @param new_r new relation from which parameters are updated
    /// @return true if relation has been updated
    bool updateRelationParams(UMLRelation new_r);
    /// @brief Updates realtion connections (entity ids) and returns true if relation has been updated
    /// @param new_id new relation entity id
    /// @return true if relation has been updated
    bool updateRelationConnectors(size_t new_id);
protected:
    /// @brief flag used for when deleting
    bool isValid = true;
    /// @brief name of target entity
    QString entity;
    /// @brief semantic type of rule
    RuleID type = RuleID::R_ERR;
    /// @brief Unique identifier
    size_t id = -1;
};

/// @brief Class that stores UML property data
class UMLProperty
{
public:
    /// @brief public code position for highlighting
    size_t pos = 0;

    /// @brief default constructor
    UMLProperty() {}
    /// @brief constructor
    UMLProperty(QString mod, QString type, QString name)
    {
        this->p_mod = mod;
        this->p_type = type;
        this->p_name = name;
    }
    /// @brief destructor
    ~UMLProperty() {}
    /// @brief Property comparator based on name
    bool operator==(const UMLProperty& rhs)
    {
        return this->p_name == rhs.p_name;
    }
    /// @brief String representation of UML property
    /// @return String representation of UML property
    QString toString();
    /// @brief Updates property parameters and returns true if anything has changed
    /// @param new_p new property from which parameters are updated
    /// @return true if property has been updated
    bool updateProperty(UMLProperty new_p);
    /// @brief Getter for property modifier
    /// @return property modifier
    QString getMod() { return p_mod; }
    /// @brief Getter for property type
    /// @return property type
    QString getType() { return p_type; }
    /// @brief Getter for property name
    /// @return property name
    QString getName() { return p_name; }
    /// @brief Getter for property duplicate flag
    /// @return property duplicate flag
    inline bool getDuplicateFlag() { return duplicate; }
    /// @brief Setter for property duplicate flag
    /// @param d new flag
    inline void setDuplicateFlag(bool d) { duplicate = d; }
protected:
    /// @brief flag used for when highlighting collisions
    bool duplicate = false;
    /// @brief property modifier
    QString p_mod;
    /// @brief property type
    QString p_type;
    /// @brief property name
    QString p_name;
};

/// @brief Class that stores UML entity data
class UMLClass
{
public:
    /// @brief default constructor
    UMLClass() {}
    /// @brief default destructor
    ~UMLClass() {}
    /// @brief Getter for class name
    /// @return class name
    QString getClassName();
    /// @brief Setter for class name, sets changed flag on change
    void updateName(QString name);
    /// @brief Setter for class type, sets changed flag on change
    void updateType(bool is_interface);
    /// @brief Getter for class type
    /// @return class type
    bool isInterface() { return is_interface; }
    /// @brief start of class code position
    size_t pos = 0;
    /// @brief end of class code position
    size_t pos_end = 0;
    /// @brief changed flag (unused)
    bool has_changed = true;
    /// @brief Getter for class attributes
    /// @return class attributes
    inline std::vector<UMLProperty> getAttributes() { return attributes; }
    /// @brief Getter for class methods
    /// @return class methods
    inline std::vector<UMLProperty> getMethods() { return methods; }
    /// @brief Getter for class inherited attributes
    /// @return class inherited attributes
    inline std::vector<UMLProperty> getInheritedAttributes() { return inheritedAttributes; }
    /// @brief Getter for class inherited methods
    /// @return class inherited methods
    inline std::vector<UMLProperty> getInheritedMethods() { return inheritedMethods; }
    /// @brief Getter for class relations
    /// @return class relations
    inline std::vector<UMLRelation>& getRelations() { return relations; }
    /// @brief Getter for class X position
    /// @return class X position
    inline int getXPos() { return x; }
    /// @brief Getter for class Y position
    /// @return class Y position
    inline int getYPos() { return y; }
    /// @brief Getter for class X position in code
    /// @return class X position in code
    inline size_t getXLine() { return x_line; }
    /// @brief Getter for class Y position in code
    /// @return class Y position in code
    inline size_t getYLine() { return y_line; }
    /// @brief Adds a new property to class
    /// @param new_p    new property
    /// @param isMethod flag for method or attribute
    /// @param n        index of property (for duplicates)
    void addProperty(UMLProperty new_p, bool isMethod, size_t n);
    /// @brief Adds a new relation to class
    /// @param new_r    new relation
    /// @param n        index of relation (for duplicates)
    void addRelation(UMLRelation new_r, size_t n);
    /// @brief updates class position
    /// @param pos      new value
    /// @param is_x     flag for x or y
    /// @param i        code line index
    /// @return true if position has been updated
    bool updatePosition(int pos, bool is_x, size_t i);
    /// @brief      After class parsing, delete exceeding properties
    /// @param a    size of attribute list
    /// @param m    size of method list
    void removeExceedingProperties(size_t a, size_t m);
    /// @brief      After class parsing, delete exceeding relations
    /// @param r    size of relation list
    void removeExceedingRelations(size_t r);
    /// @brief      Prints class data to debug console
    void printProperties();
    /// @brief      updates inherited properties and returns true if anything has changed
    /// @param isMethod flag for method or attribute
    /// @param src  list of properties to be copied
    /// @return true if inherited properties have been updated
    bool updateInherited(bool isMethod, std::vector<UMLProperty> src);
    /// @brief      Setter for error flag (also sets changed flag)
    /// @param e    new error flag
    void setErrorFlag(bool e) { has_changed |= (had_error != e); had_error = has_error; has_error = e; }
    /// @brief      Removes all inherited properties
    inline void cleanAndSetUpdatedInherited() { inheritedAttributes.clear(); inheritedMethods.clear(); has_changed = true; }
    /// @brief      Getter for error flag
    /// @return     error flag
    inline bool getErrorFlag() { return has_error; }
    /// @brief      Removes all Pos positions in code
    inline void removePosLines() { x_line = 0; y_line = 0; }
    /// @brief      Removes all Pos positions in code
    inline void removePos() { x = 0; y = 0; }
    /// @brief      Removes all Code Pos flagse
    inline void removePosFlags() { x_set = false; y_set = false; }
    /// @brief      Getter for duplicate flag
    /// @return     duplicate flag
    inline bool getDuplicateFlag() { return duplicate; }
    /// @brief      Setter for duplicate flag
    inline void setDuplicateFlag(bool d) { duplicate = d; }
    /// @brief      Class comparator based on name
    bool operator==(const UMLClass& rhs) { return this->class_name == rhs.class_name; }
protected:
    /// @brief flag used for drawing color
    bool is_interface = false;
    /// @brief flag used for when highlighting collisions
    bool duplicate = false;
    /// @brief flag used for detecting errors
    bool has_error = false;
    /// @brief flag used for saving last error state
    bool had_error = true;
    /// @brief class name
    QString class_name = "";
    /// @brief class attributes
    std::vector<UMLProperty> attributes;
    /// @brief class methods
    std::vector<UMLProperty> methods;
    /// @brief inherited attributes	
    std::vector<UMLProperty> inheritedAttributes;
    /// @brief inherited methods
    std::vector<UMLProperty> inheritedMethods;
    /// @brief class relations
    std::vector<UMLRelation> relations;
    /// @brief class X position
    int x = 0;
    /// @brief class Y position
    int y = 0;
    /// @brief class X position in code
    size_t x_line = 0;
    /// @brief class Y position in code
    size_t y_line = 0;
    /// @brief class X position set flag
    bool x_set = false;
    /// @brief class Y position set flag
    bool y_set = false;
};

class SEQAction
{
public:
    /// @brief      inicializer of SEQAction
    SEQAction(QString method, RuleID type, size_t line, QString sender, QString receiver)
        : sender(sender), receiver(receiver), method(method), type(type), line(line)
    {
    }
    ~SEQAction() {}
    /// @brief      Getter for member name
    /// @return     member name
    inline QString getMethod() { return method; }
    /// @brief      Getter for member type
    /// @return     member type
    inline RuleID getType() { return type; }
    /// @brief      Getter for member has_error
    /// @return     member has_error
    inline int getErrorLevel() { return error_level; }
    /// @brief      Getter for member sender index
    /// @return     member sender
    inline size_t getSenderIndex() { return sender_index; }
    /// @brief      Getter for member receiver index
    /// @return     member receiver
    inline size_t getReceiverIndex() { return receiver_index; }
    /// @brief      Getter for member sender index
    inline void setSenderIndex(size_t i) { sender_index = i; }
    /// @brief      Setter for member receiver index
    inline void setReceiverIndex(size_t i) { receiver_index = i; }
    /// @brief      Setter for member sender name
    /// @return     member sender
    inline QString getSender() { return sender; }
    /// @brief      Getter for member receiver name
    /// @return     member receiver
    inline QString getReceiver() { return receiver; }
    /// @brief      Setter for member has_error
    /// @param e    new value (Only increasing)
    inline void setErrorLevel(int e) { if (e > error_level) error_level = e; }
    /// @brief      Getter for member line position in code
    /// @return     line position of action
    inline size_t getLine() { return line; }
protected:
    /// @brief      method name
    const QString method;
    /// @brief      Actiom error level (0 = OK, 1 = warning, 2 = error)
    int error_level = 0;
    /// @brief      Action type
    const RuleID type;
    /// @brief      Member sender
    const QString sender;
    /// @brief      Member sender index
    size_t sender_index = 0;
    /// @brief      Member receiver
    const QString receiver;
    /// @brief      Member receiver index
    size_t receiver_index = 0;
    /// @brief      Line position in code
    const size_t line;
};

/// @brief      Class for mamber in sequence activation definition
class SEQActivation
{
public:
    /// @brief      Constructor
    /// @param start start time
    /// @param startLine   start time position in code
    SEQActivation(size_t start, size_t startLine)
    {
        this->start = start;
        this->startLine = startLine;
    }
    /// @brief      Destructor
    ~SEQActivation() {}
    /// @brief      Getter for start time
    /// @return     start time
    size_t startIndex() { return start; }
    /// @brief      Getter for end time
    /// @return     end time
    size_t endIndex() { return end; }
    /// @brief      Setter for end time
    /// @param ln   new end time
    void setEndIndex(size_t ln) { end = ln; };
protected:
    /// @brief      Start time
    size_t start = 0;
    /// @brief      Start time position in code
    size_t startLine = 0;
    /// @brief      End time
    size_t end = 0;
};

/// @brief      Class for sequence diagram member definition
class SEQMember
{
public:
    /// @brief      Constructor
    /// @param name member name
    /// @param ln   member first line occurence
    SEQMember(QString name, size_t ln)
    {
        this->name = name;
        this->first_line = ln;
    }
    /// @brief      Destructor
    ~SEQMember() {}
    /// @brief              Getter for member name
    /// @return             member name
    inline QString getName() { return name; }
    /// @brief              Getter for member is_interface
    /// @return             member is_interface
    inline bool isInterface() { return is_interface; }
    /// @brief              Getter for member has_error
    /// @return             member has_error
    inline bool getErrorFlag() { return has_error; }
    /// @brief              Setter for member has_error
    /// @param e            new value
    inline void setErrorFlag(bool e) { has_error = e; }
    /// @brief              Getter for activations (read the size from actions!)
    /// @return             vector of activations
    inline std::vector<SEQActivation> getActivations() { return activations; }
    /// @brief              Getter for activation flag
    /// @return             activation flag
    inline bool getActivatedFlag() { return is_activated; }
    /// @brief              Setter for member is_activated
    /// @param e            new value
    inline void setActivatedFlag(bool e) { is_activated = e; }
    /// @brief              Getter for first occurance in code
    /// @return             first occurance in code
    inline size_t getLine() { return first_line; }
    /// @brief              adding new activation Class (only start)
    /// @param start        start time
    /// @param startLine    line in code
    void addActivation(size_t start, size_t startLine);
    /// @brief              Setter for class_id
    /// @param id           new id
    inline void setClassID(size_t id) { class_id = id; }
    /// @brief              Getter for class_id
    /// @return             class_id
    inline size_t getClassID() { return class_id; }
    /// @brief              Setter for deactivation time (only accessess the end of vector)
    /// @param time         end time
    void setDeactivationTime(size_t time);
    /// @brief              Tests if Member was active at given time
    /// @param time         time to test
    /// @return             true if active
    bool wasActiveAtTime(size_t time);
    /// @brief              Setter for flag
    /// @param f            new flag
    void setInterfaceFlag(bool f) { is_interface = f; }
protected:
    /// @brief      Member name
    QString name = "";
    /// @brief      Member has error flag
    bool has_error = false;
    /// @brief      Member is interface flag
    bool is_interface = false;
    /// @brief      Vector of member activations
    std::vector<SEQActivation> activations;
    /// @brief      Member real class id
    size_t class_id = 0;
    /// @brief      Member activation flag
    bool is_activated = false;
    /// @brief      Member first line occurence
    size_t first_line = 0;
};

/// @brief      Class for sequence diagram definition
class Sequence
{
public:
    /// @brief      Constructor
    Sequence() {}
    /// @brief      Constructor
    /// @param name sequence name
    /// @param line sequence line start
    Sequence(QString name, size_t line)
    {
        this->name = name;
        this->line = line;
    }
    /// @brief      Destructor
    ~Sequence() {}
    /// @brief      Activate a member at specified time
    /// @param name member name
    /// @param time time to activate
    /// @param line line in code
    /// @return     true if member was activated
    bool activateMember(QString name, size_t time, size_t line);
    /// @brief      Deactivate a member at specified time
    /// @param name member name
    /// @param time time to deactivate
    /// @return     true if member was deactivated
    bool deactivateMember(QString name, size_t time);
    /// @brief      Add new action to sequence diagram
    /// @param action action to add
    void addAction(SEQAction action);
    /// @brief      Getter for members
    /// @return     vector of members
    std::vector<SEQMember> getMembers() { return members; }
    /// @brief      Getter for actions
    /// @return     vector of actions
    std::vector<SEQAction> getActions() { return actions; }
    /// @brief      Getter for name
    /// @return     sequence name
    inline QString getName() { return name; }
    /// @brief      Connect all actions with their classes
    void connectActions();
    /// @brief      Test all members if they are valid
    /// @param classes vector of members
    void testEntities(std::vector<UMLClass> classes);
    /// @brief      Disable all leftover (activated) members
    /// @param time time to disable
    void disableLeftovers(size_t time);
    /// @brief      Test all actions if they are valid
    /// @param classes vector of members
    void testActions(std::vector<UMLClass> classes);
protected:
    /// @brief      Getter member based on name
    /// @param name member name
    /// @return     member
    SEQMember* getMemberByName(QString name);
    /// @brief      Getter member index based on name
    /// @param name member name
    /// @return     member index, -1 on not found
    int getMemberIndexByName(QString name);
    /// @brief      Sequence line start
    size_t line = 0;
    /// @brief      Sequence name
    QString name = "";
    /// @brief      Vector of members
    std::vector<SEQMember> members;
    /// @brief      Vector of actions
    std::vector<SEQAction> actions;
};
