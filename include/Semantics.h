#pragma once

#include <QPair>
#include "Syntax/Analyzer.h"
#include "Services/Debug.h"
#include "Services/Code.h"

/// @brief Class that stores UML relation data
class UMLRelation
{
public:
    size_t pos = 0;
    UMLRelation() {}
    UMLRelation(QString entity, RuleID type)
    {
        this->entity = entity;
        this->type = type;
    }
    ~UMLRelation() {}
    QString toString();
    QString getEntity();
    size_t getID() { return id; }
    RuleID getType() { return type; }
    void setInvalid() { isValid = false; }
    bool getValid() { return isValid; }
    bool updateRelationParams(UMLRelation new_r);
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

class UMLProperty
{
public:
    size_t pos = 0;

    UMLProperty() {}
    UMLProperty(QString mod, QString type, QString name)
    {
        this->p_mod = mod;
        this->p_type = type;
        this->p_name = name;
    }
    ~UMLProperty() {}
    bool operator==(const UMLProperty& rhs)
    {
        return this->p_name == rhs.p_name;
    }
    QString toString();
    bool updateProperty(UMLProperty new_p);
    QString getType() { return p_type; }
    QString getName() { return p_name; }
    inline bool getDuplicateFlag() { return duplicate; }
    inline void setDuplicateFlag(bool d) { duplicate = d; }
protected:
    bool duplicate = false;
    QString p_mod;
    QString p_type;
    QString p_name;
};

class UMLClass
{
public:
    UMLClass() {}
    ~UMLClass() {}
    QString getClassName();
    void updateName(QString name);
    void updateType(bool is_interface);
    bool isInterface() { return is_interface; }
    size_t pos = 0;
    size_t pos_end = 0;
    bool has_changed = true;
    inline std::vector<UMLProperty> getAttributes() { return attributes; }
    inline std::vector<UMLProperty> getMethods() { return methods; }
    inline std::vector<UMLProperty> getInheritedAttributes() { return inheritedAttributes; }
    inline std::vector<UMLProperty> getInheritedMethods() { return inheritedMethods; }
    inline std::vector<UMLRelation>& getRelations() { return relations; }
    inline int getXPos() { return x; }
    inline int getYPos() { return y; }
    inline size_t getXLine() { return x_line; }
    inline size_t getYLine() { return y_line; }
    void addProperty(UMLProperty new_p, bool isMethod, size_t n);
    void addRelation(UMLRelation new_r, size_t n);
    bool updatePosition(int pos, bool is_x, size_t i);
    /// @brief      After class generation, delete exceeding properties
    /// @param a    size of attribute list
    /// @param m    size of method list
    void removeExceedingProperties(size_t a, size_t m);
    void removeExceedingRelations(size_t r);
    void printProperties();
    bool updateInherited(bool isMethod, std::vector<UMLProperty> src);
    void setErrorFlag(bool e) { has_changed |= (had_error != e); had_error = has_error; has_error = e; }
    inline void cleanAndSetUpdatedInherited() { inheritedAttributes.clear(); inheritedMethods.clear(); has_changed = true; }
    inline bool getErrorFlag() { return has_error; }
    inline void removePosLines() { x_line = 0; y_line = 0; }
    inline void removePos() { x = 0; y = 0; }
    inline void removePosFlags() { x_set = false; y_set = false; }
    inline bool getDuplicateFlag() { return duplicate; }
    inline void setDuplicateFlag(bool d) { duplicate = d; }
    bool operator==(const UMLClass& rhs) { return this->class_name == rhs.class_name; }
protected:
    bool is_interface = false;
    bool duplicate = false;
    bool has_error = false;
    bool had_error = true;
    QString class_name = "";
    std::vector<UMLProperty> attributes;
    std::vector<UMLProperty> methods;
    std::vector<UMLProperty> inheritedAttributes;
    std::vector<UMLProperty> inheritedMethods;
    std::vector<UMLRelation> relations;
    int x = 0;
    int y = 0;
    size_t x_line = 0;
    size_t y_line = 0;
    bool x_set = false;
    bool y_set = false;
};

class SEQAction
{
public:
    SEQAction() {}
    SEQAction(QString name, RuleID type, size_t line, QString sender, QString receiver)
    {
        this->method = method;
        this->type = type;
        this->line = line;
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
    inline bool getErrorFlag() { return has_error; }
    /// @brief      Getter for member sender index
    /// @return     member sender
    inline size_t getSenderIndex() { return sender_index; }
    /// @brief      Getter for member receiver index
    /// @return     member receiver
    inline size_t getReceiverIndex() { return receiver_index; }
    /// @brief      Getter for member sender index
    /// @return     member sender
    inline void setSenderIndex(size_t i) { sender_index = i; }
    /// @brief      Setter for member receiver index
    /// @return     member receiver
    inline void setReceiverIndex(size_t i) { receiver_index = i; }
    /// @brief      Setter for member sender name
    /// @return     member sender
    inline QString getSender() { return sender; }
    /// @brief      Getter for member receiver name
    /// @return     member receiver
    inline QString getReceiver() { return receiver; }
    /// @brief      Setter for member has_error
    /// @param e    new value
    inline void setErrorFlag(bool e) { has_error = e; }
    /// @brief      Getter for member line position in code
    /// @return     line position of action
    inline size_t getLine() { return line; }
protected:
    QString method = "";
    bool has_error = false;
    RuleID type = RuleID::R_ERR;
    QString sender;
    size_t sender_index;
    QString receiver;
    size_t receiver_index;
    size_t line;
};

class SEQActivation
{
public:
    SEQActivation(size_t start, size_t startLine) {
        this->start = start;
        this->startLine = startLine;
    }
    ~SEQActivation() {}
    size_t startIndex() { return start; }
    size_t endIndex() { return end; }
    void setEndIndex(size_t ln) { end = ln; };
protected:
    size_t start = 0;
    size_t startLine = 0;
    size_t end = 0;
};

class SEQMember
{
public:
    SEQMember(QString name)
    {
        this->name = name;
    }
    ~SEQMember() {}
    /// @brief      Getter for member name
    /// @return     member name
    inline QString getName() { return name; }
    /// @brief      Getter for member is_interface
    /// @return     member is_interface
    inline bool isInterface() { return is_interface; }
    /// @brief      Getter for member has_error
    /// @return     member has_error
    inline bool getErrorFlag() { return has_error; }
    /// @brief      Setter for member has_error
    /// @param e    new value
    inline void setErrorFlag(bool e) { has_error = e; }
    /// @brief      Getter for activations (read the size from actions!)
    /// @return     vector of activations
    inline std::vector<SEQActivation> getActivations() { return activations; }
    /// @brief      Getter for activation flag
    /// @return     activation flag
    inline bool getActivatedFlag() { return is_activated; }
    /// @brief      Setter for member is_activated
    /// @param e    new value
    inline void setActivatedFlag(bool e) { is_activated = e; }
    /// @brief              adding new activation Class (only start)
    /// @param start        start time
    /// @param startLine    line in code
    void addActivation(size_t start, size_t startLine);
    /// @brief      Setter for deactivation time (only accessess the end of vector)
    /// @param time end time
    void setDeactivationTime(size_t time);
protected:
    QString name = "";
    bool has_error = false;
    bool is_interface = false;
    std::vector<SEQActivation> activations;
    size_t class_id = 0;
    bool is_activated = false;
};

class Sequence
{
public:
    Sequence() {}
    Sequence(QString name, size_t line) {
        this->name = name;
        this->line = line;
    }
    ~Sequence() {}
    bool activateMember(QString name, size_t time, size_t line);
    bool deactivateMember(QString name, size_t time);
    void addAction(SEQAction action);
    std::vector<SEQMember> getMembers() { return members; }
    std::vector<SEQAction> getActions() { return actions; }
protected:
    SEQMember* getMemberByName(QString name);

    size_t line = 0;
    QString name = "";
    std::vector<SEQMember> members;
    std::vector<SEQAction> actions;
};
